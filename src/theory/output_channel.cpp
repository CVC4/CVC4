/*********************                                                        */
/*! \file output_channel.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The theory output channel interface
 **/

#include "theory/output_channel.h"

namespace CVC4 {
namespace theory {

/**
 * Writes an lemma property name to a stream.
 *
 * @param out The stream to write to
 * @param p The lemma property to write to the stream
 * @return The stream
 */
std::ostream& operator<<(std::ostream& out, LemmaProperty p)
{
  if (p == LemmaProperty::NONE)
  {
    out << "NONE";
  }
  else
  {
    out << "{";
    if (p & LemmaProperty::REMOVABLE)
    {
      out << " REMOVABLE";
    }
    if (p & LemmaProperty::PREPROCESS)
    {
      out << " PREPROCESS";
    }
    if (p & LemmaProperty::SEND_ATOMS)
    {
      out << " SEND_ATOMS";
    }
    out << " }";
  }
  return out;
}

LemmaProperty operator|(LemmaProperty lhs, LemmaProperty rhs)
{
  return static_cast<LemmaProperty>(static_cast<uint32_t>(lhs)
                                    | static_cast<uint32_t>(rhs));
}
LemmaProperty& operator|=(LemmaProperty& lhs, LemmaProperty rhs)
{
  lhs = lhs | rhs;
  return lhs;
}
LemmaProperty& operator&=(LemmaProperty& lhs, LemmaProperty rhs)
{
  lhs = static_cast<LemmaProperty>(static_cast<uint32_t>(lhs)
                                   & static_cast<uint32_t>(rhs));
  return lhs;
}
bool operator&(LemmaProperty lhs, LemmaProperty rhs)
{
  LemmaProperty p = static_cast<LemmaProperty>(static_cast<uint32_t>(lhs)
                                               & static_cast<uint32_t>(rhs));
  return p != LemmaProperty::NONE;
}

LemmaStatus::LemmaStatus(TNode rewrittenLemma, unsigned level)
    : d_rewrittenLemma(rewrittenLemma), d_level(level)
{
}

TNode LemmaStatus::getRewrittenLemma() const { return d_rewrittenLemma; }

unsigned LemmaStatus::getLevel() const { return d_level; }

LemmaStatus OutputChannel::lemma(TNode n, LemmaProperty p)
{
  return lemma(n, RULE_INVALID, p);
}

LemmaStatus OutputChannel::split(TNode n)
{
  return splitLemma(n.orNode(n.notNode()));
}

void OutputChannel::trustedConflict(TrustNode pconf)
{
  Unreachable() << "OutputChannel::trustedConflict: no implementation"
                << std::endl;
}

LemmaStatus OutputChannel::trustedLemma(TrustNode lem, LemmaProperty p)
{
  Unreachable() << "OutputChannel::trustedLemma: no implementation"
                << std::endl;
}

}  // namespace theory
}  // namespace CVC4
