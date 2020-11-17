/*********************                                                        */
/*! \file sine_solver.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Solving for handling exponential function.
 **/

#ifndef CVC4__THEORY__ARITH__NL__TRANSCENDENTAL__SINE_SOLVER_H
#define CVC4__THEORY__ARITH__NL__TRANSCENDENTAL__SINE_SOLVER_H

#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "expr/node.h"
#include "theory/arith/inference_manager.h"
#include "theory/arith/nl/nl_model.h"
#include "theory/arith/nl/transcendental/transcendental_state.h"

namespace CVC4 {
namespace theory {
namespace arith {
namespace nl {
namespace transcendental {

/** Transcendental solver class
 *
 * This class implements model-based refinement schemes
 * for transcendental functions, described in:
 *
 * - "Satisfiability Modulo Transcendental
 * Functions via Incremental Linearization" by Cimatti
 * et al., CADE 2017.
 *
 * It's main functionality are methods that implement lemma schemas below,
 * which return a set of lemmas that should be sent on the output channel.
 */
class SineSolver
{
 public:
  SineSolver(TranscendentalState* tstate);
  ~SineSolver();

  void initLastCall(const std::vector<Node>& assertions,
                    const std::vector<Node>& false_asserts,
                    const std::vector<Node>& xts);

  /**
   * check initial refine
   *
   * Constructs a set of valid theory lemmas, based on
   * simple facts about the sine function.
   * This mostly follows the initial axioms described in
   * Section 4 of "Satisfiability
   * Modulo Transcendental Functions via Incremental
   * Linearization" by Cimatti et al., CADE 2017.
   *
   * Examples:
   *
   * sin( x ) = -sin( -x )
   * ( PI > x > 0 ) => 0 < sin( x ) < 1
   */
  void checkInitialRefine();

  /**
   * check monotonicity
   *
   * Constructs a set of valid theory lemmas, based on a
   * lemma scheme that ensures that applications
   * of the sine function respect monotonicity.
   *
   * Examples:
   *
   * PI/2 > x > y > 0 => sin( x ) > sin( y )
   * PI > x > y > PI/2 => sin( x ) < sin( y )
   */
  void checkMonotonic();

  void mkTangentLemma(TNode e, TNode c, TNode poly_approx, int region);

/** region to lower bound
   *
   * Returns the term corresponding to the lower
   * bound of the region of transcendental function
   * with kind k. Returns Node::null if the region
   * is invalid, or there is no lower bound for the
   * region.
   */
  Node regionToLowerBound(int region)
  {
    switch (region)
    {
      case 1: return d_data->d_pi_2;
      case 2: return d_data->d_zero;
      case 3: return d_data->d_pi_neg_2;
      case 4: return d_data->d_pi_neg;
      default: return Node();
    }
  }

/** region to upper bound
   *
   * Returns the term corresponding to the upper
   * bound of the region of transcendental function
   * with kind k. Returns Node::null if the region
   * is invalid, or there is no upper bound for the
   * region.
   */
  Node regionToUpperBound(int region)
  {
    switch (region)
    {
      case 1: return d_data->d_pi;
      case 2: return d_data->d_pi_2;
      case 3: return d_data->d_zero;
      case 4: return d_data->d_pi_neg_2;
      default: return Node();
    }
  }
 private:
  int regionToMonotonicityDir(int region)
  {
    switch (region)
    {
      case 1:
      case 4: return -1;
      case 2:
      case 3: return 1;
      default: return 0;
    }
  }
  int regionToConcavity(int region)
  {
    switch (region)
    {
      case 1:
      case 2: return -1;
      case 3:
      case 4: return 1;
      default: return 0;
    }
  }


  TranscendentalState* d_data;

  /** The transcendental functions we have done initial refinements on */
  std::map<Node, bool> d_tf_initial_refine;

}; /* class SineSolver */

}  // namespace transcendental
}  // namespace nl
}  // namespace arith
}  // namespace theory
}  // namespace CVC4

#endif /* CVC4__THEORY__ARITH__TRANSCENDENTAL_SOLVER_H */
