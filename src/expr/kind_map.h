/*********************                                                        */
/*! \file kind_map.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Dejan Jovanovic, Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief A bitmap of Kinds
 **
 ** This is a class representation for a bitmap of Kinds that is
 ** iterable, manipulable, and packed.
 **/

#include "cvc4_private.h"

#ifndef CVC4__KIND_MAP_H
#define CVC4__KIND_MAP_H

#include <bitset>

#include "base/check.h"
#include "expr/kind.h"

namespace CVC4 {

/** A bitmap for Kinds */
class KindMap : public std::bitset<kind::LAST_KIND>
{
  /** The base class to ease calling overriden methods */
  using Base = std::bitset<kind::LAST_KIND>;

 public:
  /** Set the bit for k */
  void set(Kind k) { Base::set(fromKind(k)); }
  /** Check whether the bit for k is set */
  bool test(Kind k) const { return Base::test(fromKind(k)); }
  /** Check whether the bit for k is set */
  bool operator[](Kind k) const { return test(k); }

 private:
  /** Convert kind to std::size_t and check bounds */
  static std::size_t fromKind(Kind k)
  {
    AssertArgument(k >= Kind(0) && k < kind::LAST_KIND, k, "invalid kind");
    return static_cast<std::size_t>(k);
  }
};

}  // namespace CVC4

#endif /* CVC4__KIND_MAP_H */
