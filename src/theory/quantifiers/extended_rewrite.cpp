/*********************                                                        */
/*! \file extended_rewrite.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of extended rewriting techniques
 **/

#include "theory/quantifiers/extended_rewrite.h"

#include "options/quantifiers_options.h"
#include "theory/arith/arith_msum.h"
#include "theory/bv/theory_bv_utils.h"
#include "theory/datatypes/datatypes_rewriter.h"
#include "theory/quantifiers/term_util.h"
#include "theory/rewriter.h"

using namespace CVC4::kind;
using namespace std;

namespace CVC4 {
namespace theory {
namespace quantifiers {

struct ExtRewriteAttributeId
{
};
typedef expr::Attribute<ExtRewriteAttributeId, Node> ExtRewriteAttribute;

ExtendedRewriter::ExtendedRewriter(bool aggr) : d_aggr(aggr)
{
}
void ExtendedRewriter::setCache(Node n, Node ret)
{
  ExtRewriteAttribute era;
  n.setAttribute(era, ret);
}

Node ExtendedRewriter::extendedRewrite(Node n)
{
  n = Rewriter::rewrite(n);
  if (!options::sygusExtRew())
  {
    return n;
  }

  // has it already been computed?
  if (n.hasAttribute(ExtRewriteAttribute()))
  {
    return n.getAttribute(ExtRewriteAttribute());
  }

  Node ret = n;
  NodeManager* nm = NodeManager::currentNM();

  //--------------------pre-rewrite
  Node pre_new_ret;
  if (ret.getKind() == IMPLIES)
  {
    pre_new_ret = nm->mkNode(OR, ret[0].negate(), ret[1]);
    debugExtendedRewrite(ret, pre_new_ret, "IMPLIES elim");
  }
  else if (ret.getKind() == XOR)
  {
    pre_new_ret = nm->mkNode(EQUAL, ret[0].negate(), ret[1]);
    debugExtendedRewrite(ret, pre_new_ret, "XOR elim");
  }
  else if (ret.getKind() == NOT)
  {
    pre_new_ret = extendedRewriteNnf(ret);
    debugExtendedRewrite(ret, pre_new_ret, "NNF");
  }
  if (!pre_new_ret.isNull())
  {
    ret = extendedRewrite(pre_new_ret);
    Trace("q-ext-rewrite-debug") << "...ext-pre-rewrite : " << n << " -> "
                                 << pre_new_ret << std::endl;
    setCache(n, ret);
    return ret;
  }
  //--------------------end pre-rewrite

  //--------------------rewrite children
  if (n.getNumChildren() > 0)
  {
    std::vector<Node> children;
    if (n.getMetaKind() == metakind::PARAMETERIZED)
    {
      children.push_back(n.getOperator());
    }
    Kind k = n.getKind();
    bool childChanged = false;
    bool isNonAdditive = TermUtil::isNonAdditive(k);
    for (unsigned i = 0; i < n.getNumChildren(); i++)
    {
      Node nc = extendedRewrite(n[i]);
      childChanged = nc != n[i] || childChanged;
      // If the operator is non-additive, do not consider duplicates
      if (isNonAdditive
          && std::find(children.begin(), children.end(), nc) != children.end())
      {
        childChanged = true;
      }
      else
      {
        children.push_back(nc);
      }
    }
    Assert(!children.empty());
    // Some commutative operators have rewriters that are agnostic to order,
    // thus, we sort here.
    if (TermUtil::isComm(k) && (d_aggr || children.size() <= 5))
    {
      childChanged = true;
      std::sort(children.begin(), children.end());
    }
    if (childChanged)
    {
      if (isNonAdditive && children.size() == 1)
      {
        // we may have subsumed children down to one
        ret = children[0];
      }
      else
      {
        ret = nm->mkNode(k, children);
      }
    }
  }
  ret = Rewriter::rewrite(ret);
  //--------------------end rewrite children

  // now, do extended rewrite
  Trace("q-ext-rewrite-debug") << "Do extended rewrite on : " << ret
                               << " (from " << n << ")" << std::endl;
  Node new_ret;

  //---------------------- theory-independent post-rewriting
  if (ret.getKind() == ITE)
  {
    new_ret = extendedRewriteIte(ITE, ret);
  }
  else if (ret.getKind() == AND || ret.getKind() == OR)
  {
    // all kinds are legal to substitute over : hence we give the empty map
    std::map<Kind, bool> bcp_kinds;
    new_ret = extendedRewriteBcp(AND, OR, NOT, bcp_kinds, ret);
    debugExtendedRewrite(ret, new_ret, "Bool bcp");
    if( new_ret.isNull() )
    {
      // equality resolution
      new_ret = extendedRewriteEqRes(AND, OR, EQUAL, XOR, NOT, bcp_kinds,ret);
      debugExtendedRewrite(ret, new_ret, "Bool eq res");
    }
  }
  else if (ret.getKind() == EQUAL)
  {
    new_ret = extendedRewriteEqChain(EQUAL, AND, OR, NOT, ret);
    debugExtendedRewrite(ret, new_ret, "Bool eq-chain simplify");
  }
  if (new_ret.isNull() && ret.getKind() != ITE)
  {
    // simple ITE pulling
    new_ret = extendedRewritePullIte(ITE, ret);
  }
  //----------------------end theory-independent post-rewriting

  //----------------------theory-specific post-rewriting
  if (new_ret.isNull())
  {
    Node atom = ret.getKind() == NOT ? ret[0] : ret;
    bool pol = ret.getKind() != NOT;
    TheoryId tid = Theory::theoryOf(atom);
    if (tid == THEORY_ARITH)
    {
      new_ret = extendedRewriteArith(atom, pol);
    }
    // add back negation if not processed
    if (!pol && !new_ret.isNull())
    {
      new_ret = new_ret.negate();
    }
  }
  //----------------------end theory-specific post-rewriting

  //----------------------aggressive rewrites
  if (new_ret.isNull() && d_aggr)
  {
    new_ret = extendedRewriteAggr(ret);
  }
  //----------------------end aggressive rewrites

  setCache(n, ret);
  if (!new_ret.isNull())
  {
    ret = extendedRewrite(new_ret);
  }
  Trace("q-ext-rewrite-debug") << "...ext-rewrite : " << n << " -> " << ret
                               << std::endl;
  setCache(n, ret);
  return ret;
}

Node ExtendedRewriter::extendedRewriteAggr(Node n)
{
  Node new_ret;
  Trace("q-ext-rewrite-debug2")
      << "Do aggressive rewrites on " << n << std::endl;
  bool polarity = n.getKind() != NOT;
  Node ret_atom = n.getKind() == NOT ? n[0] : n;
  if ((ret_atom.getKind() == EQUAL && ret_atom[0].getType().isReal())
      || ret_atom.getKind() == GEQ)
  {
    // ITE term removal in polynomials
    // e.g. ite( x=0, x, y ) = x+1 ---> ( x=0 ^ y = x+1 )
    Trace("q-ext-rewrite-debug2")
        << "Compute monomial sum " << ret_atom << std::endl;
    // compute monomial sum
    std::map<Node, Node> msum;
    if (ArithMSum::getMonomialSumLit(ret_atom, msum))
    {
      for (std::map<Node, Node>::iterator itm = msum.begin(); itm != msum.end();
           ++itm)
      {
        Node v = itm->first;
        Trace("q-ext-rewrite-debug2")
            << itm->first << " * " << itm->second << std::endl;
        if (v.getKind() == ITE)
        {
          Node veq;
          int res = ArithMSum::isolate(v, msum, veq, ret_atom.getKind());
          if (res != 0)
          {
            Trace("q-ext-rewrite-debug")
                << "  have ITE relation, solved form : " << veq << std::endl;
            // try pulling ITE
            new_ret = extendedRewritePullIte(ITE, veq);
            if (!new_ret.isNull())
            {
              if (!polarity)
              {
                new_ret = new_ret.negate();
              }
              break;
            }
          }
          else
          {
            Trace("q-ext-rewrite-debug")
                << "  failed to isolate " << v << " in " << n << std::endl;
          }
        }
      }
    }
    else
    {
      Trace("q-ext-rewrite-debug")
          << "  failed to get monomial sum of " << n << std::endl;
    }
  }
  // TODO (#1706) : conditional rewriting, condition merging
  return new_ret;
}

Node ExtendedRewriter::extendedRewriteIte(Kind itek, Node n, bool full)
{
  Assert(n.getKind() == itek);
  Assert(n[1] != n[2]);

  NodeManager* nm = NodeManager::currentNM();

  Trace("ext-rew-ite") << "Rewrite ITE : " << n << std::endl;

  Node flip_cond;
  if (n[0].getKind() == NOT)
  {
    flip_cond = n[0][0];
  }
  else if (n[0].getKind() == OR)
  {
    // a | b ---> ~( ~a & ~b )
    flip_cond = TermUtil::simpleNegate(n[0]);
  }
  if (!flip_cond.isNull())
  {
    Node new_ret = nm->mkNode(ITE, flip_cond, n[2], n[1]);
    // only print debug trace if full=true
    if (full)
    {
      debugExtendedRewrite(n, new_ret, "ITE flip");
    }
    return new_ret;
  }

  // get entailed equalities in the condition
  std::vector<Node> eq_conds;
  Kind ck = n[0].getKind();
  if (ck == EQUAL)
  {
    eq_conds.push_back(n[0]);
  }
  else if (ck == AND)
  {
    for (const Node& cn : n[0])
    {
      if (cn.getKind() == EQUAL)
      {
        eq_conds.push_back(cn);
      }
    }
  }

  Node new_ret;
  Node b;
  Node e;
  Node t1 = n[1];
  Node t2 = n[2];
  std::stringstream ss_reason;

  for (const Node& eq : eq_conds)
  {
    // simple invariant ITE
    for (unsigned i = 0; i <= 1; i++)
    {
      // ite( x = y ^ C, y, x ) ---> x
      // this is subsumed by the rewrites below
      if (t2 == eq[i] && t1 == eq[1 - i])
      {
        new_ret = t2;
        ss_reason << "ITE simple rev subs";
        break;
      }
    }
    if (!new_ret.isNull())
    {
      break;
    }
  }

  if (new_ret.isNull() && d_aggr)
  {
    // If x is less than t based on an ordering, then we use { x -> t } as a
    // substitution to the children of ite( x = t ^ C, s, t ) below.
    std::vector<Node> vars;
    std::vector<Node> subs;
    for (const Node& eq : eq_conds)
    {
      inferSubstitution(eq, vars, subs);
    }

    if (!vars.empty())
    {
      // reverse substitution to opposite child
      // r{ x -> t } = s  implies  ite( x=t ^ C, s, r ) ---> r
      Node nn =
          t2.substitute(vars.begin(), vars.end(), subs.begin(), subs.end());
      if (nn != t2)
      {
        nn = Rewriter::rewrite(nn);
        if (nn == t1)
        {
          new_ret = t2;
          ss_reason << "ITE rev subs";
        }
      }

      // ite( x=t ^ C, s, r ) ---> ite( x=t ^ C, s{ x -> t }, r )
      nn = t1.substitute(vars.begin(), vars.end(), subs.begin(), subs.end());
      if (nn != t1)
      {
        // If full=false, then we've duplicated a term u in the children of n.
        // For example, when ITE pulling, we have n is of the form:
        //   ite( C, f( u, t1 ), f( u, t2 ) )
        // We must show that at least one copy of u dissappears in this case.
        nn = Rewriter::rewrite(nn);
        if (nn == t2)
        {
          new_ret = nn;
          ss_reason << "ITE subs invariant";
        }
        else if (full || nn.isConst())
        {
          new_ret = nm->mkNode(itek, n[0], nn, t2);
          ss_reason << "ITE subs";
        }
      }
    }
  }

  // only print debug trace if full=true
  if (!new_ret.isNull() && full)
  {
    debugExtendedRewrite(n, new_ret, ss_reason.str().c_str());
  }

  return new_ret;
}

Node ExtendedRewriter::extendedRewritePullIte(Kind itek, Node n)
{
  NodeManager* nm = NodeManager::currentNM();
  TypeNode tn = n.getType();
  std::vector<Node> children;
  bool hasOp = (n.getMetaKind() == metakind::PARAMETERIZED);
  if (hasOp)
  {
    children.push_back(n.getOperator());
  }
  unsigned nchildren = n.getNumChildren();
  for (unsigned i = 0; i < nchildren; i++)
  {
    children.push_back(n[i]);
  }
  std::map<unsigned, std::map<unsigned, Node> > ite_c;
  for (unsigned i = 0; i < nchildren; i++)
  {
    if (n[i].getKind() == itek)
    {
      unsigned ii = hasOp ? i + 1 : i;
      for (unsigned j = 0; j < 2; j++)
      {
        children[ii] = n[i][j + 1];
        Node pull = nm->mkNode(n.getKind(), children);
        Node pullr = Rewriter::rewrite(pull);
        children[ii] = n[i];
        ite_c[i][j] = pullr;
      }
      if (ite_c[i][0] == ite_c[i][1])
      {
        // ITE dual invariance
        // f( t1..s1..tn ) ---> t  and  f( t1..s2..tn ) ---> t implies
        // f( t1..ite( A, s1, s2 )..tn ) ---> t
        debugExtendedRewrite(n, ite_c[i][0], "ITE dual invariant");
        return ite_c[i][0];
      }
      else if (d_aggr)
      {
        for (unsigned j = 0; j < 2; j++)
        {
          Node pullr = ite_c[i][j];
          if (pullr.isConst() || pullr == n[i][j + 1])
          {
            // ITE single child elimination
            // f( t1..s1..tn ) ---> t  where t is a constant or s1 itself
            // implies
            // f( t1..ite( A, s1, s2 )..tn ) ---> ite( A, t, f( t1..s2..tn ) )
            Node new_ret;
            if (tn.isBoolean())
            {
              // remove false/true child immediately
              bool pol = pullr.getConst<bool>();
              std::vector<Node> new_children;
              new_children.push_back((j == 0) == pol ? n[i][0]
                                                     : n[i][0].negate());
              new_children.push_back(ite_c[i][1 - j]);
              new_ret = nm->mkNode(pol ? OR : AND, new_children);
              debugExtendedRewrite(n, new_ret, "ITE Bool single elim");
            }
            else
            {
              new_ret = nm->mkNode(itek, n[i][0], ite_c[i][0], ite_c[i][1]);
              debugExtendedRewrite(n, new_ret, "ITE single elim");
            }
            return new_ret;
          }
        }
      }
    }
  }

  for (std::pair<const unsigned, std::map<unsigned, Node> >& ip : ite_c)
  {
    Node nite = n[ip.first];
    Assert(nite.getKind() == itek);
    // now, simply pull the ITE and try ITE rewrites
    Node pull_ite = nm->mkNode(itek, nite[0], ip.second[0], ip.second[1]);
    pull_ite = Rewriter::rewrite(pull_ite);
    if (pull_ite.getKind() == ITE)
    {
      Node new_pull_ite = extendedRewriteIte(itek, pull_ite, false);
      if (!new_pull_ite.isNull())
      {
        debugExtendedRewrite(n, new_pull_ite, "ITE pull rewrite");
        return new_pull_ite;
      }
    }
    else
    {
      // A general rewrite could eliminate the ITE by pulling.
      // An example is:
      //   ~( ite( C, ~x, ~ite( C, y, x ) ) ) --->
      //   ite( C, ~~x, ite( C, y, x ) ) --->
      //   x
      // where ~ is bitvector negation.
      debugExtendedRewrite(n, pull_ite, "ITE pull basic elim");
      return pull_ite;
    }
  }

  return Node::null();
}

Node ExtendedRewriter::extendedRewriteNnf(Node ret)
{
  Assert(ret.getKind() == NOT);

  Kind nk = ret[0].getKind();
  bool neg_ch = false;
  bool neg_ch_1 = false;
  if (nk == AND || nk == OR)
  {
    neg_ch = true;
    nk = nk == AND ? OR : AND;
  }
  else if (nk == IMPLIES)
  {
    neg_ch = true;
    neg_ch_1 = true;
    nk = AND;
  }
  else if (nk == ITE)
  {
    neg_ch = true;
    neg_ch_1 = true;
  }
  else if (nk == XOR)
  {
    nk = EQUAL;
  }
  else if (nk == EQUAL && ret[0][0].getType().isBoolean())
  {
    neg_ch_1 = true;
  }
  else
  {
    return Node::null();
  }

  std::vector<Node> new_children;
  for (unsigned i = 0, nchild = ret[0].getNumChildren(); i < nchild; i++)
  {
    Node c = ret[0][i];
    c = (i == 0 ? neg_ch_1 : false) != neg_ch ? c.negate() : c;
    new_children.push_back(c);
  }
  return NodeManager::currentNM()->mkNode(nk, new_children);
}

Node ExtendedRewriter::extendedRewriteBcp(
    Kind andk, Kind ork, Kind notk, std::map<Kind, bool>& bcp_kinds, Node ret)
{
  Kind k = ret.getKind();
  Assert(k == andk || k == ork);
  Trace("ext-rew-bcp") << "BCP: **** INPUT: " << ret << std::endl;

  NodeManager* nm = NodeManager::currentNM();

  TypeNode tn = ret.getType();
  Node truen = TermUtil::mkTypeMaxValue(tn);
  Node falsen = TermUtil::mkTypeValue(tn, 0);

  // terms to process
  std::vector<Node> to_process;
  for (const Node& cn : ret)
  {
    to_process.push_back(cn);
  }
  // the processing terms
  std::vector<Node> clauses;
  // the terms we have propagated information to
  std::unordered_set<Node, NodeHashFunction> prop_clauses;
  // the assignment
  std::map<Node, Node> assign;
  std::vector<Node> avars;
  std::vector<Node> asubs;

  Kind ok = k == andk ? ork : andk;
  // global polarity : when k=ork, everything is negated
  bool gpol = k == andk;

  do
  {
    // process the current nodes
    while (!to_process.empty())
    {
      std::vector<Node> new_to_process;
      for (const Node& cn : to_process)
      {
        Trace("ext-rew-bcp-debug") << "process " << cn << std::endl;
        Kind cnk = cn.getKind();
        bool pol = cnk != notk;
        Node cln = cnk == notk ? cn[0] : cn;
        Assert(cln.getKind() != notk);
        if ((pol && cln.getKind() == k) || (!pol && cln.getKind() == ok))
        {
          // flatten
          for (const Node& ccln : cln)
          {
            Node lccln = pol ? ccln : TermUtil::mkNegate(notk, ccln);
            new_to_process.push_back(lccln);
          }
        }
        else
        {
          // add it to the assignment
          Node val = gpol == pol ? truen : falsen;
          std::map<Node, Node>::iterator it = assign.find(cln);
          Trace("ext-rew-bcp") << "BCP: assign " << cln << " -> " << val
                               << std::endl;
          if (it != assign.end())
          {
            if (val != it->second)
            {
              Trace("ext-rew-bcp") << "BCP: conflict!" << std::endl;
              // a conflicting assignment: we are done
              return gpol ? falsen : truen;
            }
          }
          else
          {
            assign[cln] = val;
            avars.push_back(cln);
            asubs.push_back(val);
          }

          // also, treat it as clause if possible
          if (cln.getNumChildren() > 0
              & (bcp_kinds.empty()
                 || bcp_kinds.find(cln.getKind()) != bcp_kinds.end()))
          {
            if (std::find(clauses.begin(), clauses.end(), cn) == clauses.end()
                && prop_clauses.find(cn) == prop_clauses.end())
            {
              Trace("ext-rew-bcp") << "BCP: new clause: " << cn << std::endl;
              clauses.push_back(cn);
            }
          }
        }
      }
      to_process.clear();
      to_process.insert(
          to_process.end(), new_to_process.begin(), new_to_process.end());
    }

    // apply substitution to all subterms of clauses
    std::vector<Node> new_clauses;
    for (const Node& c : clauses)
    {
      bool cpol = c.getKind() != notk;
      Node ca = c.getKind() == notk ? c[0] : c;
      bool childChanged = false;
      std::vector<Node> ccs_children;
      for (const Node& cc : ca)
      {
        Node ccs = cc;
        if (bcp_kinds.empty())
        {
          Trace("ext-rew-bcp-debug") << "...do ordinary substitute"
                                     << std::endl;
          ccs = cc.substitute(
              avars.begin(), avars.end(), asubs.begin(), asubs.end());
        }
        else
        {
          Trace("ext-rew-bcp-debug") << "...do partial substitute" << std::endl;
          // substitution is only applicable to compatible kinds
          ccs = partialSubstitute(ccs, assign, bcp_kinds);
        }
        childChanged = childChanged || ccs != cc;
        ccs_children.push_back(ccs);
      }
      if (childChanged)
      {
        if (ca.getMetaKind() == metakind::PARAMETERIZED)
        {
          ccs_children.insert(ccs_children.begin(), ca.getOperator());
        }
        Node ccs = nm->mkNode(ca.getKind(), ccs_children);
        ccs = cpol ? ccs : TermUtil::mkNegate(notk, ccs);
        Trace("ext-rew-bcp") << "BCP: propagated " << c << " -> " << ccs
                             << std::endl;
        ccs = Rewriter::rewrite(ccs);
        Trace("ext-rew-bcp") << "BCP: rewritten to " << ccs << std::endl;
        to_process.push_back(ccs);
        // store this as a node that propagation touched. This marks c so that
        // it will not be included in the final construction.
        prop_clauses.insert(ca);
      }
      else
      {
        new_clauses.push_back(c);
      }
    }
    clauses.clear();
    clauses.insert(clauses.end(), new_clauses.begin(), new_clauses.end());
  } while (!to_process.empty());

  // remake the node
  if (!prop_clauses.empty())
  {
    std::vector<Node> children;
    for (std::pair<const Node, Node>& l : assign)
    {
      Node a = l.first;
      // if propagation did not touch a
      if (prop_clauses.find(a) == prop_clauses.end())
      {
        Assert(l.second == truen || l.second == falsen);
        Node ln = (l.second == truen) == gpol ? a : TermUtil::mkNegate(notk, a);
        children.push_back(ln);
      }
    }
    Node new_ret = children.size() == 1 ? children[0] : nm->mkNode(k, children);
    Trace("ext-rew-bcp") << "BCP: **** OUTPUT: " << new_ret << std::endl;
    return new_ret;
  }

  return Node::null();
}

Node ExtendedRewriter::extendedRewriteEqRes(
    Kind andk, Kind ork, Kind eqk, Kind xork, Kind notk, std::map<Kind, bool>& bcp_kinds, Node n)
{
  Assert( n.getKind()==andk || n.getKind()==ork );
  bool gpol = (n.getKind()==andk);
  for( unsigned i=0, nchild = n.getNumChildren(); i<nchild; i++ )
  {
    Node lit = n[i];
    Node atom = lit.getKind()==notk ? lit[0] : lit;
    bool apol = lit.getKind()!=notk;
    if( atom.getKind()==eqk || atom.getKind()==xork )
    {
      
    }
  }
  
  return Node::null();
}
  
Node ExtendedRewriter::extendedRewriteEqChain(
    Kind eqk, Kind andk, Kind ork, Kind notk, Node ret, bool isXor)
{
  Assert(ret.getKind() == eqk);

  NodeManager* nm = NodeManager::currentNM();

  TypeNode tn = ret[0].getType();

  // sort/cancelling for Boolean EQUAL/XOR-chains
  Trace("ext-rew-eqchain") << "Eq-Chain : " << ret << std::endl;

  // get the children on either side
  bool gpol = true;
  std::vector<Node> children;
  for (unsigned r = 0, size = ret.getNumChildren(); r < size; r++)
  {
    Node curr = ret[r];
    // assume, if necessary, right associative
    while (curr.getKind() == eqk && curr[0].getType() == tn)
    {
      children.push_back(curr[0]);
      curr = curr[1];
    }
    children.push_back(curr);
  }

  std::map<Node, bool> cstatus;
  // add children to status
  for (const Node& c : children)
  {
    Node a = c;
    if (a.getKind() == notk)
    {
      gpol = !gpol;
      a = a[0];
    }
    Trace("ext-rew-eqchain") << "...child : " << a << std::endl;
    std::map<Node, bool>::iterator itc = cstatus.find(a);
    if (itc == cstatus.end())
    {
      cstatus[a] = true;
    }
    else
    {
      // cancels
      cstatus.erase(a);
      if (isXor)
      {
        gpol = !gpol;
      }
    }
  }
  Trace("ext-rew-eqchain") << "Global polarity : " << gpol << std::endl;

  if (cstatus.empty())
  {
    return TermUtil::mkTypeConst(tn, gpol);
  }

  children.clear();

  // cancel AND/OR children if possible
  for (std::pair<const Node, bool>& cp : cstatus)
  {
    if (cp.second)
    {
      Node c = cp.first;
      Kind ck = c.getKind();
      if (ck == andk || ck == ork)
      {
        for (unsigned j = 0, nchild = c.getNumChildren(); j < nchild; j++)
        {
          Node cl = c[j];
          Node ca = cl.getKind() == notk ? cl[0] : cl;
          bool capol = cl.getKind() != notk;
          // if this already exists as a child of the equality chain
          std::map<Node, bool>::iterator itc = cstatus.find(ca);
          if (itc != cstatus.end() && itc->second)
          {
            // cancel it
            cstatus[ca] = false;
            cstatus[c] = false;
            // make new child
            // x = ( y | ~x ) ---> y & x
            // x = ( y | x ) ---> ~y | x
            // x = ( y & x ) ---> y | ~x
            // x = ( y & ~x ) ---> ~y & ~x
            std::vector<Node> new_children;
            for (unsigned k = 0, nchild = c.getNumChildren(); k < nchild; k++)
            {
              if (j != k)
              {
                new_children.push_back(c[k]);
              }
            }
            Node nc[2];
            nc[0] = c[j];
            nc[1] = new_children.size() == 1 ? new_children[0]
                                             : nm->mkNode(ck, new_children);
            // negate the proper child
            unsigned nindex = (ck == andk) == capol ? 0 : 1;
            nc[nindex] = TermUtil::mkNegate(notk, nc[nindex]);
            Kind nk = capol ? ork : andk;
            // store as new child
            children.push_back(nm->mkNode(nk, nc[0], nc[1]));
            if (isXor)
            {
              gpol = !gpol;
            }
            break;
          }
        }
      }
    }
  }

  // sorted right associative chain
  bool has_const = false;
  unsigned const_index = 0;
  for (std::pair<const Node, bool>& cp : cstatus)
  {
    if (cp.second)
    {
      if (cp.first.isConst())
      {
        has_const = true;
        const_index = children.size();
      }
      children.push_back(cp.first);
    }
  }
  std::sort(children.begin(), children.end());

  Node new_ret;
  if (!gpol)
  {
    // negate the constant child if it exists
    unsigned nindex = has_const ? const_index : 0;
    children[nindex] = TermUtil::mkNegate(notk, children[nindex]);
  }
  new_ret = children.back();
  unsigned index = children.size() - 1;
  while (index > 0)
  {
    index--;
    new_ret = nm->mkNode(eqk, children[index], new_ret);
  }
  new_ret = Rewriter::rewrite(new_ret);
  if (new_ret != ret)
  {
    return new_ret;
  }
  return Node::null();
}

Node ExtendedRewriter::partialSubstitute(Node n,
                                         std::map<Node, Node>& assign,
                                         std::map<Kind, bool>& rkinds)
{
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
      std::map<Node, Node>::iterator it = assign.find(cur);
      if (it != assign.end())
      {
        visited[cur] = it->second;
      }
      else
      {
        // can only recurse on these kinds
        Kind k = cur.getKind();
        if (rkinds.find(k) != rkinds.end())
        {
          visited[cur] = Node::null();
          visit.push_back(cur);
          for (const Node& cn : cur)
          {
            visit.push_back(cn);
          }
        }
        else
        {
          visited[cur] = cur;
        }
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
      if (childChanged)
      {
        ret = NodeManager::currentNM()->mkNode(cur.getKind(), children);
      }
      visited[cur] = ret;
    }
  } while (!visit.empty());
  Assert(visited.find(n) != visited.end());
  Assert(!visited.find(n)->second.isNull());
  return visited[n];
}

Node ExtendedRewriter::solveEquality(Node n)
{
  // TODO (#1706) : implement
  Assert(n.getKind() == EQUAL);

  return Node::null();
}

bool ExtendedRewriter::inferSubstitution(Node n,
                                         std::vector<Node>& vars,
                                         std::vector<Node>& subs)
{
  if (n.getKind() == EQUAL)
  {
    // see if it can be put into form x = y
    Node slv_eq = solveEquality(n);
    if (!slv_eq.isNull())
    {
      n = slv_eq;
    }
    for (unsigned i = 0; i < 2; i++)
    {
      TNode r1 = n[i];
      TNode r2 = n[1 - i];
      if (r1.isVar() && ((r2.isVar() && r1 < r2) || r2.isConst()))
      {
        // TODO (#1706) : union find
        if (std::find(vars.begin(), vars.end(), r1) == vars.end())
        {
          vars.push_back(r1);
          subs.push_back(r2);
          return true;
        }
      }
    }
  }
  return false;
}

Node ExtendedRewriter::extendedRewriteArith(Node ret, bool& pol)
{
  Kind k = ret.getKind();
  NodeManager* nm = NodeManager::currentNM();
  Node new_ret;
  if (k == DIVISION || k == INTS_DIVISION || k == INTS_MODULUS)
  {
    // rewrite as though total
    std::vector<Node> children;
    bool all_const = true;
    for (unsigned i = 0, size = ret.getNumChildren(); i < size; i++)
    {
      if (ret[i].isConst())
      {
        children.push_back(ret[i]);
      }
      else
      {
        all_const = false;
        break;
      }
    }
    if (all_const)
    {
      Kind new_k = (ret.getKind() == DIVISION ? DIVISION_TOTAL
                                              : (ret.getKind() == INTS_DIVISION
                                                     ? INTS_DIVISION_TOTAL
                                                     : INTS_MODULUS_TOTAL));
      new_ret = nm->mkNode(new_k, children);
      debugExtendedRewrite(ret, new_ret, "total-interpretation");
    }
  }
  return new_ret;
}

void ExtendedRewriter::debugExtendedRewrite(Node n,
                                            Node ret,
                                            const char* c) const
{
  if (Trace.isOn("q-ext-rewrite"))
  {
    if (!ret.isNull())
    {
      Trace("q-ext-rewrite-apply") << "sygus-extr : apply " << c << std::endl;
      Trace("q-ext-rewrite") << "sygus-extr : " << c << " : " << n
                             << " rewrites to " << ret << std::endl;
    }
  }
}

} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */
