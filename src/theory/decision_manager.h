/*********************                                                        */
/*! \file decision_manager.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief decision_manager
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__DECISION_MANAGER__H
#define __CVC4__THEORY__DECISION_MANAGER__H

#include <map>
#include "context/cdo.h"
#include "expr/node.h"
#include "theory/valuation.h"

namespace CVC4 {
namespace theory {

/**
 * Virtual base class for decision strategies.
 */
class DecisionStrategy
{
 public:
  DecisionStrategy() {}
  virtual ~DecisionStrategy() {}
  /** 
   * Initalize this strategy, This is called once per satisfiability call by 
   * the DecisionManager, prior to using this strategy.
   */
  virtual void initialize() = 0;
  /** 
   * If this method returns a non-null node n, then n is the required next
   * decision of this strategy. It must be the case that n is a literal in
   * the current CNF stream.
   */
  virtual Node getNextDecisionRequest() = 0;
  /** identify this strategy (for debugging) */
  virtual std::string identify() const = 0;
};

/**
 * Decision strategy finite model finding.
 *
 * This is a virtual base class for decision strategies that follow the
 * "finite model finding" pattern for decisions. Such strategies have the
 * distinguishing feature that they are interested in a stream of literals
 * L_1, L_2, L_3, ... and so on. At any given time, they request that L_i is
 * asserted true for a minimal i.
 *
 * To enforce this strategy, this class maintains a SAT-context dependent
 * index d_curr_literal, which corresponds to the
 */
class DecisionStrategyFmf : public DecisionStrategy
{
 public:
  DecisionStrategyFmf(context::Context* satContext, Valuation valuation);
  virtual ~DecisionStrategyFmf() {}
  /** initialize */
  void initialize() override;
  /** get next decision request */
  Node getNextDecisionRequest() override;
  /** are we currently ready to make the decision? */
  virtual bool isReadyForDecision();
  /** Make the n^th literal of this strategy */
  virtual Node mkLiteral(unsigned n) = 0;
  /**
   * This method returns true iff there exists a (minimal) i such that L_i
   * is a literal allocated by this class, and is asserted true in the current
   * context. If it returns true, the argument i is set to this i.
   */
  bool getAssertedLiteralIndex(unsigned& i);
  /**
   * This method returns the literal L_i allocated by this class that
   * has been asserted true in the current context and is such that i is
   * minimal. It returns null if no such literals exist.
   */
  Node getAssertedLiteral();
  /** Get the n^th literal of this strategy */
  Node getLiteral(unsigned lit);

 private:
  /**
   * The valuation of this class, used for knowing what literals are asserted,
   * and with what polarity.
   */
  Valuation d_valuation;
  /**
   * The (SAT-context-dependent) flag that is true if there exists a literal
   * of this class that is asserted true in the current context, according to
   * d_valuation.
   */
  context::CDO<bool> d_has_curr_literal;
  /**
   * The (SAT-context-dependent) index of the current literal of this strategy.
   * This corresponds to the first literal that is not asserted false in the
   * current context, according to d_valuation.
   */
  context::CDO<unsigned> d_curr_literal;
  /** the list of literals of this strategy */
  std::vector<Node> d_literals;
};

/**
 * Special case of above where we only wish to allocate a single literal L_1.
 */
class DecisionStrategySingleton : public DecisionStrategyFmf
{
 public:
  DecisionStrategySingleton(const char* name,
                            Node lit,
                            context::Context* satContext,
                            Valuation valuation);
  /**
   * Make the n^th literal of this strategy. This method returns mkLiteral if
   * n=0, null otherwise.
   */
  Node mkLiteral(unsigned n) override;
  /** Get single literal */
  Node getSingleLiteral();
  /** identify */
  std::string identify() const override { return d_name; }

 private:
  /** the name of this strategy */
  std::string d_name;
  /** the literal to decide on */
  Node d_literal;
};

/** DecisionManager
 *
 * This class manages all "decision strategies" for theory solvers in
 * TheoryEngine. A decision strategy is a callback in the SAT solver for
 * imposing its next decision. This is useful, for instance, in
 * branch-and-bound algorithms where we require that the first decision
 * is a bound on some quantity (for instance, a bound on the cardinality
 * of an uninterpreted sort, for finite model finding).
 * 
 * This class maintains a user-context-dependent set of pointers to
 * DecisionStrategy objects, which implement indivdual decision strategies.
 * 
 * Decision strategies may be registered to this class via registerStrategy
 * at any time during solving. They are cleared via a call to reset during
 * TheoryEngine's postSolve method.
 * 
 * Decision strategies have a fixed order, which is managed by the enumeration
 * type StrategyId, where strategies with smaller id have higher precedence
 * in our global decision strategy.
 */
class DecisionManager
{
 public:
  enum StrategyId
  {
    // The order of the global decision strategy used by the TheoryEngine
    // for getNextDecision.

    //----- assume-feasibile strategies
    //  These are required to go first for the sake of model-soundness. In
    //  other words, if these strategies did not go first, we might answer
    //  "sat" for problems that are unsat.
    strat_quant_cegqi_feasible,
    strat_quant_sygus_feasible,
    strat_quant_sygus_stream_feasible,
    strat_last_m_sound,

    //----- finite model finding strategies
    //  We require these go here for the sake of finite-model completeness. In
    //  other words, if these strategies did not go before other decisions, we
    //  might be non-terminating instead of answering "sat" with a solution
    //  within a given a bound.
    strat_uf_combined_card,
    strat_uf_card,
    strat_dt_sygus_enum_active,
    strat_dt_sygus_enum_size,
    strat_quant_bound_int_size,
    strat_quant_cegis_unif_num_enums,
    strat_strings_sum_lengths,
    strat_sep_neg_guard,
    strat_last_fm_complete,

    //----- decision strategies that are optimizations
    strat_arrays,

    strat_last
  };
  DecisionManager(context::Context* satContext);
  ~DecisionManager() {}
  /** reset the strategy
   *
   * This clears all decision strategies that are registered to this manager.
   * We require that each satisfiability check beyond the first calls this
   * function exactly once. Currently, it is called during
   * TheoryEngine::postSolve.
   */
  void reset();
  /**
   * Registers the strategy ds with this manager. The id specifies when the
   * strategy should be run.
   */
  void registerStrategy(StrategyId id,
                        DecisionStrategy* ds,
                        bool append = true);
  /** Get the next decision request 
   * 
   * If this method returns a non-null node n, then n is a literal corresponding
   * to the next decision that the SAT solver should take. If this method 
   * returns null, then no decisions are required by a decision strategy
   * registered to this class. In the latter case, the SAT solver will choose
   * a decision based on its given heuristic.
   */
  Node getNextDecisionRequest(unsigned& priorty);

 private:
  /** Map containing all strategies registered to this manager */
  std::map<StrategyId, std::vector<DecisionStrategy*> > d_reg_strategy;
  // std::vector<DecisionStrategy*> d_strategy;
  // context::CDO<unsigned> d_curr_strategy;
};

}  // namespace theory
}  // namespace CVC4

#endif /* __CVC4__THEORY__DECISION_MANAGER__H */
