/*********************                                                        */
/*! \file sygus_reconstruct.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief utility for reconstruct
 **
 **/

#include "theory/quantifiers/sygus/sygus_reconstruct.h"

#include "expr/match_trie.h"
#include "expr/node_algorithm.h"
#include "smt/command.h"
#include "theory/datatypes/sygus_datatype_utils.h"
#include "theory/quantifiers/sygus/sygus_enumerator.h"

using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace quantifiers {

SygusReconstruct::SygusReconstruct(TermDbSygus* tds, SygusStatistics& s)
    : d_tds(tds), d_stats(s)
{
}

Node SygusReconstruct::reconstructSolution(Node sol,
                                           TypeNode stn,
                                           int& reconstructed,
                                           int enumLimit)
{
  Trace("sygus-rcons") << "SygusReconstruct::reconstructSolution: " << sol
                       << std::endl;
  Trace("sygus-rcons") << "- target sygus type is " << stn << std::endl;
  Trace("sygus-rcons") << "- enumeration limit is " << enumLimit << std::endl;

  // this method may get called multiple times with the same object. We need to
  // reset the state to avoid conflicts
  reset();

  initializeEnumeratorsAndDatabases(stn);

  NodeManager* nm = NodeManager::currentNM();

  // paramaters sol and stn constitute the main obligation to satisfy
  Node mainOb = nm->mkSkolem("sygus_rcons", stn);

  // add the main obligation to the set of obligations that are not yet
  // satisfied
  d_unsolvedObs[stn].emplace(mainOb);
  d_builtinTerm[mainOb] = sol;
  d_ob[stn][sol] = mainOb;

  // We need to add the main obligation to the crd in case it cannot be broken
  // down by unification. By doing so, we can solve the obligation using
  // enumeration and crd (if it is in the grammar)
  std::stringstream out;
  d_crds[stn]->addTerm(sol, false, out);

  // the set of unique (up to rewriting) patterns/shapes in the grammar used by
  // unification
  std::unordered_map<TypeNode, std::vector<Node>, TypeNodeHashFunction> pool;

  size_t count = 0;

  // algorithm
  while (d_sol[mainOb] == Node::null() && count < enumLimit)
  {
    // enumeration phase
    // a temporary set of new obligations cached for processing in the match
    // phase
    std::unordered_map<TypeNode,
                       std::unordered_set<Node, NodeHashFunction>,
                       TypeNodeHashFunction>
        obsPrime;
    for (const std::pair<const TypeNode,
                         std::unordered_set<Node, NodeHashFunction>>& pair :
         d_unsolvedObs)
    {
      // enumerate a new term
      Node sz = nextEnum(pair.first);
      if (sz == Node::null())
      {
        continue;
      }
      Node builtin = Rewriter::rewrite(datatypes::utils::sygusToBuiltin(sz));
      // if enumerated term does not contain free variables, then its
      // corresponding obligation can be solved immediately
      if (sz.isConst())
      {
        Node rep = d_crds[pair.first]->addTerm(builtin, false, out);
        Node k = d_ob[pair.first][rep];
        // check if the enumerated term solves an obligation
        if (k == Node::null())
        {
          // if not, create an "artifical" obligation whose solution would be
          // the enumerated term
          k = nm->mkSkolem("sygus_rcons", pair.first);
          d_builtinTerm[k] = builtin;
          d_ob[pair.first][builtin] = k;
        }
        // mark the obligation as solved
        markSolved(k, sz);
        // with the candidate rewrite database, there is no point in adding
        // ground terms to the pool
        continue;
      }
      // if there are no matches (all calls to notify return true)...
      if (d_poolTrie.getMatches(builtin, this))
      {
        // then, this is a new term and we should add it to pool
        d_poolTrie.addTerm(builtin);
        pool[pair.first].push_back(sz);
        for (Node k : pair.second)
        {
          if (d_sol[k] == Node::null())
          {
            Trace("sygus-rcons") << "ob: " << ob(k) << std::endl;
            // try to match/unify obligation k with the enumerated term sz
            std::unordered_map<TypeNode,
                               std::unordered_set<Node, NodeHashFunction>,
                               TypeNodeHashFunction>
                temp = matchNewObs(k, sz);
            // cache the new obligations for processing in the match phase
            for (const std::pair<const TypeNode,
                                 std::unordered_set<Node, NodeHashFunction>>&
                     tempPair : temp)
            {
              obsPrime[tempPair.first].insert(tempPair.second.cbegin(),
                                              tempPair.second.cend());
            }
          }
        }
      }
    }
    // match phase
    while (!obsPrime.empty())
    {
      // a temporary set of new obligations cached for later processing
      std::unordered_map<TypeNode,
                         std::unordered_set<Node, NodeHashFunction>,
                         TypeNodeHashFunction>
          obsDPrime;
      for (const std::pair<const TypeNode,
                           std::unordered_set<Node, NodeHashFunction>>& pair :
           obsPrime)
      {
        for (const Node& k : pair.second)
        {
          d_unsolvedObs[pair.first].emplace(k);
          if (d_sol[k] == Node::null())
          {
            Trace("sygus-rcons") << "ob: " << ob(k) << std::endl;
            for (Node sz : pool[pair.first])
            {
              // try to match/unify each newly generated and cached obligation
              // with patterns in pool
              std::unordered_map<TypeNode,
                                 std::unordered_set<Node, NodeHashFunction>,
                                 TypeNodeHashFunction>
                  temp = matchNewObs(k, sz);
              // cache the new obligations for later processing
              for (const std::pair<const TypeNode,
                                   std::unordered_set<Node, NodeHashFunction>>&
                       tempPair : temp)
              {
                obsDPrime[tempPair.first].insert(tempPair.second.cbegin(),
                                                 tempPair.second.cend());
              }
            }
          }
        }
      }
      obsPrime = std::move(obsDPrime);
    }
    // remove solved obligations from d_unsolvedObs
    removeSolvedObs();
    ++count;
  }

  if (Trace("sygus-rcons").isConnected())
  {
    printCandSols(mainOb);
    printPool(pool);
  }

  // if the main obligation is solved, return the solution
  if (d_sol[mainOb] != Node::null())
  {
    reconstructed = 1;
    return d_sol[mainOb];
  }

  // we ran out of elements, return null
  reconstructed = -1;
  Warning() << CommandFailure(
      "Cannot get synth function: reconstruction to syntax failed.");
  // could return sol here, however, we choose to fail by returning null, since
  // it indicates a failure.
  return Node::null();
}

std::unordered_map<TypeNode,
                   std::unordered_set<Node, NodeHashFunction>,
                   TypeNodeHashFunction>
SygusReconstruct::matchNewObs(Node k, Node sz)
{
  NodeManager* nm = NodeManager::currentNM();
  std::unordered_map<TypeNode,
                     std::unordered_set<Node, NodeHashFunction>,
                     TypeNodeHashFunction>
      obsPrime;

  // obligations generated by match. Note that we might have already seen (and
  // even solved) those obligations, hence the name "candidate obligations"
  std::unordered_map<Node, Node, NodeHashFunction> candObs;
  // the builtin terms corresponding to sygus variables in the grammar are bound
  // variables. However, we want the match function to treat them as ground
  // terms. So, we add redundant substitutions
  candObs.insert(d_groundVars.cbegin(), d_groundVars.cend());

  // try to match/unify the obligation's builtin term with the pattern sz
  if (expr::match(Rewriter::rewrite(datatypes::utils::sygusToBuiltin(sz)),
                  d_builtinTerm[k],
                  candObs))
  {
    // the bound variables z generated by the enumerators are reused across
    // enumerated terms, so we need to replace them with our own skolems
    std::vector<std::pair<Node, Node>> subs;
    Trace("sygus-rcons") << "-- ct: " << sz << std::endl;
    // remove redundaunt substitutions
    for (const std::pair<const Node, Node>& pair : d_groundVars)
    {
      candObs.erase(pair.first);
    }
    // for each candidate obligation
    std::stringstream out;
    for (const std::pair<const Node, Node>& candOb : candObs)
    {
      TypeNode stn =
          datatypes::utils::builtinVarToSygus(candOb.first).getType();
      Node newVar;
      // Question: should we consider equality by crd here too? (more efficient)
      // have we come across a similar obligation before?
      Node rep = d_crds[stn]->addTerm(candOb.second, false, out);
      if (d_ob[stn][rep] != Node::null())
      {
        // if so, use the original obligation
        newVar = d_ob[stn][rep];
      }
      else
      {
        // otherwise, create a new obligation of the corresponding sygus type
        newVar = nm->mkSkolem("sygus_rcons", stn);
        d_builtinTerm[newVar] = candOb.second;
        d_ob[stn][candOb.second] = newVar;
        // if the candidate obligation is a constant and the grammar allows
        // random constants
        if (candOb.second.isConst()
            && k.getType().getDType().getSygusAllowConst())
        {
          // then immediately solve the obligation
          markSolved(newVar, d_tds->getProxyVariable(stn, candOb.second));
        }
        else
        {
          // otherwise, add this candidate obligation to this list of
          // obligations
          obsPrime[stn].emplace(newVar);
        }
      }
      subs.emplace_back(datatypes::utils::builtinVarToSygus(candOb.first),
                        newVar);
    }
    // replace original free vars in sz with new ones
    if (!subs.empty())
    {
      sz = sz.substitute(subs.cbegin(), subs.cend());
    }
    // sz is solved if it has no sub-obligations or if all of them are solved
    bool isSolved = true;
    for (const std::pair<Node, Node>& sub : subs)
    {
      if (d_sol[sub.second] == Node::null())
      {
        isSolved = false;
        d_subObs[sz].push_back(sub.second);
      }
    }

    if (isSolved)
    {
      Node s = sz.substitute(d_sol.cbegin(), d_sol.cend());
      markSolved(k, s);
    }
    else
    {
      // add sz as a possible solution to obligation k
      d_candSols[k].emplace(sz);
      d_parentOb[sz] = k;
      d_watchSet[d_subObs[sz].back()].emplace(sz);
    }
  }

  return obsPrime;
}

void SygusReconstruct::markSolved(Node k, Node s)
{
  // return if obligation k is already solved
  if (d_sol[k] != Node::null())
  {
    return;
  }

  Trace("sygus-rcons") << "sol: " << s << std::endl;
  Trace("sygus-rcons") << "builtin sol: " << datatypes::utils::sygusToBuiltin(s)
                       << std::endl;

  // some free terms in the reconstructed solution get eliminated by the
  // rewriter. For example, rewite((ite true 0 z)) = 0. We replace those with
  // ground values.
  if (!s.isConst())
  {
    Trace("sygus-rcons") << datatypes::utils::sygusToBuiltin(s) << std::endl;
    s = replaceVarsWithGroundValues(s);
  }
  Assert(s.isConst());

  // First, mark `k` as solved
  d_sol[k] = s;
  d_candSols[k].emplace(s);
  d_parentOb[s] = k;

  std::vector<Node> stack;
  stack.push_back(k);

  while (!stack.empty())
  {
    Node curr = stack.back();
    stack.pop_back();

    // for each partial solution/parent of the now solved obligation `curr`
    for (Node parent : d_watchSet[curr])
    {
      // remove `curr` and (possibly) other solved obligations from its list
      // of children
      while (!d_subObs[parent].empty()
             && d_sol[d_subObs[parent].back()] != Node::null())
      {
        d_subObs[parent].pop_back();
      }

      // if the partial solution does not have any more children...
      if (d_subObs[parent].empty())
      {
        // then it is completely solved and can be used as a solution of its
        // corresponding obligation
        Node parentSol = parent.substitute(d_sol);
        Node parentOb = d_parentOb[parent];
        // proceed only if parent obligation is not already solved
        if (d_sol[parentOb] == Node::null())
        {
          d_sol[parentOb] = parentSol;
          d_candSols[parentOb].emplace(parentSol);
          d_parentOb[parentSol] = parentOb;
          // repeat the same process for the parent obligation
          stack.push_back(parentOb);
        }
      }
      else
      {
        // if it does have remaining children, add it to the watch list of one
        // of them (picked arbitrarily)
        d_watchSet[d_subObs[parent].back()].emplace(parent);
      }
    }
  }
}

void SygusReconstruct::initializeEnumeratorsAndDatabases(TypeNode stn)
{
  std::vector<Node> builtinVars;

  // Cache the sygus variables introduced by the problem (which we treat as
  // ground terms when calling the match function) as opposed to the sygus
  // variables introduced by the enumerators (which we treat as bound
  // variables).
  for (Node sv : stn.getDType().getSygusVarList())
  {
    builtinVars.push_back(datatypes::utils::sygusToBuiltin(sv));
    d_groundVars.emplace(sv, sv);
  }

  SygusTypeInfo stnInfo;
  stnInfo.initialize(d_tds, stn);

  // find the non-terminals of the grammar
  std::vector<TypeNode> sfTypes;
  stnInfo.getSubfieldTypes(sfTypes);

  d_sygusSamplers.reserve(sfTypes.size());

  NodeManager* nm = NodeManager::currentNM();

  // initialize the enumerators and candidate rewrite databases. Notice here
  // that we treat the sygus variables introduced by the problem as bound
  // variables (needed for making sure that obligations are equal). This is fine
  // as we will never add variables that were introduced by the enumerators to
  // the database.
  for (TypeNode tn : sfTypes)
  {
    d_enumerators.emplace(tn,
                          new SygusEnumerator(d_tds, nullptr, d_stats, true));
    d_enumerators[tn]->initialize(nm->mkSkolem("sygus_rcons", tn));
    d_crds.emplace(tn, new CandidateRewriteDatabase(true, false, true));
    d_sygusSamplers.emplace_back();
    d_sygusSamplers.back().initialize(tn, builtinVars, 5);
    d_crds[tn]->initialize(builtinVars, &d_sygusSamplers.back());
  }
}

Node SygusReconstruct::nextEnum(TypeNode stn)
{
  if (!d_enumerators[stn]->increment())
  {
    Trace("sygus-rcons") << "enum: " << stn << ": no increment" << std::endl;
    return Node::null();
  }

  Node sz = d_enumerators[stn]->getCurrent();

  if (sz == Node::null())
  {
    Trace("sygus-rcons") << "enum: " << stn << ": null" << std::endl;
  }
  else
  {
    Trace("sygus-rcons") << "enum: " << stn << ": "
                         << datatypes::utils::sygusToBuiltin(sz) << std::endl;
  }
  return sz;
}

void SygusReconstruct::removeSolvedObs()
{
  for (std::pair<const TypeNode, std::unordered_set<Node, NodeHashFunction>>&
           tempPair : d_unsolvedObs)
  {
    std::unordered_set<Node, NodeHashFunction>::iterator it =
        tempPair.second.begin();
    while (it != tempPair.second.end())
    {
      if (d_sol[*it] == Node::null())
      {
        ++it;
      }
      else
      {
        it = tempPair.second.erase(it);
      }
    }
  }
}

Node SygusReconstruct::replaceVarsWithGroundValues(Node n)
{
  // get the set of bound variables in n
  std::unordered_set<TNode, TNodeHashFunction> vars;
  expr::getVariables(n, vars);

  std::unordered_map<TNode, TNode, TNodeHashFunction> subs;

  // generate a ground value for each one of those variables
  for (const TNode& var : vars)
  {
    subs.emplace(var, var.getType().mkGroundValue());
  }

  // substitute the variables with ground values
  return n.substitute(subs);
}

bool SygusReconstruct::notify(Node s,
                              Node n,
                              std::vector<Node>& vars,
                              std::vector<Node>& subs)
{
  for (size_t i = 0; i < vars.size(); ++i)
  {
    // We consider sygus variables as ground terms. So, if they are not equal to
    // their substitution, then s is not matchable with n and we try the next
    // term s Example: If s = (+ z x) and n = (+ z y), then s is not matchable
    // with n and we return true
    if (d_groundVars.find(vars[i]) != d_groundVars.cend() && vars[i] != subs[i])
    {
      return true;
    }
  }
  // Note: false here means that we finally found an s that is matchable with n,
  // so we should not add n to the pool
  return false;
}

void SygusReconstruct::reset()
{
  d_unsolvedObs.clear();
  d_builtinTerm.clear();
  d_ob.clear();
  d_sol.clear();
  d_candSols.clear();
  d_subObs.clear();
  d_parentOb.clear();
  d_watchSet.clear();
  d_groundVars.clear();
  d_enumerators.clear();
  d_crds.clear();
  d_sygusSamplers.clear();
  d_poolTrie.clear();
}

std::string SygusReconstruct::ob(Node k)
{
  return "ob<" + d_builtinTerm[k].toString() + ", " + k.getType().toString()
         + ">";
}

void SygusReconstruct::printCandSols(const Node& mainOb)
{
  std::unordered_set<Node, NodeHashFunction> visited;
  std::vector<Node> stack;
  stack.push_back(mainOb);

  Trace("sygus-rcons") << "\nEq classes: \n[";

  while (!stack.empty())
  {
    const Node& k = stack.back();
    stack.pop_back();
    visited.emplace(k);

    Trace("sygus-rcons") << std::endl
                         << datatypes::utils::sygusToBuiltin(k) << " " << ob(k)
                         << ":\n [";

    for (const Node& j : d_candSols[k])
    {
      Trace("sygus-rcons") << datatypes::utils::sygusToBuiltin(j) << " ";
      std::unordered_set<TNode, TNodeHashFunction> subObs;
      expr::getVariables(j, subObs);
      for (const TNode& l : subObs)
      {
        if (visited.find(l) == visited.cend())
        {
          stack.push_back(l);
        }
      }
    }
    Trace("sygus-rcons") << "]" << std::endl;
  }

  Trace("sygus-rcons") << "]" << std::endl;
}

void SygusReconstruct::printPool(
    const std::unordered_map<TypeNode, std::vector<Node>, TypeNodeHashFunction>&
        pool)
{
  Trace("sygus-rcons") << "\nPool:\n[";

  for (const std::pair<const TypeNode, std::vector<Node>>& pair : pool)
  {
    Trace("sygus-rcons") << std::endl << pair.first << ":\n[" << std::endl;

    for (const Node& sygusTerm : pair.second)
    {
      Trace("sygus-rcons") << "  "
                           << Rewriter::rewrite(
                                  datatypes::utils::sygusToBuiltin(sygusTerm))
                                  .toString()
                           << std::endl;
    }

    Trace("sygus-rcons") << "]" << std::endl;
  }

  Trace("sygus-rcons") << "]" << std::endl;
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC4
