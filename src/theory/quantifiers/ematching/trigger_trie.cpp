/*********************                                                        */
/*! \file trigger_trie.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Morgan Deters, Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of trigger trie class
 **/

#include "theory/quantifiers/ematching/trigger_trie.h"

namespace CVC4 {
namespace theory {
namespace inst {

TriggerTrie::TriggerTrie() {}

TriggerTrie::~TriggerTrie()
{
  for (size_t i = 0, ntriggers = d_tr.size(); i < ntriggers; i++)
  {
    delete d_tr[i];
  }
}

inst::Trigger* TriggerTrie::getTrigger(std::vector<Node>& nodes)
{
  std::vector<Node> temp;
  temp.insert(temp.begin(), nodes.begin(), nodes.end());
  std::sort(temp.begin(), temp.end());
  TriggerTrie* tt = this;
  for (const Node& n : temp)
  {
    std::map<TNode, TriggerTrie>::iterator itt = tt->d_children.find(n);
    if (itt == tt->d_children.end())
    {
      return NULL;
    }
    else
    {
      tt = itt->second;
    }
  }
  return tt->d_tr.empty() ? NULL : tt->d_tr[0];
}

void TriggerTrie::addTrigger(std::vector<Node>& nodes, inst::Trigger* t)
{
  std::vector<Node> temp;
  temp.insert(temp.begin(), nodes.begin(), nodes.end());
  std::sort(temp.begin(), temp.end());
  TriggerTrie* tt = this;
  for (const Node& n : temp)
  {
    std::map<TNode, TriggerTrie>::iterator itt = tt->d_children.find(n);
    if (itt == tt->d_children.end())
    {
      TriggerTrie* ttn = &tt->d_children[n];
      tt = ttn;
    }
    else
    {
      tt = itt->second;
    }
  }
  tt->d_tr.push_back(t);
}

}  // namespace inst
}  // namespace theory
}  // namespace CVC4
