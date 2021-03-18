/*********************                                                        */
/*! \file statistics_public.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Gereon Kremer
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Registration of public statistics
 **
 ** Registration and documentation for all public statistics.
 **/

#include "cvc4_private_library.h"

#ifndef CVC4__UTIL__STATISTICS_PUBLIC_H
#define CVC4__UTIL__STATISTICS_PUBLIC_H

namespace CVC4 {

class StatisticsRegistry;

/**
 * Preregisters all public (non-expert) statistics.
 */
void register_public_statistics(StatisticsRegistry& reg);

}  // namespace CVC4

#endif
