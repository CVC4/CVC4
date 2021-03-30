/*********************                                                        */
/*! \file justify_stats.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of quantifiers statistics class
 **/

#include "decision/justify_stats.h"

#include "smt/smt_statistics_registry.h"

namespace CVC4 {

JustifyStatistics::JustifyStatistics()
    : d_numStatusNoDecision("JustifyStrategy::StatusNoDecision", 0),
      d_numStatusDecision("JustifyStrategy::StatusDecision", 0),
      d_numStatusBacktrack("JustifyStrategy::StatusBacktrack", 0),
      d_maxStackSize("JustifyStrategy::MaxStackSize", 0),
      d_maxAssertionsSize("JustifyStrategy::MaxAssertionsSize", 0),
      d_maxSkolemDefsSize("JustifyStrategy::MaxSkolemDefsSize", 0)
{
  smtStatisticsRegistry()->registerStat(&d_numStatusNoDecision);
  smtStatisticsRegistry()->registerStat(&d_numStatusDecision);
  smtStatisticsRegistry()->registerStat(&d_numStatusBacktrack);
  smtStatisticsRegistry()->registerStat(&d_maxStackSize);
  smtStatisticsRegistry()->registerStat(&d_maxAssertionsSize);
  smtStatisticsRegistry()->registerStat(&d_maxSkolemDefsSize);
}

JustifyStatistics::~JustifyStatistics()
{
  smtStatisticsRegistry()->unregisterStat(&d_numStatusNoDecision);
  smtStatisticsRegistry()->unregisterStat(&d_numStatusDecision);
  smtStatisticsRegistry()->unregisterStat(&d_numStatusBacktrack);
  smtStatisticsRegistry()->unregisterStat(&d_maxStackSize);
  smtStatisticsRegistry()->unregisterStat(&d_maxAssertionsSize);
  smtStatisticsRegistry()->unregisterStat(&d_maxSkolemDefsSize);
}

}  // namespace CVC4
