/*********************                                                        */
/*! \file shared_check_data.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Gereon Kremer
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Common data shared by multiple checks
 **/

#ifndef CVC4__THEORY__ARITH__NL__NL__SHARED_CHECK_DATA_H
#define CVC4__THEORY__ARITH__NL__NL__SHARED_CHECK_DATA_H

#include <vector>

#include "expr/node.h"
#include "theory/arith/inference_manager.h"
#include "theory/arith/nl/nl_model.h"
#include "theory/arith/nl/nl_monomial.h"

namespace CVC4 {
namespace theory {
namespace arith {
namespace nl {

struct SharedCheckData
{
  Node d_false = NodeManager::currentNM()->mkConst(false);
  Node d_true = NodeManager::currentNM()->mkConst(true);
  Node d_zero = NodeManager::currentNM()->mkConst(Rational(0));
  Node d_one = NodeManager::currentNM()->mkConst(Rational(1));
  Node d_neg_one = NodeManager::currentNM()->mkConst(Rational(-1));

  /** The inference manager that we push conflicts and lemmas to. */
  InferenceManager& d_im;
  /** Reference to the non-linear model object */
  NlModel& d_model;

  // information about monomials
  std::vector<Node> d_ms;
  std::vector<Node> d_ms_vars;
  std::vector<Node> d_mterms;

  /** Context-independent database of monomial information */
  MonomialDb d_mdb;

  // ( x*y, x*z, y ) for each pair of monomials ( x*y, x*z ) with common factors
  std::map<Node, std::map<Node, Node> > d_mono_diff;
  /** the set of monomials we should apply tangent planes to */
  std::unordered_set<Node, NodeHashFunction> d_tplane_refine;

  SharedCheckData(InferenceManager& im, NlModel& model)
      : d_im(im), d_model(model)
  {
  }

  void init(const std::vector<Node>& xts);
};

}  // namespace nl
}  // namespace arith
}  // namespace theory
}  // namespace CVC4

#endif
