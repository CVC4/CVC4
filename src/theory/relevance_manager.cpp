/*********************                                                        */
/*! \file relevance_manager.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of relevance manager.
 **/

#include "theory/relevance_manager.h"

using namespace CVC4::kind;

namespace CVC4 {
namespace theory {

RelevanceManager::RelevanceManager(context::UserContext* userContext,
                                   Valuation val)
    : d_val(val),
      d_input(userContext),
      d_computed(false),
      d_success(false)
{
}

void RelevanceManager::notifyPreprocessedAssertions(
    const std::vector<Node>& assertions)
{
  // add to input list, which is user-context dependent
  std::vector<Node> toProcess;
  for (const Node& a : assertions)
  {
    if (a.getKind() == AND)
    {
      // split top-level AND
      for (const Node& ac : a)
      {
        toProcess.push_back(ac);
      }
    }
    else
    {
      d_input.push_back(a);
    }
  }
  size_t i = 0;
  while (i < toProcess.size())
  {
    Node a = toProcess[i];
    if (a.getKind() == AND)
    {
      // split AND
      for (const Node& ac : a)
      {
        toProcess.push_back(ac);
      }
    }
    else
    {
      // note that a could be a literal, in which case we could add it to
      // an "always relevant" set here.
      d_input.push_back(a);
    }
    i++;
  }
}

void RelevanceManager::notifyPreprocessedAssertion(Node n)
{
  d_input.push_back(n);
}

void RelevanceManager::resetRound()
{
  d_computed = false;
  d_rset.clear();
}

void RelevanceManager::computeRelevance()
{
  d_computed = true;
  Trace("rel-manager") << "RelevanceManager::computeRelevance..." << std::endl;
  std::unordered_map<TNode, int, TNodeHashFunction> cache;
  for (NodeList::const_iterator it = d_input.begin(); it != d_input.end(); ++it)
  {
    TNode n = *it;
    int val = justify(n, cache);
    if (val != 1)
    {
      std::stringstream serr;
      serr << "RelevanceManager::computeRelevance: WARNING: failed to justify "
           << n;
      Trace("rel-manager") << serr.str() << std::endl;
      Assert(false) << serr.str();
      d_success = false;
      return;
    }
  }
  Trace("rel-manager") << "...success, size = " << d_rset.size() << std::endl;
  d_success = true;
}

// iterative?
/*
int RelevanceManager::justify(
    TNode n, std::unordered_map<TNode, int, TNodeHashFunction>& cache)
{
  std::unordered_map<TNode, size_t, TNodeHashFunction> visited;
  std::unordered_map<TNode, size_t, TNodeHashFunction>::iterator it;
  std::vector<TNode> visit;
  TNode cur;
  Kind k;
  visit.push_back(n);
  do {
    cur = visit.back();
    visit.pop_back();
    it = visited.find(cur);
    k = cur.getKind();
    if (it == visited.end()) {
      if (k==NOT || k==IMPLIES || k==AND || k==OR || k==ITE || k==XOR || (k==EQUAL && cur[0].getType().isBoolean()))
      {
        visited[cur] = 0;
        visit.push_back(cur);
        visit.insert(visit.end(),cur.begin(),cur.end());
      }
    }
    else if( !it->second<cur.getNumChildren() )
    {
      size_t currChild = it->second;
      // we are getting notified of the result of the index^th child.
      
      visited[cur] = true;
    }
  } while (!visit.empty());
}
*/

int RelevanceManager::justify(
    TNode n, std::unordered_map<TNode, int, TNodeHashFunction>& cache)
{
  std::unordered_map<TNode, int, TNodeHashFunction>::iterator it =
      cache.find(n);
  if (it != cache.end())
  {
    return it->second;
  }
  Kind k = n.getKind();
  int ret;
  if (k == NOT)
  {
    ret = -justify(n[0], cache);
  }
  else if (k == IMPLIES)
  {
    int cret = justify(n[0], cache);
    ret = cret == 1 ? justify(n[1], cache) : -cret;
  }
  else if (k == AND || k == OR)
  {
    // Boolean connective, recurse
    ret = k == AND ? 1 : -1;
    for (const Node& nc : n)
    {
      int cret = justify(nc, cache);
      if (cret == 0)
      {
        ret = 0;
      }
      else if (ret != cret)
      {
        ret = cret;
        break;
      }
    }
  }
  else if (k == ITE)
  {
    int cret = justify(n[0], cache);
    ret = cret == 1 ? justify(n[1], cache)
                    : (cret == -1 ? justify(n[2], cache) : 0);
  }
  else if (k == XOR)
  {
    int cret = justify(n[0], cache);
    ret = cret == 0 ? 0 : -cret * justify(n[1], cache);
  }
  else if (k == EQUAL && n[0].getType().isBoolean())
  {
    int cret = justify(n[0], cache);
    ret = cret == 0 ? 0 : cret * justify(n[1], cache);
  }
  else
  {
    ret = 0;
    // otherwise we look up the value
    bool value;
    if (d_val.hasSatValue(n, value))
    {
      ret = value ? 1 : -1;
      d_rset.insert(n);
    }
  }
  cache[n] = ret;
  return ret;
}

bool RelevanceManager::isRelevant(Node lit)
{
  if (!d_computed)
  {
    computeRelevance();
  }
  if (!d_success)
  {
    // always relevant if we failed to compute
    return true;
  }
  // agnostic to negation
  while (lit.getKind() == NOT)
  {
    lit = lit[0];
  }
  return d_rset.find(lit) != d_rset.end();
}

}  // namespace theory
}  // namespace CVC4
