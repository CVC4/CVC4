/*********************                                                        */
/*! \file cnf_proof.h
 ** \verbatim
 ** Original author: Liana Hadarean
 ** Major contributors: Morgan Deters, Andrew Reynolds
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2014  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief A manager for CnfProofs.
 **
 ** A manager for CnfProofs.
 **
 **
 **/

#ifndef __CVC4__CNF_PROOF_H
#define __CVC4__CNF_PROOF_H

#include "cvc4_private.h"
#include "util/proof.h"
#include "proof/sat_proof.h"

#include <ext/hash_set>
#include <ext/hash_map>
#include <iostream>

namespace CVC4 {
namespace prop {
  class CnfStream;
}/* CVC4::prop namespace */

class CnfProof;

typedef __gnu_cxx::hash_map < ClauseId, const prop::SatClause* > IdToClause;
typedef __gnu_cxx::hash_map<Expr, prop::SatVariable, ExprHashFunction > ExprToSatVar;
typedef __gnu_cxx::hash_map<prop::SatVariable, Expr> SatVarToExpr;

class CnfProof {
protected:
  CVC4::prop::CnfStream* d_cnfStream;
  ExprToSatVar d_atomToSatVar;
  SatVarToExpr d_satVarToAtom;
  IdToClause d_inputClauses;
  std::string d_name;
public:
  CnfProof(CVC4::prop::CnfStream* cnfStream, const std::string& name);

  typedef IdToClause::const_iterator clause_iterator;
  clause_iterator begin_input_clauses() const { return d_inputClauses.begin(); }
  clause_iterator end_input_clauses() const { return d_inputClauses.end(); }
  void addInputClause(ClauseId id, const prop::SatClause* clause); 
  void collectAtoms(const prop::SatClause* clause);
  
  typedef ExprToSatVar::const_iterator atom_iterator;
  atom_iterator begin_atoms() { return d_atomToSatVar.begin(); }
  atom_iterator end_atoms() { return d_atomToSatVar.end(); }
  Expr getAtom(prop::SatVariable var);
  prop::SatLiteral getLiteral(TNode node);
  Expr getAssertion(uint64_t id);

  virtual void printAtomMapping(std::ostream& os, std::ostream& paren) = 0;
  virtual void printClauses(std::ostream& os, std::ostream& paren) = 0;
  virtual void printClause(const prop::SatClause& clause, std::ostream& os, std::ostream& paren) = 0;
  virtual ~CnfProof();
};/* class CnfProof */

class LFSCCnfProof : public CnfProof {
  void printPreprocess(std::ostream& os, std::ostream& paren);
  void printInputClauses(std::ostream& os, std::ostream& paren);
  void printTheoryLemmas(std::ostream& os, std::ostream& paren);

  Expr clauseToExpr( const prop::SatClause& clause,
                     std::map< Expr, unsigned >& childIndex,
                     std::map< Expr, bool >& childPol );
 
public:
  LFSCCnfProof(CVC4::prop::CnfStream* cnfStream, const std::string& name)
    : CnfProof(cnfStream, name)
  {}

  virtual void printClauses(std::ostream& os, std::ostream& paren);
  void printClause(const prop::SatClause& clause, std::ostream& os, std::ostream& paren);
  void printAtomMapping(std::ostream& os, std::ostream& paren);
};/* class LFSCCnfProof */

} /* CVC4 namespace */

#endif /* __CVC4__CNF_PROOF_H */
