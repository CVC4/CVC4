/*********************                                                        */
/*! \file quantifiers_registry.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The quantifiers registry
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__QUANTIFIERS__QUANTIFIERS_REGISTRY_H
#define CVC4__THEORY__QUANTIFIERS__QUANTIFIERS_REGISTRY_H

#include "expr/node.h"

namespace CVC4 {
namespace theory {

class QuantifiersModule;

namespace quantifiers {

/**
 * The quantifiers registry, which manages basic information about what
 * quantifiers are registered to quantifiers modules, which quantifiers
 * are reduced and which quantifiers modules have ownership of quantified
 * formulas.
 */
class QuantifiersRegistry
{
 public:
  QuantifiersRegistry();
  ~QuantifiersRegistry() {}
  /** get the owner of quantified formula q */
  QuantifiersModule* getOwner(Node q);
  /**
   * Set owner of quantified formula q to module m with given priority. If
   * the quantified formula has previously been assigned an owner with
   * lower priority, that owner is overwritten.
   */
  void setOwner(Node q, QuantifiersModule* m, int32_t priority = 0);
  /**
   * Return true if module m has ownership of quantified formula q. This means
   * that either q does not have an owner, or the owner is m.
   */
  bool hasOwnership(Node q, QuantifiersModule* m) const;
};

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC4

#endif /* CVC4__THEORY__QUANTIFIERS__QUANTIFIERS_REGISTRY_H */
