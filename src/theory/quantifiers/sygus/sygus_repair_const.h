/*********************                                                        */
/*! \file sygus_repair_const.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief sygus_repair_const
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__QUANTIFIERS__SYGUS_REPAIR_CONST_H
#define __CVC4__THEORY__QUANTIFIERS__SYGUS_REPAIR_CONST_H

#include <unordered_set>
#include "expr/node.h"
#include "theory/quantifiers_engine.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

class CegConjecture;

/** SygusRepairConst
 *
 * This module is used to repair portions of candidate solutions. In particular,
 * given a synthesis conjecture:
 *   exists f. forall x. P( f, x )
 * and a candidate solution f = \x. t[x,c] where c are constants, this function
 * checks whether there exists a term of the form \x. t[x,c'] for some constants
 * c' such that:
 *   forall x. P( (\x. t[x,c']), x )  [***]
 * is satisfiable, where notice that the above formula after beta-reduction may
 * be one in pure first-order logic in a decidable theory (say linear
 * arithmetic). To check this, we invoke a separate instance of the SmtEngine
 * within repairSolution(...) below, which if, satisfiable gives us the
 * valuation for c'.
 */
class SygusRepairConst
{
 public:
  SygusRepairConst(QuantifiersEngine* qe);
  ~SygusRepairConst() {}
  /** initialize
   * 
   * Initialize this class with the base instantiation of the sygus conjecture 
   * (see CegConjecture::d_base_inst) and its candidate variables (see
   * CegConjecture::d_candidates).
   */
  void initialize(Node base_inst, const std::vector< Node >& candidates);
  /** repair solution
   *
   * This function is called when candidates -> candidate_values is a (failed)
   * candidate solution for the synthesis conjecture.
   *
   * If this function returns true, then this class adds to repair_cv the
   * repaired version of the solution candidate_values for each candidate,
   * where for each index i, repair_cv[i] is obtained by replacing constant
   * subterms in candidate_values[i] with others. Moreover, it is the case that
   *    repair_cv[j] != candidate_values[j], for at least one j.
   */
  bool repairSolution(const std::vector<Node>& candidates,
                      const std::vector<Node>& candidate_values,
                      std::vector<Node>& repair_cv);

 private:
  /** reference to quantifier engine */
  QuantifiersEngine* d_qe;
  /** pointer to the sygus term database of d_qe */
  TermDbSygus * d_tds;
  /** 
   * The deep embedding form of the synthesis conjecture associated with this
   * class.
   */
  Node d_base_inst;
  /** 
   * whether any sygus type for the candidate variables of d_base_inst (the 
   * syntactic restrictions) allows all constants. If this flag is false, then
   * this class is a no-op.
   */
  bool d_allow_constant_grammar;
  /** map from skeleton variables to first-order variables */
  std::map< Node, Node > d_sk_to_fo;
  /** a cache of satisfiability queries of the form [***] above we have tried */
  std::unordered_set<Node, NodeHashFunction> d_queries;
  /** 
   * Register information for sygus type tn, tprocessed stores the set of 
   * already registered types.
   */
  void registerSygusType(TypeNode tn, std::map<TypeNode, bool >& tprocessed);
};

} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */

#endif /* __CVC4__THEORY__QUANTIFIERS__SYGUS_REPAIR_CONST_H */
