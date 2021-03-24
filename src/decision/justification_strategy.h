/*********************                                                        */
/*! \file justification_strategy.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Justification strategy
 **/

#include "cvc4_private.h"

#ifndef CVC4__DECISION__JUSTIFICATION_STRATEGY_H
#define CVC4__DECISION__JUSTIFICATION_STRATEGY_H

#include "context/cdinsert_hashmap.h"
#include "context/cdo.h"
#include "decision/assertion_list.h"
#include "decision/justify_info.h"
#include "expr/node.h"
#include "prop/cnf_stream.h"
#include "prop/sat_solver.h"
#include "prop/sat_solver_types.h"

namespace CVC4 {
class JustificationStrategy
{
 public:
  /** Constructor */
  JustificationStrategy(context::Context* c, context::UserContext* u);

  /** Finish initialize */
  void finishInit(prop::CDCLTSatSolverInterface* ss, prop::CnfStream* cs);

  /** Gets the next decision based on strategies that are enabled */
  prop::SatLiteral getNext(bool& stopSearch);

  /** Is the DecisionEngine in a state where it has solved everything? */
  bool isDone();

  /**
   * Notify this class that assertion is an (input) assertion, not corresponding
   * to a skolem definition.
   */
  void addAssertion(TNode assertion);
  /**
   * Notify this class that lem is an active assertion in this SAT context. This
   * is triggered when lem is a skolem definition for skolem k, and k appears
   * in a newly asserted literal.
   */
  void notifyRelevantSkolemAssertion(TNode lem);

 private:
  /** Refresh current */
  bool refreshCurrentAssertion();
  /** Reference current assertion from list */
  bool refreshCurrentAssertionFromList(AssertionList& al);
  /** Push to stack */
  void pushToStack(TNode n, prop::SatValue desiredVal);
  /** Pop from stack */
  void popStack();
  /**
   * Get next justify node.
   *
   * Either:
   * (1) Returns the justify node corresponding to the next node to consider
   * adding to the stack
   * (2) Returns a null justify node and updates lastChildVal to the value
   * of the current node referenced by ji.
   */
  JustifyNode getNextJustifyNode(JustifyInfo* ji, prop::SatValue& lastChildVal);
  /**
   * Get or allocate justify info at position i. This does not impact
   * d_stackSizeValid.
   */
  JustifyInfo* getOrAllocJustifyInfo(size_t i);
  /**
   * Lookup value, return value of n if one can be determined.
   */
  prop::SatValue lookupValue(TNode n);
  /** Is n a theory literal? */
  static bool isTheoryLiteral(TNode n);
  /** Is n a theory atom? */
  static bool isTheoryAtom(TNode n);
  /** Pointer to the SAT context */
  context::Context* d_context;
  /** Pointer to the CNF stream */
  prop::CnfStream* d_cnfStream;
  /** Pointer to the SAT solver */
  prop::CDCLTSatSolverInterface* d_satSolver;
  /** The assertions, which are user-context dependent. */
  AssertionList d_assertions;
  /** The skolem assertions */
  AssertionList d_skolemAssertions;
  /** The current assertion we are trying to satisfy */
  context::CDO<TNode> d_current;
  /** Mapping from non-negated nodes to their SAT value */
  context::CDInsertHashMap<Node, prop::SatValue, NodeHashFunction> d_justified;
  /** Stack of justify info, valid up to index d_stackSizeValid-1 */
  context::CDList<std::shared_ptr<JustifyInfo> > d_stack;
  /** Current number of entries in the stack that are valid */
  context::CDO<size_t> d_stackSizeValid;
};

}  // namespace CVC4

#endif /* CVC4__DECISION__JUSTIFICATION_STRATEGY_H */
