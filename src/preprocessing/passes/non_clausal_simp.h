/*********************                                                        */
/*! \file non_clausal_simp.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Aina Niemetz
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Non-clausal simplification preprocessing pass.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__PREPROCESSING__PASSES__NON_CLAUSAL_SIMP_H
#define __CVC4__PREPROCESSING__PASSES__NON_CLAUSAL_SIMP_H

#include <vector>

#include "expr/node.h"
#include "preprocessing/preprocessing_pass.h"
#include "preprocessing/preprocessing_pass_context.h"

namespace CVC4 {
namespace preprocessing {
namespace passes {

class NonClausalSimp : public PreprocessingPass
{
 public:
  NonClausalSimp(PreprocessingPassContext* preprocContext);

 protected:
  PreprocessingPassResult applyInternal(
      AssertionPipeline* assertionsToPreprocess) override;

 private:
  struct Statistics
  {
    IntStat d_numConstantProps;
    Statistics();
    ~Statistics();
  };

  Statistics d_statistics;

  /** Learned literals */
  std::vector<Node> d_nonClausalLearnedLiterals;

  SatValue solveByCryptominisat(CVC4::prop::TseitinCnfStream* d_cnfStream,
                                AssertionPipeline* assertionsToPreprocess,
                                unsigned substs_index);
  bool solveByCircuitPropagator(theory::booleans::CircuitPropagator* propagator,
                                AssertionPipeline* assertionsToPreprocess,
                                unsigned substs_index);
};

}  // namespace passes
}  // namespace preprocessing
}  // namespace CVC4

#endif
