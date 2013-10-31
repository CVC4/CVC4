/*********************                                                        */
/*! \file proof_manager.h
 ** \verbatim
 ** Original author: Liana Hadarean
 ** Major contributors: Morgan Deters
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief A manager for Proofs.
 **
 ** A manager for Proofs.
 **
 ** 
 **/

#include "cvc4_private.h"

#ifndef __CVC4__PROOF_MANAGER_H
#define __CVC4__PROOF_MANAGER_H

#include <iostream> 
#include "proof/proof.h"
#include "util/proof.h"


// forward declarations
namespace Minisat {
  class Solver;
}

namespace CVC4 {

namespace prop {
  class CnfStream;
}
typedef int ClauseId;

class Proof;
class SatProof;
class CnfProof;
class TheoryProof;

class LFSCSatProof;
class LFSCCnfProof;
class LFSCTheoryProof;

namespace prop {
typedef uint64_t SatVariable;
class SatLiteral;
typedef std::vector<SatLiteral> SatClause; 
}

// different proof modes
enum ProofFormat {
  LFSC,
  NATIVE
};/* enum ProofFormat */

std::string append(const std::string& str, uint64_t num);

typedef __gnu_cxx::hash_map < ClauseId, const prop::SatClause* > IdToClause; 
typedef __gnu_cxx::hash_set<prop::SatVariable > VarSet;
typedef __gnu_cxx::hash_set<Expr, ExprHashFunction > ExprSet;

typedef int ClauseId;

enum ClauseKind {
  INPUT,
  THEORY_LEMMA,
  LEARNT
};

class ProofManager {
  SatProof*    d_satProof;
  CnfProof*    d_cnfProof;
  TheoryProof* d_theoryProof; 

  // information that will need to be shared across proofs
  IdToClause d_inputClauses;
  IdToClause d_theoryLemmas;
  ExprSet    d_inputFormulas;
  VarSet     d_propVars;
  
  Proof* d_fullProof; 
  ProofFormat d_format;
  
  static ProofManager* proofManager; 
  static bool isInitialized; 
  ProofManager(ProofFormat format = LFSC);
  ~ProofManager(); 
public:
  static ProofManager* currentPM();
  // initialization 
  static void         initSatProof(Minisat::Solver* solver); 
  static void         initCnfProof(CVC4::prop::CnfStream* cnfStream);
  static void         initTheoryProof();
  
  static Proof*       getProof();
  static SatProof*    getSatProof();
  static CnfProof*    getCnfProof();
  static TheoryProof* getTheoryProof();

  // iterators over data shared by proofs
  typedef IdToClause::const_iterator clause_iterator;
  typedef ExprSet::const_iterator assertions_iterator; 
  typedef VarSet::const_iterator var_iterator;
  
  clause_iterator begin_input_clauses() const { return d_inputClauses.begin(); }
  clause_iterator end_input_clauses() const { return d_inputClauses.end(); }

  clause_iterator begin_lemmas() const { return d_theoryLemmas.begin(); }
  clause_iterator end_lemmas() const { return d_theoryLemmas.end(); }

  assertions_iterator begin_assertions() const { return d_inputFormulas.begin(); }
  assertions_iterator end_assertions() const { return d_inputFormulas.end(); }

  var_iterator begin_vars() const { return d_propVars.begin(); }
  var_iterator end_vars() const { return d_propVars.end(); }
  
  void addAssertion(Expr formula);
  void addClause(ClauseId id, const prop::SatClause* clause, ClauseKind kind); 
  
  // variable prefixes
  static std::string getInputClauseName(ClauseId id);
  static std::string getLemmaClauseName(ClauseId id);
  static std::string getLearntClauseName(ClauseId id);

  static std::string getVarName(prop::SatVariable var);
  static std::string getAtomName(prop::SatVariable var);
  static std::string getLitName(prop::SatLiteral lit);
};/* class ProofManager */

class LFSCProof : public Proof {
  LFSCSatProof* d_satProof;
  LFSCCnfProof* d_cnfProof;
  LFSCTheoryProof* d_theoryProof; 
public:
  LFSCProof(LFSCSatProof* sat, LFSCCnfProof* cnf, LFSCTheoryProof* theory); 
  virtual void toStream(std::ostream& out);
  virtual ~LFSCProof() {}
};
  
}/* CVC4 namespace */

#endif /* __CVC4__PROOF_MANAGER_H */
