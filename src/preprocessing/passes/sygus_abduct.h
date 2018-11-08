/*********************                                                        */
/*! \file sygus_abduct.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief SygusAbduct
 **/

#ifndef __CVC4__PREPROCESSING__PASSES__SYGUS_ABDUCT_H_
#define __CVC4__PREPROCESSING__PASSES__SYGUS_ABDUCT_H_

#include <map>
#include <string>
#include <vector>
#include "expr/node.h"

#include "preprocessing/preprocessing_pass.h"
#include "preprocessing/preprocessing_pass_context.h"

namespace CVC4 {
namespace preprocessing {
namespace passes {

/** SygusAbduct
 *
 * A preprocessing utility that turns a set of quantifier-free assertions into
 * a sygus conjecture that encodes an abduction problem. In detail, if our
 * input formula is F( x ) for free symbols x, then we construct the sygus
 * conjecture:
 *
 * exists A. forall x. ( A( x ) => ~F( x ) )
 *
 * where A( x ) is a predicate over the free symbols of our input. In other
 * words, A( x ) is a sufficient condition for showing ~F( x ).
 *
 * Another way to view this is A( x ) is any condition such that A( x ) ^ F( x )
 * is unsatisfiable.
 *
 * A common use case is to find the weakest such A that meets the above
 * specification. We do this by streaming solutions (sygus-stream) for A
 * while filtering stronger solutions (sygus-filter-sol=strong). These options
 * are enabled by default when this preprocessing class is used (sygus-abduct).
 *
 * If the input F( x ) is partitioned into assumptions and negated conjecture
 * Fa( x ) ^ Fc( x ), then the sygus conjecture we construct is:
 *
 * exists A. ( exists y. A( y ) ^ Fa( y ) ) ^ forall x. ( A( x ) => ~F( x ) )
 *
 * In other words, A( y ) must be consistent with our assumptions and imply
 * ~F( x ). We encode this conjecture using SygusSideConditionAttribute.
 */
class SygusAbduct : public PreprocessingPass
{
 public:
  SygusAbduct(PreprocessingPassContext* preprocContext);

 protected:
  /**
   * Replaces the set of assertions by an abduction sygus problem described
   * above.
   */
  PreprocessingPassResult applyInternal(
      AssertionPipeline* assertionsToPreprocess) override;
};

}  // namespace passes
}  // namespace preprocessing
}  // namespace CVC4

#endif /* __CVC4__PREPROCESSING__PASSES__SYGUS_ABDUCT_H_ */
