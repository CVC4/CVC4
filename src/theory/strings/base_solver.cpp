/*********************                                                        */
/*! \file base_solver.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Base solver for the theory of strings. This class implements term
 ** indexing and constant inference for the theory of strings.
 **/

#include "theory/strings/base_solver.h"

#include "theory/strings/theory_strings_utils.h"
#include "options/strings_options.h"
#include "theory/strings/theory_strings_rewriter.h"


using namespace std;
using namespace CVC4::context;
using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace strings {

BaseSolver::BaseSolver(context::Context* c, context::UserContext* u, SolverState& s, InferenceManager& im) :
d_state(s), d_im(im),
d_congruent(c)
{
  d_emptyString = NodeManager::currentNM()->mkConst( ::CVC4::String("") );
  d_false = NodeManager::currentNM()->mkConst( false );
}

BaseSolver::~BaseSolver() {

}

void BaseSolver::checkInit() {
  //build term index
  d_eqcToConst.clear();
  d_eqcToConst_base.clear();
  d_eqcToConst_exp.clear();
  d_termIndex.clear();
  d_stringsEqc.clear();

  std::map< Kind, unsigned > ncongruent;
  std::map< Kind, unsigned > congruent;
  eq::EqualityEngine* ee = d_state.getEqualityEngine();
  Assert(d_state.getRepresentative(d_emptyString)==d_emptyString);
  eq::EqClassesIterator eqcs_i = eq::EqClassesIterator( ee );
  while( !eqcs_i.isFinished() ){
    Node eqc = (*eqcs_i);
    TypeNode tn = eqc.getType();
    if( !tn.isRegExp() ){
      if( tn.isString() ){
        d_stringsEqc.push_back( eqc );
      }
      Node var;
      eq::EqClassIterator eqc_i = eq::EqClassIterator( eqc, ee );
      while( !eqc_i.isFinished() ) {
        Node n = *eqc_i;
        if( n.isConst() ){
          d_eqcToConst[eqc] = n;
          d_eqcToConst_base[eqc] = n;
          d_eqcToConst_exp[eqc] = Node::null();
        }else if( tn.isInteger() ){
          // do nothing
        }else if( n.getNumChildren()>0 ){
          Kind k = n.getKind();
          if( k!=EQUAL ){
            if( d_congruent.find( n )==d_congruent.end() ){
              std::vector< Node > c;
              Node nc = d_termIndex[k].add(n, 0, d_state, d_emptyString, c);
              if( nc!=n ){
                //check if we have inferred a new equality by removal of empty components
                if (n.getKind() == STRING_CONCAT
                    && !d_state.areEqual(nc, n))
                {
                  std::vector< Node > exp;
                  unsigned count[2] = { 0, 0 };
                  while( count[0]<nc.getNumChildren() || count[1]<n.getNumChildren() ){
                    //explain empty prefixes
                    for( unsigned t=0; t<2; t++ ){
                      Node nn = t==0 ? nc : n;
                      while (
                          count[t] < nn.getNumChildren()
                          && (nn[count[t]] == d_emptyString
                              || d_state.areEqual(nn[count[t]], d_emptyString)))
                      {
                        if( nn[count[t]]!=d_emptyString ){
                          exp.push_back( nn[count[t]].eqNode( d_emptyString ) );
                        }
                        count[t]++;
                      }
                    }
                    //explain equal components
                    if( count[0]<nc.getNumChildren() ){
                      Assert(count[1] < n.getNumChildren());
                      if( nc[count[0]]!=n[count[1]] ){
                        exp.push_back( nc[count[0]].eqNode( n[count[1]] ) );
                      }
                      count[0]++;
                      count[1]++;
                    }
                  }
                  //infer the equality
                  d_im.sendInference(exp, n.eqNode(nc), "I_Norm");
                }
                else
                {
                  //mark as congruent : only process if neither has been reduced
                  d_im.markCongruent( nc, n );
                }
                //this node is congruent to another one, we can ignore it
                Trace("strings-process-debug")
                    << "  congruent term : " << n << " (via " << nc << ")"
                    << std::endl;
                d_congruent.insert( n );
                congruent[k]++;
              }else if( k==STRING_CONCAT && c.size()==1 ){
                Trace("strings-process-debug") << "  congruent term by singular : " << n << " " << c[0] << std::endl;
                //singular case
                if (!d_state.areEqual(c[0], n))
                {
                  Node ns;
                  std::vector< Node > exp;
                  //explain empty components
                  bool foundNEmpty = false;
                  for( unsigned i=0; i<n.getNumChildren(); i++ ){
                    if (d_state.areEqual(n[i], d_emptyString))
                    {
                      if( n[i]!=d_emptyString ){
                        exp.push_back( n[i].eqNode( d_emptyString ) );
                      }
                    }
                    else
                    {
                      Assert(!foundNEmpty);
                      ns = n[i];
                      foundNEmpty = true;
                    }
                  }
                  AlwaysAssert(foundNEmpty);
                  //infer the equality
                  d_im.sendInference(exp, n.eqNode(ns), "I_Norm_S");
                }
                d_congruent.insert( n );
                congruent[k]++;
              }else{
                ncongruent[k]++;
              }
            }else{
              congruent[k]++;
            }
          }
        }else{
          if( d_congruent.find( n )==d_congruent.end() ){
            // We mark all but the oldest variable in the equivalence class as
            // congruent.
            if (var.isNull())
            {
              var = n;
            }
            else if (var > n)
            {
              Trace("strings-process-debug")
                  << "  congruent variable : " << var << std::endl;
              d_congruent.insert(var);
              var = n;
            }
            else
            {
              Trace("strings-process-debug")
                  << "  congruent variable : " << n << std::endl;
              d_congruent.insert(n);
            }
          }
        }
        ++eqc_i;
      }
    }
    ++eqcs_i;
  }
  if( Trace.isOn("strings-process") ){
    for( std::map< Kind, TermIndex >::iterator it = d_termIndex.begin(); it != d_termIndex.end(); ++it ){
      Trace("strings-process") << "  Terms[" << it->first << "] = " << ncongruent[it->first] << "/" << (congruent[it->first]+ncongruent[it->first]) << std::endl;
    }
  }
}

void BaseSolver::checkConstantEquivalenceClasses()
{
  // do fixed point
  unsigned prevSize;
  std::vector<Node> vecc;
  do
  {
    vecc.clear();
    Trace("strings-process-debug") << "Check constant equivalence classes..."
                                   << std::endl;
    prevSize = d_eqcToConst.size();
    checkConstantEquivalenceClasses(&d_termIndex[STRING_CONCAT], vecc);
  } while (!d_im.hasProcessed() && d_eqcToConst.size() > prevSize);
}

void BaseSolver::checkConstantEquivalenceClasses( TermIndex* ti, std::vector< Node >& vecc ) {
  Node n = ti->d_data;
  if( !n.isNull() ){
    //construct the constant
    Node c = utils::mkNConcat(vecc);
    if (!d_state.areEqual(n, c))
    {
      Trace("strings-debug") << "Constant eqc : " << c << " for " << n << std::endl;
      Trace("strings-debug") << "  ";
      for( unsigned i=0; i<vecc.size(); i++ ){
        Trace("strings-debug") << vecc[i] << " ";
      }
      Trace("strings-debug") << std::endl;
      unsigned count = 0;
      unsigned countc = 0;
      std::vector< Node > exp;
      while( count<n.getNumChildren() ){
        while (count < n.getNumChildren()
               && d_state.areEqual(n[count], d_emptyString))
        {
          d_im.addToExplanation(n[count], d_emptyString, exp);
          count++;
        }
        if( count<n.getNumChildren() ){
          Trace("strings-debug") << "...explain " << n[count] << " " << vecc[countc] << std::endl;
          if (!d_state.areEqual(n[count], vecc[countc]))
          {
            Node nrr = d_state.getRepresentative(n[count]);
            Assert(!d_eqcToConst_exp[nrr].isNull());
            d_im.addToExplanation(n[count], d_eqcToConst_base[nrr], exp);
            exp.push_back( d_eqcToConst_exp[nrr] );
          }
          else
          {
            d_im.addToExplanation(n[count], vecc[countc], exp);
          }
          countc++;
          count++;
        }
      }
      //exp contains an explanation of n==c
      Assert(countc == vecc.size());
      if (d_state.hasTerm(c))
      {
        d_im.sendInference(exp, n.eqNode(c), "I_CONST_MERGE");
        return;
      }
      else if (!d_im.hasProcessed())
      {
        Node nr = d_state.getRepresentative(n);
        std::map< Node, Node >::iterator it = d_eqcToConst.find( nr );
        if( it==d_eqcToConst.end() ){
          Trace("strings-debug") << "Set eqc const " << n << " to " << c << std::endl;
          d_eqcToConst[nr] = c;
          d_eqcToConst_base[nr] = n;
          d_eqcToConst_exp[nr] = utils::mkAnd(exp);
        }else if( c!=it->second ){
          //conflict
          Trace("strings-debug") << "Conflict, other constant was " << it->second << ", this constant was " << c << std::endl;
          if( d_eqcToConst_exp[nr].isNull() ){
            // n==c ^ n == c' => false
            d_im.addToExplanation(n, it->second, exp);
          }else{
            // n==c ^ n == d_eqcToConst_base[nr] == c' => false
            exp.push_back( d_eqcToConst_exp[nr] );
            d_im.addToExplanation(n, d_eqcToConst_base[nr], exp);
          }
          d_im.sendInference(exp, d_false, "I_CONST_CONFLICT");
          return;
        }else{
          Trace("strings-debug") << "Duplicate constant." << std::endl;
        }
      }
    }
  }
  for( std::map< TNode, TermIndex >::iterator it = ti->d_children.begin(); it != ti->d_children.end(); ++it ){
    std::map< Node, Node >::iterator itc = d_eqcToConst.find( it->first );
    if( itc!=d_eqcToConst.end() ){
      vecc.push_back( itc->second );
      checkConstantEquivalenceClasses( &it->second, vecc );
      vecc.pop_back();
      if (d_im.hasProcessed())
      {
        break;
      }
    }
  }
}


Node BaseSolver::getConstantEqc( Node eqc ) {
  std::map< Node, Node >::iterator it = d_eqcToConst.find( eqc );
  if( it!=d_eqcToConst.end() ){
    return it->second;
  }
  return Node::null();
}

const std::vector<Node>& BaseSolver::getStringEqc() const
{
  return d_stringsEqc;
}

Node BaseSolver::TermIndex::add(TNode n,
                                   unsigned index,
                                   const SolverState& s,
                                   Node er,
                                   std::vector<Node>& c)
{
  if( index==n.getNumChildren() ){
    if( d_data.isNull() ){
      d_data = n;
    }
    return d_data;
  }
  Assert(index < n.getNumChildren());
  TNode nir = s.getRepresentative(n[index]);
  //if it is empty, and doing CONCAT, ignore
  if( nir==er && n.getKind()==STRING_CONCAT ){
    return add(n, index + 1, s, er, c);
  }
  c.push_back( nir );
  return d_children[nir].add(n, index + 1, s, er, c);
}


}/* CVC4::theory::strings namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */
