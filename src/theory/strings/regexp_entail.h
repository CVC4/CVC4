/*********************                                                        */
/*! \file regexp_entail.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Andres Noetzli, Tianyi Liang
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Entailment tests involving regular expressions
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__STRINGS__REGEXP_ENTAIL_H
#define CVC4__THEORY__STRINGS__REGEXP_ENTAIL_H

#include <climits>
#include <utility>
#include <vector>

#include "expr/attribute.h"
#include "theory/strings/rewrites.h"
#include "theory/theory_rewriter.h"
#include "theory/type_enumerator.h"

namespace CVC4 {
namespace theory {
namespace strings {

class RegExpEntail 
{
 public:
  /** simple regular expression consume
   *
   * This method is called when we are rewriting a membership of the form
   *   s1 ++ ... ++ sn in r1 ++ ... ++ rm
   * We have that mchildren consists of the strings s1...sn, and children
   * consists of the regular expressions r1...rm.
   *
   * This method tries to strip off parts of the concatenation terms. It updates
   * the vectors such that the resulting vectors are such that the membership
   * mchildren[n'...n''] in children[m'...m''] is equivalent to the input
   * membership. The argument dir indicates the direction to consider, where
   * 0 means strip off the front, 1 off the back, and < 0 off of both.
   *
   * If this method returns the false node, then we have inferred that no
   * string in the language of r1 ++ ... ++ rm is a prefix (when dir!=1) or
   * suffix (when dir!=0) of s1 ++ ... ++ sn. Otherwise, it returns the null
   * node.
   *
   * For example, given input
   *   mchildren = { "ab", x }, children = { [["a"]], ([["cd"]])* } and dir = 0,
   * this method updates:
   *   mchildren = { "b", x }, children = { ("cd")* }
   * and returns null.
   *
   * For example, given input
   *   { x, "abb", x }, { [[x]], ["a"..."b"], allchar, [[y]], [[x]]} and dir=-1,
   * this method updates:
   *   { "b" }, { [[y]] }
   * where [[.]] denotes str.to.re, and returns null.
   *
   * Notice that the above requirement for returning false is stronger than
   * determining that s1 ++ ... ++ sn in r1 ++ ... ++ rm is equivalent to false.
   * For example, for input "bb" in "b" ++ ( "a" )*, we do not return false
   * since "b" is in the language of "b" ++ ( "a" )* and is a prefix of "bb".
   * We do not return false even though the above membership is equivalent
   * to false. We do this because the function is used e.g. to test whether a
   * possible unrolling leads to a conflict. This is demonstrated by the
   * following examples:
   *
   * For example, given input
   *   { "bb", x }, { "b", ("a")* } and dir=-1,
   * this method updates:
   *   { "b" }, { ("a")* }
   * and returns null.
   *
   * For example, given input
   *   { "cb", x }, { "b", ("a")* } and dir=-1,
   * this method leaves children and mchildren unchanged and returns false.
   *
   * Notice that based on this, we can determine that:
   *   "cb" ++ x  in ( "b" ++ ("a")* )*
   * is equivalent to false, whereas we cannot determine that:
   *   "bb" ++ x  in ( "b" ++ ("a")* )*
   * is equivalent to false.
   */
  static Node simpleRegexpConsume(std::vector<Node>& mchildren,
                                  std::vector<Node>& children,
                                  int dir = -1);
  static bool isConstRegExp(TNode t);
  static bool testConstStringInRegExp(CVC4::String& s,
                                      unsigned int index_start,
                                      TNode r);
  static bool hasEpsilonNode(TNode node);
};

}  // namespace strings
}  // namespace theory
}  // namespace CVC4

#endif /* CVC4__THEORY__STRINGS__REGEXP_ENTAIL_H */
