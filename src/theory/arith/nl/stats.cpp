/*********************                                                        */
/*! \file stats.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Statistics for non-linear arithmetic
 **/

#include "theory/arith/nl/stats.h"

#include "smt/smt_statistics_registry.h"

namespace CVC4 {
namespace theory {
namespace arith {
namespace nl {

NlStats::NlStats()
    : d_checkRuns("nl::checkRuns", 0),
      d_inferences("nl::inferences"),
      d_cdSimplifications("nl::cdSimplifications")
{
  smtStatisticsRegistry()->registerStat(&d_checkRuns);
  smtStatisticsRegistry()->registerStat(&d_inferences);
  smtStatisticsRegistry()->registerStat(&d_cdSimplifications);
}

NlStats::~NlStats()
{
  smtStatisticsRegistry()->unregisterStat(&d_checkRuns);
  smtStatisticsRegistry()->unregisterStat(&d_inferences);
  smtStatisticsRegistry()->unregisterStat(&d_cdSimplifications);
}

}  // namespace nl
}  // namespace arith
}  // namespace theory
}  // namespace CVC4
