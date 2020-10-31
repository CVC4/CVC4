/*********************                                                        */
/*! \file proof_checker.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Bit-Vector proof checker utility
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__BV__PROOF_CHECKER_H
#define CVC4__THEORY__BV__PROOF_CHECKER_H

#include "expr/node.h"
#include "expr/proof_checker.h"
#include "expr/proof_node.h"

namespace CVC4 {
namespace theory {
namespace bv {

class BBSimple;

/** A checker for array reasoning in proofs */
class BVProofRuleChecker : public ProofRuleChecker
{
 public:
  BVProofRuleChecker();
  ~BVProofRuleChecker() {}

  /** Register all rules owned by this rule checker into pc. */
  void registerTo(ProofChecker* pc) override;

 protected:
  /** Return the conclusion of the given proof step, or null if it is invalid */
  Node checkInternal(PfRule id,
                     const std::vector<Node>& children,
                     const std::vector<Node>& args) override;

 private:
  std::unique_ptr<BBSimple> d_bb;
};

}  // namespace bv
}  // namespace theory
}  // namespace CVC4

#endif /* CVC4__THEORY__BV__PROOF_CHECKER_H */
