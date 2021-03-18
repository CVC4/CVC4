/*********************                                                        */
/*! \file statistics_reg.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Gereon Kremer
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Central statistics registry.
 **
 ** The StatisticsRegistry that issues statistic proxy objects.
 **/

#include "util/statistics_reg.h"

#include "util/statistics_public.h"

namespace CVC4 {

StatisticRegistry::StatisticRegistry(bool register_public) {
  if (register_public) {
  register_public_statistics(*this);
  }
}

void StatisticRegistry::print(std::ostream& os, bool expert) const {
  for (const auto& s : d_stats)
  {
    if (!expert && s.second->d_expert) continue;
    os << s.first << " = ";
    s.second->print(os);
    os << std::endl;
  }
}
void StatisticRegistry::print_safe(int fd, bool expert) const {
  for (const auto& s : d_stats)
  {
    if (!expert && s.second->d_expert) continue;
    safe_print(fd, s.first);
    safe_print(fd, " = ");
    s.second->print_safe(fd);
    safe_print(fd, '\n');
  }
}

std::ostream& operator<<(std::ostream& os, const StatisticRegistry& sr)
{
  sr.print(os);
  return os;
}

}  // namespace CVC4
