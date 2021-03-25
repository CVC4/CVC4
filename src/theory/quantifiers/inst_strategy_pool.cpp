/*********************                                                        */
/*! \file inst_strategy_pool.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Morgan Deters
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of a pool instantiation strategy.
 **/

#include "theory/quantifiers/inst_strategy_pool.h"

#include "theory/quantifiers/first_order_model.h"
#include "theory/quantifiers/term_pools.h"
#include "theory/quantifiers/term_registry.h"
#include "theory/quantifiers_engine.h"
#include "theory/quantifiers/term_tuple_enumerator.h"

using namespace CVC4::kind;
using namespace CVC4::context;

namespace CVC4 {
namespace theory {
namespace quantifiers {

InstStrategyPool::InstStrategyPool(QuantifiersEngine* qe,
                                   QuantifiersState& qs,
                                   QuantifiersInferenceManager& qim,
                                   QuantifiersRegistry& qr)
    : QuantifiersModule(qs, qim, qr, qe)
{
}

void InstStrategyPool::presolve() {}

bool InstStrategyPool::needsCheck(Theory::Effort e)
{
  return d_qstate.getInstWhenNeedsCheck(e);
}

void InstStrategyPool::reset_round(Theory::Effort e) {}

void InstStrategyPool::registerQuantifier(Node q)
{
  // take into account user pools
  if (q.getNumChildren() == 3)
  {
    Node subsPat = d_qreg.substituteBoundVariablesToInstConstants(q[2], q);
    // add patterns
    for (const Node& p : subsPat)
    {
      if (p.getKind() == INST_POOL)
      {
        d_userPools[q].push_back(p);
      }
    }
  }
}

void InstStrategyPool::check(Theory::Effort e, QEffort quant_e)
{
  bool doCheck = !d_userPools.empty();
  if (!doCheck)
  {
    return;
  }
  double clSet = 0;
  if (Trace.isOn("pool-engine"))
  {
    clSet = double(clock()) / double(CLOCKS_PER_SEC);
    Trace("pool-engine") << "---Pool instantiation, effort = " << e << "---"
                         << std::endl;
  }
  FirstOrderModel* fm = d_quantEngine->getModel();
  uint64_t addedLemmas = 0;
  size_t nquant = fm->getNumAssertedQuantifiers();
  std::map<Node, std::vector<Node> >::iterator uit;
  for (size_t i = 0; i < nquant; i++)
  {
    Node q = fm->getAssertedQuantifier(i, true);
    if (!d_qreg.hasOwnership(q, this) || !fm->isQuantifierActive(q))
    {
      continue;
    }
    uit = d_userPools.find(q);
    if (uit == d_userPools.end())
    {
      continue;
    }
    // process with each user pool
    for (const Node& p : uit->second)
    {
      process(q, p);
    }
  }
  if (Trace.isOn("pool-engine"))
  {
    Trace("pool-engine") << "Added lemmas = " << addedLemmas << std::endl;
    double clSet2 = double(clock()) / double(CLOCKS_PER_SEC);
    Trace("pool-engine") << "Finished pool instantiation, time = "
                         << (clSet2 - clSet) << std::endl;
  }
}

std::string InstStrategyPool::identify() const
{
  return std::string("InstStrategyPool");
}
  
void InstStrategyPool::process(Node q, Node p)
{
  TermPools * tp = d_quantEngine->getTermRegistry().getTermPools();
  std::shared_ptr<TermTupleEnumeratorInterface> ttei = mkTermTupleEnumeratorPool(
    q, context, tp, p);
  // TODO
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC4
