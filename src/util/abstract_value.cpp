/*********************                                                        */
/*! \file abstract_value.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Morgan Deters, Tim King, Paul Meng
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Representation of abstract values
 **
 ** Representation of abstract values.
 **/

#include "util/abstract_value.h"

#include <iostream>
#include <sstream>
#include <string>

#include "base/cvc4_assert.h"

using namespace std;

namespace CVC4 {

std::ostream& operator<<(std::ostream& out, const AbstractValue& val) {
  return out << "@" << val.getIndex();
}

AbstractValue::AbstractValue(Integer index) throw(IllegalArgumentException) :
    d_index(index) {
    PrettyCheckArgument(index >= 1, index) <<  "index >= 1 required for abstract value, not `" << index.toString() << "'" << std::endl;
}

}/* CVC4 namespace */
