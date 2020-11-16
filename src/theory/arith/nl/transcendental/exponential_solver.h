/*********************                                                        */
/*! \file exponential_solver.h
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

#ifndef CVC4__THEORY__ARITH__NL__TRANSCENDENTAL__EXPONENTIAL_SOLVER_H
#define CVC4__THEORY__ARITH__NL__TRANSCENDENTAL__EXPONENTIAL_SOLVER_H

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
class ExponentialSolver
{
 public:
  ExponentialSolver(TranscendentalState* tstate);
  ~ExponentialSolver();

  void initLastCall(const std::vector<Node>& assertions,
                                        const std::vector<Node>& false_asserts,
                                        const std::vector<Node>& xts);

  void checkInitialRefine();

 private:

 TranscendentalState* d_data;

 /** The transcendental functions we have done initial refinements on */
  std::map<Node, bool> d_tf_initial_refine;


}; /* class ExponentialSolver */

}
}  // namespace nl
}  // namespace arith
}  // namespace theory
}  // namespace CVC4

#endif /* CVC4__THEORY__ARITH__TRANSCENDENTAL_SOLVER_H */
