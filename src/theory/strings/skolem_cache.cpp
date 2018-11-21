/*********************                                                        */
/*! \file skolem_cache.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of a cache of skolems for theory of strings.
 **/

#include "theory/strings/skolem_cache.h"

#include "theory/rewriter.h"

namespace CVC4 {
namespace theory {
namespace strings {

SkolemCache::SkolemCache()
{
  d_strType = NodeManager::currentNM()->stringType();
}

Node SkolemCache::mkSkolemCached(Node a, Node b, SkolemId id, const char* c)
{
  return mkTypedSkolemCached(d_strType, a, b, id, c);
}

Node SkolemCache::mkSkolemCached(Node a, SkolemId id, const char* c)
{
  return mkSkolemCached(a, Node::null(), id, c);
}

Node SkolemCache::mkTypedSkolemCached(
    TypeNode tn, Node a, Node b, SkolemId id, const char* c)
{
  a = a.isNull() ? a : Rewriter::rewrite(a);
  b = b.isNull() ? b : Rewriter::rewrite(b);
  std::map<SkolemId, Node>::iterator it = d_skolemCache[a][b].find(id);
  if (it == d_skolemCache[a][b].end())
  {
    Node sk = mkTypedSkolem(tn, c);
    d_skolemCache[a][b][id] = sk;
    return sk;
  }
  return it->second;
}
Node SkolemCache::mkTypedSkolemCached(TypeNode tn,
                                      Node a,
                                      SkolemId id,
                                      const char* c)
{
  return mkTypedSkolemCached(tn, a, Node::null(), id, c);
}

Node SkolemCache::mkSkolem(const char* c)
{
  return mkTypedSkolem(d_strType, c);
}

Node SkolemCache::mkTypedSkolem(TypeNode tn, const char* c)
{
  Node n = NodeManager::currentNM()->mkSkolem(c, tn, "string skolem");
  d_allSkolems.insert(n);
  return n;
}

bool SkolemCache::isSkolem(Node n) const
{
  return d_allSkolems.find(n) != d_allSkolems.end();
}

}  // namespace strings
}  // namespace theory
}  // namespace CVC4
