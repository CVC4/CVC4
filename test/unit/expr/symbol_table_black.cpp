/*********************                                                        */
/*! \file symbol_table_black.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Aina Niemetz, Christopher L. Conway, Morgan Deters
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Black box testing of CVC4::SymbolTable
 **
 ** Black box testing of CVC4::SymbolTable.
 **/

#include <cxxtest/TestSuite.h>

#include <sstream>
#include <string>

#include "api/cvc4cpp.h"
#include "base/check.h"
#include "base/exception.h"
#include "context/context.h"
#include "expr/expr.h"
#include "expr/expr_manager.h"
#include "expr/symbol_table.h"
#include "expr/type.h"
#include "test.h"

namespace CVC4 {

using namespace CVC4::kind;
using namespace CVC4::context;

namespace test {

class TestSymbolTableBlack : public TestInternal
{
 protected:
  api::Solver d_slv;
};

TEST_F(TestSymbolTableBlack, bind1)
{
  SymbolTable symtab;
  api::Sort booleanType = d_slv.getBooleanSort();
  api::Term x = d_slv.mkConst(booleanType);
  symtab.bind("x", x);
  ASSERT_TRUE(symtab.isBound("x"));
  ASSERT_EQ(symtab.lookup("x"), x);
}

TEST_F(TestSymbolTableBlack, bind2)
{
  SymbolTable symtab;
  api::Sort booleanType = d_slv.getBooleanSort();
  // var name attribute shouldn't matter
  api::Term y = d_slv.mkConst(booleanType, "y");
  symtab.bind("x", y);
  ASSERT_TRUE(symtab.isBound("x"));
  ASSERT_EQ(symtab.lookup("x"), y);
}

TEST_F(TestSymbolTableBlack, bind3)
{
  SymbolTable symtab;
  api::Sort booleanType = d_slv.getBooleanSort();
  api::Term x = d_slv.mkConst(booleanType);
  symtab.bind("x", x);
  api::Term y = d_slv.mkConst(booleanType);
  // new binding covers old
  symtab.bind("x", y);
  ASSERT_TRUE(symtab.isBound("x"));
  ASSERT_EQ(symtab.lookup("x"), y);
}

TEST_F(TestSymbolTableBlack, bind4)
{
  SymbolTable symtab;
  api::Sort booleanType = d_slv.getBooleanSort();
  api::Term x = d_slv.mkConst(booleanType);
  symtab.bind("x", x);

  api::Sort t = d_slv.mkUninterpretedSort("T");
  // duplicate binding for type is OK
  symtab.bindType("x", t);

  ASSERT_TRUE(symtab.isBound("x"));
  ASSERT_EQ(symtab.lookup("x"), x);
  ASSERT_TRUE(symtab.isBoundType("x"));
  ASSERT_EQ(symtab.lookupType("x"), t);
}

TEST_F(TestSymbolTableBlack, bind_type1)
{
  SymbolTable symtab;
  api::Sort s = d_slv.mkUninterpretedSort("S");
  symtab.bindType("S", s);
  ASSERT_TRUE(symtab.isBoundType("S"));
  ASSERT_EQ(symtab.lookupType("S"), s);
}

TEST_F(TestSymbolTableBlack, bind_type2)
{
  SymbolTable symtab;
  // type name attribute shouldn't matter
  api::Sort s = d_slv.mkUninterpretedSort("S");
  symtab.bindType("T", s);
  ASSERT_TRUE(symtab.isBoundType("T"));
  ASSERT_EQ(symtab.lookupType("T"), s);
}

TEST_F(TestSymbolTableBlack, bind_type3)
{
  SymbolTable symtab;
  api::Sort s = d_slv.mkUninterpretedSort("S");
  symtab.bindType("S", s);
  api::Sort t = d_slv.mkUninterpretedSort("T");
  // new binding covers old
  symtab.bindType("S", t);
  ASSERT_TRUE(symtab.isBoundType("S"));
  ASSERT_EQ(symtab.lookupType("S"), t);
}

TEST_F(TestSymbolTableBlack, push_scope)
{
  SymbolTable symtab;
  api::Sort booleanType = d_slv.getBooleanSort();
  api::Term x = d_slv.mkConst(booleanType);
  symtab.bind("x", x);
  symtab.pushScope();

  ASSERT_TRUE(symtab.isBound("x"));
  ASSERT_EQ(symtab.lookup("x"), x);

  api::Term y = d_slv.mkConst(booleanType);
  symtab.bind("x", y);

  ASSERT_TRUE(symtab.isBound("x"));
  ASSERT_EQ(symtab.lookup("x"), y);

  symtab.popScope();
  ASSERT_TRUE(symtab.isBound("x"));
  ASSERT_EQ(symtab.lookup("x"), x);
}

TEST_F(TestSymbolTableBlack, bad_pop)
{
  SymbolTable symtab;
  ASSERT_THROW(symtab.popScope(), ScopeException);
}

}  // namespace test
}  // namespace CVC4
