/*********************                                                        */
/*! \file theory_model_builder.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Tim King, Morgan Deters
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Model class
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__THEORY_MODEL_BUILDER_H
#define CVC4__THEORY__THEORY_MODEL_BUILDER_H

#include <unordered_map>
#include <unordered_set>

#include "theory/theory_model.h"

namespace CVC4 {
namespace theory {

/** TheoryEngineModelBuilder class
 *
 * This is the class used by TheoryEngine
 * for constructing TheoryModel objects, which is the class
 * that represents models for a set of assertions.
 *
 * A call to TheoryEngineModelBuilder::buildModel(...) is made
 * after a full effort check passes with no theory solvers
 * adding lemmas or conflicts, and theory combination passes
 * with no splits on shared terms. If buildModel is successful,
 * this will set up the data structures in TheoryModel to represent
 * a model for the current set of assertions.
 */
class TheoryEngineModelBuilder : public ModelBuilder
{
  typedef std::unordered_map<Node, Node, NodeHashFunction> NodeMap;
  typedef std::unordered_set<Node, NodeHashFunction> NodeSet;

 public:
  TheoryEngineModelBuilder(TheoryEngine* te);
  virtual ~TheoryEngineModelBuilder() {}
  /** Build model function.
   *
   * Should be called only on TheoryModels m.
   *
   * This constructs the model m, via the following steps:
   * (1) call TheoryEngine::collectModelInfo,
   * (2) builder-specified pre-processing,
   * (3) find the equivalence classes of m's
   *     equality engine that initially contain constants,
   * (4) assign constants to all equivalence classes
   *     of m's equality engine, through alternating
   *     iterations of evaluation and enumeration,
   * (5) builder-specific processing, which includes assigning total
   *     interpretations to uninterpreted functions.
   *
   * This function returns false if any of the above
   * steps results in a lemma sent on an output channel.
   * Lemmas may be sent on an output channel by this
   * builder in steps (2) or (5), for instance, if the model we
   * are building fails to satisfy a quantified formula.
   */
  bool buildModel(Model* m) override;

  /** postprocess model
   *
   * This is called when m is a model that will be returned to the user. This
   * method checks the internal consistency of the model if we are in a debug
   * build.
   */
  void postProcessModel(bool incomplete, Model* m);

 protected:
  /** pointer to theory engine */
  TheoryEngine* d_te;

  //-----------------------------------virtual functions
  /** pre-process build model
   * Do pre-processing specific to this model builder.
   * Called in step (2) of the build construction,
   * described above.
   */
  virtual bool preProcessBuildModel(TheoryModel* m);
  /** process build model
   * Do processing specific to this model builder.
   * Called in step (5) of the build construction,
   * described above.
   * By default, this assigns values to each function
   * that appears in m's equality engine.
   */
  virtual bool processBuildModel(TheoryModel* m);
  /** debug the model
   * Check assertions and printing debug information for the model.
   * Calls after step (5) described above is complete.
   */
  virtual void debugModel(TheoryModel* m) {}
  //-----------------------------------end virtual functions

  /** Debug check model.
   *
   * This throws an assertion failure if the model contains an equivalence
   * class with two terms t1 and t2 such that t1^M != t2^M.
   */
  void debugCheckModel(TheoryModel* m);

  /** Evaluate equivalence class
   *
   * If this method returns a non-null node c, then c is a constant and some
   * term in the equivalence class of t evaluates to c based on the current
   * state of the model m.
   */
  Node evaluateEqc(TheoryModel* m, TNode r);
  /** is equivalence class r assignable?
   *
   * An equivalence class r is assignable if:
   * (1) It contains at least one assignable expression (see below) and
   * (2) for each term t in r, all terms in the assignment exclusion set of t
   * (if it has one) have already been assigned values.
   *
   * If r is assignable, this method returns true and appends the constants that
   * have been assigned to the assignment exclusion set of all terms t in r to
   * eset.
   *
   * It sets evaluable to true if there is a term in r that is not assignable.
   */
  bool isAssignableEqc(TheoryModel* m,
                       TNode r,
                       std::vector<Node>& eset,
                       bool& evaluable);
  /** is n an assignable expression?
   *
   * A term n is an assignable expression if its value is unconstrained by a
   * standard model. Examples of assignable terms are:
   * - variables,
   * - applications of array select,
   * - applications of datatype selectors,
   * - applications of uninterpreted functions.
   * Assignable terms must be first-order, that is, all instances of the above
   * terms are not assignable if they have a higher-order (function) type.
   */
  bool isAssignableExpression(TNode n);
  /** Process equivalence class internal
   *
   * This is a helper function for evaluateEqc and isAssignableEqc.
   *
   * If doEval is true, it returns a non-null node c if some term t in r
   * evaluates to c based on the current state of m.
   *
   * If doComputeEset is true, it adds terms to eset that are in the assignment
   * exclusion set of some term t in r.
   *
   * It sets assignable to true if there is an assignable term in r, it sets
   * evaluable to true if there is a term in r that is not assignable.
   */
  Node processEqcInternal(TheoryModel* m,
                          TNode r,
                          bool& assignable,
                          bool& evaluable,
                          std::vector<Node>& eset,
                          bool doEval,
                          bool doComputeEset);
  /** add assignable subterms
   * Adds all assignable subterms of n to tm's equality engine.
   */
  void addAssignableSubterms(TNode n, TheoryModel* tm, NodeSet& cache);
  /** normalize representative r
   *
   * This returns a term that is equivalent to r's
   * interpretation in the model m. It may do so
   * by rewriting the application of r's operator to the
   * result of normalizing each of r's children, if
   * each child is constant.
   */
  Node normalize(TheoryModel* m, TNode r, bool evalOnly);
  /** assign constant representative
   *
   * Called when equivalence class eqc is assigned a constant
   * representative const_rep.
   *
   * eqc should be a representative of tm's equality engine.
   */
  void assignConstantRep(TheoryModel* tm, Node eqc, Node const_rep);
  /** add to type list
   *
   * This adds to type_list the list of types that tn is built from.
   * For example, if tn is (Array Int Bool) and type_list is empty,
   * then we append ( Int, Bool, (Array Int Bool) ) to type_list.
   */
  void addToTypeList(
      TypeNode tn,
      std::vector<TypeNode>& type_list,
      std::unordered_set<TypeNode, TypeNodeHashFunction>& visiting);
  /** assign function f based on the model m.
  * This construction is based on "table form". For example:
  * (f 0 1) = 1
  * (f 0 2) = 2
  * (f 1 1) = 3
  * ...
  * becomes:
  * f = (lambda xy. (ite (and (= x 0) (= y 1)) 1
  *                 (ite (and (= x 0) (= y 2)) 2
  *                 (ite (and (= x 1) (= y 1)) 3 ...)))
  */
  void assignFunction(TheoryModel* m, Node f);
  /** assign function f based on the model m.
  * This construction is based on "dag form". For example:
  * (f 0 1) = 1
  * (f 0 2) = 2
  * (f 1 1) = 3
  * ...
  * becomes:
  * f = (lambda xy. (ite (= x 0) (ite (= y 1) 1
  *                              (ite (= y 2) 2 ...))
  *                 (ite (= x 1) (ite (= y 1) 3 ...)
  *                              ...))
  *
  * where the above is represented as a directed acyclic graph (dag).
  * This construction is accomplished by assigning values to (f c)
  * terms before f, e.g.
  * (f 0) = (lambda y. (ite (= y 1) 1
  *                    (ite (= y 2) 2 ...))
  * (f 1) = (lambda y. (ite (= y 1) 3 ...))
  * where
  * f = (lambda xy. (ite (= x 0) ((f 0) y)
  *                 (ite (= x 1) ((f 1) y) ...))
  */
  void assignHoFunction(TheoryModel* m, Node f);
  /** assign functions
   *
   * Assign all unassigned functions in the model m (those returned by
   * TheoryModel::getFunctionsToAssign),
   * using the two functions above. Currently:
   * If ufHo is disabled, we call assignFunction for all functions.
   * If ufHo is enabled, we call assignHoFunction.
   */
  void assignFunctions(TheoryModel* m);

 private:
  /** normalized cache
   * A temporary cache mapping terms to their
   * normalized form, used during buildModel.
   */
  NodeMap d_normalizedCache;
  /** mapping from terms to the constant associated with their equivalence class
   */
  std::map<Node, Node> d_constantReps;

  /** Theory engine model builder assigner class
   *
   * This manages the assignment of values to a terms of a given type.
   * In particular, it is a wrapper over a type enumerator that is restricted
   * by a set of values that it cannot generate, called an "assignment exclusion
   * set".
   */
  class Assigner
  {
   public:
    Assigner() : d_te(nullptr), d_isActive(false) {}
    /**
     * Initialize this assigner to generate values of type tn, with properties
     * tep and assignment exclusion set aes.
     */
    void initialize(TypeNode tn,
                    TypeEnumeratorProperties* tep,
                    const std::vector<Node>& aes);
    /** get the next term, or null if it does not exist */
    Node getNextAssignment();
    /** The type enumerator */
    std::unique_ptr<TypeEnumerator> d_te;
    /** The assignment exclusion set of this */
    std::vector<Node> d_assignExcSet;
    /**
     * Is active, flag set to true when all values in d_assignExcSet are
     * constant.
     */
    bool d_isActive;
  };
  /** Is the given Assigner ready to assign values?
   *
   * This returns true if all values in the assignment exclusion set of a have
   * known value according to the state of this model builder (via a lookup
   * in d_constantReps). It updates the assignment exclusion vector of a to
   * these values whenever possible.
   */
  bool isAssignerActive(TheoryModel* tm, Assigner& a);

  //------------------------------------for codatatypes
  /** is v an excluded codatatype value?
   *
   * If this returns true, then v is a value
   * that cannot be used during enumeration in step (4)
   * of model construction.
   *
   * repSet is the set of representatives of the same type as v,
   * assertedReps is a map from representatives t,
   * eqc is the equivalence class that v reside.
   *
   * This function is used to avoid alpha-equivalent
   * assignments for codatatype terms, described in
   * Reynolds/Blanchette CADE 2015. In particular,
   * this function returns true if v is in
   * the set V^{x}_I from page 9, where x is eqc
   * and I is the model we are building.
   */
  bool isExcludedCdtValue(Node v,
                          std::set<Node>* repSet,
                          std::map<Node, Node>& assertedReps,
                          Node eqc);
  /** is codatatype value match
   *
   * This returns true if v is r{ eqc -> t } for some t.
   * If this function returns true, then t above is
   * stored in eqc_m.
   */
  bool isCdtValueMatch(Node v, Node r, Node eqc, Node& eqc_m);
  //------------------------------------end for codatatypes

  //---------------------------------for debugging finite model finding
  /** does type tn involve an uninterpreted sort? */
  bool involvesUSort(TypeNode tn);
  /** is v an excluded value based on uninterpreted sorts?
   * This gives an assertion failure in the case that v contains
   * an uninterpreted constant whose index is out of the bounds
   * specified by eqc_usort_count.
   */
  bool isExcludedUSortValue(std::map<TypeNode, unsigned>& eqc_usort_count,
                            Node v,
                            std::map<Node, bool>& visited);
  //---------------------------------end for debugging finite model finding

}; /* class TheoryEngineModelBuilder */

} /* CVC4::theory namespace */
} /* CVC4 namespace */

#endif /* CVC4__THEORY__THEORY_MODEL_BUILDER_H */
