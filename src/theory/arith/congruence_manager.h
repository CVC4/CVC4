/*********************                                                        */
/*! \file congruence_manager.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Tim King, Andrew Reynolds, Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "cvc4_private.h"

#pragma once

#include "context/cdlist.h"
#include "context/cdmaybe.h"
#include "context/cdo.h"
#include "context/cdtrail_queue.h"
#include "context/context.h"
#include "expr/proof_node_manager.h"
#include "theory/arith/arithvar.h"
#include "theory/arith/constraint_forward.h"
#include "theory/arith/partial_model.h"
#include "theory/eager_proof_generator.h"
#include "theory/ee_setup_info.h"
#include "theory/trust_node.h"
#include "theory/uf/equality_engine.h"
#include "theory/uf/proof_equality_engine.h"
#include "util/dense_map.h"
#include "util/statistics_registry.h"

namespace CVC4 {
namespace theory {
namespace arith {

class ArithCongruenceManager {
private:
  context::CDRaised d_inConflict;
  RaiseEqualityEngineConflict d_raiseConflict;

  /**
   * The set of ArithVars equivalent to a pair of terms.
   * If this is 0 or cannot be 0, this can be signalled.
   * The pair of terms for the congruence is stored in watched equalities.
   */
  DenseSet d_watchedVariables;
  /** d_watchedVariables |-> (= x y) */
  ArithVarToNodeMap d_watchedEqualities;


  class ArithCongruenceNotify : public eq::EqualityEngineNotify {
  private:
    ArithCongruenceManager& d_acm;
  public:
    ArithCongruenceNotify(ArithCongruenceManager& acm);

    bool eqNotifyTriggerPredicate(TNode predicate, bool value) override;

    bool eqNotifyTriggerTermEquality(TheoryId tag,
                                     TNode t1,
                                     TNode t2,
                                     bool value) override;

    void eqNotifyConstantTermMerge(TNode t1, TNode t2) override;
    void eqNotifyNewClass(TNode t) override;
    void eqNotifyMerge(TNode t1, TNode t2) override;
    void eqNotifyDisequal(TNode t1, TNode t2, TNode reason) override;
  };
  ArithCongruenceNotify d_notify;

  context::CDList<Node> d_keepAlive;

  /** Store the propagations. */
  context::CDTrailQueue<Node> d_propagatations;

  /* This maps the node a theory engine will request on an explain call to
   * to its corresponding PropUnit.
   * This is node is potentially both the propagation or
   * Rewriter::rewrite(propagation).
   */
  typedef context::CDHashMap<Node, size_t, NodeHashFunction> ExplainMap;
  ExplainMap d_explanationMap;

  ConstraintDatabase& d_constraintDatabase;
  SetupLiteralCallBack d_setupLiteral;

  const ArithVariables& d_avariables;

  /** The equality engine being used by this class */
  eq::EqualityEngine* d_ee;
  /** The sat context */
  context::Context* d_satContext;
  /** The user context */
  context::UserContext* d_userContext;

  /** proof manager */
  ProofNodeManager* d_pnm;
  /** A proof generator for storing proofs of facts that are asserted to the EQ
   * engine. Note that these proofs **are not closed**, and assume the
   * explanation of these facts. This is why this generator is separate from the
   * TheoryArithPrivate generator, which stores closed proofs.
   */
  std::unique_ptr<EagerProofGenerator> d_pfGenEe;
  /** A proof generator for TrustNodes sent to TheoryArithPrivate.
   *
   * When TheoryArithPrivate requests an explanation from
   * ArithCongruenceManager, it can request a TrustNode for that explanation.
   * This proof generator is the one used in that TrustNode: it stores the
   * (closed) proofs of implications proved by the
   * ArithCongruenceManager/EqualityEngine.
   *
   * It is insufficient to just use the ProofGenerator from the ProofEqEngine,
   * since sometimes the ArithCongruenceManager needs to add some
   * arith-specific reasoning to extend the proof (e.g. rewriting the result
   * into a normal form).
   * */
  std::unique_ptr<EagerProofGenerator> d_pfGenExplain;

  /** Pointer to the proof equality engine of TheoryArith */
  theory::eq::ProofEqEngine* d_pfee;


  void raiseConflict(Node conflict);
  /**
   * Are proofs enabled? This is true if a non-null proof manager was provided
   * to the constructor of this class.
   */
  bool isProofEnabled() const;

 public:
  bool inConflict() const;

  bool hasMorePropagations() const;

  const Node getNextPropagation();

  bool canExplain(TNode n) const;

private:
  Node externalToInternal(TNode n) const;

  void pushBack(TNode n);

  void pushBack(TNode n, TNode r);

  void pushBack(TNode n, TNode r, TNode w);

  bool propagate(TNode x);
  void explain(TNode literal, std::vector<TNode>& assumptions);


  /** This sends a shared term to the uninterpreted equality engine. */
  void assertionToEqualityEngine(bool eq, ArithVar s, TNode reason);

  /** Dequeues the delay queue and asserts these equalities.*/
  void enableSharedTerms();
  void dequeueLiterals();

  void enqueueIntoNB(const std::set<TNode> all, NodeBuilder<>& nb);

  Node explainInternal(TNode internal);
public:

 public:
  ArithCongruenceManager(context::Context* satContext,
                         context::UserContext* u,
                         ConstraintDatabase&,
                         SetupLiteralCallBack,
                         const ArithVariables&,
                         RaiseEqualityEngineConflict raiseConflict,
                         ProofNodeManager* pnm);
  ~ArithCongruenceManager();

  //--------------------------------- initialization
  /**
   * Returns true if we need an equality engine, see
   * Theory::needsEqualityEngine.
   */
  bool needsEqualityEngine(EeSetupInfo& esi);
  /**
   * Finish initialize. This class is instructed by TheoryArithPrivate to use
   * the equality engine ee and proof equality engine pfee.
   */
  void finishInit(eq::EqualityEngine* ee, eq::ProofEqEngine* pfee);
  //--------------------------------- end initialization

  TrustNode explain(TNode literal);
  void explain(TNode lit, NodeBuilder<>& out);

  void addWatchedPair(ArithVar s, TNode x, TNode y);

  inline bool isWatchedVariable(ArithVar s) const {
    return d_watchedVariables.isMember(s);
  }

  /** Assert an equality. */
  void watchedVariableIsZero(ConstraintCP eq);

  /** Assert a conjunction from lb and ub. */
  void watchedVariableIsZero(ConstraintCP lb, ConstraintCP ub);

  /** Assert that the value cannot be zero. */
  void watchedVariableCannotBeZero(ConstraintCP c);

  /** Assert that the value cannot be zero. */
  void watchedVariableCannotBeZero(ConstraintCP c, ConstraintCP d);


  /** Assert that the value is congruent to a constant. */
  void equalsConstant(ConstraintCP eq);
  void equalsConstant(ConstraintCP lb, ConstraintCP ub);


  void addSharedTerm(Node x);

 private:
  class Statistics {
  public:
    IntStat d_watchedVariables;
    IntStat d_watchedVariableIsZero;
    IntStat d_watchedVariableIsNotZero;

    IntStat d_equalsConstantCalls;

    IntStat d_propagations;
    IntStat d_propagateConstraints;
    IntStat d_conflicts;

    Statistics();
    ~Statistics();
  } d_statistics;

};/* class ArithCongruenceManager */

std::vector<Node> andComponents(TNode an);

}/* CVC4::theory::arith namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */
