/*********************                                                        */
/*! \file bv_solver_bitblast.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Bit-blasting solver
 **
 ** Bit-blasting solver that supports multiple SAT backends.
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__BV__BV_SOLVER_BITBLAST_H
#define CVC4__THEORY__BV__BV_SOLVER_BITBLAST_H

#include <unordered_map>

#include "prop/cnf_stream.h"
#include "prop/sat_solver.h"
#include "theory/bv/bitblast/simple_bitblaster.h"
#include "theory/bv/bv_solver.h"
#include "theory/bv/proof_checker.h"
#include "theory/eager_proof_generator.h"

namespace CVC4 {

namespace theory {
namespace bv {

class BVSolverBitblast : public BVSolver
{
 public:
  BVSolverBitblast(TheoryState* state,
                   TheoryInferenceManager& inferMgr,
                   ProofNodeManager* pnm);
  ~BVSolverBitblast() = default;

  bool needsEqualityEngine(EeSetupInfo& esi) override { return true; }

  void preRegisterTerm(TNode n) override {}

  void postCheck(Theory::Effort level) override;

  bool preNotifyFact(TNode atom,
                     bool pol,
                     TNode fact,
                     bool isPrereg,
                     bool isInternal) override;

  std::string identify() const override { return "BVSolverBitblast"; };

  Theory::PPAssertStatus ppAssert(
      TrustNode in, TrustSubstitutionMap& outSubstitutions) override
  {
    return Theory::PPAssertStatus::PP_ASSERT_STATUS_UNSOLVED;
  }

  bool collectModelValues(TheoryModel* m,
                          const std::set<Node>& termSet) override;

 private:
  Node getValueFromSatSolver(TNode node);

  /** Bit-blaster used to bit-blast atoms/terms. */
  std::unique_ptr<BBSimple> d_bitblaster;

  std::unique_ptr<prop::NullRegistrar> d_nullRegistrar;
  std::unique_ptr<context::Context> d_nullContext;
  std::unique_ptr<prop::SatSolver> d_satSolver;
  std::unique_ptr<prop::CnfStream> d_cnfStream;

  context::CDList<Node> d_facts;

  /** Proof generator that manages proofs for lemmas generated by this class. */
  std::unique_ptr<EagerProofGenerator> d_epg;

  BVProofRuleChecker d_bvProofChecker;
};

}  // namespace bv
}  // namespace theory
}  // namespace CVC4

#endif
