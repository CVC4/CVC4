/*********************                                                        */
/*! \file fp_converter.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Martin Brain
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Converts floating-point nodes to bit-vector expressions.
 **
 ** Uses the symfpu library to convert from floating-point operations to
 ** bit-vectors and propositions allowing the theory to be solved by
 ** 'bit-blasting'.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__FP__FP_CONVERTER_H
#define __CVC4__THEORY__FP__FP_CONVERTER_H

#include "context/cdhashmap.h"
#include "context/cdlist.h"
#include "theory/valuation.h"
#include "util/floatingpoint.h"
#include "util/hash.h"

namespace CVC4 {
namespace theory {
namespace fp {

typedef PairHashFunction<TypeNode, TypeNode, TypeNodeHashFunction,
                         TypeNodeHashFunction>
    PairTypeNodeHashFunction;

class FpConverter {
 public:
  context::CDList<Node> d_additionalAssertions;

  FpConverter(context::UserContext *);

  /** Adds a node to the conversion, returns the converted node */
  Node convert(TNode);

  /** Gives the node representing the value of a given variable */
  Node getValue(Valuation &, TNode);
};

}  // namespace fp
}  // namespace theory
}  // namespace CVC4

#endif /* __CVC4__THEORY__FP__THEORY_FP_H */
