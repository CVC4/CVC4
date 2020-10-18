/*********************                                                        */
/*! \file normal_form.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Mudathir Mohamed
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Normal form for bag constants.
 **/

#include <expr/node.h>

#include "cvc4_private.h"

#ifndef CVC4__THEORY__BAGS__NORMAL_FORM_H
#define CVC4__THEORY__BAGS__NORMAL_FORM_H

namespace CVC4 {
namespace theory {
namespace bags {

class NormalForm
{
 public:
  /**
   * Returns true if n is considered a to be a (canonical) constant bag value.
   * A canonical bag value is one whose AST is:
   *   (disjoint-union (mk-bag e1 n1) ...
   *        (disjoint-union (mk-bag e_{n-1} n_{n-1}) (mk-bag e_n n_n))))
   * where c1 ... cn are constants and the node identifier of these constants
   * are such that:
   *   c1 > ... > cn.
   * Also handles the corner cases of empty bag and singleton bag.
   */
  static bool checkNormalConstant(TNode n);
  /**
   * check whether all children of the given node are in normal form
   */
  static bool areChildrenConstants(TNode n);
  /**
   * evaluate the node n to a constant value
   */
  static Node evaluate(TNode n);

 private:
  /**
   * evaluate n as follows:
   * - (mkBag a 0) = (emptybag T) where T is the type of a
   * - (mkBag a (-c)) = (emptybag T) where T is the type of a, and c > 0 is a
   *   constant
   * - otherwise = n
   */
  static Node evaluateMakeBag(TNode n);
  /**
   * evaluate n as follows:
   * - (mkBag a 0) = (emptybag T) where T is the type of a
   * - (mkBag a (-c)) = (emptybag T) where T is the type of a, and c > 0 is a
   *   constant
   * - otherwise = n
   */
  static Node evaluateUnionDisjoint(TNode n);

  static std::map<Node, Rational> getBagElements(TNode n);
  static Node constructBagFromElements(TypeNode t,
                                       const std::map<Node, Rational>& map);
};
}  // namespace bags
}  // namespace theory
}  // namespace CVC4

#endif /* CVC4__THEORY__BAGS__NORMAL_FORM_H */
