/*********************                                                        */
/*! \file equality_query.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Equality query class
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__QUANTIFIERS_EQUALITY_QUERY_H
#define __CVC4__THEORY__QUANTIFIERS_EQUALITY_QUERY_H

#include "context/cdo.h"
#include "expr/node.h"
#include "theory/quantifiers_engine.h"
#include "theory/quantifiers/quant_util.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

/** EqualityQueryQuantifiersEngine class
* This is a wrapper class around an equality engine that is used for
* queries required by algorithms in the quantifiers theory.
* It uses an equality engine, as determined by the quantifiers engine it points to.
*
* The main extension of this class wrt EqualityQuery is the function
* getInternalRepresentative, which is used by instantiation-based methods
* that are agnostic with respect to choosing terms within an equivalence class.
* Examples of such methods are finite model finding and enumerative instantiation.
* Method getInternalRepresentative returns the "best" representative based on the internal heuristic,
* which is currently based on choosing the term that was previously chosen as a representative
* earliest.
*/
class EqualityQueryQuantifiersEngine : public EqualityQuery
{
public:
  EqualityQueryQuantifiersEngine( context::Context* c, QuantifiersEngine* qe );
  virtual ~EqualityQueryQuantifiersEngine();
  /** reset */
  virtual bool reset( Theory::Effort e );
  /* Called for new quantifiers */
  virtual void registerQuantifier( Node q ) {}
  /** identify */
  virtual std::string identify() const { return "EqualityQueryQE"; }
  /** does the equality engine have term a */
  bool hasTerm( Node a );
  /** get the representative of a */
  Node getRepresentative( Node a );
  /** are a and b disequal? */
  bool areEqual( Node a, Node b );
  /** are a and b disequal? */
  bool areDisequal( Node a, Node b );
  /** get equality engine
  * This may either be the master equality engine or the model's equality engine.
  */
  eq::EqualityEngine* getEngine();
  /** get list of members in the equivalence class of a */
  void getEquivalenceClass( Node a, std::vector< Node >& eqc );
  /** get congruent term
  * If possible, returns a term n such that:
  * (1) n is a term in the equality engine from getEngine().
  * (2) n is of the form f( t1, ..., tk ) where ti is in the equivalence class of args[i] for i=1...k
  * Otherwise, returns the null node.
  *
  * Notice that f should be a "match operator", returned by TermDb::getMatchOperator.
  */
  TNode getCongruentTerm( Node f, std::vector< TNode >& args );
  /** getInternalRepresentative gets the current best representative in the equivalence class of a, based on some criteria.
      If cbqi is active, this will return a term in the equivalence class of "a" that does
      not contain instantiation constants, if such a term exists.
   */
  Node getInternalRepresentative( Node a, Node f, int index );
private:
  /** pointer to theory engine */
  QuantifiersEngine* d_qe;
  /** quantifiers equality inference */
  context::CDO< unsigned > d_eqi_counter;
  /** internal representatives */
  std::map< TypeNode, std::map< Node, Node > > d_int_rep;
  /** rep score */
  std::map< Node, int > d_rep_score;
  /** the number of times reset( e ) has been called */
  int d_reset_count;
  /** processInferences : will merge equivalence classes in master equality engine, if possible */
  bool processInferences( Theory::Effort e );
  /** node contains */
  Node getInstance( Node n, const std::vector< Node >& eqc, std::unordered_map<TNode, Node, TNodeHashFunction>& cache );
  /** get score */
  int getRepScore( Node n, Node f, int index, TypeNode v_tn );
  /** flatten representatives */
  void flattenRepresentatives( std::map< TypeNode, std::vector< Node > >& reps );
}; /* EqualityQueryQuantifiersEngine */

}/* CVC4::theory::quantifiers namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* __CVC4__THEORY__QUANTIFIERS_EQUALITY_QUERY_H */
