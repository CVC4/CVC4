/*********************                                                        */
/*! \file bv_solver_proof.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Simple bitblast solver
 **
 ** Simple bitblast solver that sends bitblast lemmas directly to MiniSat.
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__BV__BV_SOLVER_PROOF_H
#define CVC4__THEORY__BV__BV_SOLVER_PROOF_H

#include <unordered_map>

#include "theory/bv/bv_solver.h"

namespace CVC4 {

namespace theory {
namespace bv {

class BBSimple;

class BVSolverProof : public BVSolver
{
 public:
  BVSolverProof(TheoryState& state, TheoryInferenceManager& inferMgr);
  ~BVSolverProof() = default;

  void preRegisterTerm(TNode n) override {}

  bool preNotifyFact(TNode atom,
                     bool pol,
                     TNode fact,
                     bool isPrereg,
                     bool isInternal) override;

  std::string identify() const override { return "BVSolverProof"; };

  Theory::PPAssertStatus ppAssert(TNode in,
                                  SubstitutionMap& outSubstitutions) override
  {
    return Theory::PPAssertStatus::PP_ASSERT_STATUS_UNSOLVED;
  }

  bool collectModelValues(TheoryModel* m,
                          const std::set<Node>& termSet) override;

 private:
  std::unique_ptr<BBSimple> d_bitblaster;
  /** Caches lemmas added for BV atoms. */
  std::unordered_set<Node, NodeHashFunction> d_lemmasCache;

  void addBBLemma(TNode fact);
};

}  // namespace bv
}  // namespace theory
}  // namespace CVC4

#endif
