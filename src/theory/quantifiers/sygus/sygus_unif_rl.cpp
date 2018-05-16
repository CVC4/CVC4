/*********************                                                        */
/*! \file sygus_unif_rl.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Haniel Barbosa, Andrew Reynolds
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
void SygusUnifRl::initializeCandidate(
    QuantifiersEngine* qe,
    Node f,
    std::vector<Node>& enums,
    std::map<Node, std::vector<Node>>& strategy_lemmas)
{
  // initialize
  std::vector<Node> all_enums;
  SygusUnif::initializeCandidate(qe, f, all_enums, strategy_lemmas);
  // based on the strategy inferred for each function, determine if we are
  // using a unification strategy that is compatible our approach.
  d_strategy[f].staticLearnRedundantOps(strategy_lemmas);
  registerStrategy(f, enums);
  // Copy candidates and check whether CegisUnif for any of them
  if (d_unif_candidates.find(f) != d_unif_candidates.end())
  {
    d_hd_to_pt[f].clear();
    d_cand_to_eval_hds[f].clear();
    d_cand_to_hd_count[f] = 0;
  }
}

void SygusUnifRl::notifyEnumeration(Node e, Node v, std::vector<Node>& lemmas)
{
  // we do not use notify enumeration
  Assert(false);
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
  // Recurse
  unsigned size = n.getNumChildren();
  Kind k = n.getKind();
  // We retrive model value now because purified node may not have a value
  Node nv = n;
  // Whether application of a function-to-synthesize
  bool fapp = k == APPLY_UF && size > 0;
  bool u_fapp = false;
  bool nu_fapp = false;
  if (fapp)
  {
    Assert(std::find(d_candidates.begin(), d_candidates.end(), n[0])
           != d_candidates.end());
    // Whether application of a (non-)unification function-to-synthesize
    u_fapp = usingUnif(n[0]);
    nu_fapp = !usingUnif(n[0]);
    // get model value of non-top level applications of functions-to-synthesize
    // occurring under a unification function-to-synthesize
    if (ensureConst)
    {
      std::map<Node, Node>::iterator it = d_cand_to_sol.find(n[0]);
      // if function-to-synthesize, retrieve its built solution to replace in
      // the application before computing the model value
      AlwaysAssert(!u_fapp || it != d_cand_to_sol.end());
      if (it != d_cand_to_sol.end())
      {
        TNode cand = n[0];
        Node tmp = n.substitute(cand, it->second);
        nv = d_tds->evaluateWithUnfolding(tmp);
        Trace("sygus-unif-rl-purify") << "PurifyLemma : model value for " << tmp
                                      << " is " << nv << "\n";
      }
      else
      {
        nv = d_parent->getModelValue(n);
        Trace("sygus-unif-rl-purify") << "PurifyLemma : model value for " << n
                                      << " is " << nv << "\n";
      }
      Assert(n != nv);
    }
  }
  // Travese to purify
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
    // Arguments of non-unif functions do not need to be constant
    Node child = purifyLemma(
        n[i], !nu_fapp && (ensureConst || u_fapp), model_guards, cache);
    children.push_back(child);
    childChanged = childChanged || child != n[i];
  }
  Node nb;
  if (childChanged)
  {
    if (fapp && n.hasOperator())
    {
      Trace("sygus-unif-rl-purify-debug") << "Node " << n
                                          << " has operator and fapp is true\n";
      children.insert(children.begin(), n.getOperator());
    }
    if (Trace.isOn("sygus-unif-rl-purify-debug"))
    {
      Trace("sygus-unif-rl-purify-debug")
          << "...rebuilding " << n << " with kind " << k << " and children:\n";
      for (const Node& child : children)
      {
        Trace("sygus-unif-rl-purify-debug") << "...... " << child << "\n";
      }
    }
    nb = NodeManager::currentNM()->mkNode(k, children);
    Trace("sygus-unif-rl-purify") << "PurifyLemma : transformed " << n
                                  << " into " << nb << "\n";
  }
  else
  {
    nb = n;
  }
  // Map to point enumerator every unification function-to-synthesize
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
      // Build purified head with fresh skolem and recreate node
      std::stringstream ss;
      ss << nb[0] << "_" << d_cand_to_hd_count[nb[0]]++;
      Node new_f = nm->mkSkolem(ss.str(),
                                nb[0].getType(),
                                "head of unif evaluation point",
                                NodeManager::SKOLEM_EXACT_NAME);
      // Adds new enumerator to map from candidate
      Trace("sygus-unif-rl-purify") << "...new enum " << new_f
                                    << " for candidate " << nb[0] << "\n";
      d_cand_to_eval_hds[nb[0]].push_back(new_f);
      // Maps new enumerator to its respective tuple of arguments
      d_hd_to_pt[new_f] =
          std::vector<Node>(children.begin() + 2, children.end());
      if (Trace.isOn("sygus-unif-rl-purify-debug"))
      {
        Trace("sygus-unif-rl-purify-debug") << "...[" << new_f << "] --> (";
        for (const Node& pt_i : d_hd_to_pt[new_f])
        {
          Trace("sygus-unif-rl-purify-debug") << pt_i << " ";
        }
        Trace("sygus-unif-rl-purify-debug") << ")\n";
      }
      // replace first child and rebulid node
      children[1] = new_f;
      Assert(children.size() > 1);
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
  // Add equality between purified fapp and model value
  if (ensureConst && fapp)
  {
    model_guards.push_back(
        NodeManager::currentNM()->mkNode(EQUAL, nv, nb).negate());
    nb = nv;
    Trace("sygus-unif-rl-purify") << "PurifyLemma : adding model eq "
                                  << model_guards.back() << "\n";
  }
  nb = Rewriter::rewrite(nb);
  // every non-top level application of function-to-synthesize must be reduced
  // to a concrete constant
  Assert(!ensureConst || nb.isConst());
  Trace("sygus-unif-rl-purify-debug") << "... caching [" << n << "] = " << nb
                                      << "\n";
  cache[BoolNodePair(ensureConst, n)] = nb;
  return nb;
}

Node SygusUnifRl::addRefLemma(Node lemma,
                              std::map<Node, std::vector<Node>>& eval_hds)
{
  Trace("sygus-unif-rl-purify") << "Registering lemma at SygusUnif : " << lemma
                                << "\n";
  std::vector<Node> model_guards;
  BoolNodePairMap cache;
  // cache previous sizes
  std::map<Node, unsigned> prev_n_eval_hds;
  for (const std::pair<const Node, std::vector<Node>>& cp : d_cand_to_eval_hds)
  {
    prev_n_eval_hds[cp.first] = cp.second.size();
  }

  // Make the purified lemma which will guide the unification utility.
  Node plem = purifyLemma(lemma, false, model_guards, cache);
  if (!model_guards.empty())
  {
    model_guards.push_back(plem);
    plem = NodeManager::currentNM()->mkNode(OR, model_guards);
  }
  plem = Rewriter::rewrite(plem);
  Trace("sygus-unif-rl-purify") << "Purified lemma : " << plem << "\n";

  Trace("sygus-unif-rl-purify") << "Collect new evaluation points...\n";
  for (const std::pair<const Node, std::vector<Node>>& cp : d_cand_to_eval_hds)
  {
    Node c = cp.first;
    unsigned prevn = 0;
    std::map<Node, unsigned>::iterator itp = prev_n_eval_hds.find(c);
    if (itp != prev_n_eval_hds.end())
    {
      prevn = itp->second;
    }
    for (unsigned j = prevn, size = cp.second.size(); j < size; j++)
    {
      eval_hds[c].push_back(cp.second[j]);
      // Add new point to respective decision trees
      Assert(d_cand_cenums.find(c) != d_cand_cenums.end());
      for (const Node& cenum : d_cand_cenums[c])
      {
        Assert(d_cenum_to_stratpt.find(cenum) != d_cenum_to_stratpt.end());
        for (const Node& stratpt : d_cenum_to_stratpt[cenum])
        {
          Assert(d_stratpt_to_dt.find(stratpt) != d_stratpt_to_dt.end());
          Trace("sygus-unif-rl-dt") << "Register point with head "
                                    << cp.second[j] << " to strategy point "
                                    << stratpt << "\n";
          // Register new point from new head
          d_stratpt_to_dt[stratpt].d_hds.push_back(cp.second[j]);
        }
      }
    }
  }

  return plem;
}

void SygusUnifRl::initializeConstructSol() { d_sepPairs.clear(); }
void SygusUnifRl::initializeConstructSolFor(Node f) {}
bool SygusUnifRl::constructSolution(std::vector<Node>& sols, std::vector< Node >& lemmas)
{
  initializeConstructSol();
  bool successful = true;
  for (const Node& c : d_candidates)
  {
    if (!usingUnif(c))
    {
      Node v = d_parent->getModelValue(c);
      sols.push_back(v);
      continue;
    }
    initializeConstructSolFor(c);
    Node v = constructSol(c, d_strategy[c].getRootEnumerator(), role_equal, 0, lemmas);
    if (v.isNull())
    {
      // we continue trying to build solutions to accumulate potentitial
      // separation conditions from other decision trees
      successful = false;
      continue;
    }
    sols.push_back(v);
    d_cand_to_sol[c] = v;
  }
  return successful;
}

Node SygusUnifRl::constructSol(Node f, Node e, NodeRole nrole, int ind, std::vector< Node >& lemmas)
{
  indent("sygus-unif-sol", ind);
  Trace("sygus-unif-sol") << "ConstructSol: SygusRL : " << e << std::endl;
  // retrieve strategy information
  TypeNode etn = e.getType();
  EnumTypeInfo& tinfo = d_strategy[f].getEnumTypeInfo(etn);
  StrategyNode& snode = tinfo.getStrategyNode(nrole);
  if (nrole != role_equal)
  {
    return Node::null();
  }
  // is there a decision tree strategy?
  std::map<Node, DecisionTreeInfo>::iterator itd = d_stratpt_to_dt.find(e);
  // for now only considering simple case of sole "ITE(cond, e, e)" strategy
  if (itd == d_stratpt_to_dt.end())
  {
    return Node::null();
  }
  indent("sygus-unif-sol", ind);
  Trace("sygus-unif-sol") << "...it has a decision tree strategy.\n";
  // whether empty set of points
  if (d_cand_to_eval_hds[f].empty())
  {
    Trace("sygus-unif-sol") << "...... no points, return root enum value "
                            << d_parent->getModelValue(e) << "\n";
    return d_parent->getModelValue(e);
  }
  EnumTypeInfoStrat* etis = snode.d_strats[itd->second.getStrategyIndex()];
  Node sol = itd->second.buildSol(etis->d_cons, lemmas);
  if (sol.isNull())
  {
    Assert(!lemmas.empty());
  }
  return sol;
}

bool SygusUnifRl::usingUnif(Node f) const
{
  return d_unif_candidates.find(f) != d_unif_candidates.end();
}

Node SygusUnifRl::getConditionForEvaluationPoint(Node e) const
{
  std::map<Node, DecisionTreeInfo>::const_iterator it = d_stratpt_to_dt.find(e);
  Assert(it != d_stratpt_to_dt.end());
  return it->second.getConditionEnumerator();
}

void SygusUnifRl::setConditions(Node e, const std::vector< Node >& enums, const std::vector<Node>& conds)
{
  std::map<Node, DecisionTreeInfo>::iterator it = d_stratpt_to_dt.find(e);
  Assert(it != d_stratpt_to_dt.end());
  // set the conditions for the appropriate tree
  it->second.setConditions(enums, conds);
}

std::vector<Node> SygusUnifRl::getEvalPointHeads(Node c)
{
  std::map<Node, std::vector<Node>>::iterator it = d_cand_to_eval_hds.find(c);
  if (it == d_cand_to_eval_hds.end())
  {
    return std::vector<Node>();
  }
  return it->second;
}

void SygusUnifRl::registerStrategy(Node f, std::vector<Node>& enums)
{
  if (Trace.isOn("sygus-unif-rl-strat"))
  {
    Trace("sygus-unif-rl-strat") << "Strategy for " << f
                                 << " is : " << std::endl;
    d_strategy[f].debugPrint("sygus-unif-rl-strat");
  }
  Trace("sygus-unif-rl-strat") << "Register..." << std::endl;
  Node e = d_strategy[f].getRootEnumerator();
  std::map<Node, std::map<NodeRole, bool>> visited;
  registerStrategyNode(f, e, role_equal, visited, enums);
}

void SygusUnifRl::registerStrategyNode(
    Node f,
    Node e,
    NodeRole nrole,
    std::map<Node, std::map<NodeRole, bool>>& visited,
    std::vector<Node>& enums)
{
  Trace("sygus-unif-rl-strat") << "  register node " << e << std::endl;
  if (visited[e].find(nrole) != visited[e].end())
  {
    return;
  }
  visited[e][nrole] = true;
  TypeNode etn = e.getType();
  EnumTypeInfo& tinfo = d_strategy[f].getEnumTypeInfo(etn);
  StrategyNode& snode = tinfo.getStrategyNode(nrole);
  for (unsigned j = 0, size = snode.d_strats.size(); j < size; j++)
  {
    EnumTypeInfoStrat* etis = snode.d_strats[j];
    StrategyType strat = etis->d_this;
    // is this a simple recursive ITE strategy?
    if (strat == strat_ITE && nrole == role_equal)
    {
      bool success = true;
      for (unsigned c = 1; c <= 2; c++)
      {
        std::pair<Node, NodeRole> child = etis->d_cenum[c];
        if (child.first != e || child.second != nrole)
        {
          success = false;
          break;
        }
      }
      if (success)
      {
        Node cond = etis->d_cenum[0].first;
        Assert(etis->d_cenum[0].second == role_ite_condition);
        Trace("sygus-unif-rl-strat")
            << "  ...detected recursive ITE strategy, condition enumerator : "
            << cond << std::endl;
        // indicate that we will be enumerating values for cond
        registerConditionalEnumerator(f, e, cond, j);
        // we will be using a strategy for e
        enums.push_back(e);
      }
    }
    // TODO: recurse? for (std::pair<Node, NodeRole>& cec : etis->d_cenum)
  }
}

void SygusUnifRl::registerConditionalEnumerator(Node f,
                                                Node e,
                                                Node cond,
                                                unsigned strategy_index)
{
  // only allow one decision tree per strategy point
  if (d_stratpt_to_dt.find(e) != d_stratpt_to_dt.end())
  {
    return;
  }
  // we will do unification for this candidate
  d_unif_candidates.insert(f);
  // add to the list of all conditional enumerators
  if (std::find(d_cond_enums.begin(), d_cond_enums.end(), cond)
      == d_cond_enums.end())
  {
    d_cond_enums.push_back(cond);
    d_cand_cenums[f].push_back(cond);
    d_cenum_to_stratpt[cond].clear();
  }
  // register that this strategy node has a decision tree construction
  d_stratpt_to_dt[e].initialize(cond, this, &d_strategy[f], strategy_index);
  // associate conditional enumerator with strategy node
  d_cenum_to_stratpt[cond].push_back(e);
}

void SygusUnifRl::DecisionTreeInfo::initialize(Node cond_enum,
                                               SygusUnifRl* unif,
                                               SygusUnifStrategy* strategy,
                                               unsigned strategy_index)
{
  d_cond_enum = cond_enum;
  d_unif = unif;
  d_strategy = strategy;
  d_strategy_index = strategy_index;
  // Retrieve template
  EnumInfo& eiv = d_strategy->getEnumInfo(d_cond_enum);
  d_template = NodePair(eiv.d_template, eiv.d_template_arg);
  // Initialize classifier
  d_pt_sep.initialize(this);
}

void SygusUnifRl::DecisionTreeInfo::setConditions(
    const std::vector< Node >& enums, const std::vector<Node>& conds)
{
  Assert( enums.size()==conds.size() );
  // clear old condition values
  d_enums.clear();
  d_conds.clear();
  // set new condition values
  d_enums.insert(d_enums.end(), enums.begin(), enums.end());
  d_conds.insert(d_conds.end(), conds.begin(), conds.end());
}

void SygusUnifRl::DecisionTreeInfo::addPoint(Node f)
{
  d_pt_sep.d_trie.add(f, &d_pt_sep, d_conds.size());
}

void SygusUnifRl::DecisionTreeInfo::addCondValue(Node condv)
{
  d_conds.push_back(condv);
  d_pt_sep.d_trie.addClassifier(&d_pt_sep, d_conds.size() - 1);
}

unsigned SygusUnifRl::DecisionTreeInfo::getStrategyIndex() const
{
  return d_strategy_index;
}

using UNodePair = std::pair<unsigned, Node>;

Node SygusUnifRl::DecisionTreeInfo::buildSol(Node cons,
                                             std::vector<Node>& lemmas)
{
  if (!d_template.first.isNull())
  {
    Trace("sygus-unif-sol") << "...templated conditions unsupported\n";
    return Node::null();
  }
  NodeManager* nm = NodeManager::currentNM();
  // model values for evaluation heads
  std::map< Node, Node > hd_mv;
  // reset the trie
  d_pt_sep.d_trie.clear();
  // the current explanation of why there has not yet been a separation conflict
  std::vector< Node > exp;
  // the index of the head we are considering
  unsigned hd_counter = 0;
  // the index of the condition we are considering
  unsigned c_counter = 0;
  while( hd_counter<d_hds.size() )
  {
    // add the head to the trie
    Node e = d_hds[hd_counter];
    hd_counter++;
    hd_mv[e] = d_unif->d_parent->getModelValue(e);
    // get the representative of the trie
    Node er = d_pt_sep.d_trie.add(f, &d_pt_sep, d_conds.size());
    // are we in conflict?
    if( er==e )
    {
      // new separation class, no conflict
      continue;
    }
    Assert( hd_mv.find(er)!=hd_mv.end() );
    if( hd_mv[er]==hd_mv[e] )
    {
      // merged into separation class with same model value, no conflict
      // add to explanation
      exp.push_back(er.eqNode(e));
      continue;
    }
    // we are in conflict
    // does the next condition resolve this conflict?
    Assert( c_counter<d_conds.size() );
    Node ce = d_enums[c_counter];
    Node cv = d_conds[c_counter];
    d_pt_sep.d_trie.addClassifier(&d_pt_sep, c_counter);
    c_counter++;
    // add to explanation
    Node c_exp = d_unif->d_tds->getExplain()->getExplanationForEquality(ce,cv);
    exp.push_back(c_exp);
    std::map<Node, std::vector<Node>>::iterator itr = d_pt_sep.d_rep_to_class.find(er);
    // since er is first in its separation class, it should remain a representative
    Assert(itr!=d_pt_sep.d_rep_to_class.end());
    // is e still in the separation class of er?
    std::vector< Node >& sepc_er = itr->second;
    if( std::find(sepc_er.begin(),sepc_er.end(),e)!=sepc_er.end() )
    {
      // the condition does not separate e and er
      // this violates the invariant that the i^th conditional enumerator
      // resolves the i^th separation conflict
      Node lemma = exp.size()==1 ? exp[0] : nm->mkNode(AND,exp);
      lemma = lemma.negate();
      lemmas.push_back(lemma);
      return Node::null();
    }
  }

  Trace("sygus-unif-sol") << "...ready to build solution from DT\n";
  // Traverse trie and build ITE with cons
  std::map<IndTriePair, Node> cache;
  std::map<IndTriePair, Node>::iterator it;
  std::vector<IndTriePair> visit;
  unsigned index = 0;
  LazyTrie* trie;
  IndTriePair root = IndTriePair(0, &d_pt_sep.d_trie.d_trie);
  visit.push_back(root);
  while (!visit.empty())
  {
    index = visit.back().first;
    trie = visit.back().second;
    visit.pop_back();
    IndTriePair cur = IndTriePair(index, trie);
    it = cache.find(cur);
    // traverse children so results are saved to build node for parent
    if (it == cache.end())
    {
      // leaf
      if (trie->d_children.empty())
      {
        Assert(d_hd_values.find(trie->d_lazy_child) != d_hd_values.end());
        cache[cur] = d_hd_values[trie->d_lazy_child];
        Trace("sygus-unif-sol-debug")
            << "......leaf, build "
            << d_unif->d_tds->sygusToBuiltin(cache[cur], cache[cur].getType())
            << "\n";
        continue;
      }
      cache[cur] = Node::null();
      visit.push_back(cur);
      for (std::pair<const Node, LazyTrie>& p_nt : trie->d_children)
      {
        visit.push_back(IndTriePair(index + 1, &p_nt.second));
      }
      continue;
    }
    // retrieve terms of children and build result
    Assert(it->second.isNull());
    Assert(trie->d_children.size() == 1 || trie->d_children.size() == 2);
    std::vector<Node> children(4);
    children[0] = cons;
    children[1] = d_conds[index];
    unsigned i = 0;
    for (std::pair<const Node, LazyTrie>& p_nt : trie->d_children)
    {
      i = p_nt.first.getConst<bool>() ? 2 : 3;
      Assert(cache.find(IndTriePair(index + 1, &p_nt.second)) != cache.end());
      children[i] = cache[IndTriePair(index + 1, &p_nt.second)];
      Assert(!children[i].isNull());
    }
    // condition is useless or result children are equal, no no need for ITE
    if (trie->d_children.size() == 1 || children[2] == children[3])
    {
      cache[cur] = children[i];
      Trace("sygus-unif-sol-debug")
          << "......no cond, build "
          << d_unif->d_tds->sygusToBuiltin(cache[cur], cache[cur].getType())
          << "\n";
      continue;
    }
    Assert(trie->d_children.size() == 2);
    cache[cur] = nm->mkNode(APPLY_CONSTRUCTOR, children);
    Trace("sygus-unif-sol-debug")
        << "......build node "
        << d_unif->d_tds->sygusToBuiltin(cache[cur], cache[cur].getType())
        << "\n";
  }
  Assert(cache.find(root) != cache.end());
  Assert(!cache.find(root)->second.isNull());
  Trace("sygus-unif-sol") << "...solution is "
                          << d_unif->d_tds->sygusToBuiltin(
                                 cache[root], cache[root].getType())
                          << "\n";
  return cache[root];
}

void SygusUnifRl::DecisionTreeInfo::PointSeparator::initialize(
    DecisionTreeInfo* dt)
{
  d_dt = dt;
}

Node SygusUnifRl::DecisionTreeInfo::PointSeparator::evaluate(Node n,
                                                             unsigned index)
{
  Assert(index < d_dt->d_conds.size());
  // Retrieve respective built_in condition
  Node cond = d_dt->d_conds[index];
  TypeNode tn = cond.getType();
  Node builtin_cond = d_dt->d_unif->d_tds->sygusToBuiltin(cond, tn);
  // Retrieve evaluation point
  Assert(d_dt->d_unif->d_hd_to_pt.find(n) != d_dt->d_unif->d_hd_to_pt.end());
  std::vector<Node> pt = d_dt->d_unif->d_hd_to_pt[n];
  // compute the result
  Node res = d_dt->d_unif->d_tds->evaluateBuiltin(tn, builtin_cond, pt);
  if (Trace.isOn("sygus-unif-rl-sep"))
  {
    Trace("sygus-unif-rl-sep") << "...got res = " << res << " from cond "
                               << builtin_cond << " on pt " << n << " ( ";
    for (const Node& pti : pt)
    {
      Trace("sygus-unif-rl-sep") << pti << " ";
    }
    Trace("sygus-unif-rl-sep") << ")\n";
  }
  // If condition is templated, recompute result accordingly
  Node templ = d_dt->d_template.first;
  TNode templ_var = d_dt->d_template.second;
  if (!templ.isNull())
  {
    res = templ.substitute(templ_var, res);
    res = Rewriter::rewrite(res);
    Trace("sygus-unif-rl-sep") << "...after template res = " << res
                               << std::endl;
  }
  Assert(res.isConst());
  return res;
}

} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */
