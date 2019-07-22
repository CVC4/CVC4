/*********************                                                        */
/*! \file theory_strings_utils.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Util functions for theory strings.
 **
 ** Util functions for theory strings.
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__STRINGS__THEORY_STRINGS_UTILS_H
#define CVC4__THEORY__STRINGS__THEORY_STRINGS_UTILS_H

#include <vector>

#include "expr/node.h"

namespace CVC4 {
namespace theory {
namespace strings {
namespace utils {

/**
 * Make the conjunction of nodes in a. Removes duplicate conjuncts, returns
 * true if a is empty, and a single literal if a has size 1.
 */
Node mkAnd(std::vector<Node>& a);

/**
 * Get constant component. Returns the string constant represented by the
 * string or regular expression t. For example:
 *   "ABC" -> "ABC", (str.to.re "ABC") -> "ABC", (str.++ x "ABC") -> null
 */
Node getConstantComponent(Node t);

/**
 * Get constant prefix / suffix from expression. For example, if isSuf=false:
 *   "ABC" -> "ABC"
 *   (str.++ "ABC" x) -> "ABC"
 *   (str.to.re "ABC") -> "ABC"
 *   (re.++ (str.to.re "ABC") ...) -> "ABC"
 *   (re.in x (str.to.re "ABC")) -> "ABC"
 *   (re.in x (re.++ (str.to.re "ABC") ...)) -> "ABC"
 *   (str.++ x "ABC") -> null
 *   (re.in x (re.++ (re.* "D") (str.to.re "ABC"))) -> null
 */
Node getConstantPrefix(Node e, bool isSuf);

}  // namespace utils
}  // namespace strings
}  // namespace theory
}  // namespace CVC4

#endif
