/*********************                                                        */
/*! \file assertion_pipeline.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andres Noetzli
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief AssertionPipeline stores a list of assertions modified by
 ** preprocessing passes
 **/

#include "cvc4_private.h"

#ifndef __CVC4__PREPROCESSING__ASSERTION_PIPELINE_H
#define __CVC4__PREPROCESSING__ASSERTION_PIPELINE_H

#include <vector>

#include "context/cdo.h"
#include "context/context.h"
#include "expr/node.h"
#include "smt/term_formula_removal.h"
#include "theory/substitutions.h"

namespace CVC4 {
namespace preprocessing {

/**
 * Assertion Pipeline stores a list of assertions modified by preprocessing
 * passes.
 */
class AssertionPipeline
{
 public:
  AssertionPipeline(context::Context* context);

  size_t size() const { return d_nodes.size(); }

  void resize(size_t n) { d_nodes.resize(n); }

  void clear()
  {
    d_nodes.clear();
    d_realAssertionsEnd = 0;
  }

  Node& operator[](size_t i) { return d_nodes[i]; }
  const Node& operator[](size_t i) const { return d_nodes[i]; }
  void push_back(Node n) { d_nodes.push_back(n); }

  std::vector<Node>& ref() { return d_nodes; }
  const std::vector<Node>& ref() const { return d_nodes; }

  std::vector<Node>::const_iterator begin() const { return d_nodes.cbegin(); }
  std::vector<Node>::const_iterator end() const { return d_nodes.cend(); }

  /*
   * Replaces assertion i with node n and records the dependency between the
   * original assertion and its replacement.
   */
  void replace(size_t i, Node n);

  /*
   * Replaces assertion i with node n and records that this replacement depends
   * on assertion i and the nodes listed in addnDeps. The dependency
   * information is used for unsat cores and proofs.
   */
  void replace(size_t i, Node n, const std::vector<Node>& addnDeps);

  /*
   * Replaces an assertion with a vector of assertions and records the
   * dependencies.
   */
  void replace(size_t i, const std::vector<Node>& ns);

  IteSkolemMap& getIteSkolemMap() { return d_iteSkolemMap; }
  const IteSkolemMap& getIteSkolemMap() const { return d_iteSkolemMap; }

  context::CDO<unsigned>& getSubstitutionsIndex()
  {
    return d_substitutionsIndex;
  }

  theory::SubstitutionMap& getTopLevelSubstitutions()
  {
    return d_topLevelSubstitutions;
  }

  size_t getRealAssertionsEnd() const { return d_realAssertionsEnd; }

  void updateRealAssertionsEnd() { d_realAssertionsEnd = d_nodes.size(); }

 private:
  std::vector<Node> d_nodes;

  /**
   * Map from skolem variables to index in d_assertions containing
   * corresponding introduced Boolean ite
   */
  IteSkolemMap d_iteSkolemMap;

  /* Index for where to store substitutions */
  context::CDO<unsigned> d_substitutionsIndex;

  /* The top level substitutions */
  theory::SubstitutionMap d_topLevelSubstitutions;

  /** Size of d_nodes when preprocessing starts */
  size_t d_realAssertionsEnd;
}; /* class AssertionPipeline */

}  // namespace preprocessing
}  // namespace CVC4

#endif /* __CVC4__PREPROCESSING__PREPROCESSING_PASS_H */
