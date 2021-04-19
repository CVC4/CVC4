/******************************************************************************
 * Top contributors (to current version):
 *   Andrew Reynolds, Yoni Zohar, Haniel Barbosa
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Utility for quantifiers macro definitions.
 */

#include "theory/quantifiers/quantifier_macros.h"

#include "expr/node_algorithm.h"
#include "expr/skolem_manager.h"
#include "options/quantifiers_options.h"
#include "proof/proof_manager.h"
#include "theory/arith/arith_msum.h"
#include "theory/quantifiers/ematching/pattern_term_selector.h"
#include "theory/quantifiers/quantifiers_registry.h"
#include "theory/quantifiers/term_database.h"
#include "theory/quantifiers/term_util.h"
#include "theory/rewriter.h"

using namespace cvc5::kind;

namespace cvc5 {
namespace quantifiers {
namespace passes {

QuantifierMacros::QuantifierMacros(QuantifiersRegistry& qr) : d_qreg(qr)
{
}

bool QuantifierMacros::containsBadOp( Node n, Node op, bool reqGround ){
  std::unordered_set<TNode, TNodeHashFunction> visited;
  std::unordered_set<TNode, TNodeHashFunction>::iterator it;
  std::vector<TNode> visit;
  TNode cur;
  visit.push_back(n);
  do {
    cur = visit.back();
    visit.pop_back();
    it = visited.find(cur);
    if (it == visited.end()) {
      visited.insert(cur);
      if (cur.isClosure() && reqGround)
      {
        return true;
      }
      else if (cur==op)
      {
        return true;
      }
      visit.insert(visit.end(),cur.begin(),cur.end());
    }
  } while (!visit.empty());
  return false;
}

bool QuantifierMacros::isGroundUfTerm(Node q, Node n)
{
  Node icn = d_preprocContext->getTheoryEngine()
                 ->getQuantifiersEngine()
                 ->getQuantifiersRegistry()
                 .substituteBoundVariablesToInstConstants(n, q);
  Trace("macros-debug2") << "Get free variables in " << icn << std::endl;
  std::vector< Node > var;
  quantifiers::TermUtil::computeInstConstContainsForQuant(q, icn, var);
  Trace("macros-debug2") << "Get trigger variables for " << icn << std::endl;
  std::vector< Node > trigger_var;
  inst::PatternTermSelector::getTriggerVariables(icn, q, trigger_var);
  Trace("macros-debug2") << "Done." << std::endl;
  //only if all variables are also trigger variables
  return trigger_var.size()>=var.size();
}

bool QuantifierMacros::isBoundVarApplyUf( Node n ) {
  Assert(n.getKind() == APPLY_UF);
  TypeNode tno = n.getOperator().getType();
  std::map< Node, bool > vars;
  // allow if a vector of unique variables of the same type as UF arguments
  for (size_t i = 0, nchild = n.getNumChildren(); i < nchild; i++)
  {
    if( n[i].getKind()!=BOUND_VARIABLE ){
      return false;
    }
    if( n[i].getType()!=tno[i] ){
      return false;
    }
    if( vars.find( n[i] )==vars.end() ){
      vars[n[i]] = true;
    }else{
      return false;
    }
  }
  return true;
}

void QuantifierMacros::getMacroCandidates( Node n, std::vector< Node >& candidates, std::map< Node, bool >& visited ){
  if( visited.find( n )==visited.end() ){
    visited[n] = true;
    if( n.getKind()==APPLY_UF ){
      if( isBoundVarApplyUf( n ) ){
        candidates.push_back( n );
      }
    }else if( n.getKind()==PLUS ){
      for( size_t i=0; i<n.getNumChildren(); i++ ){
        getMacroCandidates( n[i], candidates, visited );
      }
    }else if( n.getKind()==MULT ){
      //if the LHS is a constant
      if( n.getNumChildren()==2 && n[0].isConst() ){
        getMacroCandidates( n[1], candidates, visited );
      }
    }else if( n.getKind()==NOT ){
      getMacroCandidates( n[0], candidates, visited );
    }
  }
}

Node QuantifierMacros::solveInEquality( Node n, Node lit ){
  if( lit.getKind()==EQUAL ){
    //return the opposite side of the equality if defined that way
    for( int i=0; i<2; i++ ){
      if( lit[i]==n ){
        return lit[i==0 ? 1 : 0];
      }else if( lit[i].getKind()==NOT && lit[i][0]==n ){
        return lit[i==0 ? 1 : 0].negate();
      }
    }
    std::map<Node, Node> msum;
    if (ArithMSum::getMonomialSumLit(lit, msum))
    {
      Node veq_c;
      Node val;
      int res = ArithMSum::isolate(n, msum, veq_c, val, EQUAL);
      if (res != 0 && veq_c.isNull())
      {
        return val;
      }
    }
  }
  Trace("macros-debug") << "Cannot find for " << lit << " " << n << std::endl;
  return Node::null();
}

Node QuantifierMacros::solve( Node lit ){
  Trace("macros-debug") << "  process " << n << std::endl;
  bool pol = lit.getKind()!=NOT;
  Node n = pol ? lit : lit[0];
  NodeManager* nm = NodeManager::currentNM();
  if( n.getKind()==APPLY_UF ){
    //predicate case
    if( isBoundVarApplyUf( n ) ){
      Node op = n.getOperator();
      Node n_def = nm->mkConst(pol);
      Node fdef = addMacroEq(n, n_def);
      Assert (!fdef.isNull();
      return fdef;
    }
  }
  else if (pol && n.getKind() == EQUAL)
  {
    //literal case
    Trace("macros-debug") << "Check macro literal : " << n << std::endl;
    std::map<Node, bool> visited;
    std::vector<Node> candidates;
    for (const Node& nc : n)
    {
      getMacroCandidates(nc, candidates, visited);
    }
    for (const Node& m : candidates)
    {
      Node op = m.getOperator();
      Trace("macros-debug") << "Check macro candidate : " << m << std::endl;
      if (d_macroDefs.find(op) != d_macroDefs.end())
      {
        continue;
      }
      // get definition and condition
      Node n_def = solveInEquality(m, n);  // definition for the macro
      if (n_def.isNull())
      {
        continue;
      }
      Trace("macros-debug") << m << " is possible macro in " << lit << std::endl;
      Trace("macros-debug")
          << "  corresponding definition is : " << n_def << std::endl;
      visited.clear();
      // cannot contain a defined operator
      if (!containsBadOp(n_def, op, reqGround))
      {
        Trace("macros-debug")
            << "...does not contain bad (recursive) operator." << std::endl;
        // must be ground UF term if mode is GROUND_UF
        if (options::macrosQuantMode() != options::MacrosQuantMode::GROUND_UF
            || isGroundUfTerm(lit, n_def))
        {
          Trace("macros-debug")
              << "...respects ground-uf constraint." << std::endl;
          Node fdef = addMacroEq(m, n_def);
          if (!fdef.isNull())
          {
            return fdef;
          }
        }
      }
    }
  }
  return Node::null();
}

Node QuantifierMacros::addMacroEq(Node n, Node ndef)
{
  Assert(n.getKind() == APPLY_UF);
  NodeManager* nm = NodeManager::currentNM();
  Trace("macros-debug") << "Add macro eq for " << n << std::endl;
  Trace("macros-debug") << "  def: " << ndef << std::endl;
  std::vector<Node> vars;
  std::vector<Node> fvars;
  for (const Node& nc : n)
  {
    vars.push_back(nc);
    Node v = nm->mkBoundVar(nc.getType());
    fvars.push_back(v);
  }
  Node fdef =
      ndef.substitute(vars.begin(), vars.end(), fvars.begin(), fvars.end());
  fdef = nm->mkNode(LAMBDA, nm->mkNode(BOUND_VAR_LIST, fvars), fdef);
  // If the definition has a free variable, it is malformed. This can happen
  // if the right hand side of a macro definition contains a variable not
  // contained in the left hand side
  if (expr::hasFreeVar(fdef))
  {
    return Node::null();
  }
  TNode op = n.getOperator();
  TNode fdeft = fdef;
  Assert(op.getType().isComparableTo(fdef.getType()));
  return op.eqNode(fdef);
}

}  // passes
}  // preprocessing
}  // namespace cvc5
