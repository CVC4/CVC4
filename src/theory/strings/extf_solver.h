/*********************                                                        */
/*! \file ext_solver.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Solver for extended functions of theory of strings.
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__STRINGS__EXTF_SOLVER_H
#define CVC4__THEORY__STRINGS__EXTF_SOLVER_H

#include "context/cdo.h"
#include "expr/node.h"
#include "theory/ext_theory.h"
#include "theory/strings/solver_state.h"
#include "theory/strings/inference_manager.h"
#include "theory/strings/skolem_cache.h"
#include "theory/strings/base_solver.h"

namespace CVC4 {
namespace theory {
namespace strings {

/**
 * Extended function solver for the theory of strings. This manages extended
 * functions for the theory of strings using a combination of context-dependent
 * simplification (Reynolds et al CAV 2017) and lazy reductions.
 */
class ExtfSolver{
  typedef context::CDHashSet<Node, NodeHashFunction> NodeSet;
 public:
  ExtfSolver(context::Context* c, SolverState& s, 
             InferenceManager& im, 
             SkolemCache& skc,
             BaseSolver& bs, ExtTheory* et);
  ~ExtfSolver();
  
  /** check extended functions evaluation
   *
   * This applies "context-dependent simplification" for all active extended
   * function terms in this SAT context. This infers facts of the form:
   *   x = c => f( t1 ... tn ) = c'
   * where the rewritten form of f( t1...tn ) { x |-> c } is c', and x = c
   * is a (tuple of) equalities that are asserted in this SAT context, and
   * f( t1 ... tn ) is a term from this SAT context.
   *
   * For more details, this is steps 4 when effort=0 and step 6 when
   * effort=1 from Strategy 1 in Reynolds et al, "Scaling up DPLL(T) String
   * Solvers using Context-Dependent Simplification", CAV 2017. When called with
   * effort=3, we apply context-dependent simplification based on model values.
   */
  void checkExtfEval(int effort);
  /** check extended function reductions
   *
   * This adds "reduction" lemmas for each active extended function in this SAT
   * context. These are generally lemmas of the form:
   *   F[t1...tn,k] ^ f( t1 ... tn ) = k
   * where f( t1 ... tn ) is an active extended function, k is a fresh constant
   * and F is a formula that constrains k based on the definition of f.
   *
   * For more details, this is step 7 from Strategy 1 in Reynolds et al,
   * CAV 2017. We stratify this in practice, where calling this with effort=1
   * reduces some of the "easier" extended functions, and effort=2 reduces
   * the rest.
   */
  void checkExtfReductions(int effort);
  
private:
  /**
   * Non-static information about an extended function t. This information is
   * constructed and used during the check extended function evaluation
   * inference schema.
   *
   * In the following, we refer to the "context-dependent simplified form"
   * of a term t to be the result of rewriting t * sigma, where sigma is a
   * derivable substitution in the current context. For example, the
   * context-depdendent simplified form of contains( x++y, "a" ) given
   * sigma = { x -> "" } is contains(y,"a").
   */
  class ExtfInfoTmp
  {
   public:
    ExtfInfoTmp() : d_modelActive(true) {}
    /**
     * If s is in d_ctn[true] (resp. d_ctn[false]), then contains( t, s )
     * (resp. ~contains( t, s )) holds in the current context. The vector
     * d_ctnFrom is the explanation for why this holds. For example,
     * if d_ctn[false][i] is "A", then d_ctnFrom[false][i] might be
     * t = x ++ y AND x = "" AND y = "B".
     */
    std::map<bool, std::vector<Node> > d_ctn;
    std::map<bool, std::vector<Node> > d_ctnFrom;
    /**
     * The constant that t is entailed to be equal to, or null if none exist.
     */
    Node d_const;
    /**
     * The explanation for why t is equal to its context-dependent simplified
     * form.
     */
    std::vector<Node> d_exp;
    /** This flag is false if t is reduced in the model. */
    bool d_modelActive;
  };
  /** do reduction
   *
   * This is called when an extended function application n is not able to be
   * simplified by context-depdendent simplification, and we are resorting to
   * expanding n to its full semantics via a reduction. This method returns
   * true if it successfully reduced n by some reduction and sets isCd to
   * true if the reduction was (SAT)-context-dependent, and false otherwise.
   * The argument effort has the same meaning as in checkExtfReductions.
   */
  bool doReduction(int effort, Node n, bool& isCd);
  /** check extended function inferences
   *
   * This function makes additional inferences for n that do not contribute
   * to its reduction, but may help show a refutation.
   *
   * This function is called when the context-depdendent simplified form of
   * n is nr. The argument "in" is the information object for n. The argument
   * "effort" has the same meaning as the effort argument of checkExtfEval.
   */
  void checkExtfInference(Node n, Node nr, ExtfInfoTmp& in, int effort);
  /** The solver state object */
  SolverState& d_state;
  /** The (custom) output channel of the theory of strings */
  InferenceManager& d_im;
  /** cache of all skolems */
  SkolemCache& d_skCache;
  /** reference to the base solver, used for certain queries */
  BaseSolver& d_bsolver;
  /** the extended theory object for the theory of strings */
  ExtTheory * d_extt;
  /** Common constants */
  Node d_true;
  Node d_false;
  /** Empty vector */
  std::vector< Node > d_empty_vec;
  /** map extended functions to the above information */
  std::map<Node, ExtfInfoTmp> d_extfInfoTmp;
  /** any non-reduced extended functions exist? */
  context::CDO< bool > d_has_extf;
  /** extended functions inferences cache */
  NodeSet d_extf_infer_cache;
};/* class ExtfSolver */

}/* CVC4::theory::strings namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* CVC4__THEORY__STRINGS__EXTF_SOLVER_H */
