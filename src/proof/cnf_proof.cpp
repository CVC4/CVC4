/*********************                                                        */
/*! \file cnf_proof.cpp
 ** \verbatim
 ** Original author: Liana Hadarean
 ** Major contributors: Morgan Deters, Andrew Reynolds
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

CnfProof::CnfProof(CnfStream* stream,
                   context::Context* ctx,
                   const std::string& name)
  : d_cnfStream(stream)
  // , d_atomToSatVar()
  // , d_satVarToAtom()
  , d_clauseToAssertion(ctx)
  , d_assertionToProofRule(ctx)
  , d_currentAssertionStack()
  , d_currentDefinitionStack()
  , d_clauseToDefinition(ctx)
  , d_definitions()
  , d_cnfDeps()
  , d_name(name)
{}

CnfProof::~CnfProof() {}

bool CnfProof::isAssertion(Node node) {
  return d_assertionToProofRule.find(node) !=
         d_assertionToProofRule.end();
}

bool CnfProof::isDefinition(Node node) {
  return d_definitions.find(node) !=
         d_definitions.end();
}
  
ProofRule CnfProof::getProofRule(Node node) {
  Assert (isAssertion(node));
  NodeToProofRule::iterator it = d_assertionToProofRule.find(node);
  return (*it).second;
}
ProofRule CnfProof::getProofRule(ClauseId clause) {
  TNode assertion = getAssertionForClause(clause);
  return getProofRule(assertion);
}

Node CnfProof::getAssertionForClause(ClauseId clause) {
  ClauseIdToNode::const_iterator it = d_clauseToAssertion.find(clause);
  Assert (it != d_clauseToAssertion.end());
  return (*it).second;
}

Node CnfProof::getDefinitionForClause(ClauseId clause) {
  ClauseIdToNode::const_iterator it = d_clauseToDefinition.find(clause);
  Assert (it != d_clauseToDefinition.end());
  return (*it).second;
}

void CnfProof::registerConvertedClause(ClauseId clause, bool explanation) {
  Assert (clause != ClauseIdUndef &&
          clause != ClauseIdError &&
          clause != ClauseIdEmpty);

  // Explanations do not need a CNF conversion proof since they are in CNF
  // (they will only need a theory proof as they are theory valid)
  if (explanation) {
    Debug("proof:cnf") << "CnfProof::registerConvertedClause "
                       << clause << " explanation? " << explanation << std::endl;
    Assert (d_explanations.find(clause) == d_explanations.end());
    d_explanations.insert(clause);
    return;
  }

  Node current_assertion = getCurrentAssertion();
  Node current_expr = getCurrentDefinition();
  
  Debug("proof:cnf") << "CnfProof::registerConvertedClause "
                     << clause << " assertion = " << current_assertion
                     << clause << " definition = " << current_expr << std::endl;

  setClauseAssertion(clause, current_assertion);
  setClauseDefinition(clause, current_expr);
}

void CnfProof::setClauseAssertion(ClauseId clause, Node expr) {
  Debug("proof:cnf") << "CnfProof::setClauseAssertion "
                     << clause << " assertion " << expr << std::endl;
  
  Assert (d_clauseToAssertion.find(clause) == d_clauseToAssertion.end());
  d_clauseToAssertion.insert (clause, expr);
}

void CnfProof::setClauseDefinition(ClauseId clause, Node definition) {
  Debug("proof:cnf") << "CnfProof::setClauseDefinition "
                     << clause << " definition " << definition << std::endl;
  
  Assert (d_clauseToDefinition.find(clause) == d_clauseToDefinition.end());
  d_clauseToDefinition.insert(clause, definition);
  d_definitions.insert(definition);
}
  
void CnfProof::registerAssertion(Node assertion, ProofRule reason) {
  Debug("proof:cnf") << "CnfProof::registerAssertion "
                     << assertion << " reason " << reason << std::endl;

  if (isAssertion(assertion)) {
    Assert (d_assertionToProofRule[assertion] == reason); 
  }
  d_assertionToProofRule.insert(assertion, reason);
}

void CnfProof::setCnfDependence(Node from, Node to) {
  Debug("proof:cnf") << "CnfProof::setCnfDependence "
                     << "from " << from  << std::endl
                     << "     to " << to << std::endl;

  Assert (from != to);
  // Assert (d_cnfDeps.find(from) == d_cnfDeps.end() ||
  //         d_cnfDeps[from] == to);
  d_cnfDeps.insert(std::make_pair(from, to));
}

void CnfProof::pushCurrentAssertion(Node assertion) {
  Debug("proof:cnf") << "CnfProof::pushCurrentAssertion "
                     << assertion  << std::endl;

  d_currentAssertionStack.push_back(assertion);
}

void CnfProof::popCurrentAssertion() {
  Assert (d_currentAssertionStack.size());
  
  Debug("proof:cnf") << "CnfProof::popCurrentAssertion "
                     << d_currentAssertionStack.back() << std::endl;
  
  d_currentAssertionStack.pop_back();
}

Node CnfProof::getCurrentAssertion() {
  Assert (d_currentAssertionStack.size());
  return d_currentAssertionStack.back();
}

void CnfProof::pushCurrentDefinition(Node definition) {
  Debug("proof:cnf") << "CnfProof::pushCurrentDefinition "
                     << definition  << std::endl;

  d_currentDefinitionStack.push_back(definition);
}

void CnfProof::popCurrentDefinition() {
  Assert (d_currentDefinitionStack.size());
  
  Debug("proof:cnf") << "CnfProof::popCurrentDefinition "
                     << d_currentDefinitionStack.back() << std::endl;
  
  d_currentDefinitionStack.pop_back();
}

Node CnfProof::getCurrentDefinition() {
  Assert (d_currentDefinitionStack.size());
  return d_currentDefinitionStack.back();
}


Node CnfProof::getAtom(prop::SatVariable var) {
  prop::SatLiteral lit (var);
  Node node = d_cnfStream->getNode(lit);
  return node;
}

// void CnfProof::addInputClause(ClauseId id, const prop::SatClause* clause) {
//   Assert (d_inputClauses.find(id) == d_inputClauses.end());
//   d_inputClauses[id] = clause;
//   collectAtoms(clause);
// }

void CnfProof::collectAtoms(const prop::SatClause* clause,
                            NodeSet& atoms) {
  for (unsigned i = 0; i < clause->size(); ++i) {
    SatLiteral lit = clause->operator[](i);
    SatVariable var = lit.getSatVariable();
    TNode atom = getAtom(var);
    if (atoms.find(atom) == atoms.end()) {
      Assert (atoms.find(atom) == atoms.end());
      atoms.insert(atom);
    }
  }
}

prop::SatLiteral CnfProof::getLiteral(TNode atom) {
  return d_cnfStream->getLiteral(atom);
}

// Expr CnfProof::getAssertion(uint64_t id) {
//   return d_cnfStream->getAssertion(id).toExpr();
// }

void CnfProof::collectAtomsForClauses(const IdToSatClause& clauses,
                                       NodeSet& atom_map) {
  IdToSatClause::const_iterator it = clauses.begin();
  for (; it != clauses.end(); ++it) {
    const prop::SatClause* clause = it->second;
    collectAtoms(clause, atom_map);
  }

}

void CnfProof::collectAssertionsForClauses(const IdToSatClause& clauses,
                                           NodeSet& assertions) {
  IdToSatClause::const_iterator it = clauses.begin();
  for (; it != clauses.end(); ++it) {
    TNode used_assertion =  getAssertionForClause(it->first);
    assertions.insert(used_assertion);
    // it can be the case that a definition for a clause is an assertion
    // but it is not the assertion for the clause
    // e.g. the assertions [(and a b), a]
    TNode used_definition = getDefinitionForClause(it->first);
    if (isAssertion(used_definition)) {
      assertions.insert(used_definition);
    }
  }
}

void LFSCCnfProof::printAtomMapping(const NodeSet& atoms,
                                    std::ostream& os,
                                    std::ostream& paren) {
  NodeSet::const_iterator it = atoms.begin(); 
  NodeSet::const_iterator end = atoms.end(); 

  for (;it != end;  ++it) {
    os << "(decl_atom ";
    Node atom = *it;
    prop::SatVariable var = getLiteral(atom).getSatVariable();
    //FIXME hideous
    LFSCTheoryProofEngine* pe = (LFSCTheoryProofEngine*)ProofManager::currentPM()->getTheoryProofEngine();
    pe->printLetTerm(atom.toExpr(), os);
    
    os << " (\\ " << ProofManager::getVarName(var, d_name)
       << " (\\ " << ProofManager::getAtomName(var, d_name) << "\n";
    paren << ")))";
  }
}

// maps each expr to the position it had in the clause and the polarity it had
Node LFSCCnfProof::clauseToNode(const prop::SatClause& clause,
                                std::map<Node, unsigned>& childIndex,
                                std::map<Node, bool>& childPol ) {
  std::vector< Node > children;
  for (unsigned i = 0; i < clause.size(); ++i) {
    prop::SatLiteral lit = clause[i];
    prop::SatVariable var = lit.getSatVariable();
    Node atom = getAtom(var);
    children.push_back( lit.isNegated() ? atom.negate() : atom );
    childIndex[atom] = i;
    childPol[atom] = !lit.isNegated();
  }
  return children.size()==1 ? children[0] :
         NodeManager::currentNM()->mkNode(kind::OR, children );
}

void LFSCCnfProof::printCnfProofForClause(ClauseId id,
                                          const prop::SatClause* clause,
                                          std::ostream& os,
                                          std::ostream& paren) {
  os << "(satlem _ _ ";
  std::ostringstream clause_paren;
  printClause(*clause, os, clause_paren);
  os << "(clausify_false ";
  Assert( clause->size()>0 );
  
  Node base_assertion = getDefinitionForClause(id);

  //get the assertion for the clause id
  std::map<Node, unsigned > childIndex;
  std::map<Node, bool > childPol;
  Node assertion = clauseToNode( *clause, childIndex, childPol );
  //if there is no reason, construct assertion directly.   This can happen for unit clauses.
  if( base_assertion.isNull() ){
    base_assertion = assertion;
  }
  //os_base is proof of base_assertion
  std::stringstream os_base;

  // checks if tautological definitional clause or top-level clause
  // and prints the proof of the top-level formula
  bool is_input = printProofTopLevel(base_assertion, os_base);

  //get base assertion with polarity
  bool base_pol = base_assertion.getKind()!=kind::NOT;
  base_assertion = base_assertion.getKind()==kind::NOT ? base_assertion[0] : base_assertion;

  std::map< Node, unsigned >::iterator itci = childIndex.find( base_assertion );
  bool is_in_clause = itci!=childIndex.end();
  unsigned base_index = is_in_clause ? itci->second : 0;
  Trace("cnf-pf") << std::endl;
  Trace("cnf-pf") << "; input = " << is_input << ", is_in_clause = " << is_in_clause << ", id = " << id << ", assertion = " << assertion << ", base assertion = " << base_assertion << std::endl;
  if (!is_input){
    Assert(is_in_clause);
    prop::SatLiteral blit = (*clause)[ base_index ];
    os_base << ProofManager::getLitName(blit, d_name);
    base_pol = !childPol[base_assertion]; // WHY? if the case is =>
  }
  Trace("cnf-pf") << "; polarity of base assertion = " << base_pol << std::endl;
  Trace("cnf-pf") << "; proof of base : " << os_base.str() << std::endl;

  bool success = false;
  if( is_input &&
      is_in_clause &&
      childPol[base_assertion]==base_pol ){
    //if both in input and in clause, the proof is trivial.  this is the case for unit clauses.
    Trace("cnf-pf") << "; trivial" << std::endl;
    os << "(contra _ ";
    success = true;
    prop::SatLiteral lit = (*clause)[itci->second];
    if( base_pol ){
      os << os_base.str() << " " << ProofManager::getLitName(lit, d_name);
    }else{
      os << ProofManager::getLitName(lit, d_name) << " " << os_base.str();
    }
    os << ")";
  } else if ((base_assertion.getKind()==kind::AND && !base_pol) ||
           ((base_assertion.getKind()==kind::OR ||
             base_assertion.getKind()==kind::IMPLIES) && base_pol)) {
    Trace("cnf-pf") << "; and/or case 1" << std::endl;
    success = true;
    std::stringstream os_main;
    std::stringstream os_paren;
    //eliminate each one
    for (int j = base_assertion.getNumChildren()-2; j >= 0; j--) {
      Node child_base = base_assertion[j].getKind()==kind::NOT ?
                        base_assertion[j][0] : base_assertion[j];
      bool child_pol = base_assertion[j].getKind()!=kind::NOT;
      
      if( j==0 && base_assertion.getKind()==kind::IMPLIES ){
        child_pol = !child_pol;
      }
      
      Trace("cnf-pf-debug") << "; child " << j << " "
                            << child_base << " "
                            << child_pol << " "
                            << childPol[child_base] << std::endl;
      
      std::map< Node, unsigned >::iterator itcic = childIndex.find( child_base );

      if( itcic!=childIndex.end() ){
        //Assert( child_pol==childPol[child_base] );
        os_main << "(or_elim_1 _ _ ";
        prop::SatLiteral lit = (*clause)[itcic->second];
        // Should be if in the original formula it was negated
        if( childPol[child_base] && base_pol ){
          os_main << ProofManager::getLitName(lit, d_name) << " ";
        }else{
          os_main << "(not_not_intro _ " << ProofManager::getLitName(lit, d_name) << ") ";
        }
        if( base_assertion.getKind()==kind::AND ){
          os_main << "(not_and_elim _ _ ";
          os_paren << ")";
        }
        os_paren << ")";
      }else{
        success = false;
      }
    }
    if( success ){
      if( base_assertion.getKind()==kind::IMPLIES ){
        os_main << "(impl_elim _ _ ";
      }
      os_main << os_base.str();
      if( base_assertion.getKind()==kind::IMPLIES ){
        os_main << ")";
      }
      os_main << os_paren.str();
      int last_index = base_assertion.getNumChildren()-1;
      Node child_base = base_assertion[last_index].getKind()==kind::NOT ? base_assertion[last_index][0] : base_assertion[last_index];
      //bool child_pol = base_assertion[last_index].getKind()!=kind::NOT;
      std::map< Node, unsigned >::iterator itcic = childIndex.find( child_base );
      if( itcic!=childIndex.end() ){
        os << "(contra _ ";
        prop::SatLiteral lit = (*clause)[itcic->second];
        if( childPol[child_base] && base_pol){
          os << os_main.str() << " " << ProofManager::getLitName(lit, d_name);
        }else{
          os << ProofManager::getLitName(lit, d_name) << " " << os_main.str();
        }
        os << ")";
      }else{
        success = false;
      }
    }
  }else if ((base_assertion.getKind()==kind::AND && base_pol) ||
            ((base_assertion.getKind()==kind::OR ||
              base_assertion.getKind()==kind::IMPLIES) && !base_pol)) {
    
    std::stringstream os_main;

    Node iatom;
    if (is_in_clause) {
      Assert( assertion.getNumChildren()==2 );
      iatom = assertion[ base_index==0 ? 1 : 0];
    } else {
      Assert( assertion.getNumChildren()==1 );
      iatom = assertion[0];
    }
    
    Trace("cnf-pf") << "; and/or case 2, iatom = " << iatom << std::endl;
    Node e_base = iatom.getKind()==kind::NOT ? iatom[0] : iatom;
    bool e_pol = iatom.getKind()!=kind::NOT;
    std::map< Node, unsigned >::iterator itcic = childIndex.find( e_base );
    if( itcic!=childIndex.end() ){
      prop::SatLiteral lit = (*clause)[itcic->second];
      //eliminate until we find iatom
      for( unsigned j=0; j<base_assertion.getNumChildren(); j++ ){
        Node child_base = base_assertion[j].getKind()==kind::NOT ? base_assertion[j][0] : base_assertion[j];
        bool child_pol = base_assertion[j].getKind()!=kind::NOT;
        if( j==0 && base_assertion.getKind()==kind::IMPLIES ){
          child_pol = !child_pol;
        }
        if( e_base==child_base && (e_pol==child_pol)==(base_assertion.getKind()==kind::AND) ){
          success = true;
          bool elimNn =( ( base_assertion.getKind()==kind::OR || ( base_assertion.getKind()==kind::IMPLIES && j==1 ) ) && e_pol );
          if( elimNn ){
            os_main << "(not_not_elim _ ";
          }
          std::stringstream os_paren;
          if( j+1<base_assertion.getNumChildren() ){
            os_main << "(and_elim_1 _ _ ";
            if( base_assertion.getKind()==kind::OR || base_assertion.getKind()==kind::IMPLIES ){
              os_main << "(not_" << ( base_assertion.getKind()==kind::OR ? "or" : "impl" ) << "_elim _ _ ";
              os_paren << ")";
            }
            os_paren << ")";
          }
          for( unsigned k=0; k<j; k++ ){
            os_main << "(and_elim_2 _ _ ";
            if( base_assertion.getKind()==kind::OR || base_assertion.getKind()==kind::IMPLIES ){
              os_main << "(not_" << ( base_assertion.getKind()==kind::OR ? "or" : "impl" ) << "_elim _ _ ";
              os_paren << ")";
            }
            os_paren << ")";
          }
          os_main << os_base.str() << os_paren.str();
          if( elimNn ){
            os_main << ")";
          }
          break;
        }
      }
      if( success ){
        os << "(contra _ ";
        if( !e_pol ){
          os << ProofManager::getLitName(lit, d_name) << " " << os_main.str();
        }else{
          os << os_main.str() << " " << ProofManager::getLitName(lit, d_name);
        }
        os << ")";
      }
    }
  }else if( base_assertion.getKind()==kind::XOR || base_assertion.getKind()==kind::IFF ){
    //eliminate negation
    int num_nots_2 = 0;
    int num_nots_1 = 0;
    Kind k;
    if( !base_pol ){
      if( base_assertion.getKind()==kind::IFF ){
        num_nots_2 = 1;
      }
      k = kind::IFF;
    }else{
      k = base_assertion.getKind();
    }
    std::vector< unsigned > indices;
    std::vector< bool > pols;
    success = true;
    int elimNum = 0;
    for( unsigned i=0; i<2; i++ ){
      Node child_base = base_assertion[i].getKind()==kind::NOT ? base_assertion[i][0] : base_assertion[i];
      bool child_pol = base_assertion[i].getKind()!=kind::NOT;
      std::map< Node, unsigned >::iterator itcic = childIndex.find( child_base );
      if( itcic!=childIndex.end() ){
        indices.push_back( itcic->second );
        pols.push_back( childPol[child_base] );
        if( i==0 ){
          //figure out which way to elim
          elimNum = child_pol==childPol[child_base] ? 2 : 1;
          if( (elimNum==2)==(k==kind::IFF) ){
            num_nots_2++;
          }
          if( elimNum==1 ){
            num_nots_1++;
          }
        }
      }else{
        success = false;
        break;
      }
    }
    Trace("cnf-pf") << std::endl << "; num nots = " << num_nots_2 << std::endl;
    if( success ){
      os << "(contra _ ";
      std::stringstream os_base_n;
      if( num_nots_2==2 ){
        os_base_n << "(not_not_elim _ ";
      }
      os_base_n << "(or_elim_1 _ _ ";
      prop::SatLiteral lit1 = (*clause)[indices[0]];
      if( !pols[0] || num_nots_1==1 ){
        os_base_n << "(not_not_intro _ " << ProofManager::getLitName(lit1, d_name) << ") ";
      }else{
        os_base_n << ProofManager::getLitName(lit1, d_name) << " ";
      }
      Assert( elimNum!=0 );
      os_base_n << "(" << ( k==kind::IFF ? "iff" : "xor" ) << "_elim_" << elimNum << " _ _ ";
      if( !base_pol ){
        os_base_n << "(not_" << ( base_assertion.getKind()==kind::IFF ? "iff" : "xor" ) << "_elim _ _ " << os_base.str() << ")";
      }else{
        os_base_n << os_base.str();
      }
      os_base_n << "))";
      if( num_nots_2==2 ){
        os_base_n << ")";
        num_nots_2 = 0;
      }
      prop::SatLiteral lit2 = (*clause)[indices[1]];
      if( pols[1]==(num_nots_2==0) ){
        os << os_base_n.str() << " ";
        if( num_nots_2==1 ){
          os << "(not_not_intro _ " << ProofManager::getLitName(lit2, d_name) << ")";
        }else{
          os << ProofManager::getLitName(lit2, d_name);
        }
      }else{
        os << ProofManager::getLitName(lit2, d_name) << " " << os_base_n.str();
      }
      os << ")";
    }
  }else if( base_assertion.getKind()==kind::ITE ){
    std::map< unsigned, unsigned > appears;
    std::map< unsigned, Node > appears_expr;
    unsigned appears_count = 0;
    for( unsigned r=0; r<3; r++ ){
      Node child_base = base_assertion[r].getKind()==kind::NOT ? base_assertion[r][0] : base_assertion[r];
      std::map< Node, unsigned >::iterator itcic = childIndex.find( child_base );
      if( itcic!=childIndex.end() ){
        appears[r] = itcic->second;
        appears_expr[r] = child_base;
        appears_count++;
      }
    }
    if( appears_count==2 ){
      success = true;
      int elimNum = 1;
      unsigned index1 = 0;
      unsigned index2 = 1;
      if( appears.find( 0 )==appears.end() ){
        elimNum = 3;
        index1 = 1;
        index2 = 2;
      }else if( appears.find( 1 )==appears.end() ){
        elimNum = 2;
        index1 = 0;
        index2 = 2;
      }
      std::stringstream os_main;
      os_main << "(or_elim_1 _ _ ";
      prop::SatLiteral lit1 = (*clause)[appears[index1]];
      if( !childPol[appears_expr[index1]] || elimNum==1 || ( elimNum==3 && !base_pol ) ){
        os_main << "(not_not_intro _ " << ProofManager::getLitName(lit1, d_name) << ") ";
      }else{
        os_main << ProofManager::getLitName(lit1, d_name) << " ";
      }
      os_main << "(" << ( base_pol ? "" : "not_" ) << "ite_elim_" << elimNum << " _ _ _ ";
      os_main << os_base.str() << "))";
      os << "(contra _ ";
      prop::SatLiteral lit2 = (*clause)[appears[index2]];
      if( !childPol[appears_expr[index2]] || !base_pol ){
        os << ProofManager::getLitName(lit2, d_name) << " " << os_main.str();
      }else{
        os << os_main.str() << " " << ProofManager::getLitName(lit2, d_name);
      }
      os << ")";
    }
  }else if( base_assertion.isConst() ){
    bool pol = base_assertion==NodeManager::currentNM()->mkConst( true );
    if( pol!=base_pol ){
      success = true;
      if( pol ){
        os << "(contra _ truth " << os_base.str() << ")";
      }else{
        os << os_base.str();
      }
    }
  }

  if( !success ){
    Trace("cnf-pf") << std::endl;
    Trace("cnf-pf") << ";!!!!!!!!! CnfProof : Can't process " << assertion << ", base = " << base_assertion << ", id = " << id << std::endl;
    Trace("cnf-pf") << ";!!!!!!!!! Clause is : ";
    for (unsigned i = 0; i < clause->size(); ++i) {
      Trace("cnf-pf") << (*clause)[i] << " ";
    }
    Trace("cnf-pf") << std::endl;
    os << "trust-bad";
  }

  os << ")" << clause_paren.str()
     << " (\\ " << ProofManager::getInputClauseName(id, d_name) << "\n";
  paren << "))";
}

void LFSCCnfProof::printClause(const prop::SatClause& clause,
                               std::ostream& os,
                               std::ostream& paren) {
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

// print a proof of the top-level formula e, based on the input assertions
bool LFSCCnfProof::printProofTopLevel(Node e, std::ostream& out) {
  if (!isAssertion(e)) {
    // check if deduced by CNF
    // dependence on top level fact i.e. a depends on (a and b)
    NodeToNode::const_iterator itd = d_cnfDeps.find(e);
    if (itd != d_cnfDeps.end()) {
      TNode parent = itd->second;
      //check if parent is an input assertion
      std::stringstream out_parent;
      if (printProofTopLevel(parent, out_parent)) {
        if(parent.getKind()==kind::AND ||
           (parent.getKind()==kind::NOT && (parent[0].getKind()==kind::IMPLIES ||
                                            parent[0].getKind()==kind::OR))) {
          Node parent_base = parent.getKind()==kind::NOT ? parent[0] : parent;
          Node e_base = e.getKind()==kind::NOT ? e[0] : e;
          bool e_pol = e.getKind()!=kind::NOT;
          for( unsigned i=0; i<parent_base.getNumChildren(); i++ ){
            Node child_base = parent_base[i].getKind()==kind::NOT ? parent_base[i][0] : parent_base[i];
            bool child_pol = parent_base[i].getKind()!=kind::NOT;
            if( parent_base.getKind()==kind::IMPLIES && i==0 ){
              child_pol = !child_pol;
            }
            if (e_base==child_base &&
                (e_pol==child_pol)==(parent_base.getKind()==kind::AND)) {
              bool elimNn = ((parent_base.getKind()==kind::OR ||
                              (parent_base.getKind()==kind::IMPLIES && i==1)) && e_pol);
              if (elimNn) {
                out << "(not_not_elim _ ";
              }
              std::stringstream out_paren;
              if (i+1 < parent_base.getNumChildren()) {
                out << "(and_elim_1 _ _ ";
                if( parent_base.getKind()==kind::OR ||
                    parent_base.getKind()==kind::IMPLIES  ){
                  out << "(not_" << ( parent_base.getKind()==kind::OR ? "or" : "impl" )
                      << "_elim _ _ ";
                  out_paren << ")";
                }
                out_paren << ")";
              }
              for( unsigned j=0; j<i; j++ ){
                out << "(and_elim_2 _ _ ";
                if( parent_base.getKind()==kind::OR || parent_base.getKind()==kind::IMPLIES ){
                  out << "(not_" << ( parent_base.getKind()==kind::OR ? "or" : "impl" ) << "_elim _ _ ";
                  out_paren << ")";
                }
                out_paren << ")";
              }
              out << out_parent.str();
              out << out_paren.str();
              if( elimNn ){
                out << ")";
              }
              return true;
            }
          }
        } else {
          Trace("cnf-pf-debug") << "; isInputAssertion : parent of " << e
                                << " is not correct type (" << parent << ")" << std::endl;
        }
      } else {
        Trace("cnf-pf-debug") << "; isInputAssertion : parent of " << e
                              << " is not input" << std::endl;
      }
    } else {
      Trace("cnf-pf-debug") << "; isInputAssertion : " << e
                            << " has no parent" << std::endl;
    }
    return false;
  } else {
    out << ProofManager::getPreprocessedAssertionName(e);
    return true;
  }
}


// void LFSCCnfProof::printTheoryLemmas(std::ostream& os, std::ostream& paren) {
//   os << " ;; Theory Lemmas\n";
//   ProofManager::ordered_clause_iterator it = ProofManager::currentPM()->begin_lemmas();
//   ProofManager::ordered_clause_iterator end = ProofManager::currentPM()->end_lemmas();

//   for(size_t n = 0; it != end; ++it, ++n) {
//     if(n % 100 == 0) {
//       Chat() << "proving theory conflicts...(" << n << "/" << ProofManager::currentPM()->num_lemmas() << ")" << std::endl;
//     }

//     ClauseId id = it->first;
//     const prop::SatClause* clause = it->second;
//     NodeBuilder<> c(kind::AND);
//     for(unsigned i = 0; i < clause->size(); ++i) {
//       prop::SatLiteral lit = (*clause)[i];
//       prop::SatVariable var = lit.getSatVariable();
//       if(lit.isNegated()) {
//         c << Node::fromExpr(getAtom(var));
//       } else {
//         c << Node::fromExpr(getAtom(var)).notNode();
//       }
//     }
//     Node cl = c;
//     if(ProofManager::getSatProof()->d_lemmaClauses.find(id) !=
//        ProofManager::getSatProof()->d_lemmaClauses.end()) {
//       uint64_t proof_id = ProofManager::getSatProof()->d_lemmaClauses[id];
//       TNode orig = d_cnfStream->getAssertion(proof_id & 0xffffffff);
//       if(((proof_id >> 32) & 0xffffffff) == RULE_ARRAYS_EXT) {
//         Debug("cores") << "; extensional lemma!" << std::endl;
//         Assert(cl.getKind() == kind::AND &&
// 	       cl.getNumChildren() == 2 &&
// 	       cl[0].getKind() == kind::EQUAL &&
// 	       cl[0][0].getKind() == kind::SELECT);
//         TNode myk = cl[0][0][1];
//         Debug("cores") << "; so my skolemized k is " << myk << std::endl;
//         os << "(ext _ _ " << orig[0][0] << " " << orig[0][1]
// 	   << " (\\ " << myk << " (\\ " << ProofManager::getLemmaName(id) << "\n";
//         paren << ")))";
//       }
//     }
//     os << "(satlem _ _ ";
//     std::ostringstream clause_paren;
//     printClause(*clause, os, clause_paren);

//     Debug("cores") << "\n;id is " << id << std::endl;
//     if(ProofManager::getSatProof()->d_lemmaClauses.find(id) !=
//        ProofManager::getSatProof()->d_lemmaClauses.end()) {
//       uint64_t proof_id = ProofManager::getSatProof()->d_lemmaClauses[id];
//       Debug("cores") << ";getting id " << int32_t(proof_id & 0xffffffff) << std::endl;
//       Assert(int32_t(proof_id & 0xffffffff) != -1);
//       TNode orig = d_cnfStream->getAssertion(proof_id & 0xffffffff);
//       Debug("cores") << "; ID is " << id << " and that's a lemma with " << ((proof_id >> 32) & 0xffffffff) << " / " << (proof_id & 0xffffffff) << std::endl;
//       Debug("cores") << "; that means the lemma was " << orig << std::endl;
//       if(((proof_id >> 32) & 0xffffffff) == RULE_ARRAYS_EXT) {
//         Debug("cores") << "; extensional" << std::endl;
//         os << "(clausify_false trust)\n";
//       } else if(proof_id == 0) {
//         // theory propagation caused conflict
//         //ProofManager::currentPM()->printProof(os, cl);
//         os << "(clausify_false trust)\n";
//       } else if(((proof_id >> 32) & 0xffffffff) == RULE_CONFLICT) {
//         os << "\n;; need to generate a (conflict) proof of " << cl << "\n";
//         //ProofManager::currentPM()->printProof(os, cl);
//         os << "(clausify_false trust)\n";
//       } else {
//         os << "\n;; need to generate a (lemma) proof of " << cl;
//         os << "\n;; DON'T KNOW HOW !!\n";
//         os << "(clausify_false trust)\n";
//       }
//     } else {
//       os << "\n;; need to generate a (conflict) proof of " << cl << "\n";
//       ProofManager::currentPM()->printProof(os, cl);
//     }
//     os << clause_paren.str()
//        << " (\\ " << ProofManager::getLemmaClauseName(id, d_name) << "\n";
//     paren << "))";
//   }
// }


} /* CVC4 namespace */
