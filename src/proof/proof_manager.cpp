/*********************                                                        */
/*! \file proof_manager.cpp
 ** \verbatim
 ** Original author: Liana Hadarean
 ** Major contributors: Morgan Deters
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "proof/proof_manager.h"
#include "util/proof.h"
#include "proof/sat_proof.h"
#include "proof/cnf_proof.h"
#include "proof/theory_proof.h"
#include "util/cvc4_assert.h"

namespace CVC4 {

std::string append(const std::string& str, uint64_t num) {
  std::ostringstream os;
  os << str << num; 
  return os.str(); 
}


bool          ProofManager::isInitialized = false;
ProofManager* ProofManager::proofManager = NULL;

ProofManager::ProofManager(ProofFormat format):
  d_satProof(NULL),
  d_cnfProof(NULL),
  d_theoryProof(NULL),
  d_fullProof(NULL),
  d_format(format)
{
}

ProofManager::~ProofManager() {
  delete d_satProof;
  delete d_cnfProof;
  delete d_theoryProof;
  delete d_fullProof;
  for (IdToClause::iterator it = d_inputClauses.begin(); it != d_inputClauses.end(); ++it) {
    delete it->second; 
  }
  for (IdToClause::iterator it = d_theoryLemmas.begin(); it != d_theoryLemmas.end(); ++it) {
    delete it->second; 
  }
  // FIXME: memory leak because there are deleted theory lemmas that were not used in the
  // SatProof 
}

ProofManager* ProofManager::currentPM() {
  if (isInitialized) {
    return proofManager; 
  } else {
    proofManager = new ProofManager();
    isInitialized = true; 
    return proofManager; 
  }
}

Proof* ProofManager::getProof() {
  if (currentPM()->d_fullProof != NULL)
    return currentPM()->d_fullProof;
  Assert (currentPM()->d_format == LFSC);

  currentPM()->d_fullProof = new LFSCProof((LFSCSatProof*)getSatProof(),
                                           (LFSCCnfProof*)getCnfProof(),
                                           (LFSCTheoryProof*)getTheoryProof()); 
  return currentPM()->d_fullProof;
}

SatProof* ProofManager::getSatProof() {
  Assert (currentPM()->d_satProof);
  return currentPM()->d_satProof; 
}

CnfProof* ProofManager::getCnfProof() {
  Assert (currentPM()->d_cnfProof);
  return currentPM()->d_cnfProof;
}

TheoryProof* ProofManager::getTheoryProof() {
  Assert (currentPM()->d_theoryProof);
  return currentPM()->d_theoryProof;
}


void ProofManager::initSatProof(Minisat::Solver* solver) {
  Assert (currentPM()->d_satProof == NULL);
  Assert(currentPM()->d_format == LFSC);
  currentPM()->d_satProof = new LFSCSatProof(solver);
}

void ProofManager::initCnfProof(prop::CnfStream* cnfStream) {
  Assert (currentPM()->d_cnfProof == NULL);
  Assert (currentPM()->d_format == LFSC);
  currentPM()->d_cnfProof = new LFSCCnfProof(cnfStream); 
}

void ProofManager::initTheoryProof() {
  Assert (currentPM()->d_theoryProof == NULL);
  Assert (currentPM()->d_format == LFSC);
  currentPM()->d_theoryProof = new LFSCTheoryProof();
}


std::string ProofManager::getInputClauseName(ClauseId id) {return append("pb", id); }
std::string ProofManager::getLemmaClauseName(ClauseId id) { return append("lem", id); }
std::string ProofManager::getLearntClauseName(ClauseId id) { return append("cl", id); }
std::string ProofManager::getVarName(prop::SatVariable var) { return append("v", var); }
std::string ProofManager::getAtomName(prop::SatVariable var) { return append("a", var); }
std::string ProofManager::getLitName(prop::SatLiteral lit) {return append("l", lit.toInt()); }

void ProofManager::addClause(ClauseId id, const prop::SatClause* clause, ClauseKind kind) {
  for (unsigned i = 0; i < clause->size(); ++i) {
    prop::SatLiteral lit = clause->operator[](i); 
    d_propVars.insert(lit.getSatVariable()); 
  }
  if (kind == INPUT) {
    d_inputClauses.insert(std::make_pair(id, clause));
    return;
  }
  Assert (kind == THEORY_LEMMA);
  d_theoryLemmas.insert(std::make_pair(id, clause));
}

void ProofManager::addAssertion(Expr formula) {
  d_inputFormulas.insert(formula); 
}


LFSCProof::LFSCProof(LFSCSatProof* sat, LFSCCnfProof* cnf, LFSCTheoryProof* theory)
  : d_satProof(sat)
  , d_cnfProof(cnf)
  , d_theoryProof(theory)
{
  d_satProof->constructProof();
}

void LFSCProof::toStream(std::ostream& out) {
  std::ostringstream paren;
  out << "(check \n";
  d_theoryProof->printAssertions(out, paren); 
  out << "(: (holds cln) \n";
  d_cnfProof->printAtomMapping(out, paren);
  d_cnfProof->printClauses(out, paren);
  d_satProof->printResolutions(out, paren); 
  paren <<")))\n;;";
  out << paren.str(); 
}


} /* CVC4  namespace */ 
