/*********************                                                        */
/*! \file sequence.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of the sequence data type.
 **/

#include "expr/expr_sequence.h"

#include "expr/expr.h"
#include "expr/node.h"
#include "expr/sequence.h"
#include "expr/type.h"
#include "expr/type_node.h"

namespace CVC4 {

ExprSequence::ExprSequence(const Type& t)
{
  d_type.reset(new Type(t));
  std::vector<Node> seq;
  d_sequence.reset(new Sequence(TypeNode::fromType(t), seq));
}
ExprSequence::~ExprSequence() {}

ExprSequence::ExprSequence(const ExprSequence& other)
    : d_type(new Type(other.getType())),
      d_sequence(new Sequence(other.getSequence()))
{
}

ExprSequence& ExprSequence::operator=(const ExprSequence& other)
{
  (*d_type) = other.getType();
  (*d_sequence) = other.getSequence();
  return *this;
}

const Type& ExprSequence::getType() const { return *d_type; }

const Sequence& ExprSequence::getSequence() const { return *d_sequence; }

bool ExprSequence::operator==(const ExprSequence& es) const
{
  return getType() == es.getType() && getSequence() == es.getSequence();
}

bool ExprSequence::operator!=(const ExprSequence& es) const
{
  return !(*this == es);
}

bool ExprSequence::operator<(const ExprSequence& es) const
{
  return (getType() < es.getType())
         || (getType() == es.getType() && getSequence() < es.getSequence());
}

bool ExprSequence::operator<=(const ExprSequence& es) const
{
  return (getType() < es.getType())
         || (getType() == es.getType() && getSequence() <= es.getSequence());
}

bool ExprSequence::operator>(const ExprSequence& es) const
{
  return !(*this <= es);
}

bool ExprSequence::operator>=(const ExprSequence& es) const
{
  return !(*this < es);
}

std::ostream& operator<<(std::ostream& os, const ExprSequence& s)
{
  // FIXME
  return os << "\""
            << "\"";
}

namespace strings {
size_t ExprSequenceHashFunction::operator()(const ExprSequence& es) const
{
  return TypeHashFunction()(es.getType());
}
}  // namespace strings

}  // namespace CVC4
