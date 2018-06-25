/*********************                                                        */
/*! \file ceg_dt_instantiator.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of ceg_dt_instantiator
 **/

#include "theory/quantifiers/cegqi/ceg_dt_instantiator.h"

using namespace std;
using namespace CVC4::kind;
using namespace CVC4::context;

namespace CVC4 {
namespace theory {
namespace quantifiers {
  
void DtInstantiator::reset(CegInstantiator* ci,
                           SolvedForm& sf,
                           Node pv,
                           CegInstEffort effort)
{
}

Node DtInstantiator::solve_dt( Node v, Node a, Node b, Node sa, Node sb ) {
  Trace("cegqi-arith-debug2") << "Solve dt : " << v << " " << a << " " << b << " " << sa << " " << sb << std::endl;
  Node ret;
  if( !a.isNull() && a==v ){
    ret = sb;
  }else if( !b.isNull() && b==v ){
    ret = sa;
  }else if( !a.isNull() && a.getKind()==APPLY_CONSTRUCTOR ){
    if( !b.isNull() && b.getKind()==APPLY_CONSTRUCTOR ){
      if( a.getOperator()==b.getOperator() ){
        for( unsigned i=0; i<a.getNumChildren(); i++ ){
          Node s = solve_dt( v, a[i], b[i], sa[i], sb[i] );
          if( !s.isNull() ){
            return s;
          }
        }
      }
    }else{
      unsigned cindex = Datatype::indexOf( a.getOperator().toExpr() );
      TypeNode tn = a.getType();
      const Datatype& dt = ((DatatypeType)(tn).toType()).getDatatype();
      for( unsigned i=0; i<a.getNumChildren(); i++ ){
        Node nn = NodeManager::currentNM()->mkNode( APPLY_SELECTOR_TOTAL, Node::fromExpr( dt[cindex].getSelectorInternal( tn.toType(), i ) ), sb );
        Node s = solve_dt( v, a[i], Node::null(), sa[i], nn );
        if( !s.isNull() ){
          return s;
        }
      }
    }
  }else if( !b.isNull() && b.getKind()==APPLY_CONSTRUCTOR ){
    return solve_dt( v, b, a, sb, sa );
  }
  if( !ret.isNull() ){
    //ensure does not contain
    if (ret.hasSubterm(v))
    {
      ret = Node::null();
    }
  }
  return ret;
}

bool DtInstantiator::processEqualTerms(CegInstantiator* ci,
                                       SolvedForm& sf,
                                       Node pv,
                                       std::vector<Node>& eqc,
                                       CegInstEffort effort)
{
  Trace("cegqi-dt-debug") << "try based on constructors in equivalence class."
                          << std::endl;
  // look in equivalence class for a constructor
  for( unsigned k=0; k<eqc.size(); k++ ){
    Node n = eqc[k];
    if( n.getKind()==APPLY_CONSTRUCTOR ){
      Trace("cegqi-dt-debug") << "...try based on constructor term " << n << std::endl;
      std::vector< Node > children;
      children.push_back( n.getOperator() );
      const Datatype& dt = ((DatatypeType)(d_type).toType()).getDatatype();
      unsigned cindex = Datatype::indexOf( n.getOperator().toExpr() );
      //now must solve for selectors applied to pv
      for( unsigned j=0; j<dt[cindex].getNumArgs(); j++ ){
        Node c = NodeManager::currentNM()->mkNode( APPLY_SELECTOR_TOTAL, Node::fromExpr( dt[cindex].getSelectorInternal( d_type.toType(), j ) ), pv );
        ci->pushStackVariable( c );
        children.push_back( c );
      }
      Node val = NodeManager::currentNM()->mkNode( kind::APPLY_CONSTRUCTOR, children );
      TermProperties pv_prop_dt;
      if (ci->constructInstantiationInc(pv, val, pv_prop_dt, sf))
      {
        return true;
      }else{
        //cleanup
        for( unsigned j=0; j<dt[cindex].getNumArgs(); j++ ){
          ci->popStackVariable();
        }
        break;
      }
    }
  }
  return false;
}

bool DtInstantiator::processEquality(CegInstantiator* ci,
                                     SolvedForm& sf,
                                     Node pv,
                                     std::vector<TermProperties>& term_props,
                                     std::vector<Node>& terms,
                                     CegInstEffort effort)
{
  Node val = solve_dt( pv, terms[0], terms[1], terms[0], terms[1] );
  if( !val.isNull() ){
    TermProperties pv_prop;
    if (ci->constructInstantiationInc(pv, val, pv_prop, sf))
    {
      return true;
    }
  }
  return false;
}

} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */
