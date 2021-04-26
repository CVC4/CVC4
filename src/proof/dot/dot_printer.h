/******************************************************************************
 * Top contributors (to current version):
 *   Diego Della Rocca de Camargos
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * The module for printing dot proofs.
 */

#include "cvc5_private.h"

#ifndef CVC5__PROOF__DOT__DOT_PRINTER_H
#define CVC5__PROOF__DOT__DOT_PRINTER_H

#include <iostream>

#include "expr/proof_node.h"

namespace cvc5 {
namespace proof {

class DotPrinter
{
 public:
  DotPrinter();
  ~DotPrinter() {}

  /**
   * Print the full proof of assertions => false by pn using the dot format.
   * @param out the output stream
   * @param pn the root node of the proof to print
   */
  static void print(std::ostream& out, const ProofNode* pn);

 private:
  /**
   * Print the nodes of the proof in the format:
   * $NODE_ID [ label = "{$CONCLUSION|$RULE_NAME($RULE_ARGUMENTS)}",
   * $COLORS_AND_CLASSES_RELATED_TO_THE_RULE ]; and then for each child of the
   * node $CHILD_ID -> $NODE_ID; and then recursively call the function with the
   * child as argument.
   * @param out the output stream
   * @param pn the proof node to print
   * @param ruleID the id of the rule to print
   * @param scopeCounter counter to check how many SCOPE was already sawed in
   * the proof
   * @param inPropositionalView flag used to mark the nodes that belong to the
   * propositional view proof
   */
  static void printInternal(std::ostream& out,
                            const ProofNode* pn,
                            uint64_t& ruleID,
                            uint64_t scopeCounter,
                            bool inPropositionalView);

  /**
   * Return the arguments of a ProofNode
   * @param currentArguments an ostringstream that will store the arguments of
   * pn formatted as "$ARG[0], $ARG[1], ..., $ARG[N-1]"
   * @param pn a ProofNode
   */
  static void ruleArguments(std::ostringstream& currentArguments,
                            const ProofNode* pn);

  /** Add an escape character before special characters of the given string.
   * @param s The string to have the characters processed.
   * @return The string with the special characters escaped.
   */
  static std::string sanitizeString(const std::string& s);
};

}  // namespace proof
}  // namespace cvc5

#endif