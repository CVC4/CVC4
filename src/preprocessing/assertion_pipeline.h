/*********************                                                        */
/*! \file assertion_pipeline.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andres Noetzli, Justin Xu, Morgan Deters
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief AssertionPipeline stores a list of assertions modified by
 ** preprocessing passes
 **/

#include "cvc4_private.h"

#ifndef CVC4__PREPROCESSING__ASSERTION_PIPELINE_H
#define CVC4__PREPROCESSING__ASSERTION_PIPELINE_H

#include <vector>

#include "expr/node.h"
#include "expr/proof_generator.h"
#include "expr/proof_node_manager.h"
#include "smt/preprocess_proof_generator.h"
#include "smt/term_formula_removal.h"
#include "theory/trust_node.h"

namespace CVC4 {
namespace preprocessing {

/**
 * Assertion Pipeline stores a list of assertions modified by preprocessing
 * passes. It is assumed that all assertions after d_realAssertionsEnd were
 * generated by ITE removal. Hence, d_iteSkolemMap maps into only these.
 */
class AssertionPipeline
{
 public:
  AssertionPipeline();

  size_t size() const { return d_nodes.size(); }

  void resize(size_t n) { d_nodes.resize(n); }

  /**
   * Clear the list of assertions and assumptions.
   */
  void clear();

  Node& operator[](size_t i) { return d_nodes[i]; }
  const Node& operator[](size_t i) const { return d_nodes[i]; }

  /**
   * Adds an assertion/assumption to be preprocessed.
   *
   * @param n The assertion/assumption
   * @param isAssumption If true, records that \p n is an assumption. Note that
   * all assumptions have to be added contiguously.
   * @param isInput If true, n is an input formula (an assumption in the main
   * body of the overall proof).
   * @param pg The proof generator who can provide a proof of n. The proof
   * generator is not required and is ignored if isInput is true.
   */
  void push_back(Node n,
                 bool isAssumption = false,
                 bool isInput = false,
                 ProofGenerator* pg = nullptr);
  /** Same as above, with TrustNode */
  void pushBackTrusted(theory::TrustNode trn);

  std::vector<Node>& ref() { return d_nodes; }
  const std::vector<Node>& ref() const { return d_nodes; }

  std::vector<Node>::const_iterator begin() const { return d_nodes.cbegin(); }
  std::vector<Node>::const_iterator end() const { return d_nodes.cend(); }

  /*
   * Replaces assertion i with node n and records the dependency between the
   * original assertion and its replacement.
   *
   * @param i The position of the assertion to replace.
   * @param n The replacement assertion.
   * @param pg The proof generator who can provide a proof of d_nodes[i] == n,
   * where d_nodes[i] is the assertion at position i prior to this call.
   */
  void replace(size_t i, Node n, ProofGenerator* pg = nullptr);
  /** Same as above, with TrustNode */
  void replaceTrusted(size_t i, theory::TrustNode trn);

  /*
   * Replaces assertion i with node n and records that this replacement depends
   * on assertion i and the nodes listed in addnDeps. The dependency
   * information is used for unsat cores and proofs.
   *
   * @param i The position of the assertion to replace.
   * @param n The replacement assertion.
   * @param addnDeps The dependencies.
   * @param pg The proof generator who can provide a proof of d_nodes[i] == n,
   * where d_nodes[i] is the assertion at position i prior to this call.
   */
  void replace(size_t i,
               Node n,
               const std::vector<Node>& addnDeps,
               ProofGenerator* pg = nullptr);

  IteSkolemMap& getIteSkolemMap() { return d_iteSkolemMap; }
  const IteSkolemMap& getIteSkolemMap() const { return d_iteSkolemMap; }

  size_t getRealAssertionsEnd() const { return d_realAssertionsEnd; }

  /** @return The index of the first assumption */
  size_t getAssumptionsStart() const { return d_assumptionsStart; }

  /** @return The number of assumptions */
  size_t getNumAssumptions() const { return d_numAssumptions; }

  void updateRealAssertionsEnd() { d_realAssertionsEnd = d_nodes.size(); }

  /**
   * Returns true if substitutions must be stored as assertions. This is for
   * example the case when we do incremental solving.
   */
  bool storeSubstsInAsserts() { return d_storeSubstsInAsserts; }

  /**
   * Enables storing substitutions as assertions.
   */
  void enableStoreSubstsInAsserts();

  /**
   * Disables storing substitutions as assertions.
   */
  void disableStoreSubstsInAsserts();

  /**
   * Adds a substitution node of the form (= lhs rhs) to the assertions.
   */
  void addSubstitutionNode(Node n);

  /**
   * Checks whether the assertion at a given index represents substitutions.
   *
   * @param i The index in question
   */
  bool isSubstsIndex(size_t i)
  {
    return d_storeSubstsInAsserts && i == d_substsIndex;
  }
  //------------------------------------ for proofs
  /** Set proof generator */
  void setProofGenerator(smt::PreprocessProofGenerator* pppg);
  /** Is proof enabled? */
  bool isProofEnabled() const;
  //------------------------------------ end for proofs
 private:
  /** The list of current assertions */
  std::vector<Node> d_nodes;

  /**
   * Map from skolem variables to index in d_assertions containing
   * corresponding introduced Boolean ite
   */
  IteSkolemMap d_iteSkolemMap;

  /** Size of d_nodes when preprocessing starts */
  size_t d_realAssertionsEnd;

  /**
   * If true, we store the substitutions as assertions. This is necessary when
   * doing incremental solving because we cannot apply them to existing
   * assertions while preprocessing new assertions.
   */
  bool d_storeSubstsInAsserts;

  /**
   * The index of the assertions that holds the substitutions.
   *
   * TODO(#2473): replace by separate vector of substitution assertions.
   */
  size_t d_substsIndex;

  /** Index of the first assumption */
  size_t d_assumptionsStart;
  /** The number of assumptions */
  size_t d_numAssumptions;
  /** The proof generator, if one is provided */
  smt::PreprocessProofGenerator* d_pppg;
}; /* class AssertionPipeline */

}  // namespace preprocessing
}  // namespace CVC4

#endif /* CVC4__PREPROCESSING__ASSERTION_PIPELINE_H */
