/*********************                                                        */
/*! \file sygus_process_conj.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Techniqures for static preprocessing and analysis of
 ** sygus conjectures.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__QUANTIFIERS__SYGUS_PPROCESS_CONJ_H
#define __CVC4__THEORY__QUANTIFIERS__SYGUS_PROCESSS_CONJ_H

#include "expr/node.h"
#include "theory/quantifiers_engine.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {
  
/** This structure stores information regarding
 * an argument of a function to synthesize.
 * It is used to determine whether the argument
 * position in the function to synthesize is
 * relevant. 
 */
class CegConjectureProcessArg
{
public:
  CegConjectureProcessArg() : d_parent(nullptr), d_relevant(false), d_deq_id(0) {}
  /** parent argument 
   * 
    * If non-null, this is a pointer to an argument 
   * whose relevance implies the relevance of this argument
   * 
   * Altogether, this data structure represents
   * a union-find.
   */
  CegConjectureProcessArg * d_parent;
  /** whether this argument is relevant */
  bool d_relevant;
  /** disequality id of this argument 
   * We ensure that argument positions i and j 
   * have the same disequality id if and only if 
   * all f-applications t in our synthesis
   * conjecture are such that t[i] = t[j].
   */
  unsigned d_deq_id;
  /** get parent in the union find */
  CegConjectureProcessArg * getParent();
};

/** This structure stores information regarding conjecture-specific
* analysis of a function to synthesize.
* 
* It maintains information about each of the function to 
* synthesize's arguments.
*/
struct CegConjectureProcessFun
{
 public:
  CegConjectureProcessFun() : d_deq_id_counter(0) {}
  ~CegConjectureProcessFun() {}
  /** initialize this class for function f */
  void init(Node f);
  /** process term 
  *
   * n is an f-application to process,
   * freeVars maps all subterms of n to the set 
   *   of variables (in set synth_fv) they contain.
   * 
   * This updates information regarding which arguments
   * of the function-to-synthesize are relevant.
   */
  void processTerm(Node n, Node k, Node nf,
                   std::unordered_map<Node, std::unordered_set< Node, NodeHashFunction >, NodeHashFunction >& free_vars);
 private:
  /** the synth fun associated with this */
  Node d_synth_fun;
  /** deq id counter */
  unsigned d_deq_id_counter;
  /** deq id equivalence classes */
  std::vector< std::unordered_set< unsigned > > d_deq_id_eqc;
  /** properties of each argument */
  std::map<unsigned, CegConjectureProcessArg> d_arg_props;
  /** the set of arguments that this synth-fun is independent of */
  std::map<unsigned, bool> d_arg_independent;
  /** allocate new deq id */
  unsigned allocateDeqId();
};

/** Ceg Conjecture Process
*
* This class implements static techniques for preprocessing and analysis of
* sygus conjectures.
*
* It is used as a back-end to CegConjecture, which calls it using the following
* interface:
* (1) When a sygus conjecture is asserted, we call
* CegConjectureProcess::simplify( q ),
*     where q is the sygus conjecture in original form.
*
* (2) After a sygus conjecture is simplified and converted to deep
* embedding form, we call CegConjectureProcess::initialize( n, candidates ).
*
* (3) During enumerative SyGuS search, calls may be made by
* the extension of the quantifier-free datatypes decision procedure for
* sygus to CegConjectureProcess::getSymmetryBreakingPredicate(...), which are
* used for pruning search space based on conjecture-specific analysis.
*/
class CegConjectureProcess
{
 public:
  CegConjectureProcess(QuantifiersEngine* qe);
  ~CegConjectureProcess();
  /** simplify the synthesis conjecture q
  * Returns a formula that is equivalent to q.
  */
  Node simplify(Node q);
  /** initialize
  *
  * n is the "base instantiation" of the deep-embedding version of
  *   the synthesis conjecture under "candidates".
  *   (see CegConjecture::d_base_inst)
  */
  void initialize(Node n, std::vector<Node>& candidates);
  /** get symmetry breaking predicate
  *
  * Returns a formula that restricts the enumerative search space (for a given
  * depth) for a term x of sygus type tn whose top symbol is the tindex^{th}
  * constructor, where x is a subterm of enumerator e.
  */
  Node getSymmetryBreakingPredicate(
      Node x, Node e, TypeNode tn, unsigned tindex, unsigned depth);
  /** print out debug information about this conjecture */
  void debugPrint(const char* c);

 private:
  /** process conjunct 
   * 
   * This sets up initial information about functions to synthesize
   * where n is a conjunct of the synthesis conjecture, and synth_fv
   * is the set of (inner) universal variables in the synthesis
   * conjecture.
   */
  void processConjunct(Node n, std::unordered_set< Node, NodeHashFunction >& synth_fv);
  /** flatten 
   * 
   * Flattens all synthesis functions in term n.
   * This may add new variables to synth_fv, which
   * are introduced at all positions of functions
   * to synthesize in a bottom-up fashion. For each
   * variable k introduced for a function application
   * f(t), we add ( k -> f(t) ) to defs and ( f -> k )
   * to fun_to_defs.
   */
  Node flatten(Node n, std::unordered_set< Node, NodeHashFunction >& synth_fv, 
               std::unordered_map<Node,Node,NodeHashFunction>& defs, 
               std::unordered_map<Node,std::vector<Node>,NodeHashFunction>& fun_to_defs);
  /** get free variables 
   * Constructs a map of all free variables that occur in n
   * from synth_fv and stores them in the map free_vars.
   */
  void getFreeVariables(Node n, std::unordered_set< Node, NodeHashFunction >& synth_fv,
                        std::unordered_map<Node, std::unordered_set< Node, NodeHashFunction >, NodeHashFunction >& free_vars );
  /** for each synth-fun, information that is specific to this conjecture */
  std::map<Node, CegConjectureProcessFun> d_sf_info;
  /** reference to quantifier engine */
  QuantifiersEngine* d_qe;
  /** get component vector */
  void getComponentVector( Kind k, Node n, std::vector< Node >& args );
};

} /* namespace CVC4::theory::quantifiers */
} /* namespace CVC4::theory */
} /* namespace CVC4 */

#endif
