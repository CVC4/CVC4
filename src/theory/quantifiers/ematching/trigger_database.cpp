/*********************                                                        */
/*! \file trigger_database.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief trigger database class
 **/

#include "theory/quantifiers/ematching/trigger_database.h"

#include "theory/quantifiers/term_util.h"
#include "theory/quantifiers/ematching/trigger.h"
#include "theory/quantifiers/ematching/ho_trigger.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

TriggerDatabase::TriggerDatabase(QuantifiersState& qs,
                                 QuantifiersInferenceManager& qim,
                                 QuantifiersRegistry& qr,
                                 TermRegistry& tr)
    : d_qs(qs), d_qim(qim), d_qreg(qr), d_treg(tr)
{
}
TriggerDatabase::~TriggerDatabase() {}


Trigger* TriggerDatabase::mkTrigger(Node f,
                            const std::vector<Node>& nodes,
                            bool keepAll,
                            int trOption,
                            size_t useNVars)
{
  std::vector< Node > trNodes;
  if( !keepAll ){
    size_t nvars = useNVars == 0 ? f[0].getNumChildren() : useNVars;
    if (!mkTriggerTerms(f, nodes, nvars, trNodes))
    {
      return nullptr;
    }
  }else{
    trNodes.insert( trNodes.begin(), nodes.begin(), nodes.end() );
  }

  //check for duplicate?
  if( trOption!=TR_MAKE_NEW ){
    Trigger* t = d_trie.getTrigger( trNodes );
    if( t ){
      if( trOption==TR_GET_OLD ){
        //just return old trigger
        return t;
      }else{
        return nullptr;
      }
    }
  }

  // check if higher-order
  Trace("trigger-debug") << "Collect higher-order variable triggers..."
                         << std::endl;
  std::map<Node, std::vector<Node> > ho_apps;
  HigherOrderTrigger::collectHoVarApplyTerms(f, trNodes, ho_apps);
  Trace("trigger-debug") << "...got " << ho_apps.size()
                         << " higher-order applications." << std::endl;
  Trigger* t;
  if (!ho_apps.empty())
  {
    t = new HigherOrderTrigger(qs, qim, qr, tr, f, trNodes, ho_apps);
  }
  else
  {
    t = new Trigger(qs, qim, qr, tr, f, trNodes);
  }

  d_trie.addTrigger( trNodes, t );
  return t;
}

Trigger* TriggerDatabase::mkTrigger(Node q,
                            Node n,
                            bool keepAll,
                            int trOption,
                            size_t useNVars)
{
  std::vector< Node > nodes;
  nodes.push_back( n );
  return mkTrigger(q, nodes, keepAll, trOption, useNVars);
}

bool TriggerDatabase::mkTriggerTerms(Node q,
                             const std::vector<Node>& nodes,
                             size_t nvars,
                             std::vector<Node>& trNodes)
{
  //only take nodes that contribute variables to the trigger when added
  std::vector< Node > temp;
  temp.insert( temp.begin(), nodes.begin(), nodes.end() );
  std::map< Node, bool > vars;
  std::map< Node, std::vector< Node > > patterns;
  size_t varCount = 0;
  std::map< Node, std::vector< Node > > varContains;
  for (const Node& pat : temp)
  {
    TermUtil::computeInstConstContainsForQuant(q, pat, varContains[pat]);
  }
  for (const Node& t : temp)
  {
    const std::vector<Node>& vct = varContains[t];
    bool foundVar = false;
    for (const Node& v : vct)
    {
      Assert(TermUtil::getInstConstAttr(v) == q);
      if( vars.find( v )==vars.end() ){
        varCount++;
        vars[ v ] = true;
        foundVar = true;
      }
    }
    if( foundVar ){
      trNodes.push_back(t);
      for (const Node& v : vct)
      {
        patterns[v].push_back(t);
      }
    }
    if (varCount == nvars)
    {
      break;
    }
  }
  if (varCount < nvars)
  {
    Trace("trigger-debug") << "Don't consider trigger since it does not contain specified number of variables." << std::endl;
    Trace("trigger-debug") << nodes << std::endl;
    //do not generate multi-trigger if it does not contain all variables
    return false;
  }
  // now, minimize the trigger
  for (size_t i = 0, tsize = trNodes.size(); i < tsize; i++)
  {
    bool keepPattern = false;
    Node n = trNodes[i];
    const std::vector<Node>& vcn = varContains[n];
    for (const Node& v : vcn)
    {
      if (patterns[v].size() == 1)
      {
        keepPattern = true;
        break;
      }
    }
    if (!keepPattern)
    {
      // remove from pattern vector
      for (const Node& v : vcn)
      {
        std::vector<Node>& pv = patterns[v];
        for (size_t k = 0, pvsize = pv.size(); k < pvsize; k++)
        {
          if (pv[k] == n)
          {
            pv.erase(pv.begin() + k, pv.begin() + k + 1);
            break;
          }
        }
      }
      // remove from trigger nodes
      trNodes.erase(trNodes.begin() + i, trNodes.begin() + i + 1);
      i--;
    }
  }
  return true;
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC4

#endif /* CVC4__THEORY__QUANTIFIERS__TRIGGER_DATABASE_H */
