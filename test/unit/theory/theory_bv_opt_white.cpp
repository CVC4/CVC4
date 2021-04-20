/******************************************************************************
 * Top contributors (to current version):
 *   Yancheng Ou, Michael Chang
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

class TestTheoryWhiteBVOpt : public TestSmtNoFinishInit
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

TEST_F(TestTheoryWhiteBVOpt, unsigned_min)
{
  Node x = d_nodeManager->mkVar(*d_BV32Type);

  Node a = d_nodeManager->mkConst(BitVector(32u, (unsigned)0x3FFFFFA1));
  Node b = d_nodeManager->mkConst(BitVector(32u, (unsigned)0xFFFFFFF1));

  // (unsigned)0x3FFFFFA1 <= x <= (unsigned)0xFFFFFFF1
  d_smtEngine->assertFormula(d_nodeManager->mkNode(kind::BITVECTOR_ULE, a, x));
  d_smtEngine->assertFormula(d_nodeManager->mkNode(kind::BITVECTOR_ULE, x, b));

  const ObjectiveType obj_type = ObjectiveType::OBJECTIVE_MINIMIZE;
  d_optslv->pushObj(x, obj_type, false);

  OptResult r = d_optslv->checkOpt();

  ASSERT_EQ(r, OptResult::OPT_OPTIMAL);

  ASSERT_EQ(d_optslv->objectiveGetValues()[0].getConst<BitVector>(),
            BitVector(32u, (unsigned)0x3FFFFFA1));
}

TEST_F(TestTheoryWhiteBVOpt, signed_min)
{
  d_smtEngine->resetAssertions();
  Node x = d_nodeManager->mkVar(*d_BV32Type);

  Node a = d_nodeManager->mkConst(BitVector(32u, (unsigned)0x80000000));
  Node b = d_nodeManager->mkConst(BitVector(32u, 2147483647u));
  // -2147483648 <= x <= 2147483647
  d_smtEngine->assertFormula(d_nodeManager->mkNode(kind::BITVECTOR_SLE, a, x));
  d_smtEngine->assertFormula(d_nodeManager->mkNode(kind::BITVECTOR_SLE, x, b));

  const ObjectiveType obj_type = ObjectiveType::OBJECTIVE_MINIMIZE;
  d_optslv->pushObj(x, obj_type, true);

  OptResult r = d_optslv->checkOpt();

  ASSERT_EQ(r, OptResult::OPT_OPTIMAL);

  BitVector val = d_optslv->objectiveGetValues()[0].getConst<BitVector>();

  // expect the minimum x = -1
  ASSERT_EQ(d_optslv->objectiveGetValues()[0].getConst<BitVector>(),
            BitVector(32u, (unsigned)0x80000000));
}

TEST_F(TestTheoryWhiteBVOpt, unsigned_max)
{
  Node x = d_nodeManager->mkVar(*d_BV32Type);

  Node a = d_nodeManager->mkConst(BitVector(32u, (unsigned)1));
  Node b = d_nodeManager->mkConst(BitVector(32u, (unsigned)2));

  // If the gap is too large, it will have a performance issue!!!
  // Need binary search!
  // (unsigned)1 <= x <= (unsigned)2
  d_smtEngine->assertFormula(d_nodeManager->mkNode(kind::BITVECTOR_ULE, a, x));
  d_smtEngine->assertFormula(d_nodeManager->mkNode(kind::BITVECTOR_ULE, x, b));

  const ObjectiveType obj_type = ObjectiveType::OBJECTIVE_MAXIMIZE;
  d_optslv->pushObj(x, obj_type, false);

  OptResult r = d_optslv->checkOpt();

  ASSERT_EQ(r, OptResult::OPT_OPTIMAL);

  BitVector val = d_optslv->objectiveGetValues()[0].getConst<BitVector>();

  ASSERT_EQ(d_optslv->objectiveGetValues()[0].getConst<BitVector>(),
            BitVector(32u, (unsigned)2));
}

TEST_F(TestTheoryWhiteBVOpt, signed_max)
{
  Node x = d_nodeManager->mkVar(*d_BV32Type);

  Node a = d_nodeManager->mkConst(BitVector(32u, (unsigned)0x80000000));
  Node b = d_nodeManager->mkConst(BitVector(32u, 10u));

  // -2147483648 <= x <= 10
  d_smtEngine->assertFormula(d_nodeManager->mkNode(kind::BITVECTOR_SLE, a, x));
  d_smtEngine->assertFormula(d_nodeManager->mkNode(kind::BITVECTOR_SLE, x, b));

  const ObjectiveType obj_type = ObjectiveType::OBJECTIVE_MAXIMIZE;
  d_optslv->pushObj(x, obj_type, true);

  OptResult r = d_optslv->checkOpt();

  ASSERT_EQ(r, OptResult::OPT_OPTIMAL);

  // expect the maxmum x =
  ASSERT_EQ(d_optslv->objectiveGetValues()[0].getConst<BitVector>(),
            BitVector(32u, 10u));
}

TEST_F(TestTheoryWhiteBVOpt, multigoal)
{
  d_smtEngine->resetAssertions();
  Node x = d_nodeManager->mkVar(*d_BV32Type);
  Node y = d_nodeManager->mkVar(*d_BV32Type);
  Node z = d_nodeManager->mkVar(*d_BV32Type);

  // 18 <= x
  d_smtEngine->assertFormula(d_nodeManager->mkNode(
      kind::BITVECTOR_ULE, d_nodeManager->mkConst(BitVector(32u, 18u)), x));

  // y <= x
  // I got wrong answer when changing ULE to SLE, what's happening???!
  d_smtEngine->assertFormula(d_nodeManager->mkNode(kind::BITVECTOR_SLE, y, x));

  // Box optimization
  OptimizationSolver optSolver(d_smtEngine.get(), ObjectiveOrder::OBJORDER_BOX);

  // minimize x
  optSolver.pushObj(x, ObjectiveType::OBJECTIVE_MINIMIZE, false);
  // maximize y
  optSolver.pushObj(y, ObjectiveType::OBJECTIVE_MAXIMIZE, true);
  // maximize z
  optSolver.pushObj(z, ObjectiveType::OBJECTIVE_MAXIMIZE, false);

  OptResult r = optSolver.checkOpt();

  ASSERT_EQ(r, OptResult::OPT_OPTIMAL);

  std::vector<Node> results = optSolver.objectiveGetValues();

  // x == 18
  ASSERT_EQ(results[0].getConst<BitVector>(), BitVector(32u, 18u));

  // y == 0x7FFFFFFF
  ASSERT_EQ(results[1].getConst<BitVector>(),
            BitVector(32u, (unsigned)0x7FFFFFFF));

  // z == 0xFFFFFFFF
  ASSERT_EQ(results[2].getConst<BitVector>(),
            BitVector(32u, (unsigned)0xFFFFFFFF));

  optSolver.setObjectiveOrder(ObjectiveOrder::OBJORDER_LEXICOGRAPHIC);
  r = optSolver.checkOpt();

  ASSERT_EQ(r, OptResult::OPT_OPTIMAL);

  results = optSolver.objectiveGetValues();

  // x == 18
  ASSERT_EQ(results[0].getConst<BitVector>(), BitVector(32u, 18u));

  // y == 18
  ASSERT_EQ(results[1].getConst<BitVector>(), BitVector(32u, 18u));

  // z == 0xFFFFFFFF
  ASSERT_EQ(results[2].getConst<BitVector>(),
            BitVector(32u, (unsigned)0xFFFFFFFF));
}

TEST_F(TestTheoryWhiteBVOpt, multigoalPareto)
{
  d_smtEngine->resetAssertions();
  TypeNode bv4ty(d_nodeManager->integerType());
  Node a = d_nodeManager->mkVar(bv4ty);
  Node b = d_nodeManager->mkVar(bv4ty);

  Node bv1 = d_nodeManager->mkConst(Rational(1));
  Node bv2 = d_nodeManager->mkConst(Rational(2));
  Node bv3 = d_nodeManager->mkConst(Rational(3));

  std::vector<Node> stmts = {
    // (and (= a 1) (= b 1))
    d_nodeManager->mkNode(kind::AND, 
      d_nodeManager->mkNode(kind::EQUAL, a, bv1),
      d_nodeManager->mkNode(kind::EQUAL, b, bv1)
    ), 
    // (and (= a 2) (= b 1))
    d_nodeManager->mkNode(kind::AND, 
      d_nodeManager->mkNode(kind::EQUAL, a, bv2),
      d_nodeManager->mkNode(kind::EQUAL, b, bv1)
    ), 
    // (and (= a 1) (= b 2))
    d_nodeManager->mkNode(kind::AND, 
      d_nodeManager->mkNode(kind::EQUAL, a, bv1),
      d_nodeManager->mkNode(kind::EQUAL, b, bv2)
    ), 
    // (and (= a 2) (= b 2))
    d_nodeManager->mkNode(kind::AND, 
      d_nodeManager->mkNode(kind::EQUAL, a, bv2),
      d_nodeManager->mkNode(kind::EQUAL, b, bv2)
    ), 
    // (and (= a 3) (= b 1))
    d_nodeManager->mkNode(kind::AND, 
      d_nodeManager->mkNode(kind::EQUAL, a, bv3),
      d_nodeManager->mkNode(kind::EQUAL, b, bv1)
    ), 
    // (and (= a 1) (= b 3))
    d_nodeManager->mkNode(kind::AND, 
      d_nodeManager->mkNode(kind::EQUAL, a, bv1),
      d_nodeManager->mkNode(kind::EQUAL, b, bv3)
    ),

    // // (and (= a 3) (= b 3))
    // d_nodeManager->mkNode(kind::AND, 
    //   d_nodeManager->mkNode(kind::EQUAL, a, bv3),
    //   d_nodeManager->mkNode(kind::EQUAL, b, bv3)
    // )
  }; 
  /*
  (assert (or
    (and (= a 1) (= b 1))
    (and (= a 2) (= b 1))
    (and (= a 1) (= b 2))
    (and (= a 2) (= b 2))
    (and (= a 3) (= b 1))
    (and (= a 1) (= b 3))
  ))
  */
  Node expr = d_nodeManager->mkConst(false);
  for (Node &s : stmts) {
    expr = d_nodeManager->mkNode(kind::OR, expr, s);
  }
  d_smtEngine->assertFormula(expr);

  /*
    (maximize a)
    (maximize b)
   */
  OptimizationSolver optSolver(d_smtEngine.get(), ObjectiveOrder::OBJORDER_PARETO);
  optSolver.pushObj(a, ObjectiveType::OBJECTIVE_MAXIMIZE);
  optSolver.pushObj(b, ObjectiveType::OBJECTIVE_MINIMIZE);

  OptResult r; 

  r = optSolver.checkOpt();
  ASSERT_EQ(r, OptResult::OPT_OPTIMAL);
  std::vector<Node> results = optSolver.objectiveGetValues(); 
  for (auto &rn : results) {
    std::cout << rn.getConst<Rational>() << " ";
  }
  std::cout << std::endl;

  // r = optSolver.checkOpt();
  // ASSERT_EQ(r, OptResult::OPT_OPTIMAL);
  // results = optSolver.objectiveGetValues(); 
  // for (auto &rn : results) {
  //   std::cout << rn.getConst<Rational>() << " ";
  // }
  // std::cout << std::endl;
  
  // r = optSolver.checkOpt();
  // ASSERT_EQ(r, OptResult::OPT_OPTIMAL);
  // results = optSolver.objectiveGetValues(); 
  // for (auto &rn : results) {
  //   std::cout << rn.getConst<Rational>() << " ";
  // }
  // std::cout << std::endl;

  // r = optSolver.checkOpt();
  // ASSERT_EQ(r, OptResult::OPT_UNSAT);

}

}  // namespace test
}  // namespace cvc5
