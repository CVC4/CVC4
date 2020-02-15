/*********************                                                        */
/*! \file care_graph.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Dejan Jovanovic, Tim King, Morgan Deters
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The care graph datastructure.
 **
 ** The care graph datastructure.
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__CARE_GRAPH_H
#define CVC4__THEORY__CARE_GRAPH_H

#include <set>

#include "expr/kind.h"  // For TheoryId.
#include "expr/node.h"

namespace CVC4 {
namespace theory {

/**
 * A (ordered) pair of terms a theory cares about.
 */
struct CarePair {
  const TNode a, b;
  const TheoryId theory;

  CarePair(TNode av, TNode bv, TheoryId t)
      : a(av < bv ? av : bv), b(av < bv ? bv : av), theory(t) {}

  bool operator==(const CarePair& other) const {
    return (theory == other.theory) && (a == other.a) && (b == other.b);
  }

  bool operator<(const CarePair& other) const {
    if (theory < other.theory) return true;
    if (theory > other.theory) return false;
    if (a < other.a) return true;
    if (a > other.a) return false;
    return b < other.b;
  }

}; /* struct CarePair */

/**
 * A set of care pairs.
 */
typedef std::set<CarePair> CareGraph;

}  // namespace theory
}  // namespace CVC4

#endif /* CVC4__THEORY__CARE_GRAPH_H */
