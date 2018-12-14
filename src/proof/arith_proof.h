/*********************                                                        */
/*! \file arith_proof.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Guy Katz, Mathias Preiner, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Arith proof
 **
 ** Arith proof
 **/

#include "cvc4_private.h"

#ifndef __CVC4__ARITH__PROOF_H
#define __CVC4__ARITH__PROOF_H

#include <memory>
#include <unordered_set>

#include "expr/expr.h"
#include "proof/arith_proof_recorder.h"
#include "proof/proof_manager.h"
#include "proof/theory_proof.h"
#include "theory/uf/equality_engine.h"

namespace CVC4 {

//proof object outputted by TheoryArith
class ProofArith : public Proof {
 public:
  ProofArith(std::shared_ptr<theory::eq::EqProof> pf) : d_proof(pf) {}
  void toStream(std::ostream& out) const override;

 private:
  static void toStreamLFSC(std::ostream& out, TheoryProof* tp,
                           const theory::eq::EqProof& pf,
                           const ProofLetMap& map);
  static Node toStreamRecLFSC(std::ostream& out, TheoryProof* tp,
                              const theory::eq::EqProof& pf,
                              unsigned tb, const ProofLetMap& map);
  // it is simply an equality engine proof
  std::shared_ptr<theory::eq::EqProof> d_proof;
};

namespace theory {
namespace arith {
class TheoryArith;
}
}

typedef std::unordered_set<Type, TypeHashFunction > TypeSet;


class ArithProof : public TheoryProof {
protected:
  // std::map<Expr, std::string> d_constRationalString; // all the variable/function declarations

  //   TypeSet d_sorts;        // all the uninterpreted sorts in this theory
  ExprSet d_declarations; // all the variable/function declarations

  /**
   * @brief Where farkas proofs of lemmas are stored.
   */
  proof::ArithProofRecorder d_recorder;

  bool d_realMode;
  theory::TheoryId getTheoryId() override;

 public:
  ArithProof(theory::arith::TheoryArith* arith, TheoryProofEngine* proofEngine);

  void registerTerm(Expr term) override;
};

class LFSCArithProof : public ArithProof {
public:
  LFSCArithProof(theory::arith::TheoryArith* arith, TheoryProofEngine* proofEngine)
    : ArithProof(arith, proofEngine)
  {}
  void printOwnedTerm(Expr term,
                      std::ostream& os,
                      const ProofLetMap& map) override;
  void printOwnedSort(Type type, std::ostream& os) override;


  /**
   * @brief Prints a value of type poly_formula_norm
   *
   * @param o ostream to print to
   * @param n node (asserted to be of the form [linear polynomial >= constant])
   */
  static void printLinearPolynomialPredicateNormalizer(std::ostream& o, const Node& n);

  /**
   * @brief Prints a value of type poly_norm
   *
   * @param o ostream to print to
   * @param n node (asserted to be a linear polynomial)
   */
  static void printLinearPolynomialNormalizer(std::ostream& o, const Node& n);

  /**
   * @brief Prints a value of type poly_norm
   *
   * @param o ostream to print to
   * @param n node (asserted to be a linear monomial)
   */
  static void printLinearMonomialNormalizer(std::ostream& o, const Node& n);

  /**
   * @brief Prints a LFSC rational
   *
   * @param o ostream to print to
   * @param n node (asserted to be a const rational)
   */
  static void printConstRational(std::ostream& o, const Node& n);

  /**
   * @brief prints the pn_var normalizer for n (type poly_norm)
   *
   * @param o the ostream to print to
   * @param n the node to print (asserted to be a variable)
   */
  static void printVariableNormalizer(std::ostream& o, const Node& n);
  /**
   * @brief prints a proof of the lemma
   *
   * First, we print linearity witnesses, i.e. witnesses  that each literal has
   * the form:
   *   <linear polynomial> >= 0 OR
   *   <linear polynomial> >  0
   *
   * Then we use those witnesses to prove that the above linearized constraints
   * hold.
   *
   * Then we use the farkas coefficients to combine the literals into a
   * variable-free contradiction. This bit is a bit tricky because the literals
   * may be a mix of strict and relaxed inequalities.
   *
   * @param lemma the set of literals disjoined in the lemma
   * @param os stream to print the proof to
   * @param paren global closing stream (unused)
   * @param map let map (unused)
   */
  void printTheoryLemmaProof(std::vector<Expr>& lemma,
                             std::ostream& os,
                             std::ostream& paren,
                             const ProofLetMap& map) override;
  void printSortDeclarations(std::ostream& os, std::ostream& paren) override;
  void printTermDeclarations(std::ostream& os, std::ostream& paren) override;
  void printDeferredDeclarations(std::ostream& os,
                                 std::ostream& paren) override;
  void printAliasingDeclarations(std::ostream& os,
                                 std::ostream& paren,
                                 const ProofLetMap& globalLetMap) override;
};


}/* CVC4 namespace */

#endif /* __CVC4__ARITH__PROOF_H */
