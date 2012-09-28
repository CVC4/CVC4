/*********************                                                        */
/*! \file boolean_simplification.h
 ** \verbatim
 ** Original author: taking
 ** Major contributors: mdeters
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 prototype.
 ** Copyright (c) 2009, 2010, 2011  The Analysis of Computer Systems Group (ACSys)
 ** Courant Institute of Mathematical Sciences
 ** New York University
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Simple routines for Boolean simplification
 **
 ** Simple, commonly-used routines for Boolean simplification.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__BOOLEAN_SIMPLIFICATION_H
#define __CVC4__BOOLEAN_SIMPLIFICATION_H

#include <vector>
#include <algorithm>

#include "expr/node.h"
#include "util/cvc4_assert.h"

namespace CVC4 {

/**
 * A class to contain a number of useful functions for simple
 * simplification of nodes.  One never uses it as an object (and
 * it cannot be constructed).  It is used as a namespace.
 */
class BooleanSimplification {
  // cannot construct one of these
  BooleanSimplification() CVC4_UNUSED;
  BooleanSimplification(const BooleanSimplification&) CVC4_UNUSED;

  static bool push_back_associative_commute_recursive
    (Node n, std::vector<Node>& buffer, Kind k, Kind notK, bool negateNode)
    throw(AssertionException) CVC4_WARN_UNUSED_RESULT;

public:

  /**
   * The threshold for removing duplicates.  (See removeDuplicates().)
   */
  static const uint32_t DUPLICATE_REMOVAL_THRESHOLD = 10;

  /**
   * Remove duplicate nodes from a vector, modifying it in-place.
   * If the vector has size >= DUPLICATE_REMOVAL_THRESHOLD, this
   * function is a no-op.
   */
  static void removeDuplicates(std::vector<Node>& buffer)
      throw(AssertionException) {
    if(buffer.size() < DUPLICATE_REMOVAL_THRESHOLD) {
      std::sort(buffer.begin(), buffer.end());
      std::vector<Node>::iterator new_end =
        std::unique(buffer.begin(), buffer.end());
      buffer.erase(new_end, buffer.end());
    }
  }

  /**
   * Takes a node with kind AND, collapses all AND and (NOT OR)-kinded
   * children of it (as far as possible---see
   * push_back_associative_commute()), removes duplicates, and returns
   * the resulting Node.
   */
  static Node simplifyConflict(Node andNode) throw(AssertionException) {
    AssertArgument(!andNode.isNull(), andNode);
    AssertArgument(andNode.getKind() == kind::AND, andNode);

    std::vector<Node> buffer;
    push_back_associative_commute(andNode, buffer, kind::AND, kind::OR);

    removeDuplicates(buffer);

    if(buffer.size() == 1) {
      return buffer[0];
    }

    NodeBuilder<> nb(kind::AND);
    nb.append(buffer);
    return nb;
  }

  /**
   * Takes a node with kind OR, collapses all OR and (NOT AND)-kinded
   * children of it (as far as possible---see
   * push_back_associative_commute()), removes duplicates, and returns
   * the resulting Node.
   */
  static Node simplifyClause(Node orNode) throw(AssertionException) {
    AssertArgument(!orNode.isNull(), orNode);
    AssertArgument(orNode.getKind() == kind::OR, orNode);

    std::vector<Node> buffer;
    push_back_associative_commute(orNode, buffer, kind::OR, kind::AND);

    removeDuplicates(buffer);

    Assert(buffer.size() > 0);
    if(buffer.size() == 1) {
      return buffer[0];
    }

    NodeBuilder<> nb(kind::OR);
    nb.append(buffer);
    return nb;
  }

  /**
   * Takes a node with kind IMPLIES, converts it to an OR, then
   * simplifies the result with simplifyClause().
   *
   * The input doesn't actually have to be Horn, it seems, but that's
   * the common case(?), hence the name.
   */
  static Node simplifyHornClause(Node implication) throw(AssertionException) {
    AssertArgument(implication.getKind() == kind::IMPLIES, implication);

    TNode left = implication[0];
    TNode right = implication[1];

    Node notLeft = negate(left);
    Node clause = NodeBuilder<2>(kind::OR) << notLeft << right;

    return simplifyClause(clause);
  }

  /**
   * Aids in reforming a node.  Takes a node of (N-ary) kind k and
   * copies its children into an output vector, collapsing its k-kinded
   * children into it.  Also collapses negations of notK.  For example:
   *
   *   push_back_associative_commute( [OR [OR a b] [OR b c d] [NOT [AND e f]]],
   *                                  buffer, kind::OR, kind::AND )
   *   yields a "buffer" vector of [a b b c d e f]
   *
   * @param n the node to operate upon
   * @param buffer the output vector (must be empty on entry)
   * @param k the kind to collapse (should equal the kind of node n)
   * @param notK the "negation" of kind k (e.g. OR's negation is AND),
   * or kind::UNDEFINED_KIND if none.
   * @param negateChildren true if the children of the resulting node
   * (that is, the elements in buffer) should all be negated; you want
   * this if e.g. you're simplifying the (OR...) in (NOT (OR...)),
   * intending to make the result an AND.
   */
  static inline void
  push_back_associative_commute(Node n, std::vector<Node>& buffer,
                                Kind k, Kind notK, bool negateChildren = false)
      throw(AssertionException) {
    AssertArgument(buffer.empty(), buffer);
    AssertArgument(!n.isNull(), n);
    AssertArgument(k != kind::UNDEFINED_KIND && k != kind::NULL_EXPR, k);
    AssertArgument(notK != kind::NULL_EXPR, notK);
    AssertArgument(n.getKind() == k, n,
                   "expected node to have kind %s", kindToString(k).c_str());

    bool b CVC4_UNUSED =
      push_back_associative_commute_recursive(n, buffer, k, notK, false);

    if(buffer.size() == 0) {
      // all the TRUEs for an AND (resp FALSEs for an OR) were simplified away
      buffer.push_back(NodeManager::currentNM()->mkConst(k == kind::AND ? true : false));
    }
  }/* push_back_associative_commute() */

  /**
   * Negates a node, doing all the double-negation elimination
   * that's possible.
   *
   * @param n the node to negate (cannot be the null node)
   */
  static Node negate(TNode n) throw(AssertionException) {
    AssertArgument(!n.isNull(), n);

    bool polarity = true;
    TNode base = n;
    while(base.getKind() == kind::NOT){
      base = base[0];
      polarity = !polarity;
    }
    if(n.isConst()) {
      return NodeManager::currentNM()->mkConst(!n.getConst<bool>());
    }
    if(polarity){
      return base.notNode();
    }else{
      return base;
    }
  }

  /**
   * Negates an Expr, doing all the double-negation elimination that's
   * possible.
   *
   * @param e the Expr to negate (cannot be the null Expr)
   */
  static Expr negate(Expr e) throw(AssertionException) {
    return negate(Node::fromExpr(e)).toExpr();
  }

  /**
   * Simplify an OR, AND, or IMPLIES.  This function is the identity
   * for all other kinds.
   */
  inline static Node simplify(TNode n) throw(AssertionException) {
    switch(n.getKind()) {
    case kind::AND:
      return simplifyConflict(n);

    case kind::OR:
      return simplifyClause(n);

    case kind::IMPLIES:
      return simplifyHornClause(n);

    default:
      return n;
    }
  }

};/* class BooleanSimplification */

}/* CVC4 namespace */

#endif /* __CVC4__BOOLEAN_SIMPLIFICATION_H */
