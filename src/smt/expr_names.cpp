/*********************                                                        */
/*! \file expr_names.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Utility for maintaining expression names.
 **/

#include "smt/expr_names.h"

namespace CVC4 {
namespace smt {

ExprNames::ExprNames(context::UserContext* u)
    : d_exprNames(u)
{
}
ExprNames::~ExprNames() {}

void ExprNames::setExpressionName(Node e, std::string name) {
  d_exprNames[e] = name;
}

bool ExprNames::getExpressionName(Node e, std::string& name) const {
  context::CDHashMap< Node, std::string, NodeHashFunction >::const_iterator it = d_exprNames.find(e);
  if(it!=d_exprNames.end()) {
    name = (*it).second;
    return true;
  }
  return false;
}

}  // namespace smt
}  // namespace CVC4
