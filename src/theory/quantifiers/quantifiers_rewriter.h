/*********************                                                        */
/*! \file quantifiers_rewriter.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Morgan Deters, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Rewriter for the theory of inductive quantifiers
 **
 ** Rewriter for the theory of inductive quantifiers.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__QUANTIFIERS__QUANTIFIERS_REWRITER_H
#define __CVC4__THEORY__QUANTIFIERS__QUANTIFIERS_REWRITER_H

#include "theory/rewriter.h"
#include "theory/quantifiers_engine.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

struct QAttributes;

class QuantifiersRewriter {
private:
  static int getPurifyIdLit2( Node n, std::map< Node, int >& visited );
public:
  static bool isLiteral( Node n );
private:
  static bool addCheckElimChild( std::vector< Node >& children, Node c, Kind k, std::map< Node, bool >& lit_pol, bool& childrenChanged );
  static void addNodeToOrBuilder( Node n, NodeBuilder<>& t );
  static void computeArgs( std::vector< Node >& args, std::map< Node, bool >& activeMap, Node n, std::map< Node, bool >& visited );
  static void computeArgVec( std::vector< Node >& args, std::vector< Node >& activeArgs, Node n );
  static void computeArgVec2( std::vector< Node >& args, std::vector< Node >& activeArgs, Node n, Node ipl );
  static Node computeProcessTerms2( Node body, bool hasPol, bool pol, std::map< Node, bool >& currCond, int nCurrCond,
                                    std::map< Node, Node >& cache, std::map< Node, Node >& icache,
                                    std::vector< Node >& new_vars, std::vector< Node >& new_conds, bool elimExtArith );
  static void computeDtTesterIteSplit( Node n, std::map< Node, Node >& pcons, std::map< Node, std::map< int, Node > >& ncons, std::vector< Node >& conj );
  //-------------------------------------variable elimination
  /** is variable elimination 
   * 
   * Returns true if v is not a subterm of s, and the type of s is a subtype of
   * the type of v.
   */
  static bool isVariableElim( Node v, Node s );
  /** get variable elimination literal
   * 
   * TODO
   * 
   */
  static bool getVarElimLit( Node n, bool pol, std::vector< Node >& args, std::vector< Node >& vars, std::vector< Node >& subs );
  /** get variable elimination
   * 
   * TODO
   */
  static bool getVarElim( Node n, bool pol, std::vector< Node >& args, std::vector< Node >& vars, std::vector< Node >& subs );
  /** has variable elim
   * 
   * TODO
   */
  static bool hasVariableElim( Node n, bool pol, std::vector< Node >& args );
  
  static void isVariableBoundElig( Node n, std::map< Node, int >& exclude, std::map< Node, std::map< int, bool > >& visited, bool hasPol, bool pol, 
                                   std::map< Node, bool >& elig_vars );
  /** variable eliminate for bit-vector literals
   *
   * If this returns a non-null value ret, then var is updated to a member of
   * args, and lit is equivalent to ( var = ret ).
   */
  static Node getVarElimLitBv(Node lit,
                                   std::vector<Node>& args,
                                   Node& var);
  /** compute variable elimination inequality
   * 
   * TODO
   */
  static Node getVarElimIneq( Node body, std::vector< Node >& args, std::vector< Node >& vars, std::vector< Node >& subs, QAttributes& qa );
  /** compute variable elimination 
   * 
   * TODO
   */
  static Node computeVarElimination( Node body, std::vector< Node >& args, QAttributes& qa );
  //-------------------------------------end variable elimination
 public:
  static Node computeElimSymbols( Node body );
  static Node computeMiniscoping( std::vector< Node >& args, Node body, QAttributes& qa );
  static Node computeAggressiveMiniscoping( std::vector< Node >& args, Node body );
  //cache is dependent upon currCond, icache is not, new_conds are negated conditions
  static Node computeProcessTerms( Node body, std::vector< Node >& new_vars, std::vector< Node >& new_conds, Node q, QAttributes& qa );
  /** compute conditional splitting 
   *
   */
  static Node computeCondSplit( Node body, std::vector< Node >& args, QAttributes& qa );
  static Node computePrenex( Node body, std::vector< Node >& args, std::vector< Node >& nargs, bool pol, bool prenexAgg );
  static Node computePrenexAgg( Node n, bool topLevel, std::map< unsigned, std::map< Node, Node > >& visited );
  static Node computeSplit( std::vector< Node >& args, Node body, QAttributes& qa );
private:
  enum{
    COMPUTE_ELIM_SYMBOLS = 0,
    COMPUTE_MINISCOPING,
    COMPUTE_AGGRESSIVE_MINISCOPING,
    COMPUTE_PROCESS_TERMS,
    COMPUTE_PRENEX,
    COMPUTE_VAR_ELIMINATION,
    COMPUTE_COND_SPLIT,
    //COMPUTE_FLATTEN_ARGS_UF,
    //COMPUTE_CNF,
    COMPUTE_LAST
  };
  static Node computeOperation( Node f, int computeOption, QAttributes& qa );
public:
  static RewriteResponse preRewrite(TNode in);
  static RewriteResponse postRewrite(TNode in);
  static inline void init() {}
  static inline void shutdown() {}
private:
  /** options */
  static bool doOperation( Node f, int computeOption, QAttributes& qa );
private:
  static Node preSkolemizeQuantifiers(Node n, bool polarity, std::vector< TypeNode >& fvTypes, std::vector<TNode>& fvs);
public:
  static Node rewriteRewriteRule( Node r );
  static bool containsQuantifiers( Node n );
  static bool isPrenexNormalForm( Node n );
  /** preprocess
   *
   * This returns the result of applying simple quantifiers-specific
   * preprocessing to n, including but not limited to:
   * - rewrite rule elimination,
   * - pre-skolemization,
   * - aggressive prenexing.
   * The argument isInst is set to true if n is an instance of a previously
   * registered quantified formula. If this flag is true, we do not apply
   * certain steps like pre-skolemization since we know they will have no
   * effect.
   */
  static Node preprocess( Node n, bool isInst = false );
  static Node mkForAll( std::vector< Node >& args, Node body, QAttributes& qa );
  static Node mkForall( std::vector< Node >& args, Node body, bool marked = false );
  static Node mkForall( std::vector< Node >& args, Node body, std::vector< Node >& iplc, bool marked = false );
};/* class QuantifiersRewriter */

}/* CVC4::theory::quantifiers namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* __CVC4__THEORY__QUANTIFIERS__QUANTIFIERS_REWRITER_H */


