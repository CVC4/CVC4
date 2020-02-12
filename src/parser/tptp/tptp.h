/*********************                                                        */
/*! \file tptp.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Francois Bobot, Andrew Reynolds, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Definitions of TPTP constants.
 **
 ** Definitions of TPTP constants.
 **/

#include "parser/antlr_input.h" // Needs to go first.

#include "cvc4parser_private.h"

#ifndef CVC4__PARSER__TPTP_H
#define CVC4__PARSER__TPTP_H

#include <cassert>
#include <unordered_map>
#include <unordered_set>

#include "parser/parser.h"
#include "smt/command.h"
#include "util/hash.h"
#include "api/cvc4cpp.h"

namespace CVC4 {

namespace api {
class Solver;
}

namespace parser {

class Tptp : public Parser {
 private:
  friend class ParserBuilder;
 public:
  bool cnf() const { return d_cnf; }
  void setCnf(bool cnf) { d_cnf = cnf; }

  bool fof() const { return d_fof; }
  void setFof(bool fof) { d_fof = fof; }

  void forceLogic(const std::string& logic) override;

  void addFreeVar(api::Term var);
  std::vector< api::Term > getFreeVar();

  api::Term convertRatToUnsorted(api::Term expr);
  /**
   * Returns a free constant corresponding to the string str. We ensure that
   * these constants are one-to-one with str. We assert that all these free
   * constants are pairwise distinct before issuing satisfiability queries.
   */
  api::Term convertStrToUnsorted(std::string str);

  // CNF and FOF are unsorted so we define this common type.
  // This is also the api::Sort of $i in TFF.
  api::Sort d_unsorted;

  enum Theory {
    THEORY_CORE,
  };/* enum Theory */

  enum FormulaRole {
    FR_AXIOM,
    FR_HYPOTHESIS,
    FR_DEFINITION,
    FR_ASSUMPTION,
    FR_LEMMA,
    FR_THEOREM,
    FR_CONJECTURE,
    FR_NEGATED_CONJECTURE,
    FR_UNKNOWN,
    FR_PLAIN,
    FR_FI_DOMAIN,
    FR_FI_FUNCTORS,
    FR_FI_PREDICATES,
    FR_TYPE,
  };/* enum FormulaRole */

  bool hasConjecture() const { return d_hasConjecture; }

 protected:
  Tptp(api::Solver* solver,
       Input* input,
       bool strictMode = false,
       bool parseOnly = false);

 public:
  ~Tptp();
  /**
   * Add theory symbols to the parser state.
   *
   * @param theory the theory to open (e.g., Core, Ints)
   */
  void addTheory(Theory theory);

  void makeApplication(api::Term& expr, std::string& name, std::vector<api::Term>& args,
                       bool term);

  /** creates a lambda abstraction around expression
   *
   * Given an expression expr of type argType = t1...tn -> t, creates a lambda
   * expression
   *  (lambda x1:t1,...,xn:tn . (expr x)) : t
   */
  void mkLambdaWrapper(api::Term& expr, api::Sort argType);
  /** 
   * Make builtin application
   */
  api::Term mkBuiltinApp(api::Term f, const std::vector<api::Term>& args);

  /** get assertion expression, based on the formula role.
  * expr should have Boolean type.
  * This returns the expression that should be asserted, given the formula role fr.
  * For example, if the role is "conjecture", then the return value is the negation of expr.
  */
  api::Term getAssertionExpr(FormulaRole fr, api::Term expr);

  /** get assertion for distinct constants
   *
   * This returns a node of the form distinct( k1, ..., kn ) where k1, ..., kn
   * are the distinct constants introduced by this parser (see
   * convertStrToUnsorted) if n>1, or null otherwise.
   */
  api::Term getAssertionDistinctConstants();

  /** returns the appropriate AssertCommand, given a role, expression expr to assert,
  * and information about the assertion.
  *   The assertion expr is literally what should be asserted (it is already been processed
  *   with getAssertionapi::Term above).
  *   This may set a flag in the parser to mark that we have asserted a conjecture.
  */
  Command* makeAssertCommand(FormulaRole fr, api::Term expr, bool cnf, bool inUnsatCore);
  
  /** Ugly hack because I don't know how to return an expression from a
      token */
  api::Term d_tmp_expr;

  /** Push a new stream in the lexer. When EOF is reached the previous stream
      is reused */
  void includeFile(std::string fileName);

  /** Check a TPTP let binding for well-formedness. */
  void checkLetBinding(const std::vector<api::Term>& bvlist, api::Term lhs, api::Term rhs,
                       bool formula);

 private:
  void addArithmeticOperators();

  // In CNF variable are implicitly binded
  // d_freevar collect them
  std::vector< api::Term > d_freeVar;
  api::Term d_rtu_op;
  api::Term d_stu_op;
  api::Term d_utr_op;
  api::Term d_uts_op;
  // The set of expression that already have a bridge
  std::unordered_set<api::Term, api::TermHashFunction> d_r_converted;
  std::unordered_map<std::string, api::Term> d_distinct_objects;

  std::vector< pANTLR3_INPUT_STREAM > d_in_created;

  // TPTP directory where to find includes;
  // empty if none could be determined
  std::string d_tptpDir;

  // the null expression
  api::Term d_nullExpr;

  // hack to make output SZS ontology-compliant
  bool d_hasConjecture;

  bool d_cnf; // in a cnf formula
  bool d_fof; // in an fof formula
};/* class Tptp */


namespace tptp {
/**
 * Just exists to provide the uintptr_t constructor that ANTLR
 * requires.
 */
struct myExpr : public CVC4::api::Term {
  myExpr() : CVC4::api::Term() {}
  myExpr(void*) : CVC4::api::Term() {}
  myExpr(const CVC4::api::Term& e) : CVC4::api::Term(e) {}
  myExpr(const myExpr& e) : CVC4::api::Term(e) {}
};/* struct myExpr*/

enum NonAssoc {
  NA_IFF,
  NA_IMPLIES,
  NA_REVIMPLIES,
  NA_REVIFF,
  NA_REVOR,
  NA_REVAND,
};

}/* CVC4::parser::tptp namespace */


}/* CVC4::parser namespace */
}/* CVC4 namespace */

#endif /* CVC4__PARSER__TPTP_INPUT_H */
