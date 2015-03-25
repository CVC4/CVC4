/*********************                                                        */
/*! \file cnf_proof.cpp
 ** \verbatim
 ** Original author: Liana Hadarean
 ** Major contributors: Morgan Deters
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2014  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "proof/cnf_proof.h"
#include "proof/theory_proof.h"
#include "proof/proof_manager.h"
#include "prop/sat_solver_types.h"
#include "prop/minisat/minisat.h"
#include "prop/cnf_stream.h"

using namespace CVC4::prop;

namespace CVC4 {

CnfProof::CnfProof(CnfStream* stream, const std::string& name)
  : d_cnfStream(stream)
  , d_atomToSatVar()
  , d_satVarToAtom()
  , d_inputClauses()
  , d_name(name)
{}


Expr CnfProof::getAtom(prop::SatVariable var) {
  prop::SatLiteral lit (var);
  Node node = d_cnfStream->getNode(lit);
  Expr atom = node.toExpr();
  return atom;
}

// prop::SatLiteral CnfProof::getLiteral(Expr expr) {
//   Assert(d_cnfStream->hasLiteral(expr));
//   return d_cnfStream->getLiteral(Node::fromExpr(expr)); 
// }

void CnfProof::addInputClause(ClauseId id, const prop::SatClause* clause) {
  Assert (d_inputClauses.find(id) == d_inputClauses.end());
  d_inputClauses[id] = clause;
  collectAtoms(clause);
}

void CnfProof::collectAtoms(const prop::SatClause* clause) {
  for (unsigned i = 0; i < clause->size(); ++i) {
    SatLiteral lit = clause->operator[](i);
    SatVariable var = lit.getSatVariable();
    Expr atom = getAtom(var);
    if (d_satVarToAtom.find(var) == d_satVarToAtom.end()) {
      Assert (d_atomToSatVar.find(atom) == d_atomToSatVar.end());
      d_satVarToAtom[var] = atom;
      d_atomToSatVar[atom] = var;
    }
  }
}

CnfProof::~CnfProof() {
  IdToClause::iterator it = d_inputClauses.begin();
  IdToClause::iterator end = d_inputClauses.end();
  for (; it != end; ++it) {
    delete it->second;
  }
}

void LFSCCnfProof::printAtomMapping(std::ostream& os, std::ostream& paren) {
  atom_iterator it = begin_atoms(); 
  atom_iterator end = end_atoms(); 

  for (;it != end;  ++it) {
    os << "(decl_atom ";
    prop::SatVariable var = it->second;
    Expr atom = it->first;
    //FIXME hideous
    LFSCTheoryProofEngine* pe = (LFSCTheoryProofEngine*)ProofManager::currentPM()->getTheoryProofEngine();
    pe->printLetTerm(atom, os);
    
    os << " (\\ " << ProofManager::getVarName(var, d_name)
       << " (\\ " << ProofManager::getAtomName(var, d_name) << "\n";
    paren << ")))";
  }
}

prop::SatLiteral CnfProof::getLiteral(TNode atom) {
  return d_cnfStream->getLiteral(atom);
}

Expr CnfProof::getAssertion(uint64_t id) {
  return d_cnfStream->getAssertion(id).toExpr();
}

void LFSCCnfProof::printClauses(std::ostream& os, std::ostream& paren) {
  printInputClauses(os, paren);
}

void LFSCCnfProof::printInputClauses(std::ostream& os, std::ostream& paren) {
  os << " ;; Input Clauses \n";
  clause_iterator it = begin_input_clauses();
  clause_iterator end = end_input_clauses();

  for (; it != end; ++it) {
    ClauseId id = it->first;
    const prop::SatClause* clause = it->second;
    os << "(satlem _ _ ";
    std::ostringstream clause_paren;
    printClause(*clause, os, clause_paren);
    os << "(clausify_false trust)" << clause_paren.str()
       << " (\\ " << ProofManager::getInputClauseName(id) << "\n";
    paren << "))";
  }
}

void LFSCCnfProof::printTheoryLemmas(std::ostream& os, std::ostream& paren) {
  os << " ;; Theory Lemmas\n";
  ProofManager::ordered_clause_iterator it = ProofManager::currentPM()->begin_lemmas();
  ProofManager::ordered_clause_iterator end = ProofManager::currentPM()->end_lemmas();

  for(size_t n = 0; it != end; ++it, ++n) {
    if(n % 100 == 0) {
      Chat() << "proving theory conflicts...(" << n << "/" << ProofManager::currentPM()->num_lemmas() << ")" << std::endl;
    }

    ClauseId id = it->first;
    const prop::SatClause* clause = it->second;
    NodeBuilder<> c(kind::AND);
    for(unsigned i = 0; i < clause->size(); ++i) {
      prop::SatLiteral lit = (*clause)[i];
      prop::SatVariable var = lit.getSatVariable();
      if(lit.isNegated()) {
        c << Node::fromExpr(getAtom(var));
      } else {
        c << Node::fromExpr(getAtom(var)).notNode();
      }
    }
    Node cl = c;
    if(ProofManager::getSatProof()->d_lemmaClauses.find(id) !=
       ProofManager::getSatProof()->d_lemmaClauses.end()) {
      uint64_t proof_id = ProofManager::getSatProof()->d_lemmaClauses[id];
      TNode orig = d_cnfStream->getAssertion(proof_id & 0xffffffff);
      if(((proof_id >> 32) & 0xffffffff) == RULE_ARRAYS_EXT) {
        Debug("cores") << "; extensional lemma!" << std::endl;
        Assert(cl.getKind() == kind::AND &&
	       cl.getNumChildren() == 2 &&
	       cl[0].getKind() == kind::EQUAL &&
	       cl[0][0].getKind() == kind::SELECT);
        TNode myk = cl[0][0][1];
        Debug("cores") << "; so my skolemized k is " << myk << std::endl;
        os << "(ext _ _ " << orig[0][0] << " " << orig[0][1]
	   << " (\\ " << myk << " (\\ " << ProofManager::getLemmaName(id) << "\n";
        paren << ")))";
      }
    }
    os << "(satlem _ _ ";
    std::ostringstream clause_paren;
    printClause(*clause, os, clause_paren);

    Debug("cores") << "\n;id is " << id << std::endl;
    if(ProofManager::getSatProof()->d_lemmaClauses.find(id) !=
       ProofManager::getSatProof()->d_lemmaClauses.end()) {
      uint64_t proof_id = ProofManager::getSatProof()->d_lemmaClauses[id];
      Debug("cores") << ";getting id " << int32_t(proof_id & 0xffffffff) << std::endl;
      Assert(int32_t(proof_id & 0xffffffff) != -1);
      TNode orig = d_cnfStream->getAssertion(proof_id & 0xffffffff);
      Debug("cores") << "; ID is " << id << " and that's a lemma with " << ((proof_id >> 32) & 0xffffffff) << " / " << (proof_id & 0xffffffff) << std::endl;
      Debug("cores") << "; that means the lemma was " << orig << std::endl;
      if(((proof_id >> 32) & 0xffffffff) == RULE_ARRAYS_EXT) {
        Debug("cores") << "; extensional" << std::endl;
        os << "(clausify_false trust)\n";
      } else if(proof_id == 0) {
        // theory propagation caused conflict
        //ProofManager::currentPM()->printProof(os, cl);
        os << "(clausify_false trust)\n";
      } else if(((proof_id >> 32) & 0xffffffff) == RULE_CONFLICT) {
        os << "\n;; need to generate a (conflict) proof of " << cl << "\n";
        //ProofManager::currentPM()->printProof(os, cl);
        os << "(clausify_false trust)\n";
      } else {
        os << "\n;; need to generate a (lemma) proof of " << cl;
        os << "\n;; DON'T KNOW HOW !!\n";
        os << "(clausify_false trust)\n";
      }
    } else {
      os << "\n;; need to generate a (conflict) proof of " << cl << "\n";
      ProofManager::currentPM()->printProof(os, cl);
    }
    os << clause_paren.str()
       << " (\\ " << ProofManager::getLemmaClauseName(id, d_name) << "\n";
    paren << "))";
  }
}

void LFSCCnfProof::printClause(const prop::SatClause& clause, std::ostream& os, std::ostream& paren) {
  for (unsigned i = 0; i < clause.size(); ++i) {
    prop::SatLiteral lit = clause[i];
    prop::SatVariable var = lit.getSatVariable();
    if (lit.isNegated()) {
      os << "(ast _ _ _ " << ProofManager::getAtomName(var, d_name) <<" (\\ " << ProofManager::getLitName(lit, d_name) << " ";
      paren << "))";
    } else {
      os << "(asf _ _ _ " << ProofManager::getAtomName(var, d_name) <<" (\\ " << ProofManager::getLitName(lit, d_name) << " ";
      paren << "))";
    }
  }
}

} /* CVC4 namespace */
