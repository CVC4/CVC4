/*********************                                                        */
/*! \file bv_quick_check.cpp
 ** \verbatim
 ** Original author: Liana Hadaren
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Wrapper around the SAT solver used for bitblasting.
 **
 ** Wrapper around the SAT solver used for bitblasting.
 **/

#include "theory/bv/bv_quick_check.h"
#include "theory/bv/theory_bv_utils.h"

#include "theory/bv/bitblaster_template.h"

using namespace CVC4;
using namespace CVC4::theory;
using namespace CVC4::theory::bv;
using namespace CVC4::prop;

BVQuickCheck::BVQuickCheck(const std::string& name)
  : d_ctx(new context::Context())
  , d_bitblaster(new TLazyBitblaster(d_ctx, NULL, name))
  , d_conflict()
  , d_inConflict(d_ctx, false)
{}


bool BVQuickCheck::inConflict() { return d_inConflict.get(); }

uint64_t BVQuickCheck::computeAtomWeight(TNode node, NodeSet& seen) {
  return d_bitblaster->computeAtomWeight(node, seen);
}

void BVQuickCheck::setConflict() {
  Assert (!inConflict());
  std::vector<TNode> conflict;
  d_bitblaster->getConflict(conflict);
  Node confl = utils::mkConjunction(conflict);
  d_inConflict = true;
  d_conflict = confl;
}

prop::SatValue BVQuickCheck::checkSat(std::vector<Node>& assumptions, unsigned long budget) {
  Node conflict; 

  for (unsigned i = 0; i < assumptions.size(); ++i) {
    TNode a = assumptions[i];
    Assert (a.getType().isBoolean());
    d_bitblaster->bbAtom(a);
    bool ok = d_bitblaster->assertToSat(a, false);
    if (!ok) {
      setConflict(); 
      return SAT_VALUE_FALSE; 
    }
  }
  
  if (budget == 0) {
    bool ok = d_bitblaster->propagate();
    if (!ok) {
      setConflict();
      return SAT_VALUE_FALSE;
    }
    return SAT_VALUE_UNKNOWN; // could check if assignment is full and return SAT_VALUE_TRUE
  }

  prop::SatValue res = d_bitblaster->solveWithBudget(budget);
  if (res == SAT_VALUE_FALSE) {
    setConflict();
    return res;
  }
  // could be unknown or could be sat
   return res;
}

prop::SatValue BVQuickCheck::checkSat(unsigned long budget) {
  prop::SatValue res = d_bitblaster->solveWithBudget(budget);
  if (res == SAT_VALUE_FALSE) {
    setConflict();
  }
  return res;
}

bool BVQuickCheck::addAssertion(TNode assertion) {
  Assert (assertion.getType().isBoolean());
  d_bitblaster->bbAtom(assertion);
  // assert to sat solver and run bcp to detect easy conflicts
  bool ok = d_bitblaster->assertToSat(assertion, true);
  if (!ok) {
    setConflict();
  }
  return ok;
}


void BVQuickCheck::push() {
  d_ctx->push();
}

void BVQuickCheck::pop() {
  d_ctx->pop();
}

/** 
 * Constructs a new sat solver which requires throwing out the atomBBCache
 * but keeps all the termBBCache
 * 
 */
void BVQuickCheck::clearSolver() {
  popToZero();
  d_bitblaster->clearSolver();
}

void BVQuickCheck::popToZero() {
  while (d_ctx->getLevel() > 0) {
    d_ctx->pop();
  }
}

BVQuickCheck::~BVQuickCheck() {
  delete d_bitblaster;
}

QuickXPlain::QuickXPlain(const std::string& name, BVQuickCheck* solver, unsigned long budget)
  : d_solver(solver)
  , d_budget(budget)
  , d_numCalled(0)
  , d_minRatioSum(0)
  , d_numConflicts(0)
  , d_period(20)
  , d_thresh(0.7)
  , d_hardThresh(0.9)
  , d_statistics(name)
{}
QuickXPlain::~QuickXPlain() {}

unsigned QuickXPlain::selectUnsatCore(unsigned low, unsigned high,
                                      std::vector<TNode>& conflict) {

  Assert(!d_solver->getConflict().isNull() &&
         d_solver->inConflict());
  Node query_confl = d_solver->getConflict();

  // conflict wasn't actually minimized
  if (query_confl.getNumChildren() == high - low + 1) {
    return high;
  }

  TNodeSet nodes;
  for (unsigned i = low; i <= high; i++) {
    nodes.insert(conflict[i]);
  }
  
  unsigned write = low;
  for (unsigned i = 0; i < query_confl.getNumChildren(); ++i) {
    TNode current = query_confl[i];
    // the conflict can have nodes in lower decision levels
    if (nodes.find(current) != nodes.end()) {
      conflict[write++] = current;
      nodes.erase(nodes.find(current));
    }
  }
  if (write == low) {
    return high;
  }
  Assert (write != 0);
  unsigned new_high = write - 1;

  for (TNodeSet::const_iterator it = nodes.begin(); it != nodes.end(); ++it) {
    conflict[write++] = *it;
  }
  Assert (write -1 == high);
  Assert (new_high <= high);
  
  return new_high;
}

void QuickXPlain::minimizeConflictInternal(unsigned low, unsigned high,
                                           std::vector<TNode>& conflict,
                                           std::vector<TNode>& new_conflict) {

  Assert (low <= high && high < conflict.size());
  
  if (low == high) {
    new_conflict.push_back(conflict[low]);
    return;
  }

  // check if top half is unsat
  unsigned new_low = (high - low + 1)/ 2 + low;
  d_solver->push();
  
  for (unsigned i = new_low; i <=high; ++i) {
    bool ok = d_solver->addAssertion(conflict[i]);
    if (!ok) {
      unsigned top = selectUnsatCore(new_low, i, conflict);
      minimizeConflictInternal(new_low, top, conflict, new_conflict);
      return;
    }
  }

  SatValue res = d_solver->checkSat(d_budget);

  if (res == SAT_VALUE_UNKNOWN) {
    ++(d_statistics.d_numUnknown);
  } else {
    ++(d_statistics.d_numSolved);
  }

  if (res == SAT_VALUE_FALSE) {
    unsigned top = selectUnsatCore(new_low, high, conflict);
    d_solver->pop();
    minimizeConflictInternal(new_low, top, conflict, new_conflict);
    return;
  }
  
  d_solver->pop();
  unsigned new_high = new_low - 1;
  d_solver->push();

  // check bottom half
  for (unsigned i = low; i <= new_high; ++i) {
    bool ok = d_solver->addAssertion(conflict[i]);
    if (!ok) {
      unsigned top = selectUnsatCore(low, i, conflict);
      d_solver->pop();
      minimizeConflictInternal(low, top, conflict, new_conflict);
      return;
    }
  }
  
  res = d_solver->checkSat(d_budget);

  if (res == SAT_VALUE_UNKNOWN) {
    ++(d_statistics.d_numUnknown);
  } else {
    ++(d_statistics.d_numSolved);
  }

  if (res == SAT_VALUE_FALSE) {
    unsigned top = selectUnsatCore(low, new_high, conflict);
    d_solver->pop();
    minimizeConflictInternal(low, top, conflict, new_conflict);
    return;
  }

  // conflict needs literals in both halves
  // keep bottom half in context (no pop)
  minimizeConflictInternal(new_low, high, conflict, new_conflict);
  d_solver->pop();
  d_solver->push();
  for (unsigned i = 0; i < new_conflict.size(); ++i) {
    bool ok = d_solver->addAssertion(new_conflict[i]);
    Assert (ok);
  }
  minimizeConflictInternal(low, new_high, conflict, new_conflict);
  d_solver->pop();
}


bool QuickXPlain::useHeuristic() {
  d_statistics.d_finalPeriod.setData(d_period);
  // try to minimize conflict periodically 
  if (d_numConflicts % d_period == 0)
    return true;

  if (d_numCalled == 0) {
    return true;
  }
  
  if (d_minRatioSum / d_numCalled >= d_thresh &&
      d_numCalled <= 20 ) {
    return false;
  }

  if (d_minRatioSum / d_numCalled >= d_hardThresh) {
    return false;
  }

  return true;
}

Node QuickXPlain::minimizeConflict(TNode confl) {
  ++d_numConflicts;

  if (!useHeuristic()) {
    return confl;
  }

  ++d_numCalled;
  ++(d_statistics.d_numConflictsMinimized);
  TimerStat::CodeTimer xplainTimer(d_statistics.d_xplainTime);
  Assert (confl.getNumChildren() > 2);
  std::vector<TNode> conflict;
  for (unsigned i = 0; i < confl.getNumChildren(); ++i) {
    conflict.push_back(confl[i]);
  }
  d_solver->popToZero();
  std::vector<TNode> minimized;
  minimizeConflictInternal(0, conflict.size() - 1, conflict, minimized);

  double minimization_ratio = ((double) minimized.size())/confl.getNumChildren();
  d_minRatioSum+= minimization_ratio;
  

  if (minimization_ratio >= d_hardThresh) {
    d_period = d_period * 5; 
  }

  if (minimization_ratio <= d_thresh && d_period >= 40) {
    d_period = d_period *0.5; 
  }

  if (1.5* d_statistics.d_numUnknown.getData() > d_statistics.d_numSolved.getData()) {
    d_period = d_period * 2;
  }
  d_statistics.d_avgMinimizationRatio.addEntry(minimization_ratio);
  return utils::mkAnd(minimized); 
}

QuickXPlain::Statistics::Statistics(const std::string& name)
  : d_xplainTime("theory::bv::"+name+"::QuickXplain::Time")
  , d_numSolved("theory::bv::"+name+"::QuickXplain::NumSolved", 0)
  , d_numUnknown("theory::bv::"+name+"::QuickXplain::NumUnknown", 0)
  , d_numConflictsMinimized("theory::bv::"+name+"::QuickXplain::NumConflictsMinimized", 0)
  , d_finalPeriod("theory::bv::"+name+"::QuickXplain::FinalPeriod", 0)
  , d_avgMinimizationRatio("theory::bv::"+name+"::QuickXplain::AvgMinRatio")
{
  StatisticsRegistry::registerStat(&d_xplainTime);
  StatisticsRegistry::registerStat(&d_numSolved);
  StatisticsRegistry::registerStat(&d_numUnknown);
  StatisticsRegistry::registerStat(&d_numConflictsMinimized);
  StatisticsRegistry::registerStat(&d_finalPeriod);
  StatisticsRegistry::registerStat(&d_avgMinimizationRatio);  
}

QuickXPlain::Statistics::~Statistics() {
  StatisticsRegistry::unregisterStat(&d_xplainTime);
  StatisticsRegistry::unregisterStat(&d_numSolved);
  StatisticsRegistry::unregisterStat(&d_numUnknown);
  StatisticsRegistry::unregisterStat(&d_numConflictsMinimized);
  StatisticsRegistry::unregisterStat(&d_finalPeriod);
  StatisticsRegistry::unregisterStat(&d_avgMinimizationRatio);  
}

