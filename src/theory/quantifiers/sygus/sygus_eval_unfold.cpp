/*********************                                                        */
/*! \file sygus_eval_unfold.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of sygus_eval_unfold
 **/

#include "theory/quantifiers/sygus/sygus_eval_unfold.h"

#include "options/quantifiers_options.h"
#include "theory/quantifiers/sygus/term_database_sygus.h"

using namespace std;
using namespace CVC4::kind;
using namespace CVC4::context;

namespace CVC4 {
namespace theory {
namespace quantifiers {

SygusEvalUnfold::SygusEvalUnfold(TermDbSygus* tds) : d_tds(tds) {}

void SygusEvalUnfold::registerEvalTerm(Node n)
{
  Assert(options::sygusEvalUnfold());
  if (n.getKind() != APPLY_UF || n.getType().isBoolean())
  {
    return;
  }
  TypeNode tn = n[0].getType();
  if (!tn.isDatatype())
  {
    return;
  }
  const Datatype& dt = static_cast<DatatypeType>(tn.toType()).getDatatype();
  if (!dt.isSygus())
  {
    return;
  }
  Node f = n.getOperator();
  if (n[0].getKind() == APPLY_CONSTRUCTOR)
  {
    return;
  }
  if (d_eval_processed.find(n) != d_eval_processed.end())
  {
    return;
  }
  Trace("sygus-eval-unfold")
      << "SygusEvalUnfold: register eval term : " << n << std::endl;
  d_eval_processed.insert(n);
  d_evals[n[0]].push_back(n);
  Node var_list = Node::fromExpr(dt.getSygusVarList());
  d_eval_args[n[0]].push_back(std::vector<Node>());
  bool isConst = true;
  for (unsigned j = 1, size = n.getNumChildren(); j < size; j++)
  {
    d_eval_args[n[0]].back().push_back(n[j]);
    if (!n[j].isConst())
    {
      isConst = false;
    }
  }
  d_eval_args_const[n[0]].push_back(isConst);
  Node a = TermDbSygus::getAnchor(n[0]);
  d_subterms[a][n[0]] = true;
}

void SygusEvalUnfold::registerModelValue(Node a,
                                         Node v,
                                         std::vector<Node>& terms,
                                         std::vector<Node>& vals,
                                         std::vector<Node>& exps)
{
  std::map<Node, std::map<Node, bool> >::iterator its = d_subterms.find(a);
  if (its == d_subterms.end())
  {
    return;
  }
  SygusExplain* sy_exp = d_tds->getExplain();
  Trace("sygus-eval-unfold")
      << "SygusEvalUnfold: " << a << ", has " << its->second.size()
      << " registered subterms." << std::endl;
  for (std::map<Node, bool>::iterator itss = its->second.begin();
       itss != its->second.end();
       ++itss)
  {
    Node n = itss->first;
    Trace("sygus-eval-unfold-debug") << "...process : " << n << std::endl;
    std::map<Node, std::vector<std::vector<Node> > >::iterator it =
        d_eval_args.find(n);
    if (it != d_eval_args.end() && !it->second.empty())
    {
      TNode at = a;
      TNode vt = v;
      Node vn = n.substitute(at, vt);
      vn = Rewriter::rewrite(vn);
      unsigned start = d_node_mv_args_proc[n][vn];
      // get explanation in terms of testers
      std::vector<Node> antec_exp;
      sy_exp->getExplanationForEquality(n, vn, antec_exp);
      Node antec = antec_exp.size() == 1
                       ? antec_exp[0]
                       : NodeManager::currentNM()->mkNode(kind::AND, antec_exp);
      // Node antec = n.eqNode( vn );
      TypeNode tn = n.getType();
      Assert(tn.isDatatype());
      const Datatype& dt = ((DatatypeType)(tn).toType()).getDatatype();
      Assert(dt.isSygus());
      Trace("sygus-eval-unfold")
          << "SygusEvalUnfold: Register model value : " << vn << " for " << n
          << std::endl;
      unsigned curr_size = it->second.size();
      Trace("sygus-eval-unfold")
          << "...it has " << curr_size
          << " evaluations, already processed " << start << "." << std::endl;
      Node bTerm = d_tds->sygusToBuiltin(vn, tn);
      Trace("sygus-eval-unfold") << "Built-in term : " << bTerm << std::endl;
      std::vector<Node> vars;
      Node var_list = Node::fromExpr(dt.getSygusVarList());
      for (const Node& v : var_list)
      {
        vars.push_back(v);
      }
      // evaluation children
      std::vector<Node> eval_children;
      eval_children.push_back(Node::fromExpr(dt.getSygusEvaluationFunc()));
      eval_children.push_back(n);
      // for each evaluation
      for (unsigned i = start; i < curr_size; i++)
      {
        Node res;
        Node expn;
        // should we unfold?
        bool do_unfold = false;
        if (options::sygusEvalUnfoldBool())
        {
          if (bTerm.getKind() == ITE || bTerm.getType().isBoolean())
          {
            do_unfold = true;
          }
        }
        if (do_unfold)
        {
          // TODO : this is replicated for different values, possibly do better
          // caching
          std::map<Node, Node> vtm;
          std::vector<Node> exp;
          vtm[n] = vn;
          eval_children.insert(
              eval_children.end(), it->second[i].begin(), it->second[i].end());
          Node eval_fun =
              NodeManager::currentNM()->mkNode(kind::APPLY_UF, eval_children);
          eval_children.resize(2);
          res = d_tds->unfold(eval_fun, vtm, exp);
          expn = exp.size() == 1
                     ? exp[0]
                     : NodeManager::currentNM()->mkNode(kind::AND, exp);
        }
        else
        {
          EvalSygusInvarianceTest esit;
          eval_children.insert(
              eval_children.end(), it->second[i].begin(), it->second[i].end());
          Node conj =
              NodeManager::currentNM()->mkNode(kind::APPLY_UF, eval_children);
          eval_children[1] = vn;
          Node eval_fun =
              NodeManager::currentNM()->mkNode(kind::APPLY_UF, eval_children);
          res = d_tds->evaluateWithUnfolding(eval_fun);
          esit.init(conj, n, res);
          eval_children.resize(2);
          eval_children[1] = n;

          // evaluate with minimal explanation
          std::vector<Node> mexp;
          sy_exp->getExplanationFor(n, vn, mexp, esit);
          Assert(!mexp.empty());
          expn = mexp.size() == 1
                     ? mexp[0]
                     : NodeManager::currentNM()->mkNode(kind::AND, mexp);
        }
        Assert(!res.isNull());
        terms.push_back(d_evals[n][i]);
        vals.push_back(res);
        exps.push_back(expn);
        Trace("sygus-eval-unfold")
            << "Conclude : " << d_evals[n][i] << " == " << res
            << ", given model value = " << d_eval_args_const[n][i] << std::endl;
        Trace("sygus-eval-unfold") << "   from " << expn << std::endl;
      }
      d_node_mv_args_proc[n][vn] = curr_size;
    }
  }
}

} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */
