/******************************************************************************
 * Top contributors (to current version):
 *   Andrew Reynolds
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Preprocess equality rewriter for arithmetic
 */

#include "cvc5_private.h"

#ifndef CVC5__THEORY__ARITH__PP_REWRITE_EQ__H
#define CVC5__THEORY__ARITH__PP_REWRITE_EQ__H

#include "proof/eager_proof_generator.h"
#include "expr/node.h"
#include "context/context.h"

namespace cvc5 {
namespace theory {
namespace arith {

class PreprocessRewriteEq
{
 public:
  PreprocessRewrite(context::Context* c,
                        ProofNodeManager * pnm);
  ~PreprocessRewrite() {}
  /**
   * Preprocess equality, applies ppRewrite for equalities. This method is
   * distinct from ppRewrite since it is not allowed to construct lemmas.
   */
  TrustNode ppRewriteEq(TNode eq);
private:
  /** Are proofs enabled? */
  bool proofsEnabled() const;
  /** Timer */
  TimerStat d_ppRewriteTimer;
  /** Used to prove pp-rewrites */
  EagerProofGenerator d_ppPfGen;
  /** Proof node manager */
  ProofNodeManager * d_pnm;
};

}  // namespace arith
}  // namespace theory
}  // namespace cvc5

#endif
