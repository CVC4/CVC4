/*********************                                                        */
/*! \file decision_manager.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of decision_manager
 **/

#include "theory/decision_manager.h"

#include "theory/rewriter.h"

using namespace CVC4::kind;

namespace CVC4 {
namespace theory {

DecisionStrategyFmf::DecisionStrategyFmf(context::Context* satContext,
                                         Valuation valuation)
    : d_valuation(valuation),
      d_has_curr_literal(false, satContext),
      d_curr_literal(0, satContext)
{
}

void DecisionStrategyFmf::initialize() { d_literals.clear(); }

Node DecisionStrategyFmf::getNextDecisionRequest()
{
  Trace("dec-strategy-debug")
      << "Get next decision request " << identify() << "..." << std::endl;
  if (d_has_curr_literal.get())
  {
    Trace("dec-strategy-debug") << "...already has decision" << std::endl;
    return Node::null();
  }
  bool success;
  unsigned curr_lit = d_curr_literal.get();
  do
  {
    success = true;
    // get the current literal
    Node lit = getLiteral(curr_lit);
    Trace("dec-strategy-debug")
        << "...check literal #" << curr_lit << " : " << lit << std::endl;
    // if out of literals, we are done in the current SAT context
    if (!lit.isNull())
    {
      bool value;
      if (!d_valuation.hasSatValue(lit, value))
      {
        Trace("dec-strategy-debug") << "...not assigned, return." << std::endl;
        // if it has not been decided, return it
        return lit;
      }
      else if (!value)
      {
        Trace("dec-strategy-debug")
            << "...assigned false, increment." << std::endl;
        // asserted false, the current literal is incremented
        curr_lit = d_curr_literal.get() + 1;
        d_curr_literal.set(curr_lit);
        // repeat
        success = false;
      }
      else
      {
        Trace("dec-strategy-debug") << "...already assigned true." << std::endl;
      }
    }
    else
    {
      Trace("dec-strategy-debug") << "...exhausted literals." << std::endl;
    }
  } while (!success);
  // the current literal has been decided with the right polarity, we are done
  d_has_curr_literal = true;
  return Node::null();
}

bool DecisionStrategyFmf::isReadyForDecision() { return true; }

bool DecisionStrategyFmf::getAssertedLiteralIndex(unsigned& i)
{
  if (d_has_curr_literal.get())
  {
    i = d_curr_literal.get();
    return true;
  }
  return false;
}

Node DecisionStrategyFmf::getAssertedLiteral()
{
  if (d_has_curr_literal.get())
  {
    Assert(d_curr_literal.get() < d_literals.size());
    return getLiteral(d_curr_literal.get());
  }
  return Node::null();
}

Node DecisionStrategyFmf::getLiteral(unsigned n)
{
  // allocate until the index is valid
  while (n >= d_literals.size())
  {
    Node lit = mkLiteral(d_literals.size());
    if (!lit.isNull())
    {
      lit = Rewriter::rewrite(lit);
      lit = d_valuation.ensureLiteral(lit);
    }
    d_literals.push_back(lit);
  }
  return d_literals[n];
}

DecisionStrategySingleton::DecisionStrategySingleton(
    const char* name,
    Node lit,
    context::Context* satContext,
    Valuation valuation)
    : DecisionStrategyFmf(satContext, valuation), d_name(name), d_literal(lit)
{
}

Node DecisionStrategySingleton::mkLiteral(unsigned n)
{
  if (n == 0)
  {
    return d_literal;
  }
  return Node::null();
}

Node DecisionStrategySingleton::getSingleLiteral() { return d_literal; }

DecisionManager::DecisionManager(context::Context* satContext)
//    : d_curr_strategy(0, satContext)
{
}

void DecisionManager::reset()
{
  Trace("dec-manager") << "DecisionManager: reset." << std::endl;
  d_reg_strategy.clear();
}

void DecisionManager::registerStrategy(StrategyId id,
                                       DecisionStrategy* ds,
                                       bool append)
{
  ds->initialize();
  Trace("dec-manager") << "DecisionManager: Register strategy : " << ds->identify() << ", id = " << id << std::endl;
  if (append)
  {
    d_reg_strategy[id].push_back(ds);
  }
  else
  {
    std::vector<DecisionStrategy*>& stid = d_reg_strategy[id];
    stid.insert(stid.begin(), ds);
  }
}

/*
void DecisionManager::initialize()
{
  for (unsigned i = 0; i < strat_last; i++)
  {
    StrategyId s = static_cast<StrategyId>(i);
    std::map<StrategyId, std::vector<DecisionStrategy*> >::iterator itrs =
        d_reg_strategy.find(s);
    if (itrs != d_reg_strategy.end())
    {
      for (unsigned j = 0, size = itrs->second.size(); j < size; j++)
      {
        d_strategy.push_back(itrs->second[j]);
      }
    }
  }
}
*/

Node DecisionManager::getNextDecisionRequest(unsigned& priority)
{
  Trace("dec-manager-debug") << "DecisionManager: Get next decision..." << std::endl;
  for (const std::pair<StrategyId, std::vector<DecisionStrategy*> >& rs :
       d_reg_strategy)
  {
    for (unsigned i = 0, size = rs.second.size(); i < size; i++)
    {
      DecisionStrategy* ds = rs.second[i];
      Node lit = ds->getNextDecisionRequest();
      if (!lit.isNull())
      {
        StrategyId sid = rs.first;
        priority = sid < strat_last_m_sound
                       ? 0
                       : (sid < strat_last_fm_complete ? 1 : 2);
        Trace("dec-manager") << "DecisionManager:  -> literal " << lit << " decided by strategy "
                             << ds->identify() << std::endl;
        return lit;
      }
      else
      {
        Trace("dec-manager-debug")
            << "DecisionManager:  " << ds->identify() << " has no decisions." << std::endl;
      }
    }
  }
  Trace("dec-manager-debug") << "DecisionManager:  -> no decisions." << std::endl;
  /*
  unsigned sstart = d_curr_strategy.get();
  for (unsigned i = sstart, nstrat = d_strategy.size(); i < nstrat; i++)
  {
    Node lit = d_strategy[i]->getNextDecisionRequest();
    if (!lit.isNull())
    {
      return lit;
    }
    // update d_curr_strategy?
  }
  */
  return Node::null();
}

}  // namespace theory
}  // namespace CVC4
