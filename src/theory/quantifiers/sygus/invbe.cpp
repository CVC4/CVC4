/*********************                                                        */
/*! \file inv_synth.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Haniel Barbosa
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief implementation of class for specialized approaches for invariant
 ** synthesis
 **/

#include "theory/quantifiers/sygus/invbe.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

void InvBE::getTermList(const std::vector<Node>& candidates,
                        std::vector<Node>& enums)
{
  enums.insert(enums.end(), candidates.begin(), candidates.end());
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC4
