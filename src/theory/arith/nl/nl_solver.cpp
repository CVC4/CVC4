/*********************                                                        */
/*! \file nl_solver.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Tim King, Ahmed Irfan
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of non-linear solver
 **/

#include "theory/arith/nl/nl_solver.h"

#include "options/arith_options.h"
#include "theory/arith/arith_msum.h"
#include "theory/arith/arith_utilities.h"
#include "theory/arith/theory_arith.h"
#include "theory/theory_model.h"

using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace arith {
namespace nl {

void debugPrintBound(const char* c, Node coeff, Node x, Kind type, Node rhs)
{
  Node t = ArithMSum::mkCoeffTerm(coeff, x);
  Trace(c) << t << " " << type << " " << rhs;
}

bool hasNewMonomials(Node n, const std::vector<Node>& existing)
{
  std::set<Node> visited;

  std::vector<Node> worklist;
  worklist.push_back(n);
  while (!worklist.empty())
  {
    Node current = worklist.back();
    worklist.pop_back();
    if (visited.find(current) == visited.end())
    {
      visited.insert(current);
      if (current.getKind() == NONLINEAR_MULT)
      {
        if (std::find(existing.begin(), existing.end(), current)
            == existing.end())
        {
          return true;
        }
      }
      else
      {
        worklist.insert(worklist.end(), current.begin(), current.end());
      }
    }
  }
  return false;
}

NlSolver::NlSolver(InferenceManager& im, ArithState& astate, NlModel& model)
    : d_im(im), d_astate(astate), d_model(model), d_cdb(d_mdb)
{
  NodeManager* nm = NodeManager::currentNM();
  d_true = nm->mkConst(true);
  d_false = nm->mkConst(false);
  d_zero = nm->mkConst(Rational(0));
  d_one = nm->mkConst(Rational(1));
  d_neg_one = nm->mkConst(Rational(-1));
}

NlSolver::~NlSolver() {}

void NlSolver::initLastCall(const std::vector<Node>& assertions,
                            const std::vector<Node>& false_asserts,
                            const std::vector<Node>& xts)
{
  d_ms_vars.clear();
  d_ms.clear();
  d_mterms.clear();
  d_tplane_refine.clear();
  d_ci.clear();
  d_ci_exp.clear();
  d_ci_max.clear();

  Trace("nl-ext-mv") << "Extended terms : " << std::endl;
  // for computing congruence
  std::map<Kind, ArgTrie> argTrie;
  for (unsigned i = 0, xsize = xts.size(); i < xsize; i++)
  {
    Node a = xts[i];
    d_model.computeConcreteModelValue(a);
    d_model.computeAbstractModelValue(a);
    d_model.printModelValue("nl-ext-mv", a);
    Kind ak = a.getKind();
    if (ak == NONLINEAR_MULT)
    {
      d_ms.push_back(a);

      // context-independent registration
      d_mdb.registerMonomial(a);

      const std::vector<Node>& varList = d_mdb.getVariableList(a);
      for (const Node& v : varList)
      {
        if (std::find(d_ms_vars.begin(), d_ms_vars.end(), v) == d_ms_vars.end())
        {
          d_ms_vars.push_back(v);
        }
      }
      // mark processed if has a "one" factor (will look at reduced monomial)?
    }
  }

  // register constants
  d_mdb.registerMonomial(d_one);

  // register variables
  Trace("nl-ext-mv") << "Variables in monomials : " << std::endl;
  for (unsigned i = 0; i < d_ms_vars.size(); i++)
  {
    Node v = d_ms_vars[i];
    d_mdb.registerMonomial(v);
    d_model.computeConcreteModelValue(v);
    d_model.computeAbstractModelValue(v);
    d_model.printModelValue("nl-ext-mv", v);
  }

  Trace("nl-ext") << "We have " << d_ms.size() << " monomials." << std::endl;
}

void NlSolver::checkTangentPlanes(bool asWaitingLemmas)
{
  Trace("nl-ext") << "Get monomial tangent plane lemmas..." << std::endl;
  NodeManager* nm = NodeManager::currentNM();
  const std::map<Node, std::vector<Node> >& ccMap =
      d_mdb.getContainsChildrenMap();
  unsigned kstart = d_ms_vars.size();
  for (unsigned k = kstart; k < d_mterms.size(); k++)
  {
    Node t = d_mterms[k];
    // if this term requires a refinement
    if (d_tplane_refine.find(t) == d_tplane_refine.end())
    {
      continue;
    }
    Trace("nl-ext-tplanes")
        << "Look at monomial requiring refinement : " << t << std::endl;
    // get a decomposition
    std::map<Node, std::vector<Node> >::const_iterator it = ccMap.find(t);
    if (it == ccMap.end())
    {
      continue;
    }
    std::map<Node, std::map<Node, bool> > dproc;
    for (unsigned j = 0; j < it->second.size(); j++)
    {
      Node tc = it->second[j];
      if (tc != d_one)
      {
        Node tc_diff = d_mdb.getContainsDiffNl(tc, t);
        Assert(!tc_diff.isNull());
        Node a = tc < tc_diff ? tc : tc_diff;
        Node b = tc < tc_diff ? tc_diff : tc;
        if (dproc[a].find(b) == dproc[a].end())
        {
          dproc[a][b] = true;
          Trace("nl-ext-tplanes")
              << "  decomposable into : " << a << " * " << b << std::endl;
          Node a_v_c = d_model.computeAbstractModelValue(a);
          Node b_v_c = d_model.computeAbstractModelValue(b);
          // points we will add tangent planes for
          std::vector<Node> pts[2];
          pts[0].push_back(a_v_c);
          pts[1].push_back(b_v_c);
          // if previously refined
          bool prevRefine = d_tangent_val_bound[0][a].find(b)
                            != d_tangent_val_bound[0][a].end();
          // a_min, a_max, b_min, b_max
          for (unsigned p = 0; p < 4; p++)
          {
            Node curr_v = p <= 1 ? a_v_c : b_v_c;
            if (prevRefine)
            {
              Node pt_v = d_tangent_val_bound[p][a][b];
              Assert(!pt_v.isNull());
              if (curr_v != pt_v)
              {
                Node do_extend =
                    nm->mkNode((p == 1 || p == 3) ? GT : LT, curr_v, pt_v);
                do_extend = Rewriter::rewrite(do_extend);
                if (do_extend == d_true)
                {
                  for (unsigned q = 0; q < 2; q++)
                  {
                    pts[p <= 1 ? 0 : 1].push_back(curr_v);
                    pts[p <= 1 ? 1 : 0].push_back(
                        d_tangent_val_bound[p <= 1 ? 2 + q : q][a][b]);
                  }
                }
              }
            }
            else
            {
              d_tangent_val_bound[p][a][b] = curr_v;
            }
          }

          for (unsigned p = 0; p < pts[0].size(); p++)
          {
            Node a_v = pts[0][p];
            Node b_v = pts[1][p];

            // tangent plane
            Node tplane = nm->mkNode(
                MINUS,
                nm->mkNode(
                    PLUS, nm->mkNode(MULT, b_v, a), nm->mkNode(MULT, a_v, b)),
                nm->mkNode(MULT, a_v, b_v));
            // conjuncts of the tangent plane lemma
            std::vector<Node> tplaneConj;
            for (unsigned d = 0; d < 4; d++)
            {
              Node aa = nm->mkNode(d == 0 || d == 3 ? GEQ : LEQ, a, a_v);
              Node ab = nm->mkNode(d == 1 || d == 3 ? GEQ : LEQ, b, b_v);
              Node conc = nm->mkNode(d <= 1 ? LEQ : GEQ, t, tplane);
              Node tlem = nm->mkNode(OR, aa.negate(), ab.negate(), conc);
              Trace("nl-ext-tplanes")
                  << "Tangent plane lemma : " << tlem << std::endl;
              tplaneConj.push_back(tlem);
            }

            // tangent plane reverse implication

            // t <= tplane -> ( (a <= a_v ^ b >= b_v) v
            // (a >= a_v ^ b <= b_v) ).
            // in clause form, the above becomes
            // t <= tplane -> a <= a_v v b <= b_v.
            // t <= tplane -> b >= b_v v a >= a_v.
            Node a_leq_av = nm->mkNode(LEQ, a, a_v);
            Node b_leq_bv = nm->mkNode(LEQ, b, b_v);
            Node a_geq_av = nm->mkNode(GEQ, a, a_v);
            Node b_geq_bv = nm->mkNode(GEQ, b, b_v);

            Node t_leq_tplane = nm->mkNode(LEQ, t, tplane);
            Node a_leq_av_or_b_leq_bv = nm->mkNode(OR, a_leq_av, b_leq_bv);
            Node b_geq_bv_or_a_geq_av = nm->mkNode(OR, b_geq_bv, a_geq_av);
            Node ub_reverse1 =
                nm->mkNode(OR, t_leq_tplane.negate(), a_leq_av_or_b_leq_bv);
            Trace("nl-ext-tplanes")
                << "Tangent plane lemma (reverse) : " << ub_reverse1
                << std::endl;
            tplaneConj.push_back(ub_reverse1);
            Node ub_reverse2 =
                nm->mkNode(OR, t_leq_tplane.negate(), b_geq_bv_or_a_geq_av);
            Trace("nl-ext-tplanes")
                << "Tangent plane lemma (reverse) : " << ub_reverse2
                << std::endl;
            tplaneConj.push_back(ub_reverse2);

            // t >= tplane -> ( (a <= a_v ^ b <= b_v) v
            // (a >= a_v ^ b >= b_v) ).
            // in clause form, the above becomes
            // t >= tplane -> a <= a_v v b >= b_v.
            // t >= tplane -> b >= b_v v a <= a_v
            Node t_geq_tplane = nm->mkNode(GEQ, t, tplane);
            Node a_leq_av_or_b_geq_bv = nm->mkNode(OR, a_leq_av, b_geq_bv);
            Node a_geq_av_or_b_leq_bv = nm->mkNode(OR, a_geq_av, b_leq_bv);
            Node lb_reverse1 =
                nm->mkNode(OR, t_geq_tplane.negate(), a_leq_av_or_b_geq_bv);
            Trace("nl-ext-tplanes")
                << "Tangent plane lemma (reverse) : " << lb_reverse1
                << std::endl;
            tplaneConj.push_back(lb_reverse1);
            Node lb_reverse2 =
                nm->mkNode(OR, t_geq_tplane.negate(), a_geq_av_or_b_leq_bv);
            Trace("nl-ext-tplanes")
                << "Tangent plane lemma (reverse) : " << lb_reverse2
                << std::endl;
            tplaneConj.push_back(lb_reverse2);

            Node tlem = nm->mkNode(AND, tplaneConj);
            d_im.addPendingArithLemma(
                tlem, InferenceId::NL_TANGENT_PLANE, asWaitingLemmas);
          }
        }
      }
    }
  }
}

void NlSolver::checkMonomialInferBounds(
    const std::vector<Node>& asserts,
    const std::vector<Node>& false_asserts)
{
  // sort monomials by degree
  Trace("nl-ext-proc") << "Sort monomials by degree..." << std::endl;
  d_mdb.sortByDegree(d_ms);
  // all monomials
  d_mterms.insert(d_mterms.end(), d_ms_vars.begin(), d_ms_vars.end());
  d_mterms.insert(d_mterms.end(), d_ms.begin(), d_ms.end());

  const std::map<Node, std::map<Node, ConstraintInfo> >& cim =
      d_cdb.getConstraints();

  NodeManager* nm = NodeManager::currentNM();
  // register constraints
  Trace("nl-ext-debug") << "Register bound constraints..." << std::endl;
  for (const Node& lit : asserts)
  {
    bool polarity = lit.getKind() != NOT;
    Node atom = lit.getKind() == NOT ? lit[0] : lit;
    d_cdb.registerConstraint(atom);
    bool is_false_lit =
        std::find(false_asserts.begin(), false_asserts.end(), lit)
        != false_asserts.end();
    // add information about bounds to variables
    std::map<Node, std::map<Node, ConstraintInfo> >::const_iterator itc =
        cim.find(atom);
    if (itc == cim.end())
    {
      continue;
    }
    for (const std::pair<const Node, ConstraintInfo>& itcc : itc->second)
    {
      Node x = itcc.first;
      Node coeff = itcc.second.d_coeff;
      Node rhs = itcc.second.d_rhs;
      Kind type = itcc.second.d_type;
      Node exp = lit;
      if (!polarity)
      {
        // reverse
        if (type == EQUAL)
        {
          // we will take the strict inequality in the direction of the
          // model
          Node lhs = ArithMSum::mkCoeffTerm(coeff, x);
          Node query = nm->mkNode(GT, lhs, rhs);
          Node query_mv = d_model.computeAbstractModelValue(query);
          if (query_mv == d_true)
          {
            exp = query;
            type = GT;
          }
          else
          {
            Assert(query_mv == d_false);
            exp = nm->mkNode(LT, lhs, rhs);
            type = LT;
          }
        }
        else
        {
          type = negateKind(type);
        }
      }
      // add to status if maximal degree
      d_ci_max[x][coeff][rhs] = d_cdb.isMaximal(atom, x);
      if (Trace.isOn("nl-ext-bound-debug2"))
      {
        Node t = ArithMSum::mkCoeffTerm(coeff, x);
        Trace("nl-ext-bound-debug2") << "Add Bound: " << t << " " << type << " "
                                     << rhs << " by " << exp << std::endl;
      }
      bool updated = true;
      std::map<Node, Kind>::iterator its = d_ci[x][coeff].find(rhs);
      if (its == d_ci[x][coeff].end())
      {
        d_ci[x][coeff][rhs] = type;
        d_ci_exp[x][coeff][rhs] = exp;
      }
      else if (type != its->second)
      {
        Trace("nl-ext-bound-debug2")
            << "Joining kinds : " << type << " " << its->second << std::endl;
        Kind jk = joinKinds(type, its->second);
        if (jk == UNDEFINED_KIND)
        {
          updated = false;
        }
        else if (jk != its->second)
        {
          if (jk == type)
          {
            d_ci[x][coeff][rhs] = type;
            d_ci_exp[x][coeff][rhs] = exp;
          }
          else
          {
            d_ci[x][coeff][rhs] = jk;
            d_ci_exp[x][coeff][rhs] =
                nm->mkNode(AND, d_ci_exp[x][coeff][rhs], exp);
          }
        }
        else
        {
          updated = false;
        }
      }
      if (Trace.isOn("nl-ext-bound"))
      {
        if (updated)
        {
          Trace("nl-ext-bound") << "Bound: ";
          debugPrintBound("nl-ext-bound", coeff, x, d_ci[x][coeff][rhs], rhs);
          Trace("nl-ext-bound") << " by " << d_ci_exp[x][coeff][rhs];
          if (d_ci_max[x][coeff][rhs])
          {
            Trace("nl-ext-bound") << ", is max degree";
          }
          Trace("nl-ext-bound") << std::endl;
        }
      }
      // compute if bound is not satisfied, and store what is required
      // for a possible refinement
      if (options::nlExtTangentPlanes())
      {
        if (is_false_lit)
        {
          d_tplane_refine.insert(x);
        }
      }
    }
  }
  // reflexive constraints
  Node null_coeff;
  for (unsigned j = 0; j < d_mterms.size(); j++)
  {
    Node n = d_mterms[j];
    d_ci[n][null_coeff][n] = EQUAL;
    d_ci_exp[n][null_coeff][n] = d_true;
    d_ci_max[n][null_coeff][n] = false;
  }

  Trace("nl-ext") << "Get inferred bound lemmas..." << std::endl;
  const std::map<Node, std::vector<Node> >& cpMap =
      d_mdb.getContainsParentMap();
  for (unsigned k = 0; k < d_mterms.size(); k++)
  {
    Node x = d_mterms[k];
    Trace("nl-ext-bound-debug")
        << "Process bounds for " << x << " : " << std::endl;
    std::map<Node, std::vector<Node> >::const_iterator itm = cpMap.find(x);
    if (itm == cpMap.end())
    {
      Trace("nl-ext-bound-debug") << "...has no parent monomials." << std::endl;
      continue;
    }
    Trace("nl-ext-bound-debug")
        << "...has " << itm->second.size() << " parent monomials." << std::endl;
    // check derived bounds
    std::map<Node, std::map<Node, std::map<Node, Kind> > >::iterator itc =
        d_ci.find(x);
    if (itc == d_ci.end())
    {
      continue;
    }
    for (std::map<Node, std::map<Node, Kind> >::iterator itcc =
             itc->second.begin();
         itcc != itc->second.end();
         ++itcc)
    {
      Node coeff = itcc->first;
      Node t = ArithMSum::mkCoeffTerm(coeff, x);
      for (std::map<Node, Kind>::iterator itcr = itcc->second.begin();
           itcr != itcc->second.end();
           ++itcr)
      {
        Node rhs = itcr->first;
        // only consider this bound if maximal degree
        if (!d_ci_max[x][coeff][rhs])
        {
          continue;
        }
        Kind type = itcr->second;
        for (unsigned j = 0; j < itm->second.size(); j++)
        {
          Node y = itm->second[j];
          Node mult = d_mdb.getContainsDiff(x, y);
          // x <k> t => m*x <k'> t  where y = m*x
          // get the sign of mult
          Node mmv = d_model.computeConcreteModelValue(mult);
          Trace("nl-ext-bound-debug2")
              << "Model value of " << mult << " is " << mmv << std::endl;
          if (!mmv.isConst())
          {
            Trace("nl-ext-bound-debug")
                << "     ...coefficient " << mult
                << " is non-constant (probably transcendental)." << std::endl;
            continue;
          }
          int mmv_sign = mmv.getConst<Rational>().sgn();
          Trace("nl-ext-bound-debug2")
              << "  sign of " << mmv << " is " << mmv_sign << std::endl;
          if (mmv_sign == 0)
          {
            Trace("nl-ext-bound-debug")
                << "     ...coefficient " << mult << " is zero." << std::endl;
            continue;
          }
          Trace("nl-ext-bound-debug")
              << "  from " << x << " * " << mult << " = " << y << " and " << t
              << " " << type << " " << rhs << ", infer : " << std::endl;
          Kind infer_type = mmv_sign == -1 ? reverseRelationKind(type) : type;
          Node infer_lhs = nm->mkNode(MULT, mult, t);
          Node infer_rhs = nm->mkNode(MULT, mult, rhs);
          Node infer = nm->mkNode(infer_type, infer_lhs, infer_rhs);
          Trace("nl-ext-bound-debug") << "     " << infer << std::endl;
          infer = Rewriter::rewrite(infer);
          Trace("nl-ext-bound-debug2")
              << "     ...rewritten : " << infer << std::endl;
          // check whether it is false in model for abstraction
          Node infer_mv = d_model.computeAbstractModelValue(infer);
          Trace("nl-ext-bound-debug")
              << "       ...infer model value is " << infer_mv << std::endl;
          if (infer_mv == d_false)
          {
            Node exp =
                nm->mkNode(AND,
                           nm->mkNode(mmv_sign == 1 ? GT : LT, mult, d_zero),
                           d_ci_exp[x][coeff][rhs]);
            Node iblem = nm->mkNode(IMPLIES, exp, infer);
            Node pr_iblem = iblem;
            iblem = Rewriter::rewrite(iblem);
            bool introNewTerms = hasNewMonomials(iblem, d_ms);
            Trace("nl-ext-bound-lemma")
                << "*** Bound inference lemma : " << iblem
                << " (pre-rewrite : " << pr_iblem << ")" << std::endl;
            // Trace("nl-ext-bound-lemma") << "       intro new
            // monomials = " << introNewTerms << std::endl;
            d_im.addPendingArithLemma(iblem, InferenceId::NL_INFER_BOUNDS_NT, introNewTerms);
          }
        }
      }
    }
  }
}

void NlSolver::checkMonomialInferResBounds()
{
  NodeManager* nm = NodeManager::currentNM();
  Trace("nl-ext") << "Get monomial resolution inferred bound lemmas..."
                  << std::endl;
  size_t nmterms = d_mterms.size();
  for (unsigned j = 0; j < nmterms; j++)
  {
    Node a = d_mterms[j];
    std::map<Node, std::map<Node, std::map<Node, Kind> > >::iterator itca =
        d_ci.find(a);
    if (itca == d_ci.end())
    {
      continue;
    }
    for (unsigned k = (j + 1); k < nmterms; k++)
    {
      Node b = d_mterms[k];
      std::map<Node, std::map<Node, std::map<Node, Kind> > >::iterator itcb =
          d_ci.find(b);
      if (itcb == d_ci.end())
      {
        continue;
      }
      Trace("nl-ext-rbound-debug") << "resolution inferences : compare " << a
                                   << " and " << b << std::endl;
      // if they have common factors
      std::map<Node, Node>::iterator ita = d_mono_diff[a].find(b);
      if (ita == d_mono_diff[a].end())
      {
        continue;
      }
      Trace("nl-ext-rbound") << "Get resolution inferences for [a] " << a
                             << " vs [b] " << b << std::endl;
      std::map<Node, Node>::iterator itb = d_mono_diff[b].find(a);
      Assert(itb != d_mono_diff[b].end());
      Node mv_a = d_model.computeAbstractModelValue(ita->second);
      Assert(mv_a.isConst());
      int mv_a_sgn = mv_a.getConst<Rational>().sgn();
      if (mv_a_sgn == 0)
      {
        // we don't compare monomials whose current model value is zero
        continue;
      }
      Node mv_b = d_model.computeAbstractModelValue(itb->second);
      Assert(mv_b.isConst());
      int mv_b_sgn = mv_b.getConst<Rational>().sgn();
      if (mv_b_sgn == 0)
      {
        // we don't compare monomials whose current model value is zero
        continue;
      }
      Trace("nl-ext-rbound") << "  [a] factor is " << ita->second
                             << ", sign in model = " << mv_a_sgn << std::endl;
      Trace("nl-ext-rbound") << "  [b] factor is " << itb->second
                             << ", sign in model = " << mv_b_sgn << std::endl;

      std::vector<Node> exp;
      // bounds of a
      for (std::map<Node, std::map<Node, Kind> >::iterator itcac =
               itca->second.begin();
           itcac != itca->second.end();
           ++itcac)
      {
        Node coeff_a = itcac->first;
        for (std::map<Node, Kind>::iterator itcar = itcac->second.begin();
             itcar != itcac->second.end();
             ++itcar)
        {
          Node rhs_a = itcar->first;
          Node rhs_a_res_base = nm->mkNode(MULT, itb->second, rhs_a);
          rhs_a_res_base = Rewriter::rewrite(rhs_a_res_base);
          if (hasNewMonomials(rhs_a_res_base, d_ms))
          {
            continue;
          }
          Kind type_a = itcar->second;
          exp.push_back(d_ci_exp[a][coeff_a][rhs_a]);

          // bounds of b
          for (std::map<Node, std::map<Node, Kind> >::iterator itcbc =
                   itcb->second.begin();
               itcbc != itcb->second.end();
               ++itcbc)
          {
            Node coeff_b = itcbc->first;
            Node rhs_a_res = ArithMSum::mkCoeffTerm(coeff_b, rhs_a_res_base);
            for (std::map<Node, Kind>::iterator itcbr = itcbc->second.begin();
                 itcbr != itcbc->second.end();
                 ++itcbr)
            {
              Node rhs_b = itcbr->first;
              Node rhs_b_res = nm->mkNode(MULT, ita->second, rhs_b);
              rhs_b_res = ArithMSum::mkCoeffTerm(coeff_a, rhs_b_res);
              rhs_b_res = Rewriter::rewrite(rhs_b_res);
              if (hasNewMonomials(rhs_b_res, d_ms))
              {
                continue;
              }
              Kind type_b = itcbr->second;
              exp.push_back(d_ci_exp[b][coeff_b][rhs_b]);
              if (Trace.isOn("nl-ext-rbound"))
              {
                Trace("nl-ext-rbound") << "* try bounds : ";
                debugPrintBound("nl-ext-rbound", coeff_a, a, type_a, rhs_a);
                Trace("nl-ext-rbound") << std::endl;
                Trace("nl-ext-rbound") << "               ";
                debugPrintBound("nl-ext-rbound", coeff_b, b, type_b, rhs_b);
                Trace("nl-ext-rbound") << std::endl;
              }
              Kind types[2];
              for (unsigned r = 0; r < 2; r++)
              {
                Node pivot_factor = r == 0 ? itb->second : ita->second;
                int pivot_factor_sign = r == 0 ? mv_b_sgn : mv_a_sgn;
                types[r] = r == 0 ? type_a : type_b;
                if (pivot_factor_sign == (r == 0 ? 1 : -1))
                {
                  types[r] = reverseRelationKind(types[r]);
                }
                if (pivot_factor_sign == 1)
                {
                  exp.push_back(nm->mkNode(GT, pivot_factor, d_zero));
                }
                else
                {
                  exp.push_back(nm->mkNode(LT, pivot_factor, d_zero));
                }
              }
              Kind jk = transKinds(types[0], types[1]);
              Trace("nl-ext-rbound-debug")
                  << "trans kind : " << types[0] << " + " << types[1] << " = "
                  << jk << std::endl;
              if (jk != UNDEFINED_KIND)
              {
                Node conc = nm->mkNode(jk, rhs_a_res, rhs_b_res);
                Node conc_mv = d_model.computeAbstractModelValue(conc);
                if (conc_mv == d_false)
                {
                  Node rblem = nm->mkNode(IMPLIES, nm->mkNode(AND, exp), conc);
                  Trace("nl-ext-rbound-lemma-debug")
                      << "Resolution bound lemma "
                         "(pre-rewrite) "
                         ": "
                      << rblem << std::endl;
                  rblem = Rewriter::rewrite(rblem);
                  Trace("nl-ext-rbound-lemma")
                      << "Resolution bound lemma : " << rblem << std::endl;
                  d_im.addPendingArithLemma(rblem, InferenceId::NL_RES_INFER_BOUNDS);
                }
              }
              exp.pop_back();
              exp.pop_back();
              exp.pop_back();
            }
          }
          exp.pop_back();
        }
      }
    }
  }
}

}  // namespace nl
}  // namespace arith
}  // namespace theory
}  // namespace CVC4
