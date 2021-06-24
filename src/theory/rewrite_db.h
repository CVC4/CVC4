/******************************************************************************
 * Top contributors (to current version):
 *   Andrew Reynolds
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Rewrite database
 */

#include "cvc5_private.h"

#ifndef CVC4__THEORY__REWRITE_DB__H
#define CVC4__THEORY__REWRITE_DB__H

#include <map>
#include <vector>

#include "expr/match_trie.h"
#include "expr/node.h"
#include "expr/term_canonize.h"
#include "theory/rewrite_proof_rule.h"

namespace cvc5 {
namespace theory {

/**
 * A database of conditional rewrite rules.
 */
class RewriteDb
{
 public:
  /**
   * Constructor. The body of this method is auto-generated by the rules
   * defined in the rewrite_rules files.
   */
  RewriteDb();
  ~RewriteDb() {}
  /** Add rule, return its identifier */
  DslPfRule addRule(Node a, Node b, Node cond, const std::string& name);
  /** get matches */
  void getMatches(Node eq, expr::NotifyMatch* ntm);
  /** Get the maximum rule identifier */
  DslPfRule getMaxRuleId() const;
  /** get rule for id */
  const RewriteProofRule& getRule(DslPfRule id) const;
  /** get ids for conclusion */
  const std::vector<DslPfRule>& getRuleIdsForConclusion(Node eq) const;

 private:
  /** common constants */
  Node d_true;
  Node d_false;
  /** the term canonization utility */
  expr::TermCanonize d_canon;
  /** The match trie */
  expr::MatchTrie d_mt;
  /** map ids to rewrite db rule information */
  std::map<DslPfRule, RewriteProofRule> d_rewDbRule;
  /** map conclusions to proof ids */
  std::map<Node, std::vector<DslPfRule> > d_concToRules;
  /** dummy empty vector */
  std::vector<DslPfRule> d_emptyVec;
  /** currently allocating id */
  DslPfRule d_idCounter;
};

}  // namespace theory
}  // namespace cvc5

#endif /* CVC4__THEORY__REWRITE_DB__H */
