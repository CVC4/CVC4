/*********************                                                        */
/*! \file smt_solver.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The solver for SMT queries in an SmtEngine.
 **/

#include "cvc4_private.h"

#ifndef CVC4__SMT__SMT_SOLVER_H
#define CVC4__SMT__SMT_SOLVER_H

#include <vector>

#include "expr/node.h"
#include "util/result.h"

namespace CVC4 {

class SmtEngine;
class TheoryEngine;
class ResourceManager;

namespace prop
{
  class PropEngine;
}

namespace smt {
  
class Preprocessor;
class SmtEngineStatistics;

/**
 * A solver for SMT queries.
 * 
 * This class manages the initialization of the theory engine and propositional
 * engines and implements the method for checking satisfiability of the current
 * set of assertions.
 */
class SmtSolver
{
 public:
  SmtSolver(SmtEngine& smt, smt::SmtEngineState& state, ResourceManager* rm, Preprocessor& pp, SmtEngineStatistics& stats);
  ~SmtSolver();
  /** Create theory engine, prop engine. */
  void finishInit();
  /** Reset all assertions, global declarations, etc.  */
  void resetAssertions();
  /**
   * Interrupt a running query.  This can be called from another thread
   * or from a signal handler.  Throws a ModalException if the SmtSolver
   * isn't currently in a query.
   */
  void interrupt();
  /**
   * Cleanup memory allocated by modules in this class. This is required to be
   * done explicitly so that passes are deleted before the objects they refer
   * to in the SmtEngine destructor.
   */
  void cleanup();
  /**
   * This is called by the destructor of SmtEngine, just before destroying the
   * PropEngine, TheoryEngine, and DecisionEngine (in that order).  It
   * is important because there are destruction ordering issues
   * between PropEngine and Theory.
   */
  void shutdown();
  /**
   * Check satisfiability (used to check satisfiability and entailment).
   * Returns the result of 
   */
  Result checkSatisfiability(Assertions& as,
                             const Node& assumption,
                             bool inUnsatCore,
                             bool isEntailmentCheck);
  /** Same as above, for a vector of assumptions */
  Result checkSatisfiability(Assertions& as,
                             const std::vector<Node>& assumptions,
                             bool inUnsatCore,
                             bool isEntailmentCheck);
  //------------------------------------------ access methods
  /** Get a pointer to the TheoryEngine owned by this solver. */
  TheoryEngine* getTheoryEngine() { return d_theoryEngine.get(); }
  /** Get a pointer to the PropEngine owned by this solver. */
  prop::PropEngine* getPropEngine() { return d_propEngine.get(); }
  //------------------------------------------ end access methods
 private:
  /**
   * Full check of consistency in current context.  Returns the result
   * (unsat/sat/unknown).
   */
  Result check();
  /** Reference to the parent SMT engine */
  SmtEngine& d_smt;
  /** Reference to the state of the SmtEngine */
  SmtEngineState& d_state;
  /** Pointer to a resource manager (owned by SmtEngine) */
  ResourceManager* d_rm;
  /** Reference to the preprocessor of SmtEngine */
  Preprocessor& d_pp;
  /** Reference to the statistics of SmtEngine */
  SmtEngineStatistics& d_stats;
  /** The theory engine */
  std::unique_ptr<TheoryEngine> d_theoryEngine;
  /** The propositional engine */
  std::unique_ptr<prop::PropEngine> d_propEngine;
};

}  // namespace smt
}  // namespace CVC4

#endif /* CVC4__SMT__SMT_SOLVER_H */
