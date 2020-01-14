/*********************                                                        */
/*! \file example_cache.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief utility for processing programming by examples synthesis conjectures
 **/

#include "cvc4_private.h"

#include "expr/node_algorithm.h"
#include "theory/quantifiers/sygus/example_cache.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

void ExampleCache::initialize(Node n, const std::vector<Node>& vars)
{
  AlwaysAssert(d_evalNode.isNull());
  d_evalNode = n;
  d_vars.insert(d_vars.end(), vars.begin(), vars.end());

  // compute its free variables
  std::unordered_set<Node, NodeHashFunction> fvs;
  expr::getFreeVariables(n, fvs);
  for (unsigned i = 0, vsize = vars.size(); i < vsize; i++)
  {
    if (fvs.find(vars[i]) != fvs.end())
    {
      // will use this index
      d_indices.push_back(i);
    }
  }
}

Node ExampleCache::evaluate(const std::vector<Node>& subs)
{
  Assert(d_vars.size() == subs.size());
  // get the subsequence of subs that is relevant
  std::vector<Node> relSubs;
  for (unsigned i = 0, ssize = subs.size(); i < ssize; i++)
  {
    relSubs.push_back(subs[i]);
  }
  Node res = d_trie.existsTerm(relSubs);
  if (res.isNull())
  {
    // not already cached, must evaluate
    res = d_eval.eval(d_evalNode, d_vars, subs);
    // add to trie
    d_trie.addTerm(res, relSubs);
  }
  return res;
}

}  // namespace quantifiers
}  // namespace theory
} /* namespace CVC4 */
