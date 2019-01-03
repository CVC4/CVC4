/*********************                                                        */
/*! \file bv_eager_solver.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Liana Hadarean, Mathias Preiner, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Eager bit-blasting solver.
 **
 ** Eager bit-blasting solver.
 **/

#include "theory/bv/bv_eager_solver.h"

#include "options/bv_options.h"
#include "theory/bv/bitblast/aig_bitblaster.h"
#include "theory/bv/bitblast/eager_bitblaster.h"

using namespace std;

namespace CVC4 {
namespace theory {
namespace bv {

EagerBitblastSolver::EagerBitblastSolver(context::Context* c, TheoryBV* bv)
    : d_assertionSet(c),
      d_assumptionSet(c),
      d_context(c),
      d_bitblaster(),
      d_aigBitblaster(),
      d_useAig(options::bitvectorAig()),
      d_bv(bv),
      d_bvp(nullptr)
{
}

EagerBitblastSolver::~EagerBitblastSolver() {}

void EagerBitblastSolver::turnOffAig() {
  Assert(d_aigBitblaster == nullptr && d_bitblaster == nullptr);
  d_useAig = false;
}

void EagerBitblastSolver::initialize() {
  Assert(!isInitialized());
  if (d_useAig) {
#ifdef CVC4_USE_ABC
    d_aigBitblaster.reset(new AigBitblaster());
#else
    Unreachable();
#endif
  } else {
    d_bitblaster.reset(new EagerBitblaster(d_bv, d_context));
    THEORY_PROOF(if (d_bvp) {
      d_bitblaster->setResolutionProofLog(d_bvp);
      d_bvp->setBitblaster(d_bitblaster.get());
    });
  }
}

bool EagerBitblastSolver::isInitialized() {
  const bool init = d_aigBitblaster != nullptr || d_bitblaster != nullptr;
  Assert(!init || !d_useAig || d_aigBitblaster);
  Assert(!init || d_useAig || d_bitblaster);
  return init;
}

void EagerBitblastSolver::assertFormula(TNode formula) {
  d_bv->spendResource(1);
  Assert(isInitialized());
  Debug("bitvector-eager") << "EagerBitblastSolver::assertFormula " << formula
                           << "\n";
  if (options::incrementalSolving() && d_context->getLevel() > 1)
  {
    d_assumptionSet.insert(formula);
  }
  d_assertionSet.insert(formula);
  // ensures all atoms are bit-blasted and converted to AIG
  if (d_useAig) {
#ifdef CVC4_USE_ABC
    d_aigBitblaster->bbFormula(formula);
#else
    Unreachable();
#endif
  }
  else
  {
    d_bitblaster->bbFormula(formula);
  }
}

bool EagerBitblastSolver::checkSat() {
  Assert(isInitialized());
  if (d_assertionSet.empty()) {
    return true;
  }

  if (d_useAig) {
#ifdef CVC4_USE_ABC
    const std::vector<Node> assertions = {d_assertionSet.key_begin(),
                                          d_assertionSet.key_end()};
    Assert(!assertions.empty());

    Node query = utils::mkAnd(assertions);
    return d_aigBitblaster->solve(query);
#else
    Unreachable();
#endif
  }

  if (options::incrementalSolving())
  {
    const std::vector<Node> assumptions = {d_assumptionSet.key_begin(),
                                           d_assumptionSet.key_end()};
    return d_bitblaster->solve(assumptions);
  }
  return d_bitblaster->solve();
}

bool EagerBitblastSolver::collectModelInfo(TheoryModel* m, bool fullModel)
{
  AlwaysAssert(!d_useAig && d_bitblaster);
  return d_bitblaster->collectModelInfo(m, fullModel);
}

void EagerBitblastSolver::setResolutionProofLog(
    proof::ResolutionBitVectorProof* bvp)
{
  d_bvp = bvp;
}

}  // namespace bv
}  // namespace theory
}  // namespace CVC4
