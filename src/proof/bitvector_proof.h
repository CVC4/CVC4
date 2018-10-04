/*********************                                                        */
/*! \file bitvector_proof.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Liana Hadarean, Mathias Preiner, Guy Katz
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Bitvector proof
 **
 ** Bitvector proof
 **/

#include "cvc4_private.h"

#ifndef __CVC4__BITVECTOR__PROOF_H
#define __CVC4__BITVECTOR__PROOF_H

#include <iostream>
#include <set>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "expr/expr.h"
#include "proof/theory_proof.h"
#include "prop/bvminisat/core/Solver.h"


namespace CVC4 {

namespace prop {
class CnfStream;
} /* namespace CVC4::prop */

namespace theory {
namespace bv {
class TheoryBV;
template <class T> class TBitblaster;
} /* namespace CVC4::theory::bv */
} /* namespace CVC4::theory */

class CnfProof;
} /* namespace CVC4 */

namespace CVC4 {

template <class Solver> class TSatProof;
typedef TSatProof< CVC4::BVMinisat::Solver> BVSatProof;

typedef std::unordered_set<Expr, ExprHashFunction> ExprSet;
typedef std::unordered_map<Expr, ClauseId, ExprHashFunction> ExprToClauseId;
typedef std::unordered_map<Expr, unsigned, ExprHashFunction> ExprToId;
typedef std::unordered_map<Expr, Expr, ExprHashFunction> ExprToExpr;
typedef std::unordered_map<Expr, std::string, ExprHashFunction> ExprToString;

class BitVectorProofSuper : public TheoryProof {
protected:
  BitVectorProofSuper(theory::bv::TheoryBV* bv, TheoryProofEngine* proofEngine);

  ExprSet d_declarations;

  std::map<Expr,std::string> d_constantLetMap;
  bool d_useConstantLetification;

  ExprToClauseId d_bbConflictMap;

  std::string getBBTermName(Expr expr);

  theory::bv::TBitblaster<Node>* d_bitblaster;

  ExprSet d_usedBB; // terms and formulas that are actually relevant to the proof

  ExprSet d_seenBBTerms; // terms that need to be bit-blasted
  std::vector<Expr> d_bbTerms; // order of bit-blasting
  ExprToExpr d_bbAtoms; // atoms that need to be bit-blasted

  void printBitblasting(std::ostream& os, std::ostream& paren);

  std::set<Node> d_atomsInBitblastingProof;

public:
  void printOwnedTerm(Expr term,
                      std::ostream& os,
                      const ProofLetMap& map) override;
  virtual void calculateAtomsInBitblastingProof() = 0;
  const std::set<Node>* getAtomsInBitblastingProof();
private:
  ExprToString d_exprToVariableName;

  ExprToString d_assignedAliases;
  std::map<std::string, std::string> d_aliasToBindDeclaration;
  std::string assignAlias(Expr expr);
  bool hasAlias(Expr expr);


  void printConstant(Expr term, std::ostream& os);
  void printOperatorNary(Expr term, std::ostream& os, const ProofLetMap& map);
  void printOperatorUnary(Expr term, std::ostream& os, const ProofLetMap& map);
  void printPredicate(Expr term, std::ostream& os, const ProofLetMap& map);
  void printOperatorParametric(Expr term, std::ostream& os, const ProofLetMap& map);
  void printBitOf(Expr term, std::ostream& os, const ProofLetMap& map);

  void printOwnedSort(Type type, std::ostream& os) override;
  void printTermBitblasting(Expr term, std::ostream& os);
  void printAtomBitblasting(Expr term, std::ostream& os, bool swap);
  void printAtomBitblastingToFalse(Expr term, std::ostream& os);

  void printSortDeclarations(std::ostream& os, std::ostream& paren) override;
  void printTermDeclarations(std::ostream& os, std::ostream& paren) override;
  void printDeferredDeclarations(std::ostream& os,
                                 std::ostream& paren) override;
  void printAliasingDeclarations(std::ostream& os,
                                 std::ostream& paren,
                                 const ProofLetMap& globalLetMap) override;

  void printConstantDisequalityProof(std::ostream& os,
                                     Expr c1,
                                     Expr c2,
                                     const ProofLetMap& globalLetMap) override;
  void printRewriteProof(std::ostream& os,
                         const Node& n1,
                         const Node& n2) override;
};

class BitVectorProof : public BitVectorProofSuper {

protected:


  // map from Expr representing normalized lemma to ClauseId in SAT solver
  BVSatProof* d_resolutionProof;

  CnfProof* d_cnfProof;

  bool d_isAssumptionConflict;

  theory::TheoryId getTheoryId() override;
  context::Context d_fakeContext;
public:
  BitVectorProof(theory::bv::TheoryBV* bv, TheoryProofEngine* proofEngine);

  void initSatProof(CVC4::BVMinisat::Solver* solver);
  void initCnfProof(prop::CnfStream* cnfStream, context::Context* ctx);
  void setBitblaster(theory::bv::TBitblaster<Node>* bb);

  BVSatProof* getSatProof();
  CnfProof* getCnfProof() {return d_cnfProof; }
  void finalizeConflicts(std::vector<Expr>& conflicts);

  void startBVConflict(CVC4::BVMinisat::Solver::TCRef cr);
  void startBVConflict(CVC4::BVMinisat::Solver::TLit lit);
  /**
   * All the
   *
   * @param confl an inconsistent set of bv literals
   */
  void endBVConflict(const BVMinisat::Solver::TLitVec& confl);
  void markAssumptionConflict() { d_isAssumptionConflict = true; }
  bool isAssumptionConflict() { return d_isAssumptionConflict; }

  void registerTermBB(Expr term);
  void registerAtomBB(Expr atom, Expr atom_bb);

  void registerTerm(Expr term) override;

  virtual void printResolutionProof(std::ostream& os, std::ostream& paren, ProofLetMap& letMap) = 0;
};

class LFSCBitVectorProof: public BitVectorProof {

public:
  LFSCBitVectorProof(theory::bv::TheoryBV* bv, TheoryProofEngine* proofEngine)
    :BitVectorProof(bv, proofEngine)
  {}
  void printTheoryLemmaProof(std::vector<Expr>& lemma,
                             std::ostream& os,
                             std::ostream& paren,
                             const ProofLetMap& map) override;
  void printResolutionProof(std::ostream& os,
                            std::ostream& paren,
                            ProofLetMap& letMap) override;
  void calculateAtomsInBitblastingProof() override;
};

}/* CVC4 namespace */

#endif /* __CVC4__BITVECTOR__PROOF_H */
