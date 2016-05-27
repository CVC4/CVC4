/*********************                                                        */
/*! \file index.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Tim King, Morgan Deters, Liana Hadarean
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2016 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "cvc4_private.h"

#pragma once

#include <stdint.h>
#include <boost/static_assert.hpp>
#include <limits>

namespace CVC4 {

/**
 * Index is an unsigned integer used for array indexing.
 *
 * This gives a standardized type for independent pieces of code to use as an agreement.
 */
typedef uint32_t Index;

BOOST_STATIC_ASSERT(sizeof(Index) <= sizeof(size_t));
BOOST_STATIC_ASSERT(!std::numeric_limits<Index>::is_signed);

/* Discussion: Why is Index a uint32_t instead of size_t (or uint_fast32_t)?
 *
 * size_t is a more appropriate choice than uint32_t as the choice is dictated by
 * uniqueness in arrays and vectors. These correspond to size_t.
 * However, the using size_t with a sizeof == 8 on 64 bit platforms is noticeably
 * slower. (Limited testing suggests a ~1/16 of running time.)
 * (Interestingly, uint_fast32_t also has a sizeof == 8 on x86_64. Filthy Liars!)
 */

}/* CVC4 namespace */
