/*********************                                                        */
/*! \file node_view.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andres Noetzli
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Classes that provide different views on nodes
 **
 ** The classes in this file provide different views on a given node (e.g. an
 ** iterator that treats a certain kind as flattened).
 **/

#include "expr/node_view.h"

namespace CVC4 {
namespace expr {

template <bool ref_count>
FlatViewTemplate<ref_count>::FlatViewTemplate(NodeTemplate<ref_count> node,
                                              Kind kind,
                                              bool skipDups)
    : d_node(node), d_kind(kind), d_skipDups(skipDups)
{
  Assert(node.getKind() == kind);
}

template <bool ref_count>
FlatViewTemplate<ref_count>::iterator::iterator(NodeTemplate<ref_count> node,
                                                Kind kind,
                                                bool end,
                                                bool skipDups)
    : d_kind(kind), d_skipDups(skipDups)
{
  if (end)
  {
    d_iters.emplace_back(node.end(), node.end());
  }
  else
  {
    do
    {
      d_iters.emplace_back(node.begin(), node.end());
      node = node[0];
    } while (node.getKind() == d_kind);

    if (skipDups)
    {
      d_visited.insert(**this);
    }
  }
}

template <bool ref_count>
typename FlatViewTemplate<ref_count>::iterator&
FlatViewTemplate<ref_count>::iterator::operator++()
{
  do
  {
    NodeValue::iterator<NodeTemplate<ref_count>>* currIter =
        &d_iters.back().first;
    NodeValue::iterator<NodeTemplate<ref_count>>* currIterEnd =
        &d_iters.back().second;

    ++(*currIter);
    while (*currIter == *currIterEnd && d_iters.size() > 1)
    {
      d_iters.pop_back();
      currIter = &d_iters.back().first;
      currIterEnd = &d_iters.back().second;
      ++(*currIter);
    }

    if (*currIter != *currIterEnd)
    {
      NodeTemplate<ref_count> currNode = **currIter;
      while (currNode.getKind() == d_kind)
      {
        d_iters.emplace_back(currNode.begin(), currNode.end());
        currNode = *d_iters.back().first;
      }
    }
  } while (d_skipDups && !isDone()
           && d_visited.find(**this) != d_visited.end());

  if (!isDone())
  {
    d_visited.insert(**this);
  }

  return *this;
}

template FlatViewTemplate<true>::FlatViewTemplate(NodeTemplate<true> node,
                                                  Kind kind,
                                                  bool skipDups);
template FlatViewTemplate<false>::FlatViewTemplate(NodeTemplate<false> node,
                                                   Kind kind,
                                                   bool skipDups);
template FlatViewTemplate<true>::iterator::iterator(NodeTemplate<true> node,
                                                    Kind kind,
                                                    bool end,
                                                    bool skipDups);
template FlatViewTemplate<false>::iterator::iterator(NodeTemplate<false> node,
                                                     Kind kind,
                                                     bool end,
                                                     bool skipDups);
template typename FlatViewTemplate<true>::iterator&
FlatViewTemplate<true>::iterator::operator++();
template typename FlatViewTemplate<false>::iterator&
FlatViewTemplate<false>::iterator::operator++();

}  // namespace expr
}  // namespace CVC4
