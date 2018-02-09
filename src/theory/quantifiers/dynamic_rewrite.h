/*********************                                                        */
/*! \file dynamic_rewriter.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief dynamic_rewriter
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__QUANTIFIERS__DYNAMIC_REWRITER_H
#define __CVC4__THEORY__QUANTIFIERS__DYNAMIC_REWRITER_H

#include <map>

#include "theory/quantifiers_engine.h"
#include "theory/uf/equality_engine.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

/** DynamicRewriter
 *
 * This class is given a stream of equalities in calls to addRewrite(...).
 * The goal is to show that a subset of these can be inferred from previously
 * asserted equalities.  For example, a possible set of return values for
 * add rewrite on successive calls is the following:
 *
 *   addRewrite( x, y ) ---> true
 *   addRewrite( f( x ), f( y ) ) ---> false, since we already know x=y
 *   addRewrite( x, z ) ---> true
 *   addRewrite( x+y, x+z ) ---> false, since we already know y=x=z.
 *
 * This class can be used to filter out redundant candidate rewrite rules
 * when using the option sygusRewSynth().
 * 
 * Currently, this class infers that an equality is redundant using
 * an instance of the equality engine that does congruence over all
 * operators by mapping all operators to uninterpreted ones and doing
 * congruence on APPLY_UF.
 */
class DynamicRewriter
{
 public:
  DynamicRewriter(const std::string& name, QuantifiersEngine* qe);
  ~DynamicRewriter() {}
  /** inform this class that the equality a = b holds.
   *
   * This function returns true if this class did not already know that
   * a = b based on the previous equalities it has seen.
   */
  bool addRewrite(Node a, Node b);

 private:
  /** pointer to the quantifiers engine */
  QuantifiersEngine* d_qe;
  /** index over argument types to function skolems
   *
   * The purpose of this trie is to associate a class of interpreted operator
   * with uninterpreted symbols. It is necessary to introduce multiple
   * uninterpreted symbols per interpreted operator since they may be
   * polymorphic. For example, for array select, its associate trie may look
   * like this:
   *   d_children[array_type_1]
   *     d_children[index_type_1] : k1
   *     d_children[index_type_2] : k2
   *   d_children[array_type_2]
   *     d_children[index_type_1] : k3
   */
  class OpInternalSymTrie
  {
   public:
    Node d_sym;
    std::map<TypeNode, OpInternalSymTrie> d_children;
    Node getSymbol(Node n);
  };
  /** the internal operator symbol trie for this class */
  std::map<Node, OpInternalSymTrie> d_ois_trie;
  /**
   * Convert node a to its internal representation, which replaces all
   * interpreted operators in a by a unique uninterpreted symbol.
   */
  Node toInternal(Node a);
  /** cache of the above function */
  std::map<Node, Node> d_term_to_internal;
  /** stores congruence closure over terms given to this class. */
  eq::EqualityEngine d_equalityEngine;
};

} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */

#endif /* __CVC4__THEORY__QUANTIFIERS__DYNAMIC_REWRITER_H */
