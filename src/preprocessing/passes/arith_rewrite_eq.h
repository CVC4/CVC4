/*********************                                                        */
/*! \file theory_preprocess.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The ArithRewriteEq preprocessing pass
 **/

#include "cvc4_private.h"

#ifndef CVC4__PREPROCESSING__PASSES__ARITH_REWRITE_EQ_H
#define CVC4__PREPROCESSING__PASSES__ARITH_REWRITE_EQ_H

#include "preprocessing/preprocessing_pass.h"
#include "preprocessing/preprocessing_pass_context.h"
#include "theory/trust_node.h"

namespace CVC4 {
namespace preprocessing {
namespace passes {

  /**
   * Implements the preprocessing pass for options::arithRewriteEq() which
   * replaces arithmetic equalities with a conjunction of inequalities, as
   * described below.
   */
class ArithRewriteEq : public PreprocessingPass
{
 public:
  ArithRewriteEq(PreprocessingPassContext* preprocContext);

 protected:
  PreprocessingPassResult applyInternal(
      AssertionPipeline* assertionsToPreprocess) override;
  /**
   * Rewrite the assertion based on removing arithmetic equalities via:
   *   (= x y) ---> (and (>= x y) (<= x y))
   * Returns the trust node corresponding to the rewrite.
   */
  theory::TrustNode rewriteAssertion(TNode assertion);
};

}  // namespace passes
}  // namespace preprocessing
}  // namespace CVC4

#endif /* CVC4__PREPROCESSING__PASSES__ARITH_REWRITE_EQ_H */
