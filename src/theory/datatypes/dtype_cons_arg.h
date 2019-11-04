/*********************                                                        */
/*! \file dtype.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief A class representing a datatype definition
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__DATATYPES__DTYPE_CONS_ARG_H
#define CVC4__THEORY__DATATYPES__DTYPE_CONS_ARG_H

#include <string>
#include "base/exception.h"
#include "expr/node.h"
#include "expr/type_node.h"
#include "util/hash.h"

namespace CVC4 {
namespace theory {

class DType;
class DTypeConstructor;

/**
 * A DType constructor argument (i.e., a DType field).
 */
class DTypeConstructorArg
{
  friend class DTypeConstructor;
  friend class DType;

 public:
  /** Get the name of this constructor argument. */
  std::string getName() const;

  /**
   * Get the selector for this constructor argument; this call is
   * only permitted after resolution.
   */
  Node getSelector() const;

  /**
   * Get the associated constructor for this constructor argument;
   * this call is only permitted after resolution.
   */
  Node getConstructor() const;

  /**
   * Get the type of the selector for this constructor argument;
   * this call is only permitted after resolution.
   */
  TypeNode getType() const;

  /**
   * Get the range type of this argument.
   */
  TypeNode getRangeType() const;

  /**
   * Returns true iff this constructor argument has been resolved.
   */
  bool isResolved() const;

  /** prints this datatype constructor argument to stream */
  void toStream(std::ostream& out) const;

 private:
  /** the name of this selector */
  std::string d_name;
  /** the selector expression */
  Node d_selector;
  /** the constructor associated with this selector */
  Node d_constructor;
  /** whether this class has been resolved */
  bool d_resolved;
  /** is this selector unresolved? */
  bool isUnresolvedSelf() const;
  /** constructor */
  DTypeConstructorArg(std::string name, Node selector);
};

std::ostream& operator<<(std::ostream& os, const DTypeConstructorArg& arg);

}  // namespace theory
}  // namespace CVC4

#endif
