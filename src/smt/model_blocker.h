/*********************                                                        */
/*! \file model_blocker.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Utility for blocking the current model
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__MODEL_BLOCKER_H
#define __CVC4__THEORY__MODEL_BLOCKER_H

#include <vector>

#include "expr/expr.h"
#include "options/smt_options.h"
#include "theory/theory_model.h"
#include "context/cdlist_forward.h"

namespace CVC4 {

/**
 * A utility for blocking the current model.
 */
class ModelBlocker
{
 
 typedef context::CDList<Node> NodeList;

 public:
  /** get model blocker
   *
   * This returns a disjunction of literals ~L1 V ... V ~Ln with the following
   * properties:
   * (1) L1 ... Ln hold in the current model (given by argument m),
   * (2) L1 ... Ln are literals that occur in assertions and propositionally
   * entail all non-unit top-level assertions.
   * 
   * For example, if our input is:
   *    x > 0 ^ ( y < 0 V z < 0 V w < 0 )
   * and m is { x -> 1, y -> 2, z -> -1, w -> -1 }, then this method may
   * return ~(z < 0) or ~(w < 0).
   * 
   * Notice that we do not require that L1...Ln entail unit top-level assertions
   * since these literals are trivially entailed to be true in all models of
   * our input. In other words, we do not return ~(x < 0) V ~(w < 0) since the
   * left disjunct is always false.
   */
  static Expr getModelBlocker(const std::vector<Expr>& assertions, theory::TheoryModel* m, BlockModelsMode mode, std::vector<Node> getValueNodes);
}; /* class TheoryModelCoreBuilder */

}  // namespace CVC4

#endif /* __CVC4__THEORY__MODEL_BLOCKER_H */
