/*********************                                                        */
/*! \file theory_bags_type_rules_black.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Mudathir Mohamed
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Black box testing of bags typing rules
 **/

#include <cxxtest/TestSuite.h>

#include "api/cvc4cpp.h"
#include "expr/dtype.h"
#include "smt/smt_engine.h"

using namespace CVC4;
using namespace CVC4::api;

class SetsTypeRuleWhite : public CxxTest::TestSuite
{
 public:
  void setUp() override
  {
    d_slv.reset(new Solver());
    d_em.reset(new ExprManager());
    d_smt.reset(new SmtEngine(d_em.get()));
    d_nm.reset(NodeManager::fromExprManager(d_em.get()));
    d_smt->finishInit();
  }

  void tearDown() override
  {
    d_slv.reset();
    d_smt.reset();
    d_nm.release();
    d_em.reset();
  }

  void testSingletonTerm()
  {
    Sort setSort = d_slv->mkSetSort(d_slv->getRealSort());
    Term emptyReal = d_slv->mkEmptySet(setSort);
    Term singletonInt = d_slv->mkTerm(SINGLETON, d_slv->mkReal(1));

    Term singletonReal = d_slv->mkTerm(SINGLETON, d_slv->mkReal(1, 5));
    // (union
    //    (singleton (singleton_type Int) 1)
    //    (as emptyset (Set Real)))
    TS_ASSERT_THROWS(d_slv->mkTerm(UNION, singletonInt, emptyReal),
                     CVC4ApiException);
    // (union
    //    (singleton (singleton_type Real) (/ 1 5))
    //    (as emptyset (Set Real)))
    TS_ASSERT_THROWS_NOTHING(d_slv->mkTerm(UNION, singletonReal, emptyReal));
  }

  void testSingletonNode()
  {
    Node singletonInt = d_nm->mkConst(SingletonType(d_nm->integerType()));
    Node singletonReal = d_nm->mkConst(SingletonType(d_nm->realType()));
    Node intConstant = d_nm->mkConst(Rational(1));
    Node realConstant = d_nm->mkConst(Rational(1, 5));
    // (singleton (singleton_type Real) 1)
    TS_ASSERT_THROWS_NOTHING(
        d_nm->mkNode(kind::SINGLETON, singletonReal, intConstant));
    // (singleton (singleton_type Int) (/ 1 5))
    TS_ASSERT_THROWS(d_nm->mkNode(kind::SINGLETON, singletonInt, realConstant),
                     Exception);

    Node n = d_nm->mkNode(kind::SINGLETON, singletonReal, intConstant);
    // the type of (singleton (singleton_type Real) 1) is (Set Real)
    TS_ASSERT(n.getType() == d_nm->mkSetType(d_nm->realType()));
    // (singleton (singleton_type Real) 1) is a constant in normal form
    TS_ASSERT(n.isConst());
  }

 private:
  std::unique_ptr<Solver> d_slv;
  std::unique_ptr<ExprManager> d_em;
  std::unique_ptr<SmtEngine> d_smt;
  std::unique_ptr<NodeManager> d_nm;
}; /* class SetsTypeRuleWhite */
