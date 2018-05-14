/*********************                                                        */
/*! \file candidate_rewrite_database.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of candidate_rewrite_database
 **/

#include "theory/quantifiers/candidate_rewrite_database.h"

#include "options/base_options.h"
#include "options/quantifiers_options.h"
#include "printer/printer.h"
#include "smt/smt_engine.h"
#include "smt/smt_engine_scope.h"
#include "smt/smt_statistics_registry.h"
#include "theory/quantifiers/sygus/ce_guided_instantiation.h"
#include "theory/quantifiers/sygus/term_database_sygus.h"
#include "theory/quantifiers/term_util.h"

using namespace std;
using namespace CVC4::kind;
using namespace CVC4::context;

namespace CVC4 {
namespace theory {
namespace quantifiers {

CandidateRewriteDatabase::CandidateRewriteDatabase()
    : d_qe(nullptr), d_tds(nullptr), d_ext_rewrite(nullptr), d_using_sygus(false)
{
}
void CandidateRewriteDatabase::initialize(ExtendedRewriter * er,
                                          TypeNode tn,
                                          std::vector<Node>& vars,
                                          unsigned nsamples)
{
  d_candidate = Node::null();
  d_type = tn;
  d_using_sygus = false;
  initializeInternal(nullptr);
  d_ext_rewrite = er;
  d_sampler.initialize(tn, vars, nsamples);
}

void CandidateRewriteDatabase::initializeSygus(QuantifiersEngine* qe,
                                               Node f,
                                               unsigned nsamples,
                                               bool useSygusType)
{
  d_candidate = f;
  d_type = f.getType();
  Assert(d_type.isDatatype());
  Assert(static_cast<DatatypeType>(d_type.toType()).getDatatype().isSygus());
  d_using_sygus = true;
  initializeInternal(qe);
  d_ext_rewrite = d_tds->getExtRewriter();
  d_sampler.initializeSygus(d_tds, f, nsamples, useSygusType);
}

void CandidateRewriteDatabase::initializeInternal(QuantifiersEngine* qe)
{
  d_qe = qe;
  d_tds = d_qe==nullptr ? nullptr : qe->getTermDatabaseSygus();
  /*
  if (options::sygusRewSynthFilterCong())
  {
    // initialize the dynamic rewriter
    std::stringstream ss;
    ss << "_dyn_rewriter_" << d_type;
    d_drewrite =
        std::unique_ptr<DynamicRewriter>(new DynamicRewriter(ss.str(), &d_fake_context));
    d_sampler.setDynamicRewriter(d_drewrite.get());
  }
  */
}

bool CandidateRewriteDatabase::addTerm(Node sol, std::ostream& out, bool& rew_print)
{
  bool is_unique_term = true;
  Node eq_sol = d_sampler.registerTerm(sol);
  // eq_sol is a candidate solution that is equivalent to sol
  if (eq_sol != sol)
  {
    is_unique_term = false;
    // if eq_sol is null, then we have an uninteresting candidate rewrite,
    // e.g. one that is alpha-equivalent to another.
    if (!eq_sol.isNull())
    {
      Node solb = sol;
      Node eq_solb = eq_sol;
      if( d_using_sygus )
      {
        Assert( d_tds!=nullptr );
        solb = d_tds->sygusToBuiltin(sol);
        eq_solb = d_tds->sygusToBuiltin(eq_sol);
      }
      Assert(d_ext_rewrite!=nullptr);
      Node solbr = d_ext_rewrite->extendedRewrite(solb);
      Node eq_solr = d_ext_rewrite->extendedRewrite(eq_solb);
      bool verified = false;
      Trace("rr-check") << "Check candidate rewrite..." << std::endl;
      // verify it if applicable
      if (options::sygusRewSynthCheck())
      {
        // Notice we don't set produce-models. rrChecker takes the same
        // options as the SmtEngine we belong to, where we ensure that
        // produce-models is set.
        NodeManager* nm = NodeManager::currentNM();
        SmtEngine rrChecker(nm->toExprManager());
        rrChecker.setLogic(smt::currentSmtEngine()->getLogicInfo());
        Node crr = solbr.eqNode(eq_solr).negate();
        Trace("rr-check") << "Check candidate rewrite : " << crr << std::endl;
        // quantify over the free variables in crr
        std::vector<Node> fvs;
        TermUtil::computeVarContains(crr, fvs);
        std::map<Node, unsigned> fv_index;
        std::vector<Node> sks;
        if (!fvs.empty())
        {
          // map to skolems
          for (unsigned i = 0, size = fvs.size(); i < size; i++)
          {
            Node v = fvs[i];
            fv_index[v] = i;
            std::map<Node, Node>::iterator itf = d_fv_to_skolem.find(v);
            if (itf == d_fv_to_skolem.end())
            {
              Node sk = nm->mkSkolem("rrck", v.getType());
              d_fv_to_skolem[v] = sk;
              sks.push_back(sk);
            }
            else
            {
              sks.push_back(itf->second);
            }
          }
          crr = crr.substitute(fvs.begin(), fvs.end(), sks.begin(), sks.end());
        }
        rrChecker.assertFormula(crr.toExpr());
        Result r = rrChecker.checkSat();
        Trace("rr-check") << "...result : " << r << std::endl;
        if (r.asSatisfiabilityResult().isSat())
        {
          Trace("rr-check") << "...rewrite does not hold for: " << std::endl;
          is_unique_term = true;
          std::vector<Node> vars;
          d_sampler.getVariables(vars);
          std::vector<Node> pt;
          for (const Node& v : vars)
          {
            std::map<Node, unsigned>::iterator itf = fv_index.find(v);
            Node val;
            if (itf == fv_index.end())
            {
              // not in conjecture, can use arbitrary value
              val = v.getType().mkGroundTerm();
            }
            else
            {
              // get the model value of its skolem
              Node sk = sks[itf->second];
              val = Node::fromExpr(rrChecker.getValue(sk.toExpr()));
              Trace("rr-check") << "  " << v << " -> " << val << std::endl;
            }
            pt.push_back(val);
          }
          d_sampler.addSamplePoint(pt);
          // add the solution again
          // by construction of the above point, we should be unique now
          Node eq_sol_new = d_sampler.registerTerm(sol);
          Assert(eq_sol_new == sol);
        }
        else
        {
          verified = !r.asSatisfiabilityResult().isUnknown();
        }
      }
      else
      {
        // just insist that constants are not relevant pairs
        is_unique_term = solb.isConst() && eq_solb.isConst();
      }
      if (!is_unique_term)
      {
        // register this as a relevant pair (helps filtering)
        d_sampler.registerRelevantPair(sol, eq_sol);
        // The analog of terms sol and eq_sol are equivalent under
        // sample points but do not rewrite to the same term. Hence,
        // this indicates a candidate rewrite.
        out << "(" << (verified ? "" : "candidate-") << "rewrite ";
        if (d_using_sygus)
        {
          Printer* p = Printer::getPrinter(options::outputLanguage());
          p->toStreamSygus(out, sol);
          out << " ";
          p->toStreamSygus(out, eq_sol);
        }
        else
        {
          out << sol << " " << eq_sol;
        }
        out << ")" << std::endl;
        rew_print = true;
        // debugging information
        if (Trace.isOn("sygus-rr-debug"))
        {
          Trace("sygus-rr-debug") << "; candidate #1 ext-rewrites to: " << solbr
                                  << std::endl;
          Trace("sygus-rr-debug")
              << "; candidate #2 ext-rewrites to: " << eq_solr << std::endl;
        }
        if (options::sygusRewSynthAccel() && d_using_sygus)
        {
          Assert( d_tds!=nullptr );
          // Add a symmetry breaking clause that excludes the larger
          // of sol and eq_sol. This effectively states that we no longer
          // wish to enumerate any term that contains sol (resp. eq_sol)
          // as a subterm.
          Node exc_sol = sol;
          unsigned sz = d_tds->getSygusTermSize(sol);
          unsigned eqsz = d_tds->getSygusTermSize(eq_sol);
          if (eqsz > sz)
          {
            sz = eqsz;
            exc_sol = eq_sol;
          }
          TypeNode ptn = d_candidate.getType();
          Node x = d_tds->getFreeVar(ptn, 0);
          Node lem = d_tds->getExplain()->getExplanationForEquality(x, exc_sol);
          lem = lem.negate();
          Trace("sygus-rr-sb") << "Symmetry breaking lemma : " << lem
                               << std::endl;
          d_tds->registerSymBreakLemma(d_candidate, lem, ptn, sz);
        }
      }
    }
    // We count this as a rewrite if we did not explicitly rule it out.
    // The value of is_unique_term is false iff this call resulted in a rewrite.
    // Notice that when --sygus-rr-synth-check is enabled,
    // statistics on number of candidate rewrite rules is
    // an accurate count of (#enumerated_terms-#unique_terms) only if
    // the option sygus-rr-synth-filter-order is disabled. The reason
    // is that the sygus sampler may reason that a candidate rewrite
    // rule is not useful since its variables are unordered, whereby
    // it discards it as a redundant candidate rewrite rule before
    // checking its correctness.
  }
  return is_unique_term;
}

bool CandidateRewriteDatabase::addTerm(Node sol, std::ostream& out)
{
  bool rew_print = false;
  return addTerm(sol,out,rew_print);
}

CandidateRewriteDatabaseGen::CandidateRewriteDatabaseGen(std::vector<Node>& vars, unsigned nsamples)
    : d_nsamples(nsamples)
{
  d_vars.insert(d_vars.end(), vars.begin(), vars.end());
}

bool CandidateRewriteDatabaseGen::addTerm(Node n, std::ostream& out)
{
  TypeNode tn = n.getType();
  std::map<TypeNode, CandidateRewriteDatabase>::iterator itc = d_cdbs.find(tn);
  if (itc == d_cdbs.end())
  {
    // initialize with the extended rewriter owned by this class
    d_cdbs[tn].initialize(&d_ext_rewrite, tn, d_vars, d_nsamples);
    itc = d_cdbs.find(tn);
  }
  return itc->second.addTerm(n, out);
}

} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */
