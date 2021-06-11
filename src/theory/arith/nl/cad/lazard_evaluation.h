/******************************************************************************
 * Top contributors (to current version):
 *   Gereon Kremer
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Implements the CDCAC approach as described in
 * https://arxiv.org/pdf/2003.05633.pdf.
 */

#include "cvc5_private.h"

#ifndef CVC5__THEORY__ARITH__NL__CAD__LAZARD_EVALUATION_H
#define CVC5__THEORY__ARITH__NL__CAD__LAZARD_EVALUATION_H

#include <poly/polyxx.h>

#include <memory>

namespace cvc5::theory::arith::nl::cad {

struct LazardEvaluationState;
/**
 * This class does the heavy lifting for the modified lifting procedure that is
 * required for Lazard's projection.
 *
 * Let p \in Q[x1, ..., xn] a multivariate polynomial whose roots we want to
 * isolate over the partial sample point A = {x1 = a1, ... xn-1 = an-1} where
 * a1, ... an-1 are real algebraic numbers and xn is the last free variable.
 *
 * The modified lifting procedure conceptually works as follows:
 *
 * for (x = a) in A:
 *    while p[x // a] = 0:
 *       p = p / (x - a)
 *    p = p[x // a]
 * return roots(p)
 *
 * As the assignment contains real algebraic numbers, though, we can not do any
 * of the computations directly, as our polynomials only support coefficients
 * from Z or Q, but not extensions (in the algebraic sense) thereof.
 *
 * Our approach is as follows:
 * Let pk be the minimal polynomial for ak.
 * Instead of substituting p[xk // ak] we (canonically) embed p into the
 * quotient ring Q[xk]/<pk> and recursively build a tower of such quotient rings
 * that is isomorphic to nesting the corresponding field extensions Q(a1)(a2)...
 * When we have done that, we obtain p that is reduced with respect to all
 * minimal polynomials, but may still contain x1,... xn-1. To get rid of these,
 * we compute a Gröbner basis of p and the minimal polynomials (using a suitable
 * elimination order) and extract the polynomial in xn. This polynomial has all
 * roots (and possibly some more) that we are looking for. Instead of a Gröbner
 * basis, we can also compute the iterated resultant as follows: Res(Res(p,
 * pn-1, xn-1), pn-2, xn-2)...
 *
 * Consider
 * http://sunsite.informatik.rwth-aachen.de/Publications/AIB/2020/2020-04.pdf
 * Section 2.5.1 for a full discussion.
 */
class LazardEvaluation
{
 public:
  LazardEvaluation();
  ~LazardEvaluation();

  /**
   * Add the next assigned variable var -> ran to this construction.
   */
  void add(const poly::Variable& var, const poly::Value& val);
  /**
   * Finish by adding the free variable var.
   */
  void addFreeVariable(const poly::Variable& var);
  /**
   * Reduce the polynomial q.
   */
  std::vector<poly::Polynomial> reducePolynomial(
      const poly::Polynomial& q) const;

  std::vector<poly::Interval> infeasibleRegions(const poly::Polynomial& q,
                                                poly::SignCondition sc) const;

 private:
  std::unique_ptr<LazardEvaluationState> d_state;
};

}  // namespace cvc5::theory::arith::nl::cad

#endif