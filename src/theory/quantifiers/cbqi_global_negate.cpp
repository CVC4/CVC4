/*********************                                                        */
/*! \file cbqi_global_negate.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of cbqi_global_negate
 **/

#include "theory/quantifiers/cbqi_global_negate.h"

using namespace std;
using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace quantifiers {
  
CbqiGlobalNegate::CbqiGlobalNegate() {

}

bool CbqiGlobalNegate::simplify( std::vector< Node >& assertions, 
                 std::vector< Node >& new_assertions )
{
  NodeManager * nm = NodeManager::currentNM();
  
  // collect free variables in all assertions 
  std::unordered_set< Node, NodeHashFunction > free_vars;
  for( const Node& as : assertions ){
    TNode cur = as;
    // compute free variables
    std::unordered_set<TNode, TNodeHashFunction> visited;
    std::unordered_set<TNode, TNodeHashFunction>::iterator it;
    std::vector<TNode> visit;
    visit.push_back(cur);
    do {
      cur = visit.back();
      visit.pop_back();
      if (visited.find(cur) == visited.end()) {
        visited.insert(cur);
        if( cur.isVar() && cur.getKind()!=BOUND_VARIABLE )
        {
          free_vars.insert( cur );
        }
        for (const TNode& cn : cur) {
          visit.push_back(cn);
        }
      }
    } while (!visit.empty());
  }
  
  Node body;
  if( assertions.size()==0 ){
    body = nm->mkConst( true );
  }else if( assertions.size()==1 ){
    body = assertions[0];
  }else{
    body = nm->mkNode( kind::AND, assertions );
  }
  
  // do the negation
  body = body.negate();
  
  if( !free_vars.empty() ){
    std::vector< Node > vars;
    std::vector< Node > bvs;
    for( const Node& v : free_vars )
    {
      vars.push_back( v );
      Node bv = nm->mkBoundVar( v.getType() );
      bvs.push_back( bv );
    }
      
    body = body.substitute( vars.begin(), vars.end(), bvs.begin(), bvs.end() );
    
    Node bvl = nm->mkNode( kind::BOUND_VAR_LIST, bvs );
  
    body = nm->mkNode( kind::FORALL, bvl, body );
  }else{
    // add dummy variable?
    //Node dummy = nm->mkBoundVar( nm->booleanType() );
    //bvs.push_back( dummy );
  }
  
  new_assertions.push_back( body );
  
  Node truen = nm->mkConst(true);
  for( unsigned i=0, size=assertions.size(); i<size; i++ ){
    assertions[i] = truen;
  }
  
  return true;
}

  
} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */
