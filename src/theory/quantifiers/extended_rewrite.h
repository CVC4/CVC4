/*********************                                                        */
/*! \file extended_rewrite.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief extended rewriting class
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__QUANTIFIERS__EXTENDED_REWRITE_H
#define __CVC4__THEORY__QUANTIFIERS__EXTENDED_REWRITE_H

#include <unordered_map>

#include "expr/node.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

/** Extended rewriter
 * This class is used for all rewriting that is not necessarily
 * useful for quantifier-free solving, but may be helpful 
 * in other contexts such as SyGuS, where rewriting aids
 * in generalizing refinement lemmas.
 * 
 * This class extended the standard techniques for rewriting
 * with techniques including:
 * - ITE branch merging,
 * - ITE conditional variable elimination,
 * - ITE condition subsumption, and
 * - Aggressive rewriting for string equalities.
 */
class ExtendedRewriter
{
public:
  ExtendedRewriter();
  ~ExtendedRewriter(){}
  /** return the extended rewritten form of n */
  Node extendedRewrite( Node n );
private:
  /** true and false nodes */
  Node d_true;
  Node d_false;
  /** cache for extendedRewrite */
  std::unordered_map< Node, Node, NodeHashFunction > d_ext_rewrite_cache;
  /** pull ITE 
   * Do simple ITE pulling, e.g.:
   *   C2 --->^E false 
   * implies:
   *  ite( C, C1, C2 ) --->^E  C ^ C1
   * where ---->^E denotes extended rewriting.
   */
  Node extendedRewritePullIte( Node n );
};
  
} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */

#endif /* __CVC4__THEORY__QUANTIFIERS__EXTENDED_REWRITE_H */
