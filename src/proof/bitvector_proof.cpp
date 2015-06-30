/*********************                                                        */
/*! \file bitvector_proof.cpp
** \verbatim
** Original author: Liana Hadarean
** Major contributors: none
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

#include "theory/bv/theory_bv.h"
#include "theory/bv/bitblaster_template.h"

#include "proof/bitvector_proof.h"
#include "proof/sat_proof_implementation.h"
#include "proof/proof_utils.h"
#include "prop/bvminisat/bvminisat.h"

using namespace CVC4;
using namespace CVC4::theory;
using namespace CVC4::theory::bv;

BitVectorProof::BitVectorProof(theory::bv::TheoryBV* bv, TheoryProofEngine* proofEngine)
  : TheoryProof(bv, proofEngine)
  , d_declarations()
    // , d_terms()
    // , d_atoms()
  , d_bb_terms()
  , d_bb_atoms()
    // , d_bbIdCount(0)
  , d_resolutionProof(NULL)
  , d_cnfProof(NULL)
  , d_lazyBB(NULL)
{}

void BitVectorProof::initSatProof(::BVMinisat::Solver* solver) {
  Assert (d_resolutionProof == NULL);
  d_resolutionProof = new LFSCBVSatProof(solver, "bb", true);
}

void BitVectorProof::initCnfProof(prop::CnfStream* cnfStream,
                                  context::Context* cnf) {
  Assert (d_cnfProof == NULL);
  d_cnfProof = new LFSCCnfProof(cnfStream, cnf, "bb");
  Assert (d_resolutionProof != NULL);
  d_resolutionProof->setCnfProof(d_cnfProof);

  // true and false have to be setup in a special way
  Node true_node = NodeManager::currentNM()->mkConst<bool>(true);
  Node false_node = NodeManager::currentNM()->mkConst<bool>(false).notNode();

  d_cnfProof->pushCurrentAssertion(true_node);
  d_cnfProof->pushCurrentDefinition(true_node);
  d_cnfProof->registerConvertedClause(d_resolutionProof->getTrueUnit());
  //d_cnfProof->setClauseFact(d_resolutionProof->getTrueUnit(), true_node);
  d_cnfProof->popCurrentAssertion();
  d_cnfProof->popCurrentDefinition();         
  
  d_cnfProof->pushCurrentAssertion(false_node);
  d_cnfProof->pushCurrentDefinition(false_node);
  d_cnfProof->registerConvertedClause(d_resolutionProof->getFalseUnit());
  //d_cnfProof->setClauseFact(d_resolutionProof->getFalseUnit(), false_node);
  d_cnfProof->popCurrentAssertion();
  d_cnfProof->popCurrentDefinition();         
}

void BitVectorProof::setBitblaster(bv::TLazyBitblaster* lazyBB) {
  Assert (d_lazyBB == NULL);
  d_lazyBB = lazyBB;
}

BVSatProof* BitVectorProof::getSatProof() {
  Assert (d_resolutionProof != NULL);
  return d_resolutionProof;
}

void BitVectorProof::registerTermBB(Expr term) {
  d_bb_terms.insert(term);
}
void BitVectorProof::registerAtomBB(Expr atom, Expr atom_bb) {
  d_bb_atoms.insert(std::make_pair(atom, atom.iffExpr(atom_bb)));
}

void BitVectorProof::registerTerm(Expr term) {
  if (Theory::isLeafOf(term, theory::THEORY_BV) &&
      !term.isConst()) {
    d_declarations.insert(term);
  }

  // if (Theory::theoryOf(term) == theory::THEORY_BV) {
  //   if (term.getType().isBoolean()) {
  //     if (d_atoms.find(term) == d_atoms.end())
  //       d_atoms[term] = newBBId();

  //     if (d_lazyBB->hasBBAtom(term) && d_bb_atoms.find(term) == d_bb_atoms.end())
  //       d_bb_atoms[term] = newBBId();

  //   }
  //   if (term.getType().isBitVector()) {
  //     if (d_terms.find(term) == d_terms.end())
  //       d_terms[term] = newBBId();

  //     if (d_lazyBB->hasBBTerm(term) && d_bb_terms.find(term) == d_bb_terms.end())
  //       d_bb_terms[term] = newBBId();
  //   }
  // }
  
  // don't care about parametric operators for bv?
  for (unsigned i = 0; i < term.getNumChildren(); ++i) {
     d_proofEngine->registerTerm(term[i]); 
  }
}

// unsigned BitVectorProof::newBBId() { return d_bbIdCount++; }

std::string BitVectorProof::getBBTermName(Expr expr) {
  std::ostringstream os;
  os << "bt"<< expr.getId();
  return os.str();
}

void BitVectorProof::startBVConflict(::BVMinisat::Solver::TCRef cr) {
  d_resolutionProof->startResChain(cr);
}

void BitVectorProof::startBVConflict(::BVMinisat::Solver::TLit lit) {
  d_resolutionProof->startResChain(lit);
}

void BitVectorProof::endBVConflict(const BVMinisat::Solver::TLitVec& confl) {
  std::vector<Expr> expr_confl;
  for (int i = 0; i < confl.size(); ++i) {
    prop::SatLiteral lit = prop::BVMinisatSatSolver::toSatLiteral(confl[i]);
    Expr atom = d_cnfProof->getAtom(lit.getSatVariable()).toExpr();
    Expr expr_lit = lit.isNegated() ? atom.notExpr() : atom; 
    expr_confl.push_back(expr_lit); 
  }
  Expr conflict = utils::mkSortedExpr(kind::OR, expr_confl);

  if (d_conflictMap.find(conflict) != d_conflictMap.end()) {
    // This can only happen when we have eager explanations in the bv solver
    // if we don't get to propagate p before ~p is already asserted
    d_resolutionProof->cancelResChain();
    return;
  }

  // we don't need to check for uniqueness in the sat solver then        
  ClauseId clause_id = d_resolutionProof->registerAssumptionConflict(confl);
  d_conflictMap[conflict] = clause_id;
  d_resolutionProof->endResChain(clause_id);
  Debug("bv-proof") << "BitVectorProof::endBVConflict id"<<clause_id<< " => " << conflict << "\n"; 
  d_isAssumptionConflict = false;
}

void BitVectorProof::finalizeConflicts(std::vector<Expr>& conflicts) {
  for(unsigned i = 0; i < conflicts.size(); ++i) {
    Expr confl = conflicts[i];
    Assert (d_conflictMap.find(confl) != d_conflictMap.end());
    ClauseId id = d_conflictMap[confl];
    d_resolutionProof->collectClauses(id);
  }
}

void LFSCBitVectorProof::printTerm(Expr term, std::ostream& os, const LetMap& map) {
  Assert (Theory::theoryOf(term) == THEORY_BV);
  
  switch (term.getKind()) {
  case kind::CONST_BITVECTOR : {
    printConstant(term, os);
    return; 
  }
  case kind::BITVECTOR_AND : 
  case kind::BITVECTOR_OR :
  case kind::BITVECTOR_XOR :
  case kind::BITVECTOR_NAND :
  case kind::BITVECTOR_NOR :
  case kind::BITVECTOR_XNOR :
  case kind::BITVECTOR_COMP :
  case kind::BITVECTOR_MULT :
  case kind::BITVECTOR_PLUS :
  case kind::BITVECTOR_SUB :
  case kind::BITVECTOR_UDIV :
  case kind::BITVECTOR_UREM :
  case kind::BITVECTOR_UDIV_TOTAL :
  case kind::BITVECTOR_UREM_TOTAL :
  case kind::BITVECTOR_SDIV :
  case kind::BITVECTOR_SREM :
  case kind::BITVECTOR_SMOD :
  case kind::BITVECTOR_SHL :
  case kind::BITVECTOR_LSHR :
  case kind::BITVECTOR_ASHR :
  case kind::BITVECTOR_CONCAT : {
    printOperatorNary(term, os, map);
    return;
  }
  case kind::BITVECTOR_NEG :
  case kind::BITVECTOR_NOT :
  case kind::BITVECTOR_ROTATE_LEFT :
  case kind::BITVECTOR_ROTATE_RIGHT : {
    printOperatorUnary(term, os, map);
    return;
  }
  case kind::BITVECTOR_ULT :
  case kind::BITVECTOR_ULE :
  case kind::BITVECTOR_UGT :
  case kind::BITVECTOR_UGE :
  case kind::BITVECTOR_SLT :
  case kind::BITVECTOR_SLE :
  case kind::BITVECTOR_SGT :
  case kind::BITVECTOR_SGE : {
    printPredicate(term, os, map);
    return;
  }
  case kind::BITVECTOR_EXTRACT :
  case kind::BITVECTOR_REPEAT :
  case kind::BITVECTOR_ZERO_EXTEND :
  case kind::BITVECTOR_SIGN_EXTEND : {
    printOperatorParametric(term, os, map);
    return;
  }
  case kind::BITVECTOR_BITOF : {
    printBitOf(term, os); 
    return;
  }
  case kind::VARIABLE:
  case kind::SKOLEM: {
    os << "(a_var_bv " << utils::getSize(term)<<" " << term <<")";
    return;
  }
  default:
    Unreachable(); 
  }
}

void LFSCBitVectorProof::printBitOf(Expr term, std::ostream& os) {
  Assert (term.getKind() == kind::BITVECTOR_BITOF);
  unsigned bit = term.getOperator().getConst<BitVectorBitOf>().bitIndex;
  Expr var = term[0];
  Assert (var.getKind() == kind::VARIABLE ||
          var.getKind() == kind::SKOLEM); 
  os << "(bitof " << var <<" " << bit <<")"; 
}

void LFSCBitVectorProof::printConstant(Expr term, std::ostream& os) {
  Assert (term.isConst());
  os <<"(a_bv " << utils::getSize(term)<<" ";
  std::ostringstream paren;
  for (unsigned i = 0; i < utils::getSize(term); ++i) {
    os << "(bvc ";
    os << (utils::getBit(term, i) ? "b1" : "b0") <<" "; 
    paren << ")";
  }
  os << " bvn)";
  os << paren.str(); 
}

void LFSCBitVectorProof::printOperatorNary(Expr term, std::ostream& os, const LetMap& map) {
  std::string op = utils::toLFSCKind(term.getKind());
  std::ostringstream paren;
  std::string holes = term.getKind() == kind::BITVECTOR_CONCAT ? "_ _ " : "";
  os <<"("<< op <<" " <<  utils::getSize(term) <<" " << holes;
  for (unsigned i = 0; i < term.getNumChildren(); ++i) {
    d_proofEngine->printBoundTerm(term[i], os, map);
    os << " ";
    if (i + 2 < term.getNumChildren()) {
      os <<"(" << op <<" " << utils::getSize(term) <<" " << holes;
      paren <<")";
    }
  }
  os <<")" << paren.str();
}

void LFSCBitVectorProof::printOperatorUnary(Expr term, std::ostream& os, const LetMap& map) {
  os <<"(";
  os << utils::toLFSCKind(term.getKind()) << " " << utils::getSize(term) <<" ";
  os << " ";
  d_proofEngine->printBoundTerm(term[0], os, map); 
  os <<")";
}

void LFSCBitVectorProof::printPredicate(Expr term, std::ostream& os, const LetMap& map) {
  os <<"(";
  os << utils::toLFSCKind(term.getKind()) << " " << utils::getSize(term[0]) <<" ";
  os << " ";
  d_proofEngine->printBoundTerm(term[0], os, map);
  os << " ";
  d_proofEngine->printBoundTerm(term[1], os, map); 
  os <<")";
}

void LFSCBitVectorProof::printOperatorParametric(Expr term, std::ostream& os, const LetMap& map) {
  os <<"(";  
  os << utils::toLFSCKind(term.getKind()) << " " << utils::getSize(term) <<" "; 
  os <<" "; 
  if (term.getKind() == kind::BITVECTOR_REPEAT) {
    unsigned amount = term.getOperator().getConst<BitVectorRepeat>().repeatAmount;
    os << amount; 
  }
  if (term.getKind() == kind::BITVECTOR_SIGN_EXTEND) {
    unsigned amount = term.getOperator().getConst<BitVectorSignExtend>().signExtendAmount;
    os << amount; 
  }

  if (term.getKind() == kind::BITVECTOR_ZERO_EXTEND) {
    unsigned amount = term.getOperator().getConst<BitVectorZeroExtend>().zeroExtendAmount;
    os << amount; 
  }
  if (term.getKind() == kind::BITVECTOR_EXTRACT) {
    unsigned low = utils::getExtractLow(term);
    unsigned high = utils::getExtractHigh(term);
    os << high <<" " << low; 
  }
  os <<" ";
  Assert (term.getNumChildren() == 1); 
  d_proofEngine->printBoundTerm(term[0], os, map);
  os <<")";
}

void LFSCBitVectorProof::printSort(Type type, std::ostream& os) {
  Assert (type.isBitVector());
  unsigned width = utils::getSize(type);
  os << "(BitVec "<<width<<")";  
}

void LFSCBitVectorProof::printTheoryLemmaProof(std::vector<Expr>& lemma, std::ostream& os, std::ostream& paren) {
  std::ostringstream lemma_paren;
  for (unsigned i = 0; i < lemma.size(); ++i) {
    Expr lit = lemma[i];
    if (lit.getKind() == kind::NOT) {
      os << "(intro_assump_t _ _ _ ";
    } else {
      os << "(intro_assump_f _ _ _ ";
    }
    lemma_paren <<")";
    // print corresponding literal in main sat solver
    ProofManager* pm = ProofManager::currentPM(); 
    CnfProof* cnf = pm->getCnfProof(); 
    prop::SatLiteral main_lit = cnf->getLiteral(lit);
    os << pm->getLitName(main_lit);
    os <<" "; 
    // print corresponding literal in bv sat solver
    prop::SatVariable bb_var = d_cnfProof->getLiteral(lit).getSatVariable();
    os << pm->getAtomName(bb_var, "bb");
    os <<"(\\unit"<<bb_var<<"\n"; 
    lemma_paren <<")";
  }

  Expr lem = utils::mkOr(lemma);
  Assert (d_conflictMap.find(lem) != d_conflictMap.end()); 
  ClauseId lemma_id = d_conflictMap[lem];
  d_resolutionProof->printAssumptionsResolution(lemma_id, os, lemma_paren);
  os <<lemma_paren.str();
  // os << "(clausify_false trust)\n"; 
}
void LFSCBitVectorProof::printDeclarations(std::ostream& os, std::ostream& paren) {
  ExprSet::const_iterator it = d_declarations.begin();
  ExprSet::const_iterator end = d_declarations.end();
  for (; it != end; ++it) {
    os << "(% " << *it <<" var_bv\n";
    paren <<")"; 
  }
}


void LFSCBitVectorProof::printTermBitblasting(Expr term, std::ostream& os) {
  // TODO: once we have the operator elimination rules remove those that we
  // eliminated
  Assert (term.getType().isBitVector());
  Kind kind = term.getKind();
  
  if (Theory::isLeafOf(term, theory::THEORY_BV) &&
      !term.isConst()) {
    os << "(bv_bbl_var "<<utils::getSize(term) << " " << term <<" _ )";
    return;
  }
  
  switch(kind) {
  case kind::CONST_BITVECTOR : {
    os << "(bv_bbl_const "<< utils::getSize(term) <<" _ ";
    std::ostringstream paren;
    for (unsigned i = 0; i < utils::getSize(term); ++i) {
      os << "(bvc ";
      os << (utils::getBit(term, i) ? "b1" : "b0") <<" "; 
      paren << ")";
    }
    os << " bvn)";
    os << paren.str(); 
    return; 
  }
  case kind::BITVECTOR_AND :
  case kind::BITVECTOR_OR :
  case kind::BITVECTOR_XOR :
  case kind::BITVECTOR_NAND :
  case kind::BITVECTOR_NOR :
  case kind::BITVECTOR_XNOR :
  case kind::BITVECTOR_COMP :
  case kind::BITVECTOR_MULT :
  case kind::BITVECTOR_PLUS :
  case kind::BITVECTOR_SUB :
  case kind::BITVECTOR_UDIV :
  case kind::BITVECTOR_UREM :
  case kind::BITVECTOR_UDIV_TOTAL :
  case kind::BITVECTOR_UREM_TOTAL :
  case kind::BITVECTOR_SDIV :
  case kind::BITVECTOR_SREM :
  case kind::BITVECTOR_SMOD :
  case kind::BITVECTOR_SHL :
  case kind::BITVECTOR_LSHR :
  case kind::BITVECTOR_ASHR :
  case kind::BITVECTOR_CONCAT : {
    std::ostringstream paren;
    os <<"(bv_bbl_"<< utils::toLFSCKind(kind);
    if (kind == kind::BITVECTOR_CONCAT) {
      os << " " << utils::getSize(term) <<" _ ";
    }
    os <<" _ _ _ _ _ _ ";
    for (unsigned i = 0; i < term.getNumChildren(); ++i) {
      os << getBBTermName(term[i]);
      os << " ";
      if (i + 2 < term.getNumChildren()) {
        os <<"(bv_bbl_"<<utils::toLFSCKind(kind);
        os <<" _ _ _ _ _ _ ";
        paren <<")";
      }
    }
    os <<")" << paren.str();
    return;
  }
  case kind::BITVECTOR_NEG :
  case kind::BITVECTOR_NOT :
  case kind::BITVECTOR_ROTATE_LEFT :
  case kind::BITVECTOR_ROTATE_RIGHT : {
    os <<"(bv_bbl_"<<utils::toLFSCKind(kind);
    os <<" _ _ _ _ ";
    os << getBBTermName(term[0]);
    os <<")";
    return;
  }
  case kind::BITVECTOR_EXTRACT : {
    os <<"(bv_bbl_"<<utils::toLFSCKind(kind) <<" ";
    os << utils::getExtractHigh(term) << " ";
    os << utils::getExtractLow(term) << " ";
    os << " _ _ _ _ ";
    os << getBBTermName(term[0]);
    os <<")";
    return;
  }
  case kind::BITVECTOR_REPEAT :
  case kind::BITVECTOR_ZERO_EXTEND :
  case kind::BITVECTOR_SIGN_EXTEND : {
    os <<"(bv_bbl_"<<utils::toLFSCKind(kind) <<" ";
    if (term.getKind() == kind::BITVECTOR_REPEAT) {
      unsigned amount = term.getOperator().getConst<BitVectorRepeat>().repeatAmount;
      os << amount; 
    }
    if (term.getKind() == kind::BITVECTOR_SIGN_EXTEND) {
      unsigned amount = term.getOperator().getConst<BitVectorSignExtend>().signExtendAmount;
      os << amount; 
    }
    
    if (term.getKind() == kind::BITVECTOR_ZERO_EXTEND) {
      unsigned amount = term.getOperator().getConst<BitVectorZeroExtend>().zeroExtendAmount;
      os << amount; 
    }
    os <<" _ _ _ _ ";
    os << getBBTermName(term[0]);
    os <<")";
    return;
  }
  default:
    Unreachable("LFSCBitVectorProof Unknown operator"); 
  }
}

void LFSCBitVectorProof::printAtomBitblasting(Expr atom, std::ostream& os) {
  Kind kind = atom.getKind();
  switch(kind) {
  case kind::BITVECTOR_ULT :
  case kind::BITVECTOR_ULE :
  case kind::BITVECTOR_UGT :
  case kind::BITVECTOR_UGE :
  case kind::BITVECTOR_SLT :
  case kind::BITVECTOR_SLE :
  case kind::BITVECTOR_SGT :
  case kind::BITVECTOR_SGE :
  case kind::EQUAL:
    {
    os <<"(bv_bbl_" << utils::toLFSCKind(atom.getKind()); 
    os << " _ _ _ _ _ _ ";
    os << getBBTermName(atom[0])<<" " << getBBTermName(atom[1]) <<")"; 
    return;
  }
  default:
    Unreachable("LFSCBitVectorProof Unknown atom kind");
  }
}


void LFSCBitVectorProof::printBitblasting(std::ostream& os, std::ostream& paren) {
  // bit-blast terms
  ExprSet::const_iterator it = d_bb_terms.begin();
  ExprSet::const_iterator end = d_bb_terms.end();
  for (; it != end; ++it) {
    os <<"(decl_bblast _ _ _ ";
    printTermBitblasting(*it, os);
    os << "(\\ "<< getBBTermName(*it);
    paren <<"\n))";
  }
  // bit-blast atoms
  ExprToExpr::const_iterator ait = d_bb_atoms.begin();
  ExprToExpr::const_iterator aend = d_bb_atoms.end();
  for (; ait != aend; ++ait) {
    os << "(th_let_pf _ ";
    printAtomBitblasting(ait->first, os);
    os <<"(\\ " << ProofManager::getPreprocessedAssertionName(ait->second, "bb") <<"\n";
    paren <<"))";
  }
}

void LFSCBitVectorProof::printResolutionProof(std::ostream& os,
                                              std::ostream& paren) {
  // collect the input clauses used
  IdToSatClause used_lemmas;
  IdToSatClause used_inputs;
  d_resolutionProof->collectClausesUsed(used_inputs,
                                        used_lemmas);
  Assert (used_lemmas.empty());

  // print mapping between theory atoms and internal SAT variables
  os << ";; BB atom mapping\n";

  NodeSet atoms;
  d_cnfProof->collectAtomsForClauses(used_inputs,atoms);
  // NodeSet assertions;
  // d_cnfProof->collectAssertionsUsed(assertions, used_inputs);

  // TODO: only print bit-blasted terms that were used/?
  // first print bit-blasting
  printBitblasting(os, paren);

  // print CNF conversion proof for bit-blasted facts
  d_cnfProof->printAtomMapping(atoms, os, paren);
  os << ";; Bit-blasting definitional clauses \n";
  for (IdToSatClause::iterator it = used_inputs.begin();
       it != used_inputs.end(); ++it) {
    d_cnfProof->printCnfProofForClause(it->first, it->second, os, paren);
  }
  
  os << ";; Bit-blasting learned clauses \n";
  d_resolutionProof->printResolutions(os, paren);
}

