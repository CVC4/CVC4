/*********************                                                        */
/*! \file sygus_unif_rl.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Haniel Barbosa
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of sygus_unif_rl
 **/

#include "theory/quantifiers/sygus/sygus_unif_rl.h"

#include "theory/quantifiers/sygus/ce_guided_conjecture.h"
#include "theory/quantifiers/sygus/term_database_sygus.h"

using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace quantifiers {

SygusUnifRl::SygusUnifRl(CegConjecture* p) : d_parent(p) {}
SygusUnifRl::~SygusUnifRl() {}
void SygusUnifRl::initialize(QuantifiersEngine* qe,
                             const std::vector<Node>& funs,
                             std::vector<Node>& enums,
                             std::vector<Node>& lemmas)
{
  d_ecache.clear();
  d_cand_to_cond_enum.clear();
  d_cand_to_pt_enum.clear();
  /* TODO populate d_unif_candidates and remove lemmas cleaning */
  SygusUnif::initialize(qe, funs, enums, lemmas);
  lemmas.clear();
  /* Copy candidates and check whether CegisUnif for any of them */
  for (const Node& c : d_unif_candidates)
  {
    d_app_to_pt[c].clear();
    d_cand_to_pt_enum[c].clear();
    d_purified_count[c] = 0;
  }
}

void SygusUnifRl::notifyEnumeration(Node e, Node v, std::vector<Node>& lemmas)
{
  Trace("sygus-unif-rl-notify") << "SyGuSUnifRl: Adding to enum " << e
                                << " value " << v << "\n";
  d_ecache[e].d_enum_vals.push_back(v);
  /* Exclude v from next enumerations for e */
  Node exc_lemma =
      d_tds->getExplain()->getExplanationForEquality(e, v).negate();
  Trace("sygus-unif-rl-notify")
      << "SygusUnifRl : enumeration exclude lemma : " << exc_lemma << std::endl;
  lemmas.push_back(exc_lemma);
}

Node SygusUnifRl::purifyLemma(Node n,
                              bool ensureConst,
                              std::vector<Node>& model_guards,
                              BoolNodePairMap& cache)
{
  Trace("sygus-unif-rl-purify") << "PurifyLemma : " << n << "\n";
  BoolNodePairMap::const_iterator it = cache.find(BoolNodePair(ensureConst, n));
  if (it != cache.end())
  {
    Trace("sygus-unif-rl-purify-debug") << "... already visited " << n << "\n";
    return it->second;
  }
  /* Recurse */
  unsigned size = n.getNumChildren();
  Kind k = n.getKind();
  /* Whether application of a function-to-synthesize */
  bool fapp = k == APPLY_UF && size > 0;
  Assert(std::find(d_candidates.begin(), d_candidates.end(), n[0])
         == d_candidates.end());
  /* Whether application of a (non-)unification function-to-synthesize */
  bool u_fapp = fapp && usingUnif(n[0]);
  bool nu_fapp = fapp && !usingUnif(n[0]);
  /* We retrive model value now because purified node may not have a value */
  Node nv = n;
  /* get model value of non-top level applications of functions-to-synthesize
     occurring under a unification function-to-synthesize */
  if (ensureConst && fapp)
  {
    nv = d_parent->getModelValue(n);
    Assert(n != nv);
    Trace("sygus-unif-rl-purify") << "PurifyLemma : model value for " << n
                                  << " is " << nv << "\n";
  }
  /* Travese to purify */
  bool childChanged = false;
  std::vector<Node> children;
  NodeManager* nm = NodeManager::currentNM();
  for (unsigned i = 0; i < size; ++i)
  {
    if (i == 0 && fapp)
    {
      children.push_back(n[i]);
      continue;
    }
    /* Arguments of non-unif functions do not need to be constant */
    Node child = purifyLemma(
        n[i], !nu_fapp && (ensureConst || u_fapp), model_guards, cache);
    children.push_back(child);
    childChanged = childChanged || child != n[i];
  }
  Node nb;
  if (childChanged)
  {
    if (n.hasOperator())
    {
      children.insert(children.begin(), n.getOperator());
    }
    nb = NodeManager::currentNM()->mkNode(k, children);
    Trace("sygus-unif-rl-purify") << "PurifyLemma : transformed " << n
                                  << " into " << nb << "\n";
  }
  else
  {
    nb = n;
  }
  /* Map to point enumerator every unification function-to-synthesize  */
  if (u_fapp)
  {
    Node np;
    std::map<Node, Node>::const_iterator it = d_app_to_purified.find(nb);
    if (it == d_app_to_purified.end())
    {
      if (!childChanged)
      {
        Assert(nb.hasOperator());
        children.insert(children.begin(), n.getOperator());
      }
      /* Build purified head with fresh skolem and recreate node */
      std::stringstream ss;
      ss << nb[0] << "_" << d_purified_count[nb[0]]++;
      Node new_f = nm->mkSkolem(ss.str(), nb[0].getType());
      /* Adds new enumerator to map from candidate */
      Trace("sygus-unif-rl-purify") << "...new enum " << new_f
                                        << " for candidate " << nb[0] << "\n";
      d_cand_to_pt_enum[nb[0]].push_back(new_f);
      /* Maps new enumerator to its respective tuple of arguments */
      d_app_to_pt[new_f] =
          std::vector<Node>(children.begin() + 2, children.end());
      if (Trace.isOn("sygus-unif-rl-purify"))
      {
        Trace("sygus-unif-rl-purify") << "...[" << new_f << "] --> (";
        for (const Node& pt_i : d_app_to_pt[new_f])
        {
          Trace("sygus-unif-rl-purify") << pt_i << " ";
        }
        Trace("sygus-unif-rl-purify") << ")\n";
      }
      /* replace first child and rebulid node */
      children[1] = new_f;
      np = NodeManager::currentNM()->mkNode(k, children);
      d_app_to_purified[nb] = np;
    }
    else
    {
      np = it->second;
    }
    Trace("sygus-unif-rl-purify")
        << "PurifyLemma : purified head and transformed " << nb << " into "
        << np << "\n";
    nb = np;
  }
  /* Add equality between purified fapp and model value */
  if (ensureConst && fapp)
  {
    model_guards.push_back(
        NodeManager::currentNM()->mkNode(EQUAL, nv, nb).negate());
    nb = nv;
    Trace("sygus-unif-rl-purify") << "PurifyLemma : adding model eq "
                                  << model_guards.back() << "\n";
  }
  nb = Rewriter::rewrite(nb);
  /* every non-top level application of function-to-synthesize must be reduced
     to a concrete constant */
  Assert(!ensureConst || nb.isConst());
  Trace("sygus-unif-rl-purify-debug") << "... caching [" << n << "] = " << nb
                                      << "\n";
  cache[BoolNodePair(ensureConst, n)] = nb;
  return nb;
}

Node SygusUnifRl::addRefLemma(Node lemma)
{
  Trace("sygus-unif-rl-purify") << "Registering lemma at SygusUnif : " << lemma
                               << "\n";
  std::vector<Node> model_guards;
  BoolNodePairMap cache;
  /* Make the purified lemma which will guide the unification utility. */
  Node plem = purifyLemma(lemma, false, model_guards, cache);
  if (!model_guards.empty())
  {
    model_guards.push_back(plem);
    plem = NodeManager::currentNM()->mkNode(OR, model_guards);
  }
  plem = Rewriter::rewrite(plem);
  Trace("sygus-unif-rl-purify") << "Purified lemma : " << plem << "\n";
  return plem;
}

void SygusUnifRl::initializeConstructSol() {}
void SygusUnifRl::initializeConstructSolFor(Node f) {}
bool SygusUnifRl::constructSolution(std::vector<Node>& sols)
{
  for (const Node& c : d_candidates)
  {
    if (!usingUnif(c))
    {
      Node v = d_parent->getModelValue(c);
      Trace("sygus-unif-rl-sol") << "Adding solution " << v
                                 << " to non-unif candidate " << c << "\n";
      sols.push_back(v);
    }
    else
    {
      return false;
    }
  }
  return true;
}

Node SygusUnifRl::constructSol(Node f, Node e, NodeRole nrole, int ind)
{
  return Node::null();
}

bool SygusUnifRl::usingUnif(Node f)
{
  return d_unif_candidates.find(f) != d_unif_candidates.end();
}

} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */
