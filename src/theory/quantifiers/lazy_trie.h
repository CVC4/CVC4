/*********************                                                        */
/*! \file lazy_trie.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Haniel Barbosa
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief lazy trie
 **/

#ifndef __CVC4__THEORY__QUANTIFIERS__LAZY_TRIE_H
#define __CVC4__THEORY__QUANTIFIERS__LAZY_TRIE_H

#include "expr/node.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

/** abstract evaluator class
 *
 * This class is used for the LazyTrie data structure below.
 */
class LazyTrieEvaluator
{
 public:
  virtual ~LazyTrieEvaluator() {}
  virtual Node evaluate(Node n, unsigned index) = 0;
};

/** LazyTrie
 *
 * This is a trie where terms are added in a lazy fashion. This data structure
 * is useful, for instance, when we are only interested in when two terms
 * map to the same node in the trie but we need not care about computing
 * exactly where they are.
 *
 * In other words, when a term n is added to this trie, we do not insist
 * that n is placed at the maximal depth of the trie. Instead, we place n at a
 * minimal depth node that has no children. In this case we say n is partially
 * evaluated in this trie.
 *
 * This class relies on an abstract evaluator interface above, which evaluates
 * nodes for indices.
 *
 * For example, say we have terms a, b, c and an evaluator ev where:
 *   ev->evaluate( a, 0,1,2 ) = 0, 5, 6
 *   ev->evaluate( b, 0,1,2 ) = 1, 3, 0
 *   ev->evaluate( c, 0,1,2 ) = 1, 3, 2
 * After adding a to the trie, we get:
 *   root: a
 * After adding b to the resulting trie, we get:
 *   root: null
 *     d_children[0]: a
 *     d_children[1]: b
 * After adding c to the resulting trie, we get:
 *   root: null
 *     d_children[0]: a
 *     d_children[1]: null
 *       d_children[3]: null
 *         d_children[0] : b
 *         d_children[2] : c
 * Notice that we need not call ev->evalute( a, 1 ) and ev->evalute( a, 2 ).
 */
class LazyTrie
{
 public:
  LazyTrie() {}
  ~LazyTrie() {}
  /** the data at this node, which may be partially evaluated */
  Node d_lazy_child;
  /** the children of this node */
  std::map<Node, LazyTrie> d_children;
  /** clear the trie */
  void clear() { d_children.clear(); }
  /** add n to the trie
   *
   * This function returns a node that is mapped to the same node in the trie
   * if one exists, or n otherwise.
   *
   * ev is an evaluator which determines where n is placed in the trie
   * index is the depth of this node
   * ntotal is the maximal depth of the trie
   * forceKeep is whether we wish to force that n is chosen as a representative
   */
  Node add(Node n,
           LazyTrieEvaluator* ev,
           unsigned index,
           unsigned ntotal,
           bool forceKeep);
};

class DynamicClassifer
{
 public:
  std::map<Node, std::vector<Node>> d_rep_to_sepclass;
  void addClassifier(LazyTrieEvaluator* ev, unsigned ntotal);
  Node add(Node f, LazyTrieEvaluator* ev, unsigned ntotal);

 private:
  LazyTrie d_trie;
};

} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */

#endif /* __CVC4__THEORY__QUANTIFIERS__LAZY_TRIE_H */
