/*********************                                                        */
/*! \file inference_manager.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Gereon Kremer
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Datatypes inference manager
 **/

#include "theory/datatypes/inference_manager.h"

#include "expr/dtype.h"
#include "options/datatypes_options.h"
#include "smt/smt_statistics_registry.h"
#include "theory/theory.h"

using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace datatypes {

InferenceManager::InferenceManager(Theory& t,
                                   TheoryState& state,
                                   ProofNodeManager* pnm)
    : InferenceManagerBuffered(t, state, pnm),
      d_inferenceLemmas("theory::datatypes::inferenceLemmas"),
      d_inferenceFacts("theory::datatypes::inferenceFacts"),
      d_inferenceConflicts("theory::datatypes::inferenceConflicts"),
      d_ipc(pnm == nullptr ? nullptr
                           : new InferProofCons(state.getSatContext(), pnm))
{
  d_false = NodeManager::currentNM()->mkConst(false);
  smtStatisticsRegistry()->registerStat(&d_inferenceLemmas);
  smtStatisticsRegistry()->registerStat(&d_inferenceFacts);
  smtStatisticsRegistry()->registerStat(&d_inferenceConflicts);
}

InferenceManager::~InferenceManager()
{
  smtStatisticsRegistry()->unregisterStat(&d_inferenceLemmas);
  smtStatisticsRegistry()->unregisterStat(&d_inferenceFacts);
  smtStatisticsRegistry()->unregisterStat(&d_inferenceConflicts);
}

void InferenceManager::addPendingInference(Node conc,
                                           Node exp,
                                           bool forceLemma,
                                           InferId i)
{
  if (forceLemma)
  {
    d_pendingLem.emplace_back(new DatatypesInference(this, conc, exp, i));
  }
  else
  {
    d_pendingFact.emplace_back(new DatatypesInference(this, conc, exp, i));
  }
}

void InferenceManager::process()
{
  // process pending lemmas, used infrequently, only for definitional lemmas
  doPendingLemmas();
  // now process the pending facts
  doPendingFacts();
}

void InferenceManager::sendDtLemma(Node lem,
                                   InferId id,
                                   LemmaProperty p,
                                   bool doCache)
{
  if (isProofEnabled())
  {
    processDtLemma(lem, Node::null(), id);
    return;
  }
  // otherwise send as a normal lemma
  if (lemma(lem, p, doCache))
  {
    d_inferenceLemmas << id;
  }
}

void InferenceManager::sendDtConflict(const std::vector<Node>& conf, InferId id)
{
  if (isProofEnabled())
  {
    Node exp = NodeManager::currentNM()->mkAnd(conf);
    prepareDtInference(d_false, exp, id);
  }
  conflictExp(conf, d_ipc.get());
  d_inferenceConflicts << id;
}

bool InferenceManager::sendLemmas(const std::vector<Node>& lemmas)
{
  bool ret = false;
  for (const Node& lem : lemmas)
  {
    if (lemma(lem))
    {
      ret = true;
    }
  }
  return ret;
}

bool InferenceManager::isProofEnabled() const { return d_ipc != nullptr; }

bool InferenceManager::processDtLemma(
    Node conc, Node exp, InferId id, LemmaProperty p, bool doCache)
{
  conc = prepareDtInference(conc, exp, id);
  if (conc.getKind() == EQUAL)
  {
    // Also process it as a fact first. Some lemmas concluding equalities
    // should be processed as both internal facts and as lemmas.
    // In particular, notice that lemmas that conclude non-datatype equalities
    // are not guaranteed to send the conclusion back to the datatypes solver.
    // We assert them also as facts both for performance reasons and so that
    // the explanations are consistent with our proofs. If this were not the
    // case, the explanation for the lemma could be stored here and supercede
    // a future fact concluding conc via a different explanation, which would
    // cause free assumptions in our proofs.
    processDtFactInternal(conc, exp);
  }
  // send it as an (explained) lemma
  std::vector<Node> expv;
  if (!exp.isNull() && !exp.isConst())
  {
    expv.push_back(exp);
  }
  // Don't explain it. We set the no explain vector to expv and call lemmaExp
  // for consistency, since this will invoke the proof equality engine if
  // necessary.
  if (!lemmaExp(conc, expv, expv, d_ipc.get(), p, doCache))
  {
    Trace("dt-lemma-debug") << "...duplicate lemma" << std::endl;
    return false;
  }
  d_inferenceLemmas << id;
  return true;
}

bool InferenceManager::processDtFact(Node conc, Node exp, InferId id)
{
  conc = prepareDtInference(conc, exp, id);
  // assert the internal fact, which has the same issue as above
  processDtFactInternal(conc, exp);
  d_inferenceFacts << id;
  return true;
}

Node InferenceManager::prepareDtInference(Node conc, Node exp, InferId id)
{
  Trace("dt-lemma-debug") << "prepareDtInference : " << conc << " via " << exp
                          << " by " << id << std::endl;
  if (conc.getKind() == EQUAL && conc[0].getType().isBoolean())
  {
    // must turn (= conc false) into (not conc)
    conc = Rewriter::rewrite(conc);
  }
  if (isProofEnabled())
  {
    // If proofs are enabled, notify the proof constructor.
    // Notice that we have to reconstruct a datatypes inference here. This is
    // because the inference in the pending vector may be destroyed as we are
    // processing this inference, if we triggered to backtrack based on the
    // call below, since it is a unique pointer.
    std::shared_ptr<DatatypesInference> di =
        std::make_shared<DatatypesInference>(this, conc, exp, id);
    d_ipc->notifyFact(di);
  }
  return conc;
}

void InferenceManager::processDtFactInternal(Node conc, Node exp)
{
  bool polarity = conc.getKind() != NOT;
  TNode atom = polarity ? conc : conc[0];
  if (isProofEnabled())
  {
    std::vector<Node> expv;
    if (!exp.isNull() && !exp.isConst())
    {
      lem = NodeManager::currentNM()->mkNode(kind::IMPLIES, exp, conc);
    }
    assertInternalFact(atom, polarity, expv, d_ipc.get());
  }
  else
  {
    // use version without proofs
    assertInternalFact(atom, polarity, exp);
  }
}

}  // namespace datatypes
}  // namespace theory
}  // namespace CVC4
