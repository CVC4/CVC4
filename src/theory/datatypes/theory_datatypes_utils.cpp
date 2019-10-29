/*********************                                                        */
/*! \file theory_datatypes_utils.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Morgan Deters, Paul Meng
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of rewriter for the theory of (co)inductive datatypes.
 **
 ** Implementation of rewriter for the theory of (co)inductive datatypes.
 **/

#include "theory/datatypes/theory_datatypes_utils.h"

#include "expr/node_algorithm.h"

using namespace CVC4;
using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace datatypes {
namespace utils {

Node applySygusArgs(const Datatype& dt,
                    Node op,
                    Node n,
                    const std::vector<Node>& args)
{
  if (n.getKind() == BOUND_VARIABLE)
  {
    CVC4_DCHECK(n.hasAttribute(SygusVarNumAttribute()));
    int vn = n.getAttribute(SygusVarNumAttribute());
    CVC4_DCHECK(Node::fromExpr(dt.getSygusVarList())[vn] == n);
    return args[vn];
  }
  // n is an application of operator op.
  // We must compute the free variables in op to determine if there are
  // any substitutions we need to make to n.
  TNode val;
  if (!op.hasAttribute(SygusVarFreeAttribute()))
  {
    std::unordered_set<Node, NodeHashFunction> fvs;
    if (expr::getFreeVariables(op, fvs))
    {
      if (fvs.size() == 1)
      {
        for (const Node& v : fvs)
        {
          val = v;
        }
      }
      else
      {
        val = op;
      }
    }
    Trace("dt-sygus-fv") << "Free var in " << op << " : " << val << std::endl;
    op.setAttribute(SygusVarFreeAttribute(), val);
  }
  else
  {
    val = op.getAttribute(SygusVarFreeAttribute());
  }
  if (val.isNull())
  {
    return n;
  }
  if (val.getKind() == BOUND_VARIABLE)
  {
    // single substitution case
    int vn = val.getAttribute(SygusVarNumAttribute());
    TNode sub = args[vn];
    return n.substitute(val, sub);
  }
  // do the full substitution
  std::vector<Node> vars;
  Node bvl = Node::fromExpr(dt.getSygusVarList());
  for (unsigned i = 0, nvars = bvl.getNumChildren(); i < nvars; i++)
  {
    vars.push_back(bvl[i]);
  }
  return n.substitute(vars.begin(), vars.end(), args.begin(), args.end());
}

Kind getOperatorKindForSygusBuiltin(Node op)
{
  CVC4_DCHECK(op.getKind() != BUILTIN);
  if (op.getKind() == LAMBDA)
  {
    return APPLY_UF;
  }
  TypeNode tn = op.getType();
  if (tn.isConstructor())
  {
    return APPLY_CONSTRUCTOR;
  }
  else if (tn.isSelector())
  {
    return APPLY_SELECTOR;
  }
  else if (tn.isTester())
  {
    return APPLY_TESTER;
  }
  else if (tn.isFunction())
  {
    return APPLY_UF;
  }
  return UNDEFINED_KIND;
}

Node mkSygusTerm(const Datatype& dt,
                 unsigned i,
                 const std::vector<Node>& children)
{
  Trace("dt-sygus-util") << "Make sygus term " << dt.getName() << "[" << i
                         << "] with children: " << children << std::endl;
  CVC4_DCHECK(i < dt.getNumConstructors());
  CVC4_DCHECK(dt.isSygus());
  CVC4_DCHECK(!dt[i].getSygusOp().isNull());
  std::vector<Node> schildren;
  Node op = Node::fromExpr(dt[i].getSygusOp());
  Trace("dt-sygus-util") << "Operator is " << op << std::endl;
  if (children.empty())
  {
    // no children, return immediately
    Trace("dt-sygus-util") << "...return direct op" << std::endl;
    return op;
  }
  // if it is the any constant, we simply return the child
  if (op.getAttribute(SygusAnyConstAttribute()))
  {
    CVC4_DCHECK(children.size() == 1);
    return children[0];
  }
  if (op.getKind() != BUILTIN)
  {
    schildren.push_back(op);
  }
  schildren.insert(schildren.end(), children.begin(), children.end());
  Node ret;
  if (op.getKind() == BUILTIN)
  {
    ret = NodeManager::currentNM()->mkNode(op, schildren);
    Trace("dt-sygus-util") << "...return (builtin) " << ret << std::endl;
    return ret;
  }
  Kind ok = NodeManager::operatorToKind(op);
  Trace("dt-sygus-util") << "operator kind is " << ok << std::endl;
  if (ok != UNDEFINED_KIND)
  {
    // If it is an APPLY_UF operator, we should have at least an operator and
    // a child.
    CVC4_DCHECK(ok != APPLY_UF || schildren.size() != 1);
    ret = NodeManager::currentNM()->mkNode(ok, schildren);
    Trace("dt-sygus-util") << "...return (op) " << ret << std::endl;
    return ret;
  }
  Kind tok = getOperatorKindForSygusBuiltin(op);
  if (schildren.size() == 1 && tok == UNDEFINED_KIND)
  {
    ret = schildren[0];
  }
  else
  {
    ret = NodeManager::currentNM()->mkNode(tok, schildren);
  }
  Trace("dt-sygus-util") << "...return " << ret << std::endl;
  return ret;
}

/** get instantiate cons */
Node getInstCons(Node n, const Datatype& dt, int index)
{
  CVC4_DCHECK(index >= 0 && index < (int)dt.getNumConstructors());
  std::vector<Node> children;
  NodeManager* nm = NodeManager::currentNM();
  children.push_back(Node::fromExpr(dt[index].getConstructor()));
  Type t = n.getType().toType();
  for (unsigned i = 0, nargs = dt[index].getNumArgs(); i < nargs; i++)
  {
    Node nc = nm->mkNode(APPLY_SELECTOR_TOTAL,
                         Node::fromExpr(dt[index].getSelectorInternal(t, i)),
                         n);
    children.push_back(nc);
  }
  Node n_ic = nm->mkNode(APPLY_CONSTRUCTOR, children);
  if (dt.isParametric())
  {
    TypeNode tn = TypeNode::fromType(t);
    // add type ascription for ambiguous constructor types
    if (!n_ic.getType().isComparableTo(tn))
    {
      Debug("datatypes-parametric")
          << "DtInstantiate: ambiguous type for " << n_ic << ", ascribe to "
          << n.getType() << std::endl;
      Debug("datatypes-parametric")
          << "Constructor is " << dt[index] << std::endl;
      Type tspec =
          dt[index].getSpecializedConstructorType(n.getType().toType());
      Debug("datatypes-parametric")
          << "Type specification is " << tspec << std::endl;
      children[0] = nm->mkNode(APPLY_TYPE_ASCRIPTION,
                               nm->mkConst(AscriptionType(tspec)),
                               children[0]);
      n_ic = nm->mkNode(APPLY_CONSTRUCTOR, children);
      CVC4_DCHECK(n_ic.getType() == tn);
    }
  }
  CVC4_DCHECK(isInstCons(n, n_ic, dt) == index);
  // n_ic = Rewriter::rewrite( n_ic );
  return n_ic;
}

int isInstCons(Node t, Node n, const Datatype& dt)
{
  if (n.getKind() == APPLY_CONSTRUCTOR)
  {
    int index = indexOf(n.getOperator());
    const DatatypeConstructor& c = dt[index];
    Type nt = n.getType().toType();
    for (unsigned i = 0, size = n.getNumChildren(); i < size; i++)
    {
      if (n[i].getKind() != APPLY_SELECTOR_TOTAL
          || n[i].getOperator() != Node::fromExpr(c.getSelectorInternal(nt, i))
          || n[i][0] != t)
      {
        return -1;
      }
    }
    return index;
  }
  return -1;
}

int isTester(Node n, Node& a)
{
  if (n.getKind() == APPLY_TESTER)
  {
    a = n[0];
    return indexOf(n.getOperator());
  }
  return -1;
}

int isTester(Node n)
{
  if (n.getKind() == APPLY_TESTER)
  {
    return indexOf(n.getOperator());
  }
  return -1;
}

struct DtIndexAttributeId
{
};
typedef expr::Attribute<DtIndexAttributeId, uint64_t> DtIndexAttribute;

unsigned indexOf(Node n)
{
  if (!n.hasAttribute(DtIndexAttribute()))
  {
    CVC4_DCHECK(n.getType().isConstructor() || n.getType().isTester()
           || n.getType().isSelector());
    unsigned index = Datatype::indexOfInternal(n.toExpr());
    n.setAttribute(DtIndexAttribute(), index);
    return index;
  }
  return n.getAttribute(DtIndexAttribute());
}

Node mkTester(Node n, int i, const Datatype& dt)
{
  return NodeManager::currentNM()->mkNode(
      APPLY_TESTER, Node::fromExpr(dt[i].getTester()), n);
}

Node mkSplit(Node n, const Datatype& dt)
{
  std::vector<Node> splits;
  for (unsigned i = 0, ncons = dt.getNumConstructors(); i < ncons; i++)
  {
    Node test = mkTester(n, i, dt);
    splits.push_back(test);
  }
  NodeManager* nm = NodeManager::currentNM();
  return splits.size() == 1 ? splits[0] : nm->mkNode(OR, splits);
}

bool isNullaryApplyConstructor(Node n)
{
  CVC4_DCHECK(n.getKind() == APPLY_CONSTRUCTOR);
  for (const Node& nc : n)
  {
    if (nc.getType().isDatatype())
    {
      return false;
    }
  }
  return true;
}

bool isNullaryConstructor(const DatatypeConstructor& c)
{
  for (unsigned j = 0, nargs = c.getNumArgs(); j < nargs; j++)
  {
    if (c[j].getType().getRangeType().isDatatype())
    {
      return false;
    }
  }
  return true;
}

bool checkClash(Node n1, Node n2, std::vector<Node>& rew)
{
  Trace("datatypes-rewrite-debug")
      << "Check clash : " << n1 << " " << n2 << std::endl;
  if (n1.getKind() == APPLY_CONSTRUCTOR && n2.getKind() == APPLY_CONSTRUCTOR)
  {
    if (n1.getOperator() != n2.getOperator())
    {
      Trace("datatypes-rewrite-debug")
          << "Clash operators : " << n1 << " " << n2 << " " << n1.getOperator()
          << " " << n2.getOperator() << std::endl;
      return true;
    }
    CVC4_DCHECK(n1.getNumChildren() == n2.getNumChildren());
    for (unsigned i = 0, size = n1.getNumChildren(); i < size; i++)
    {
      if (checkClash(n1[i], n2[i], rew))
      {
        return true;
      }
    }
  }
  else if (n1 != n2)
  {
    if (n1.isConst() && n2.isConst())
    {
      Trace("datatypes-rewrite-debug")
          << "Clash constants : " << n1 << " " << n2 << std::endl;
      return true;
    }
    else
    {
      Node eq = NodeManager::currentNM()->mkNode(EQUAL, n1, n2);
      rew.push_back(eq);
    }
  }
  return false;
}

}  // namespace utils
}  // namespace datatypes
}  // namespace theory
}  // namespace CVC4
