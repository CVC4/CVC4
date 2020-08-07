/*********************                                                        */
/*! \file relevance_manager.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Relevance manager
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__RELEVANCE_MANAGER__H
#define CVC4__THEORY__RELEVANCE_MANAGER__H

#include <unordered_map>
#include <unordered_set>

#include "context/cdlist.h"
#include "expr/node.h"
#include "theory/valuation.h"

namespace CVC4 {
namespace theory {

/**
 * This class manages queries related to relevance of asserted literals.
 * In particular, note the following definition:
 * 
 * Let F be a formula, and let L = { l_1, ..., l_n } be a set of
 * literals that propositionally entail it. A "relevant selection of L with
 * respect to F" is a subset of L that also propositionally entails F.
 *
 * This class computes a relevant selection of the current assertion stack
 * at FULL effort with respect to the input formula + theory lemmas that are
 * critical to justify (see LemmaProperty::NEEDS_JUSTIFY). As an example
 * of the latter, notice that 
 *
 * Internally, it stores the input assertions and can be asked if an asserted
 * literal is part of the current relevant selection. The relevant selection
 * is computed lazily, i.e. only when someone asks if a literal is relevant,
 * and only at most once per FULL effort check.
 */
class RelevanceManager
{
  typedef context::CDList<Node> NodeList;

 public:
  RelevanceManager(context::UserContext* userContext, Valuation val);
  /** 
   * Notify (preprocessed) assertions. This is called for assertions that
   * are 
   */
  void notifyPreprocessedAssertions(const std::vector<Node>& assertions);
  /** Singleton version of above */
  void notifyPreprocessedAssertion(Node n);
  /** 
   * Reset round, called at the beginning of a full effort check in
   * TheoryEngine.
   */
  void resetRound();
  /** 
   * Is lit part of the current relevant selection? This call is valid during
   * full effort check in TheoryEngine. This means that theories can query this
   * during FULL or LAST_CALL efforts, through the Valuation class.
   */
  bool isRelevant(Node lit);
 private:
  /** compute the relevant selection */
  void computeRelevance();
  /** 
   * Justify formula n, return 1 means we justified it to be true, -1 means
   * justified it to be false, 0 means it was not justified.
   */
  int justify(TNode n,
              std::unordered_map<TNode, int, TNodeHashFunction>& cache);
  /** The valuation object, used to query current value of theory literals */
  Valuation d_val;
  /** The (non-unit) input assertions */
  NodeList d_input;
  /** The current relevant selection. */
  std::unordered_set<TNode, TNodeHashFunction> d_rset;
  /** Have we computed the relevant selection this round? */
  bool d_computed;
  /**
   * Did we succeed in computing the relevant selection? If this is false, there
   * was a syncronization issue between the input formula and the satisfying
   * assignment since this class found that the input formula was not satisfied
   * by the assignment. This should never happen, but if it does, this class
   * aborts and indicates that all literals are relevant.
   */
  bool d_success;
};

}  // namespace theory
}  // namespace CVC4

#endif /* CVC4__THEORY__RELEVANCE_MANAGER__H */
