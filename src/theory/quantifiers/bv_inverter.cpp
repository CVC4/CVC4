/*********************                                                        */
/*! \file bv_inverter.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief inverse rules for bit-vector operators
 **/

#include "theory/quantifiers/bv_inverter.h"

#include <algorithm>
#include <stack>

#include "options/quantifiers_options.h"
#include "theory/quantifiers/term_util.h"
#include "theory/rewriter.h"
#include "theory/bv/theory_bv_utils.h"


using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace quantifiers {

/* Drop child at given index from expression.
 * E.g., dropChild((x + y + z), 1) -> (x + z)  */
static Node dropChild(Node n, unsigned index)
{
  unsigned nchildren = n.getNumChildren();
  Assert(index < nchildren);
  Kind k = n.getKind();
  Assert(k == BITVECTOR_AND || k == BITVECTOR_OR || k == BITVECTOR_MULT
         || k == BITVECTOR_PLUS);
  NodeBuilder<> nb(NodeManager::currentNM(), k);
  for (unsigned i = 0; i < nchildren; ++i)
  {
    if (i == index) continue;
    nb << n[i];
  }
  return nb.constructNode();
}

Node BvInverter::getSolveVariable(TypeNode tn)
{
  std::map<TypeNode, Node>::iterator its = d_solve_var.find(tn);
  if (its == d_solve_var.end())
  {
    std::stringstream ss;
    if (tn.isFunction())
    {
      Assert(tn.getNumChildren() == 2);
      Assert(tn[0].isBoolean());
      ss << "slv_f";
    }
    else
    {
      ss << "slv";
    }
    Node k = NodeManager::currentNM()->mkSkolem(ss.str(), tn);
    // marked as a virtual term (it is eligible for instantiation)
    VirtualTermSkolemAttribute vtsa;
    k.setAttribute(vtsa, true);
    d_solve_var[tn] = k;
    return k;
  }
  else
  {
    return its->second;
  }
}

Node BvInverter::getInversionSkolemFor(Node cond, TypeNode tn)
{
  // condition should be rewritten
  Assert(Rewriter::rewrite(cond) == cond);
  std::unordered_map<Node, Node, NodeHashFunction>::iterator it =
      d_inversion_skolem_cache.find(cond);
  if (it == d_inversion_skolem_cache.end())
  {
    Node skv;
    if (cond.getKind() == EQUAL)
    {
      // optimization : if condition is ( x = v ) should just return v and not
      // introduce a Skolem this can happen when we ask for the multiplicative
      // inversion with bv1
      Node x = getSolveVariable(tn);
      for (unsigned i = 0; i < 2; i++)
      {
        if (cond[i] == x)
        {
          skv = cond[1 - i];
          Trace("cegqi-bv-skvinv")
              << "SKVINV : " << skv << " is trivially associated with conditon "
              << cond << std::endl;
          break;
        }
      }
    }
    if (skv.isNull())
    {
      // TODO : compute the value if the condition is deterministic, e.g. calc
      // multiplicative inverse of 2 constants
      skv = NodeManager::currentNM()->mkSkolem(
          "skvinv", tn, "created for BvInverter");
      Trace("cegqi-bv-skvinv")
          << "SKVINV : " << skv << " is the skolem associated with conditon "
          << cond << std::endl;
      // marked as a virtual term (it is eligible for instantiation)
      VirtualTermSkolemAttribute vtsa;
      skv.setAttribute(vtsa, true);
    }
    d_inversion_skolem_cache[cond] = skv;
    return skv;
  }
  else
  {
    Assert(it->second.getType() == tn);
    return it->second;
  }
}

Node BvInverter::getInversionSkolemFunctionFor(TypeNode tn)
{
  NodeManager* nm = NodeManager::currentNM();
  // function maps conditions to skolems
  TypeNode ftn = nm->mkFunctionType(nm->booleanType(), tn);
  return getSolveVariable(ftn);
}

Node BvInverter::getInversionNode(Node cond, TypeNode tn)
{
  // condition should be rewritten
  Node new_cond = Rewriter::rewrite(cond);
  if (new_cond != cond)
  {
    Trace("bv-invert-debug") << "Condition " << cond << " was rewritten to "
                             << new_cond << std::endl;
  }
  Node f = getInversionSkolemFunctionFor(tn);
  return NodeManager::currentNM()->mkNode(kind::APPLY_UF, f, new_cond);
}

bool BvInverter::isInvertible(Kind k, unsigned index)
{
  return  k == NOT
      ||  k == EQUAL
      ||  k == BITVECTOR_ULT
      ||  k == BITVECTOR_SLT
      ||  k == BITVECTOR_COMP
      ||  k == BITVECTOR_NOT
      ||  k == BITVECTOR_NEG
      ||  k == BITVECTOR_CONCAT
      ||  k == BITVECTOR_SIGN_EXTEND
      ||  k == BITVECTOR_PLUS
      ||  k == BITVECTOR_SUB
      ||  k == BITVECTOR_MULT
      || (k == BITVECTOR_UREM_TOTAL && index == 1)
      ||  k == BITVECTOR_UDIV_TOTAL
      ||  k == BITVECTOR_AND
      ||  k == BITVECTOR_OR
      ||  k == BITVECTOR_XOR
      || (k == BITVECTOR_LSHR && index == 0)
      || (k == BITVECTOR_ASHR && index == 0)
      || (k == BITVECTOR_SHL && index == 0);
}

Node BvInverter::getPathToPv(
    Node lit,
    Node pv,
    Node sv,
    std::vector<unsigned>& path,
    std::unordered_set<TNode, TNodeHashFunction>& visited)
{
  if (visited.find(lit) == visited.end())
  {
    visited.insert(lit);
    if (lit == pv)
    {
      return sv;
    }
    else
    {
      unsigned rmod = 0;  // TODO : randomize?
      for (unsigned i = 0; i < lit.getNumChildren(); i++)
      {
        unsigned ii = (i + rmod) % lit.getNumChildren();
        // only recurse if the kind is invertible
        // this allows us to avoid paths that go through skolem functions
        if (!isInvertible(lit.getKind(), ii))
        {
          continue;
        }
        Node litc = getPathToPv(lit[ii], pv, sv, path, visited);
        if (!litc.isNull())
        {
          // path is outermost term index last
          path.push_back(ii);
          std::vector<Node> children;
          if (lit.getMetaKind() == kind::metakind::PARAMETERIZED)
          {
            children.push_back(lit.getOperator());
          }
          for (unsigned j = 0; j < lit.getNumChildren(); j++)
          {
            children.push_back(j == ii ? litc : lit[j]);
          }
          return NodeManager::currentNM()->mkNode(lit.getKind(), children);
        }
      }
    }
  }
  return Node::null();
}

Node BvInverter::eliminateSkolemFunctions(TNode n,
                                          std::vector<Node>& side_conditions)
{
  std::unordered_map<TNode, Node, TNodeHashFunction> visited;
  std::unordered_map<TNode, Node, TNodeHashFunction>::iterator it;
  std::stack<TNode> visit;
  TNode cur;

  visit.push(n);
  do
  {
    cur = visit.top();
    visit.pop();
    it = visited.find(cur);

    if (it == visited.end())
    {
      visited[cur] = Node::null();
      visit.push(cur);
      for (unsigned i = 0; i < cur.getNumChildren(); i++)
      {
        visit.push(cur[i]);
      }
    }
    else if (it->second.isNull())
    {
      Trace("bv-invert-debug")
          << "eliminateSkolemFunctions from " << cur << "..." << std::endl;

      Node ret = cur;
      bool childChanged = false;
      std::vector<Node> children;
      if (cur.getMetaKind() == kind::metakind::PARAMETERIZED)
      {
        children.push_back(cur.getOperator());
      }
      for (unsigned i = 0; i < cur.getNumChildren(); i++)
      {
        it = visited.find(cur[i]);
        Assert(it != visited.end());
        Assert(!it->second.isNull());
        childChanged = childChanged || cur[i] != it->second;
        children.push_back(it->second);
      }
      if (childChanged)
      {
        ret = NodeManager::currentNM()->mkNode(cur.getKind(), children);
      }
      // now, check if it is a skolem function
      if (ret.getKind() == APPLY_UF)
      {
        Node op = ret.getOperator();
        TypeNode tnp = op.getType();
        // is this a skolem function?
        std::map<TypeNode, Node>::iterator its = d_solve_var.find(tnp);
        if (its != d_solve_var.end() && its->second == op)
        {
          Assert(ret.getNumChildren() == 1);
          Assert(ret[0].getType().isBoolean());

          Node cond = ret[0];
          // must rewrite now to ensure we lookup the correct skolem
          cond = Rewriter::rewrite(cond);

          // if so, we replace by the (finalized) skolem variable
          // Notice that since we are post-rewriting, skolem functions are
          // already eliminated from cond
          ret = getInversionSkolemFor(cond, ret.getType());

          // also must add (substituted) side condition to vector
          // substitute ( solve variable -> inversion skolem )
          TNode solve_var = getSolveVariable(ret.getType());
          TNode tret = ret;
          cond = cond.substitute(solve_var, tret);
          if (std::find(side_conditions.begin(), side_conditions.end(), cond)
              == side_conditions.end())
          {
            side_conditions.push_back(cond);
          }
        }
      }
      Trace("bv-invert-debug") << "eliminateSkolemFunctions from " << cur
                               << " returned " << ret << std::endl;
      visited[cur] = ret;
    }
  } while (!visit.empty());
  Assert(visited.find(n) != visited.end());
  Assert(!visited.find(n)->second.isNull());
  return visited[n];
}

Node BvInverter::getPathToPv(
    Node lit, Node pv, Node sv, Node pvs, std::vector<unsigned>& path)
{
  std::unordered_set<TNode, TNodeHashFunction> visited;
  Node slit = getPathToPv(lit, pv, sv, path, visited);
  // if we are able to find a (invertible) path to pv
  if (!slit.isNull())
  {
    // substitute pvs for the other occurrences of pv
    TNode tpv = pv;
    TNode tpvs = pvs;
    slit = slit.substitute(tpv, tpvs);
  }
  return slit;
}

Node BvInverter::solve_bv_lit(Node sv,
                              Node lit,
                              std::vector<unsigned>& path,
                              BvInverterModelQuery* m,
                              BvInverterStatus& status)
{
  Assert(!path.empty());

  bool pol = true;
  unsigned index, nchildren;
  NodeManager* nm = NodeManager::currentNM();
  Kind k;

  Assert(!path.empty());
  index = path.back();
  Assert(index < lit.getNumChildren());
  path.pop_back();
  k = lit.getKind();
  
  /* Note: option --bool-to-bv is currently disabled when CBQI BV
   *       is enabled. We currently do not support Boolean operators
   *       that are interpreted as bit-vector operators of width 1.  */

  /* Boolean layer ----------------------------------------------- */
  
  if (k == NOT)
  {
    pol = !pol;
    lit = lit[index];
    Assert(!path.empty());
    index = path.back();
    Assert(index < lit.getNumChildren());
    path.pop_back();
    k = lit.getKind();
  }

  Assert(k == EQUAL
      || k == BITVECTOR_ULT
      || k == BITVECTOR_SLT
      || k == BITVECTOR_COMP);

  Node sv_t = lit[index];
  Node t = lit[1-index];

  switch (k)
  {
    case BITVECTOR_ULT:
    {
      TypeNode solve_tn = sv_t.getType();
      Node x = getSolveVariable(solve_tn);
      Node sc;
      if (index == 0)
      {
        if (pol == true)
        {
          /* x < t
           * with side condition:
           * t != 0  */
          Node scl = nm->mkNode(
              DISTINCT, t, bv::utils::mkZero(bv::utils::getSize(t)));
          Node scr = nm->mkNode(k, x, t);
          sc = nm->mkNode(IMPLIES, scl, scr);
        }
        else
        {
          sc = nm->mkNode(NOT, nm->mkNode(k, x, t));
        }
      }
      else if (index == 1)
      {
        if (pol == true)
        {
          /* t < x
           * with side condition:
           * t != ~0  */
          Node scl = nm->mkNode(
              DISTINCT, t, bv::utils::mkOnes(bv::utils::getSize(t)));
          Node scr = nm->mkNode(k, t, x);
          sc = nm->mkNode(IMPLIES, scl, scr);
        }
        else
        {
          sc = nm->mkNode(NOT, nm->mkNode(k, t, x));
        }
      }
      status.d_conds.push_back(sc);
      /* t = skv (fresh skolem constant)  */
      Node skv = getInversionNode(sc, solve_tn);
      t = skv;
      if (!path.empty())
      {
        index = path.back();
        Assert(index < sv_t.getNumChildren());
        path.pop_back();
        sv_t = sv_t[index];
        k = sv_t.getKind();
      }
      break;
    }

    case BITVECTOR_SLT:
    {
      TypeNode solve_tn = sv_t.getType();
      Node x = getSolveVariable(solve_tn);
      Node sc;
      unsigned w = bv::utils::getSize(t);
      if (index == 0)
      {
        if (pol == true)
        {
          /* x < t
           * with side condition:
           * t != 10...0 */
          Node min = bv::utils::mkConst(BitVector(w).setBit(w - 1));
          Node scl = nm->mkNode(DISTINCT, min, t);
          Node scr = nm->mkNode(k, x, t);
          sc = nm->mkNode(IMPLIES, scl, scr);
        }
        else
        {
          sc = nm->mkNode(NOT, nm->mkNode(k, x, t));
        }
      }
      else if (index == 1)
      {
        if (pol == true)
        {
          /* t < x
           * with side condition:
           * t != 01...1  */
          BitVector bv = BitVector(w).setBit(w - 1);
          Node max = bv::utils::mkConst(~bv);
          Node scl = nm->mkNode(DISTINCT, t, max);
          Node scr = nm->mkNode(k, t, x);
          sc = nm->mkNode(IMPLIES, scl, scr);
        }
        else
        {
          sc = nm->mkNode(NOT, nm->mkNode(k, t, x));
        }
      }
      status.d_conds.push_back(sc);
      /* t = skv (fresh skolem constant)  */
      Node skv = getInversionNode(sc, solve_tn);
      t = skv;
      if (!path.empty())
      {
        index = path.back();
        Assert(index < sv_t.getNumChildren());
        path.pop_back();
        sv_t = sv_t[index];
        k = sv_t.getKind();
      }
      break;
    }

    default:
      Assert(k == EQUAL);
      if (pol == false)
      {
        /* x != t
         * <->
         * x < t || x > t  (ULT)
         * with side condition:
         * t != 0 || t != ~0  */
        TypeNode solve_tn = sv_t.getType();
        Node x = getSolveVariable(solve_tn);
        unsigned w = bv::utils::getSize(t);
        Node scl = nm->mkNode(
            OR,
            nm->mkNode(DISTINCT, t, bv::utils::mkZero(w)),
            nm->mkNode(DISTINCT, t, bv::utils::mkOnes(w)));
        Node scr = nm->mkNode(DISTINCT, x, t);
        Node sc = nm->mkNode(IMPLIES, scl, scr);
        status.d_conds.push_back(sc);
        /* t = skv (fresh skolem constant)  */
        Node skv = getInversionNode(sc, solve_tn);
        t = skv;
        if (!path.empty())
        {
          index = path.back();
          Assert(index < sv_t.getNumChildren());
          path.pop_back();
          sv_t = sv_t[index];
          k = sv_t.getKind();
        }
      }
  }

  /* Bit-vector layer -------------------------------------------- */

  while (!path.empty())
  {
    index = path.back();
    Assert(index < sv_t.getNumChildren());
    path.pop_back();
    k = sv_t.getKind();
    nchildren = sv_t.getNumChildren();

    if (k == BITVECTOR_NOT || k == BITVECTOR_NEG)
    {
      t = nm->mkNode(k, t);
    }
    else if (k == BITVECTOR_CONCAT)
    {
      /* x = t[upper:lower]
       * where
       * upper = getSize(t) - 1 - sum(getSize(sv_t[i])) for i < index
       * lower = getSize(sv_t[i]) for i > index
       */
      unsigned upper, lower;
      upper = bv::utils::getSize(t) - 1;
      lower = 0;
      NodeBuilder<> nb(nm, BITVECTOR_CONCAT);
      for (unsigned i = 0; i < nchildren; i++)
      {
        if (i < index) { upper -= bv::utils::getSize(sv_t[i]); }
        else if (i > index) { lower += bv::utils::getSize(sv_t[i]); }
      }
      t = bv::utils::mkExtract(t, upper, lower);
    }
    else if (k == BITVECTOR_SIGN_EXTEND)
    {
      t = bv::utils::mkExtract(t, bv::utils::getSize(sv_t[index]) - 1, 0);
    }
    else if (k == BITVECTOR_EXTRACT)
    {
      Trace("bv-invert") << "bv-invert : Unsupported for index " << index
                         << ", from " << sv_t << std::endl;
      return Node::null();
    }
    else
    {
      Assert(nchildren >= 2);
      Node s = nchildren == 2 ? sv_t[1 - index] : dropChild(sv_t, index);
      /* Note: All n-ary kinds except for CONCAT (i.e., AND, OR, MULT, PLUS)
       *       are commutative (no case split based on index). */
      switch (k)
      {
        case BITVECTOR_COMP:
          t = s;
          break;

        case BITVECTOR_PLUS:
          t = nm->mkNode(BITVECTOR_SUB, t, s);
          break;

        case BITVECTOR_SUB:
          t = nm->mkNode(BITVECTOR_PLUS, t, s);
          break;

        case BITVECTOR_MULT:
        {
          /* with side condition:
           * ctz(t) >= ctz(s) <-> x * s = t
           * where
           * ctz(t) >= ctz(s) -> (t & -t) >= (s & -s)  */
          TypeNode solve_tn = sv_t[index].getType();
          Node x = getSolveVariable(solve_tn);
          /* left hand side of side condition  */
          Node scl = nm->mkNode(
              BITVECTOR_UGE,
              nm->mkNode(BITVECTOR_AND, t, nm->mkNode(BITVECTOR_NEG, t)),
              nm->mkNode(BITVECTOR_AND, s, nm->mkNode(BITVECTOR_NEG, s)));
          /* right hand side of side condition  */
          Node scr = nm->mkNode(EQUAL, nm->mkNode(BITVECTOR_MULT, x, s), t);
          /* overall side condition  */
          Node sc = nm->mkNode(IMPLIES, scl, scr);
          /* add side condition  */
          status.d_conds.push_back(sc);

          /* t = skv (fresh skolem constant)
           * get the skolem node for this side condition  */
          Node skv = getInversionNode(sc, solve_tn);
          /* now solving with the skolem node as the RHS  */
          t = skv;
          break;
        }

        case BITVECTOR_UREM_TOTAL:
        {
          /* t = skv (fresh skolem constant)  */
          TypeNode solve_tn = sv_t[index].getType();
          Node x = getSolveVariable(solve_tn);
          Node scl, scr;
          if (index == 0)
          {
            /* x % s = t is rewritten to x - x / y * y */
            Trace("bv-invert") << "bv-invert : Unsupported for index " << index
                               << ", from " << sv_t << std::endl;
            return Node::null();
          }
          else
          {
            /* s % x = t
             * with side condition:
             * s > t
             * && s-t > t
             * && (t = 0 || t != s-1)  */
            Node s_gt_t = nm->mkNode(BITVECTOR_UGT, s, t);
            Node s_m_t = nm->mkNode(BITVECTOR_SUB, s, t);
            Node smt_gt_t = nm->mkNode(BITVECTOR_UGT, s_m_t, t);
            Node t_eq_z = nm->mkNode(EQUAL,
                t, bv::utils::mkZero(bv::utils::getSize(t)));
            Node s_m_o = nm->mkNode(BITVECTOR_SUB,
                s, bv::utils::mkOne(bv::utils::getSize(s)));
            Node t_d_smo = nm->mkNode(DISTINCT, t, s_m_o);

            scl = nm->mkNode(AND,
                             nm->mkNode(AND, s_gt_t, smt_gt_t),
                             nm->mkNode(OR, t_eq_z, t_d_smo));
            scr = nm->mkNode(EQUAL, nm->mkNode(BITVECTOR_UREM_TOTAL, s, x), t);
          }
          Node sc = nm->mkNode(IMPLIES, scl, scr);
          status.d_conds.push_back(sc);
          Node skv = getInversionNode(sc, solve_tn);
          t = skv;
          break;
        }

        case BITVECTOR_UDIV_TOTAL:
        {
          TypeNode solve_tn = sv_t[index].getType();
          Node x = getSolveVariable(solve_tn);
          Node s = sv_t[1 - index];
          unsigned w = bv::utils::getSize(s);
          Node scl, scr;
          Node zero = bv::utils::mkZero(w);

          if (index == 0)
          {
            /* x udiv s = t
             * with side condition:
             * !umulo(s * t)
             */
            scl = nm->mkNode(NOT, bv::utils::mkUmulo(s, t));
            scr = nm->mkNode(EQUAL, nm->mkNode(BITVECTOR_UDIV_TOTAL, x, s), t);
          }
          else
          {
            /* s udiv x = t
             * with side condition:
             * (t = 0 && (s = 0 || s != 2^w-1))
             * || s >= t
             * || t = 2^w-1
             */
            Node ones = bv::utils::mkOnes(w);
            Node t_eq_zero = nm->mkNode(EQUAL, t, zero);
            Node s_eq_zero = nm->mkNode(EQUAL, s, zero);
            Node s_ne_ones = nm->mkNode(DISTINCT, s, ones);
            Node s_ge_t = nm->mkNode(BITVECTOR_UGE, s, t);
            Node t_eq_ones = nm->mkNode(EQUAL, t, ones);
            scl = nm->mkNode(
                OR,
                nm->mkNode(
                  AND, t_eq_zero, nm->mkNode(OR, s_eq_zero, s_ne_ones)),
                s_ge_t,
                t_eq_ones);
            scr = nm->mkNode(EQUAL, nm->mkNode(BITVECTOR_UDIV_TOTAL, s, x), t);
          }

          /* overall side condition */
          Node sc = nm->mkNode(IMPLIES, scl, scr);
          /* add side condition */
          status.d_conds.push_back(sc);

          /* t = skv (fresh skolem constant)
           * get the skolem node for this side condition */
          Node skv = getInversionNode(sc, solve_tn);
          /* now solving with the skolem node as the RHS */
          t = skv;
          break;
        }

        case BITVECTOR_AND:
        case BITVECTOR_OR:
        {
          /* with side condition:
           * t & s = t
           * t | s = t */
          TypeNode solve_tn = sv_t[index].getType();
          Node x = getSolveVariable(solve_tn);
          Node scl = nm->mkNode(EQUAL, t, nm->mkNode(k, t, s));
          Node scr = nm->mkNode(EQUAL, nm->mkNode(k, x, s), t);
          Node sc = nm->mkNode(IMPLIES, scl, scr);
          status.d_conds.push_back(sc);
          /* t = skv (fresh skolem constant)  */
          Node skv = getInversionNode(sc, solve_tn);
          t = skv;
          break;
        }

        case BITVECTOR_XOR:
          t = nm->mkNode(BITVECTOR_XOR, t, s);
          break;

        case BITVECTOR_LSHR:
        {
          TypeNode solve_tn = sv_t[index].getType();
          Node x = getSolveVariable(solve_tn);
          Node scl, scr;
          if (index == 0)
          {
            /* x >> s = t
             * with side condition:
             * s = 0 || clz(t) >= s
             * ->
             * s = 0 || ((z o t) << s)[2w-1 : w] = z
             * with w = getSize(t) = getSize(s)
             * and z = 0 with getSize(z) = w  */
            unsigned w = bv::utils::getSize(s);
            Node z = bv::utils::mkZero(w);
            Node z_o_t = nm->mkNode(BITVECTOR_CONCAT, z, t);
            Node z_o_s = nm->mkNode(BITVECTOR_CONCAT, z, s);
            Node zot_shl_zos = nm->mkNode(BITVECTOR_SHL, z_o_t, z_o_s);
            Node ext = bv::utils::mkExtract(zot_shl_zos, 2*w-1, w);
            scl = nm->mkNode(OR,
                nm->mkNode(EQUAL, s, z),
                nm->mkNode(EQUAL, ext, z));
            scr = nm->mkNode(EQUAL, nm->mkNode(BITVECTOR_LSHR, x, s), t);
            Node sc = nm->mkNode(IMPLIES, scl, scr);
            status.d_conds.push_back(sc);
            /* t = skv (fresh skolem constant)  */
            Node skv = getInversionNode(sc, solve_tn);
            t = skv;
          }
          else
          {
            /* s >> x = t
             * with side condition:
             * clz(t) >= clz(s)
             *   && (t = 0
             *    || "remaining shifted bits in t "
             *       "match corresponding bits in s")  */
            Trace("bv-invert") << "bv-invert : Unsupported for index " << index
                               << ", from " << sv_t << std::endl;
            return Node::null();
          }
          break;
        }

        case BITVECTOR_ASHR:
        {
          TypeNode solve_tn = sv_t[index].getType();
          Node x = getSolveVariable(solve_tn);
          Node scl, scr;
          if (index == 0)
          {
            /* x >> s = t
             * with side condition:
             * s = 0 || (sext(t,w) << s)[2w-1 : w] = sext(t[w-1:w-1], w-1)
             * with w = getSize(t) = getSize(s)  */
            unsigned w = bv::utils::getSize(s);
            Node z = bv::utils::mkZero(w);
            Node s1 = bv::utils::mkSignExtend(t, w);
            Node z_o_s = nm->mkNode(BITVECTOR_CONCAT, z, s);
            Node s1_shl_zos = nm->mkNode(BITVECTOR_SHL, s1, z_o_s);
            Node msb_t = bv::utils::mkExtract(t, w-1, w-1);
            Node s2 = bv::utils::mkSignExtend(msb_t, w-1);
            Node ext = bv::utils::mkExtract(s1_shl_zos, 2*w-1, w);
            scl = nm->mkNode(OR,
                nm->mkNode(EQUAL, s, z),
                nm->mkNode(EQUAL, ext, s2));
            scr = nm->mkNode(EQUAL, nm->mkNode(BITVECTOR_LSHR, x, s), t);
            Node sc = nm->mkNode(IMPLIES, scl, scr);
            status.d_conds.push_back(sc);
            /* t = skv (fresh skolem constant)  */
            Node skv = getInversionNode(sc, solve_tn);
            t = skv;
          }
          else
          {
            /* s >> x = t
             * with side condition:
             * clx(msb(s),t) >= clx(msb(s),s)
             *   && (t = 0
             *    || t = ~0
             *    || "remaining shifted bits in t "
             *          "match corresponding bits in s")  */

            Trace("bv-invert") << "bv-invert : Unsupported for index " << index
                               << ", from " << sv_t << std::endl;
            return Node::null();
          }
          break;
        }

        case BITVECTOR_SHL:
        {
          TypeNode solve_tn = sv_t[index].getType();
          Node x = getSolveVariable(solve_tn);
          Node s = sv_t[1 - index];
          unsigned w = bv::utils::getSize(s);
          Node scl, scr;

          if (index == 0)
          {
            /* x << s = t
             * with side condition:
             * (s = 0 || ctz(t) >= s)
             * <->
             * (s = 0 || ((t o z) >> (z o s))[w-1:0] = z)
             *
             * where
             * w = getSize(s) = getSize(t) = getSize (z) && z = 0
             */
            Node zero = bv::utils::mkConst(w, 0u);
            Node s_eq_zero = nm->mkNode(EQUAL, s, zero);
            Node t_conc_zero = nm->mkNode(BITVECTOR_CONCAT, t, zero);
            Node zero_conc_s = nm->mkNode(BITVECTOR_CONCAT, zero, s);
            Node shr_s = nm->mkNode(BITVECTOR_LSHR, t_conc_zero, zero_conc_s);
            Node extr_shr_s = bv::utils::mkExtract(shr_s, w - 1, 0);
            Node ctz_t_ge_s = nm->mkNode(EQUAL, extr_shr_s, zero);
            scl = nm->mkNode(OR, s_eq_zero, ctz_t_ge_s);
            scr = nm->mkNode(EQUAL, nm->mkNode(BITVECTOR_SHL, x, s), t);
          }
          else
          {
            /* s << x = t
             * with side condition:
             * ctz(t) >= ctz(s)
             * && (t = 0 ||
             *     "remaining shifted bits in t"
             *     "match corresponding bits in s")
             */
            Trace("bv-invert") << "bv-invert : Unsupported for index " << index
                               << "for bit-vector term " << sv_t << std::endl;
            return Node::null();
          }

          /* overall side condition */
          Node sc = nm->mkNode(IMPLIES, scl, scr);
          /* add side condition */
          status.d_conds.push_back(sc);

          /* t = skv (fresh skolem constant)
           * get the skolem node for this side condition */
          Node skv = getInversionNode(sc, solve_tn);
          /* now solving with the skolem node as the RHS */
          t = skv;
          break;
        }

        default:
          Trace("bv-invert") << "bv-invert : Unknown kind " << k
                             << " for bit-vector term " << sv_t << std::endl;
          return Node::null();
      }
    }
    sv_t = sv_t[index];
  }
  Assert(sv_t == sv);
  return t;
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC4
