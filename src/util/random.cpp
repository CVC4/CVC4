/*********************                                                        */
/*! \file random.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Aina Niemetz
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief A Random Number Generator.
 **
 ** A random number generator, implements the xorshift* generator
 ** (see S. Vigna, An experimental exploration of Marsaglia's xorshift
 ** generators, scrambled. ACM Trans. Math. Softw. 42(4): 30:1-30:23, 2016).
 **/

#include "util/random.h"

#include <cfloat>
#include "base/cvc4_assert.h"

namespace CVC4 {

uint64_t Random::rand()
{
  /* xorshift* generator (see S. Vigna, An experimental exploration of
   * Marsaglia's xorshift generators, scrambled. ACM Trans. Math. Softw.
   * 42(4): 30:1-30:23, 2016). */
  d_state ^= d_state >> 12;
  d_state ^= d_state << 25;
  d_state ^= d_state >> 27;
  return d_state * uint64_t{2685821657736338717};
}

uint64_t Random::pick(uint64_t from, uint64_t to)
{
  Assert(from <= to);
  Assert(to < UINT64_MAX);
  return (Random::rand() % (to - from + 1)) + from;
}

double Random::pickDouble(double from, double to)
{
  Assert(from <= to);
  Assert(to <= DBL_MAX);
  return Random::rand() * (to - from) + from;
}

bool Random::pickWithProb(double probability)
{
  Assert(probability <= 1);
  uint64_t p = (uint64_t) (probability * 1000);
  uint64_t r = pick(0, 999);
  return r < p;
}

}
