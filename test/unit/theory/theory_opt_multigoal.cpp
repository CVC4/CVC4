/******************************************************************************
 * Top contributors (to current version):
 *   Yancheng Ou
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * White-box testing for optimization module for BitVectors.
 */
#include <iostream>

#include "smt/optimization_solver.h"
#include "test_smt.h"

namespace cvc5 {

using namespace theory;
using namespace smt;

namespace test {

class TestTheoryOptMultigoal : public TestSmtNoFinishInit
{
 protected:
  void SetUp() override
  {
    TestSmtNoFinishInit::SetUp();
    d_smtEngine->setOption("produce-assertions", "true");
    d_smtEngine->finishInit();

    d_optslv.reset(new OptimizationSolver(d_smtEngine.get()));
    d_BV32Type.reset(new TypeNode(d_nodeManager->mkBitVectorType(32u)));
    d_BV16Type.reset(new TypeNode(d_nodeManager->mkBitVectorType(16u)));
  }

  std::unique_ptr<OptimizationSolver> d_optslv;
  std::unique_ptr<TypeNode> d_BV32Type;
  std::unique_ptr<TypeNode> d_BV16Type;
};

TEST_F(TestTheoryOptMultigoal, box)
{
  d_smtEngine->resetAssertions();
  Node x = d_nodeManager->mkVar(*d_BV32Type);
  Node y = d_nodeManager->mkVar(*d_BV32Type);
  Node z = d_nodeManager->mkVar(*d_BV32Type);

  // 18 <= x
  d_smtEngine->assertFormula(d_nodeManager->mkNode(
      kind::BITVECTOR_ULE, d_nodeManager->mkConst(BitVector(32u, 18u)), x));

  // y <= x
  d_smtEngine->assertFormula(d_nodeManager->mkNode(kind::BITVECTOR_SLE, y, x));

  // Box optimization
  OptimizationSolver optSolver(d_smtEngine.get());

  optSolver.setObjectiveCombination(OptimizationSolver::BOX);

  // minimize x
  optSolver.pushObjective(x, OptimizationObjective::MINIMIZE, false);
  // maximize y
  optSolver.pushObjective(y, OptimizationObjective::MAXIMIZE, true);
  // maximize z
  optSolver.pushObjective(z, OptimizationObjective::MAXIMIZE, false);

  OptimizationResult::ResultType r = optSolver.checkOpt();

  ASSERT_EQ(r, OptimizationResult::OPTIMAL);

  std::vector<OptimizationResult> results = optSolver.getValues();

  // x == 18
  ASSERT_EQ(results[0].getValue().getConst<BitVector>(), BitVector(32u, 18u));

  // y == 0x7FFFFFFF
  ASSERT_EQ(results[1].getValue().getConst<BitVector>(),
            BitVector(32u, (unsigned)0x7FFFFFFF));

  // z == 0xFFFFFFFF
  ASSERT_EQ(results[2].getValue().getConst<BitVector>(),
            BitVector(32u, (unsigned)0xFFFFFFFF));
}


}  // namespace test
}  // namespace cvc5
