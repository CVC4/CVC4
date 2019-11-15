/*********************                                                        */
/*! \file sygus_type_constructor.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Haniel Barbosa, Andrew Reynolds, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief implementation of class for for simplifying SyGuS grammars after they
 ** are encoded into datatypes.
 **/

#include "theory/quantifiers/sygus/sygus_type_constructor.h"

#include "expr/node_manager_attributes.h"
#include "printer/sygus_print_callback.h"
#include "smt/smt_engine.h"
#include "smt/smt_engine_scope.h"
#include "theory/datatypes/theory_datatypes_utils.h"
#include "theory/rewriter.h"

using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace quantifiers {

SygusTypeConstructor::SygusTypeConstructor(TypeNode src_tn, TypeNode unres_tn)
    : d_tn(src_tn),
      d_unres_tn(unres_tn),
      d_dt(Datatype(unres_tn.getAttribute(expr::VarNameAttr())))
{
}

Kind SygusTypeConstructor::getEliminateKind(Kind ok)
{
  Kind nk = ok;
  // We also must ensure that builtin operators which are eliminated
  // during expand definitions are replaced by the proper operator.
  if (ok == BITVECTOR_UDIV)
  {
    nk = BITVECTOR_UDIV_TOTAL;
  }
  else if (ok == BITVECTOR_UREM)
  {
    nk = BITVECTOR_UREM_TOTAL;
  }
  else if (ok == DIVISION)
  {
    nk = DIVISION_TOTAL;
  }
  else if (ok == INTS_DIVISION)
  {
    nk = INTS_DIVISION_TOTAL;
  }
  else if (ok == INTS_MODULUS)
  {
    nk = INTS_MODULUS_TOTAL;
  }
  return nk;
}

Node SygusTypeConstructor::eliminatePartialOperators(Node n)
{
  NodeManager* nm = NodeManager::currentNM();
  std::unordered_map<TNode, Node, TNodeHashFunction> visited;
  std::unordered_map<TNode, Node, TNodeHashFunction>::iterator it;
  std::vector<TNode> visit;
  TNode cur;
  visit.push_back(n);
  do
  {
    cur = visit.back();
    visit.pop_back();
    it = visited.find(cur);

    if (it == visited.end())
    {
      visited[cur] = Node::null();
      visit.push_back(cur);
      for (const Node& cn : cur)
      {
        visit.push_back(cn);
      }
    }
    else if (it->second.isNull())
    {
      Node ret = cur;
      bool childChanged = false;
      std::vector<Node> children;
      if (cur.getMetaKind() == metakind::PARAMETERIZED)
      {
        children.push_back(cur.getOperator());
      }
      for (const Node& cn : cur)
      {
        it = visited.find(cn);
        Assert(it != visited.end());
        Assert(!it->second.isNull());
        childChanged = childChanged || cn != it->second;
        children.push_back(it->second);
      }
      Kind ok = cur.getKind();
      Kind nk = getEliminateKind(ok);
      if (nk != ok || childChanged)
      {
        ret = nm->mkNode(nk, children);
      }
      visited[cur] = ret;
    }
  } while (!visit.empty());
  Assert(visited.find(n) != visited.end());
  Assert(!visited.find(n)->second.isNull());
  return visited[n];
}

void SygusTypeConstructor::addConsInfo(const DatatypeConstructor& cons,
                                       std::vector<TypeNode>& consTypes)
{
  Trace("sygus-type-cons") << "...for " << cons.getName() << "\n";
  /* Recover the sygus operator to not lose reference to the original
   * operator (NOT, ITE, etc) */
  Node sygus_op = Node::fromExpr(cons.getSygusOp());
  Trace("sygus-type-cons-debug")
      << ".....operator is " << sygus_op << std::endl;
  Node exp_sop_n = sygus_op;
  if (exp_sop_n.isConst())
  {
    // If it is a builtin operator, convert to total version if necessary.
    // First, get the kind for the operator.
    Kind ok = NodeManager::operatorToKind(exp_sop_n);
    Trace("sygus-type-cons-debug") << "...builtin kind is " << ok << std::endl;
    Kind nk = getEliminateKind(ok);
    if (nk != ok)
    {
      Trace("sygus-type-cons-debug")
          << "...replace by builtin operator " << nk << std::endl;
      exp_sop_n = NodeManager::currentNM()->operatorOf(nk);
    }
  }
  else
  {
    // Only expand definitions if the operator is not constant, since calling
    // expandDefinitions on them should be a no-op. This check ensures we don't
    // try to expand e.g. bitvector extract operators, whose type is undefined,
    // and thus should not be passed to expandDefinitions.
    exp_sop_n = Node::fromExpr(
        smt::currentSmtEngine()->expandDefinitions(sygus_op.toExpr()));
    exp_sop_n = Rewriter::rewrite(exp_sop_n);
    Trace("sygus-type-cons-debug")
        << ".....operator (post-rewrite) is " << exp_sop_n << std::endl;
    // eliminate all partial operators from it
    exp_sop_n = eliminatePartialOperators(exp_sop_n);
    Trace("sygus-type-cons-debug")
        << ".....operator (eliminate partial operators) is " << exp_sop_n
        << std::endl;
    // rewrite again
    exp_sop_n = Rewriter::rewrite(exp_sop_n);
  }

  d_ops.push_back(exp_sop_n);
  Trace("sygus-type-cons-defs")
      << "\tOriginal op: " << cons.getSygusOp()
      << "\n\tExpanded one: " << exp_sop_n
      << "\n\tRewritten one: " << d_ops.back() << "\n\n";
  d_cons_names.push_back(cons.getName());
  d_pc.push_back(cons.getSygusPrintCallback());
  d_weight.push_back(cons.getWeight());
  d_cons_args_t.push_back(consTypes);
}

void SygusTypeConstructor::addAnyConstantConstructor(TypeNode tn)
{
  // add an "any constant" proxy variable
  Node av = NodeManager::currentNM()->mkSkolem("_any_constant", tn);
  // mark that it represents any constant
  SygusAnyConstAttribute saca;
  av.setAttribute(saca, true);
  std::stringstream ss;
  ss << d_unres_tn << "_any_constant";
  std::string cname(ss.str());
  std::vector<TypeNode> builtin_arg;
  builtin_arg.push_back(tn);
  // we add this constructor first since we use left associative chains
  // and our symmetry breaking should group any constants together
  // beneath the same application
  // we set its weight to zero since it should be considered at the
  // same level as constants.
  d_ops.insert(d_ops.begin(), av.toExpr());
  d_cons_names.insert(d_cons_names.begin(), cname);
  d_cons_args_t.insert(d_cons_args_t.begin(), builtin_arg);
  d_pc.insert(d_pc.begin(), printer::SygusEmptyPrintCallback::getEmptyPC());
  d_weight.insert(d_weight.begin(), 0);
}

void SygusTypeConstructor::buildDatatype(Node sygusVars,
                                         const Datatype& dt,
                                         std::vector<Datatype>& dts,
                                         std::vector<TypeNode>& unres)
{
  /* Use the sygus type to not lose reference to the original types (Bool,
   * Int, etc) */
  d_dt.setSygus(dt.getSygusType(),
                sygusVars.toExpr(),
                dt.getSygusAllowConst(),
                dt.getSygusAllowAll());
  for (unsigned i = 0, size_d_ops = d_ops.size(); i < size_d_ops; ++i)
  {
    // must convert to type now
    std::vector<Type> cargs;
    for (TypeNode& ct : d_cons_args_t[i])
    {
      cargs.push_back(ct.toType());
    }
    // add (sygus) constructor
    d_dt.addSygusConstructor(
        d_ops[i].toExpr(), d_cons_names[i], cargs, d_pc[i], d_weight[i]);
  }
  Trace("sygus-type-cons") << "...built datatype " << d_dt << " ";
  /* Add to global accumulators */
  dts.push_back(d_dt);
  unres.push_back(d_unres_tn);
  Trace("sygus-type-cons") << "---------------------------------\n";
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC4
