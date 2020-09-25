/*********************                                                        */
/*! \file synth_engine.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Mathias Preiner, Haniel Barbosa
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of the quantifiers module for managing all approaches
 ** to synthesis, in particular, those described in Reynolds et al CAV 2015.
 **
 **/
#include "theory/quantifiers/sygus/synth_engine.h"

#include "expr/node_algorithm.h"
#include "options/quantifiers_options.h"
#include "theory/quantifiers/quantifiers_attributes.h"
#include "theory/quantifiers/sygus/term_database_sygus.h"
#include "theory/quantifiers/term_util.h"
#include "theory/quantifiers_engine.h"
#include "theory/smt_engine_subsolver.h"
#include "theory/theory_engine.h"

using namespace CVC4::kind;
using namespace std;

namespace CVC4 {
namespace theory {
namespace quantifiers {

SynthEngine::SynthEngine(QuantifiersEngine* qe, context::Context* c)
    : QuantifiersModule(qe), d_tds(qe->getTermDatabaseSygus())
{
  d_conjs.push_back(std::unique_ptr<SynthConjecture>(
      new SynthConjecture(d_quantEngine, this, d_statistics)));
  d_conj = d_conjs.back().get();
}

SynthEngine::~SynthEngine() {}

void SynthEngine::presolve()
{
  Trace("sygus-engine") << "SynthEngine::presolve" << std::endl;
  for (unsigned i = 0, size = d_conjs.size(); i < size; i++)
  {
    d_conjs[i]->presolve();
  }
  Trace("sygus-engine") << "SynthEngine::presolve finished" << std::endl;
}

bool SynthEngine::needsCheck(Theory::Effort e)
{
  return e >= Theory::EFFORT_LAST_CALL;
}

QuantifiersModule::QEffort SynthEngine::needsModel(Theory::Effort e)
{
  return QEFFORT_MODEL;
}

void SynthEngine::check(Theory::Effort e, QEffort quant_e)
{
  // are we at the proper effort level?
  if (quant_e != QEFFORT_MODEL)
  {
    return;
  }

  // if we are waiting to assign the conjecture, do it now
  bool assigned = !d_waiting_conj.empty();
  while (!d_waiting_conj.empty())
  {
    Node q = d_waiting_conj.back();
    d_waiting_conj.pop_back();
    Trace("sygus-engine") << "--- Conjecture waiting to assign: " << q
                          << std::endl;
    assignConjecture(q);
  }
  if (assigned)
  {
    // assign conjecture always uses the output channel, either by reducing a
    // quantified formula to another, or adding initial lemmas during
    // SynthConjecture::assign. Thus, we return here and re-check.
    return;
  }

  Trace("sygus-engine") << "---Counterexample Guided Instantiation Engine---"
                        << std::endl;
  Trace("sygus-engine-debug") << std::endl;
  Valuation& valuation = d_quantEngine->getValuation();
  std::vector<SynthConjecture*> activeCheckConj;
  for (unsigned i = 0, size = d_conjs.size(); i < size; i++)
  {
    SynthConjecture* sc = d_conjs[i].get();
    bool active = false;
    bool value;
    if (valuation.hasSatValue(sc->getConjecture(), value))
    {
      active = value;
    }
    else
    {
      Trace("sygus-engine-debug") << "...no value for quantified formula."
                                  << std::endl;
    }
    Trace("sygus-engine-debug")
        << "Current conjecture status : active : " << active << std::endl;
    if (active && sc->needsCheck())
    {
      activeCheckConj.push_back(sc);
    }
  }
  std::vector<SynthConjecture*> acnext;
  do
  {
    Trace("sygus-engine-debug") << "Checking " << activeCheckConj.size()
                                << " active conjectures..." << std::endl;
    for (unsigned i = 0, size = activeCheckConj.size(); i < size; i++)
    {
      SynthConjecture* sc = activeCheckConj[i];
      if (!checkConjecture(sc))
      {
        if (!sc->needsRefinement())
        {
          acnext.push_back(sc);
        }
      }
    }
    activeCheckConj.clear();
    activeCheckConj = acnext;
    acnext.clear();
  } while (!activeCheckConj.empty()
           && !d_quantEngine->theoryEngineNeedsCheck());
  Trace("sygus-engine")
      << "Finished Counterexample Guided Instantiation engine." << std::endl;
}

void SynthEngine::assignConjecture(Node q)
{
  Trace("sygus-engine") << "SynthEngine::assignConjecture " << q << std::endl;
  if (options::sygusQePreproc())
  {
    // the following does quantifier elimination as a preprocess step
    // for "non-ground single invocation synthesis conjectures":
    //   exists f. forall xy. P[ f(x), x, y ]
    // We run quantifier elimination:
    //   exists y. P[ z, x, y ] ----> Q[ z, x ]
    // Where we replace the original conjecture with:
    //   exists f. forall x. Q[ f(x), x ]
    // For more details, see Example 6 of Reynolds et al. SYNT 2017.
    Node body = q[1];
    if (body.getKind() == NOT && body[0].getKind() == FORALL)
    {
      body = body[0][1];
    }
    NodeManager* nm = NodeManager::currentNM();
    Trace("cegqi-qep") << "Compute single invocation for " << q << "..."
                       << std::endl;
    quantifiers::SingleInvocationPartition sip;
    std::vector<Node> funcs0;
    funcs0.insert(funcs0.end(), q[0].begin(), q[0].end());
    sip.init(funcs0, body);
    Trace("cegqi-qep") << "...finished, got:" << std::endl;
    sip.debugPrint("cegqi-qep");

    if (!sip.isPurelySingleInvocation() && sip.isNonGroundSingleInvocation())
    {
      // create new smt engine to do quantifier elimination
      std::unique_ptr<SmtEngine> smt_qe;
      initializeSubsolver(smt_qe);
      Trace("cegqi-qep") << "Property is non-ground single invocation, run "
                            "QE to obtain single invocation."
                         << std::endl;
      // partition variables
      std::vector<Node> all_vars;
      sip.getAllVariables(all_vars);
      std::vector<Node> si_vars;
      sip.getSingleInvocationVariables(si_vars);
      std::vector<Node> qe_vars;
      std::vector<Node> nqe_vars;
      for (unsigned i = 0, size = all_vars.size(); i < size; i++)
      {
        Node v = all_vars[i];
        if (std::find(funcs0.begin(), funcs0.end(), v) != funcs0.end())
        {
          Trace("cegqi-qep") << "- fun var: " << v << std::endl;
        }
        else if (std::find(si_vars.begin(), si_vars.end(), v) == si_vars.end())
        {
          qe_vars.push_back(v);
          Trace("cegqi-qep") << "- qe var: " << v << std::endl;
        }
        else
        {
          nqe_vars.push_back(v);
          Trace("cegqi-qep") << "- non qe var: " << v << std::endl;
        }
      }
      std::vector<Node> orig;
      std::vector<Node> subs;
      // skolemize non-qe variables
      for (unsigned i = 0, size = nqe_vars.size(); i < size; i++)
      {
        Node k = nm->mkSkolem(
            "k", nqe_vars[i].getType(), "qe for non-ground single invocation");
        orig.push_back(nqe_vars[i]);
        subs.push_back(k);
        Trace("cegqi-qep") << "  subs : " << nqe_vars[i] << " -> " << k
                           << std::endl;
      }
      std::vector<Node> funcs1;
      sip.getFunctions(funcs1);
      for (unsigned i = 0, size = funcs1.size(); i < size; i++)
      {
        Node f = funcs1[i];
        Node fi = sip.getFunctionInvocationFor(f);
        Node fv = sip.getFirstOrderVariableForFunction(f);
        Assert(!fi.isNull());
        orig.push_back(fi);
        Node k =
            nm->mkSkolem("k",
                         fv.getType(),
                         "qe for function in non-ground single invocation");
        subs.push_back(k);
        Trace("cegqi-qep") << "  subs : " << fi << " -> " << k << std::endl;
      }
      Node conj_se_ngsi = sip.getFullSpecification();
      Trace("cegqi-qep") << "Full specification is " << conj_se_ngsi
                         << std::endl;
      Node conj_se_ngsi_subs = conj_se_ngsi.substitute(
          orig.begin(), orig.end(), subs.begin(), subs.end());
      Assert(!qe_vars.empty());
      conj_se_ngsi_subs = nm->mkNode(EXISTS,
                                     nm->mkNode(BOUND_VAR_LIST, qe_vars),
                                     conj_se_ngsi_subs.negate());

      Trace("cegqi-qep") << "Run quantifier elimination on "
                         << conj_se_ngsi_subs << std::endl;
      Node qeRes =
          smt_qe->getQuantifierElimination(conj_se_ngsi_subs, true, false);
      Trace("cegqi-qep") << "Result : " << qeRes << std::endl;

      // create single invocation conjecture, if QE was successful
      if (!expr::hasBoundVar(qeRes))
      {
        qeRes = qeRes.substitute(
            subs.begin(), subs.end(), orig.begin(), orig.end());
        if (!nqe_vars.empty())
        {
          qeRes =
              nm->mkNode(EXISTS, nm->mkNode(BOUND_VAR_LIST, nqe_vars), qeRes);
        }
        Assert(q.getNumChildren() == 3);
        qeRes = nm->mkNode(FORALL, q[0], qeRes, q[2]);
        Trace("cegqi-qep") << "Converted conjecture after QE : " << qeRes
                           << std::endl;
        qeRes = Rewriter::rewrite(qeRes);
        Node nq = qeRes;
        // must assert it is equivalent to the original
        Node lem = q.eqNode(nq);
        Trace("cegqi-lemma")
            << "Cegqi::Lemma : qe-preprocess : " << lem << std::endl;
        d_quantEngine->getOutputChannel().lemma(lem);
        // we've reduced the original to a preprocessed version, return
        return;
      }
    }
  }
  // allocate a new synthesis conjecture if not assigned
  if (d_conjs.back()->isAssigned())
  {
    d_conjs.push_back(std::unique_ptr<SynthConjecture>(
        new SynthConjecture(d_quantEngine, this, d_statistics)));
  }
  d_conjs.back()->assign(q);
}

void SynthEngine::registerQuantifier(Node q)
{
  Trace("cegqi-debug") << "SynthEngine: Register quantifier : " << q
                       << std::endl;
  if (d_quantEngine->getOwner(q) != this)
  {
    return;
  }
  if (d_quantEngine->getQuantAttributes()->isFunDef(q))
  {
    Assert(options::sygusRecFun());
    // If it is a recursive function definition, add it to the function
    // definition evaluator class.
    Trace("cegqi") << "Registering function definition : " << q << "\n";
    FunDefEvaluator* fde = d_tds->getFunDefEvaluator();
    fde->assertDefinition(q);
    return;
  }
  Trace("cegqi") << "Register conjecture : " << q << std::endl;
  if (options::sygusQePreproc())
  {
    d_waiting_conj.push_back(q);
  }
  else
  {
    // assign it now
    assignConjecture(q);
  }
}

bool SynthEngine::checkConjecture(SynthConjecture* conj)
{
  Node q = conj->getEmbeddedConjecture();
  Node aq = conj->getConjecture();
  if (Trace.isOn("sygus-engine-debug"))
  {
    conj->debugPrint("sygus-engine-debug");
    Trace("sygus-engine-debug") << std::endl;
  }

  if (!conj->needsRefinement())
  {
    Trace("sygus-engine-debug") << "Do conjecture check..." << std::endl;
    Trace("sygus-engine-debug")
        << "  *** Check candidate phase..." << std::endl;
    std::vector<Node> cclems;
    bool ret = conj->doCheck(cclems);
    bool addedLemma = false;
    for (const Node& lem : cclems)
    {
      if (d_quantEngine->addLemma(lem))
      {
        ++(d_statistics.d_cegqi_lemmas_ce);
        addedLemma = true;
      }
      else
      {
        // this may happen if we eagerly unfold, simplify to true
        Trace("sygus-engine-debug")
            << "  ...FAILED to add candidate!" << std::endl;
      }
    }
    if (addedLemma)
    {
      Trace("sygus-engine-debug")
          << "  ...check for counterexample." << std::endl;
      return true;
    }
    else
    {
      if (conj->needsRefinement())
      {
        // immediately go to refine candidate
        return checkConjecture(conj);
      }
    }
    return ret;
  }
  else
  {
    Trace("sygus-engine-debug")
        << "  *** Refine candidate phase..." << std::endl;
    std::vector<Node> rlems;
    conj->doRefine(rlems);
    bool addedLemma = false;
    for (unsigned i = 0; i < rlems.size(); i++)
    {
      Node lem = rlems[i];
      Trace("cegqi-lemma") << "Cegqi::Lemma : candidate refinement : " << lem
                           << std::endl;
      bool res = d_quantEngine->addLemma(lem);
      if (res)
      {
        ++(d_statistics.d_cegqi_lemmas_refine);
        conj->incrementRefineCount();
        addedLemma = true;
      }
      else
      {
        Trace("cegqi-warn") << "  ...FAILED to add refinement!" << std::endl;
      }
    }
    if (addedLemma)
    {
      Trace("sygus-engine-debug") << "  ...refine candidate." << std::endl;
    }
  }
  return true;
}

void SynthEngine::printSynthSolution(std::ostream& out)
{
  Assert(!d_conjs.empty());
  for (unsigned i = 0, size = d_conjs.size(); i < size; i++)
  {
    if (d_conjs[i]->isAssigned())
    {
      d_conjs[i]->printSynthSolution(out);
    }
  }
}

bool SynthEngine::getSynthSolutions(
    std::map<Node, std::map<Node, Node> >& sol_map)
{
  bool ret = true;
  for (unsigned i = 0, size = d_conjs.size(); i < size; i++)
  {
    if (d_conjs[i]->isAssigned())
    {
      if (!d_conjs[i]->getSynthSolutions(sol_map))
      {
        // if one conjecture fails, we fail overall
        ret = false;
        break;
      }
    }
  }
  return ret;
}

void SynthEngine::preregisterAssertion(Node n)
{
  // check if it sygus conjecture
  if (QuantAttributes::checkSygusConjecture(n))
  {
    // this is a sygus conjecture
    Trace("cegqi") << "Preregister sygus conjecture : " << n << std::endl;
    d_conj->preregisterConjecture(n);
  }
}

}  // namespace quantifiers
}  // namespace theory
} /* namespace CVC4 */
