/******************************************************************************
 * Top contributors (to current version):
 *   Yoni Zohar, Andrew Reynolds, Mathias Preiner
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Utility for detecting quantifier macro definitions.
 */

#include "cvc5_private.h"

#ifndef CVC5__THEORY__QUANTIFIERS__QUANTIFIERS_MACROS_H
#define CVC5__THEORY__QUANTIFIERS__QUANTIFIERS_MACROS_H

#include <map>
#include <vector>
#include "expr/node.h"

namespace cvc5 {
namespace theory {
namespace quantifiers {

class QuantifiersRegistry;

class QuantifierMacros
{
 public:
  QuantifierMacros(QuantifiersRegistry& qr);
  ~QuantifierMacros() {}
  /**
   * Called on quantified formulas lit of the form
   *   forall x1 ... xn. n = ndef
   * where n is of the form U(x1...xn). Returns an equality of the form
   *   U = lambda x1 ... xn. ndef
   * if this is a legal macro definition for U, and the null node otherwise.
   */
  Node solve(Node lit, bool reqGround = false);

 private:
  bool isBoundVarApplyUf(Node n);
  bool containsBadOp(Node n,
                     Node op,
                     std::vector<Node>& opc,
                     std::map<Node, bool>& visited);
  bool isGroundUfTerm(Node f, Node n);
  void getMacroCandidates(Node n,
                          std::vector<Node>& candidates,
                          std::map<Node, bool>& visited);
  Node solveInEquality(Node n, Node lit);
  /**
   * Called when we have inferred a quantified formula is of the form
   *   forall x1 ... xn. n = ndef
   * where n is of the form U(x1...xn).
   */
  Node solveEq(Node n, Node ndef);
  /** Reference to the quantifiers registry */
  QuantifiersRegistry& d_qreg;
};

}  // namespace quantifiers
}  // namespace theory
}  // namespace cvc5

#endif /*CVC5__THEORY__QUANTIFIERS__QUANTIFIER_MACROS_H */
