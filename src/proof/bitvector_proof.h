/*********************                                                        */
/*! \file bitvector_proof.h
 ** \verbatim
 ** Original author: Liana Hadarean
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2014  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Bitvector proof
 **
 ** Bitvector proof
 **/

#include "cvc4_private.h"

#ifndef __CVC4__BITVECTOR__PROOF_H
#define __CVC4__BITVECTOR__PROOF_H

#include <iostream>
#include <stdint.h>
#include <vector>
#include <set>
#include <ext/hash_map>
#include <ext/hash_set>
#include <sstream>
#include "expr/expr.h"
#include "proof/theory_proof.h"
#include "prop/bvminisat/core/Solver.h"

// namespace BVMinisat {
// class Solver; 
// }

namespace CVC4 {

namespace prop {
class CnfStream;
}
namespace theory {
namespace bv{
class TheoryBV;
}
}

class CnfProof; 

template <class Solver> class TSatProof;
typedef TSatProof< ::BVMinisat::Solver> BVSatProof;

template <class Solver> class LFSCSatProof;
typedef LFSCSatProof< ::BVMinisat::Solver> LFSCBVSatProof;

typedef __gnu_cxx::hash_set<Expr, ExprHashFunction> ExprSet;
typedef __gnu_cxx::hash_map<Expr, ClauseId, ExprHashFunction> ExprToClauseId;

class BitVectorProof : public TheoryProof {
protected:
  ExprSet d_declarations; 
  // map from Expr representing normalized lemma to ClauseId in SAT solver
  ExprToClauseId d_conflictMap;
  BVSatProof* d_resolutionProof;

  CnfProof* d_cnfProof;
  bool d_isAssumptionConflict;
  // TODO:  add proofs for all subtheories
  // BitblastingProof* d_bbProof;
  
  /** 
   * Set up SatProof data-structures to explain all
   * conflicts beloning to this theory required by
   * the main proof. This also enables the theory to
   * figure out which bit-blasting lemmas are necessary. 
   * 
   * @param conflicts 
   */

public:
  BitVectorProof(theory::bv::TheoryBV* bv, TheoryProofEngine* proofEngine);

  void initSatProof(::BVMinisat::Solver* solver);
  void initCnfProof(prop::CnfStream* cnfStream);
  BVSatProof* getSatProof();
  void finalizeConflicts(std::vector<Expr>& conflicts);

  void startBVConflict(::BVMinisat::Solver::TCRef cr);
  void startBVConflict(::BVMinisat::Solver::TLit lit);
  /** 
   * All the 
   * 
   * @param confl an inconsistent set of bv literals
   */
  void endBVConflict(const BVMinisat::Solver::TLitVec& confl);
  void markAssumptionConflict() { d_isAssumptionConflict = true; }
  bool isAssumptionConflict() { return d_isAssumptionConflict; }
  virtual void registerTerm(Expr term);
  
  virtual void printTerm(Expr term, std::ostream& os, const LetMap& map) = 0;
  virtual void printSort(Type type, std::ostream& os) = 0;
  // TODO ask for all off the lemmas at once, to be able to compute the
  // relevant learnt clauses that we need and which we will print upfront
  virtual void printTheoryLemmaProof(std::vector<Expr>& lemma, std::ostream& os, std::ostream& paren) = 0;
  virtual void printDeclarations(std::ostream& os, std::ostream& paren) = 0;
  virtual void printBitblasting(std::ostream& os, std::ostream& paren) = 0;
};

class LFSCBitVectorProof: public BitVectorProof {
  
  void printConstant(Expr term, std::ostream& os);
  void printOperatorNary(Expr term, std::ostream& os, const LetMap& map);
  void printOperatorUnary(Expr term, std::ostream& os, const LetMap& map);
  void printPredicate(Expr term, std::ostream& os, const LetMap& map);
  void printOperatorParametric(Expr term, std::ostream& os, const LetMap& map);
  void printBitOf(Expr term, std::ostream& os);
public:
  LFSCBitVectorProof(theory::bv::TheoryBV* bv, TheoryProofEngine* proofEngine)
    :BitVectorProof(bv, proofEngine)
  {}
  virtual void printTerm(Expr term, std::ostream& os, const LetMap& map);
  virtual void printSort(Type type, std::ostream& os);
  virtual void printTheoryLemmaProof(std::vector<Expr>& lemma, std::ostream& os, std::ostream& paren);
  virtual void printDeclarations(std::ostream& os, std::ostream& paren);
  virtual void printBitblasting(std::ostream& os, std::ostream& paren); 
}; 

}/* CVC4 namespace */

#endif /* __CVC4__BITVECTOR__PROOF_H */
