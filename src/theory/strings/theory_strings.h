/*********************                                                        */
/*! \file theory_strings.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Tianyi Liang, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Theory of strings
 **
 ** Theory of strings.
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__STRINGS__THEORY_STRINGS_H
#define CVC4__THEORY__STRINGS__THEORY_STRINGS_H

#include <climits>
#include <deque>

#include "context/cdhashset.h"
#include "context/cdlist.h"
#include "expr/node_trie.h"
#include "theory/strings/base_solver.h"
#include "theory/strings/core_solver.h"
#include "theory/strings/extf_solver.h"
#include "theory/strings/infer_info.h"
#include "theory/strings/inference_manager.h"
#include "theory/strings/normal_form.h"
#include "theory/strings/regexp_elim.h"
#include "theory/strings/regexp_operation.h"
#include "theory/strings/regexp_solver.h"
#include "theory/strings/sequences_stats.h"
#include "theory/strings/solver_state.h"
#include "theory/strings/strategy.h"
#include "theory/strings/strings_fmf.h"
#include "theory/strings/strings_rewriter.h"
#include "theory/strings/term_registry.h"
#include "theory/theory.h"
#include "theory/uf/equality_engine.h"

namespace CVC4 {
namespace theory {
namespace strings {

/**
 * A theory solver for strings. At a high level, the solver implements
 * techniques described in:
 * - Liang et al, CAV 2014,
 * - Reynolds et al, CAV 2017,
 * - Reynolds et al, IJCAR 2020.
 * Its rewriter is described in:
 * - Reynolds et al, CAV 2019.
 */
class TheoryStrings : public Theory {
  friend class InferenceManager;
  typedef context::CDHashSet<Node, NodeHashFunction> NodeSet;
  typedef context::CDHashSet<TypeNode, TypeNodeHashFunction> TypeNodeSet;
 public:
  TheoryStrings(context::Context* c, context::UserContext* u,
                OutputChannel& out, Valuation valuation,
                const LogicInfo& logicInfo);
  ~TheoryStrings();

  TheoryRewriter* getTheoryRewriter() override { return &d_rewriter; }

  void setMasterEqualityEngine(eq::EqualityEngine* eq) override;
  std::string identify() const override { return std::string("TheoryStrings"); }
  void propagate(Effort e) override;
  bool propagate(TNode literal);
  Node explain(TNode literal) override;
  eq::EqualityEngine* getEqualityEngine() override { return &d_equalityEngine; }
  bool getCurrentSubstitution(int effort,
                              std::vector<Node>& vars,
                              std::vector<Node>& subs,
                              std::map<Node, std::vector<Node> >& exp) override;
  /** presolve */
  void presolve() override;
  /** shutdown */
  void shutdown() override {}
  /** add shared term */
  void addSharedTerm(TNode n) override;
  /** get equality status */
  EqualityStatus getEqualityStatus(TNode a, TNode b) override;
  /** preregister term */
  void preRegisterTerm(TNode n) override;
  /** Expand definition */
  Node expandDefinition(Node n) override;
  /** Check at effort e */
  void check(Effort e) override;
  /** needs check last effort */
  bool needsCheckLastEffort() override;
  /** Conflict when merging two constants */
  void conflict(TNode a, TNode b);
  /** called when a new equivalence class is created */
  void eqNotifyNewClass(TNode t);
  /** preprocess rewrite */
  Node ppRewrite(TNode atom) override;
  /**
   * Get all relevant information in this theory regarding the current
   * model. Return false if a contradiction is discovered.
   */
  bool collectModelInfo(TheoryModel* m) override;
  //--------------------------- helper functions
  /** get normal string
   *
   * This method returns the node that is equivalent to the normal form of x,
   * and adds the corresponding explanation to nf_exp.
   *
   * For example, if x = y ++ z is an assertion in the current context, then
   * this method returns the term y ++ z and adds x = y ++ z to nf_exp.
   */
  Node getNormalString(Node x, std::vector<Node>& nf_exp);
  //-------------------------- end helper functions

  // NotifyClass for equality engine
  class NotifyClass : public eq::EqualityEngineNotify {
  public:
   NotifyClass(TheoryStrings& ts) : d_str(ts), d_state(ts.d_state) {}
   bool eqNotifyTriggerEquality(TNode equality, bool value) override
   {
     Debug("strings") << "NotifyClass::eqNotifyTriggerEquality(" << equality
                      << ", " << (value ? "true" : "false") << ")" << std::endl;
     if (value)
     {
       return d_str.propagate(equality);
     }
     else
     {
       // We use only literal triggers so taking not is safe
       return d_str.propagate(equality.notNode());
     }
    }
    bool eqNotifyTriggerPredicate(TNode predicate, bool value) override
    {
      Debug("strings") << "NotifyClass::eqNotifyTriggerPredicate(" << predicate << ", " << (value ? "true" : "false") << ")" << std::endl;
      if (value) {
        return d_str.propagate(predicate);
      } else {
        return d_str.propagate(predicate.notNode());
      }
    }
    bool eqNotifyTriggerTermEquality(TheoryId tag,
                                     TNode t1,
                                     TNode t2,
                                     bool value) override
    {
      Debug("strings") << "NotifyClass::eqNotifyTriggerTermMerge(" << tag << ", " << t1 << ", " << t2 << ")" << std::endl;
      if (value) {
        return d_str.propagate(t1.eqNode(t2));
      } else {
        return d_str.propagate(t1.eqNode(t2).notNode());
      }
    }
    void eqNotifyConstantTermMerge(TNode t1, TNode t2) override
    {
      Debug("strings") << "NotifyClass::eqNotifyConstantTermMerge(" << t1 << ", " << t2 << ")" << std::endl;
      d_str.conflict(t1, t2);
    }
    void eqNotifyNewClass(TNode t) override
    {
      Debug("strings") << "NotifyClass::eqNotifyNewClass(" << t << std::endl;
      d_str.eqNotifyNewClass(t);
    }
    void eqNotifyPreMerge(TNode t1, TNode t2) override
    {
      Debug("strings") << "NotifyClass::eqNotifyPreMerge(" << t1 << ", " << t2 << std::endl;
      d_state.eqNotifyPreMerge(t1, t2);
    }
    void eqNotifyPostMerge(TNode t1, TNode t2) override
    {
      Debug("strings") << "NotifyClass::eqNotifyPostMerge(" << t1 << ", " << t2 << std::endl;
    }
    void eqNotifyDisequal(TNode t1, TNode t2, TNode reason) override
    {
      Debug("strings") << "NotifyClass::eqNotifyDisequal(" << t1 << ", " << t2 << ", " << reason << std::endl;
      d_state.eqNotifyDisequal(t1, t2, reason);
    }

   private:
    /** The theory of strings object to notify */
    TheoryStrings& d_str;
    /** The solver state of the theory of strings */
    SolverState& d_state;
  };/* class TheoryStrings::NotifyClass */

 private:
  /** Commonly used constants */
  Node d_true;
  Node d_false;
  Node d_zero;
  Node d_one;
  Node d_neg_one;
  /** the cardinality of the alphabet */
  uint32_t d_cardSize;
  /** The notify class */
  NotifyClass d_notify;
  /**
   * Statistics for the theory of strings/sequences. All statistics for these
   * theories is collected in this object.
   */
  SequencesStatistics d_statistics;
  /** Equaltity engine */
  eq::EqualityEngine d_equalityEngine;
  /** The solver state object */
  SolverState d_state;
  /** The term registry for this theory */
  TermRegistry d_termReg;
  /** The (custom) output channel of the theory of strings */
  InferenceManager d_im;
  /** The theory rewriter for this theory. */
  StringsRewriter d_rewriter;
  /**
   * The base solver, responsible for reasoning about congruent terms and
   * inferring constants for equivalence classes.
   */
  BaseSolver d_bsolver;
  /**
   * The core solver, responsible for reasoning about string concatenation
   * with length constraints.
   */
  CoreSolver d_csolver;
  /**
   * Extended function solver, responsible for reductions and simplifications
   * involving extended string functions.
   */
  std::unique_ptr<ExtfSolver> d_esolver;
  /** regular expression solver module */
  std::unique_ptr<RegExpSolver> d_rsolver;
  /** regular expression elimination module */
  RegExpElimination d_regexp_elim;
  /** Strings finite model finding decision strategy */
  StringsFmf d_stringsFmf;
  /** The representation of the strategy */
  Strategy d_strat;
  /** compute care graph */
  void computeCareGraph() override;
  /**
   * Are x and y shared terms that are not equal? This is used for constructing
   * the care graph in the above function.
   */
  bool areCareDisequal(TNode x, TNode y);
  /** Add care pairs */
  void addCarePairs(TNodeTrie* t1,
                    TNodeTrie* t2,
                    unsigned arity,
                    unsigned depth);
  /** Collect model info for type tn
   *
   * Assigns model values (in m) to all relevant terms of the string-like type
   * tn in the current context, which are stored in repSet.
   *
   * Returns false if a conflict is discovered while doing this assignment.
   */
  bool collectModelInfoType(
      TypeNode tn,
      const std::unordered_set<Node, NodeHashFunction>& repSet,
      TheoryModel* m);

  /** assert pending fact
   *
   * This asserts atom with polarity to the equality engine of this class,
   * where exp is the explanation of why (~) atom holds.
   *
   * This call may trigger further initialization steps involving the terms
   * of atom, including calls to registerTerm.
   */
  void assertPendingFact(Node atom, bool polarity, Node exp);
  //-----------------------inference steps
  /** check register terms pre-normal forms
   *
   * This calls registerTerm(n,2) on all non-congruent strings in the
   * equality engine of this class.
   */
  void checkRegisterTermsPreNormalForm();
  /** check codes
   *
   * This inference schema ensures that constraints between str.code terms
   * are satisfied by models that correspond to extensions of the current
   * assignment. In particular, this method ensures that str.code can be
   * given an interpretation that is injective for string arguments with length
   * one. It may add lemmas of the form:
   *   str.code(x) == -1 V str.code(x) != str.code(y) V x == y
   */
  void checkCodes();
  /** check register terms for normal forms
   *
   * This calls registerTerm(str.++(t1, ..., tn ), 3) on the normal forms
   * (t1, ..., tn) of all string equivalence classes { s1, ..., sm } such that
   * there does not exist a term of the form str.len(si) in the current context.
   */
  void checkRegisterTermsNormalForms();
  //-----------------------end inference steps
  /** run the given inference step */
  void runInferStep(InferStep s, int effort);
  /** run strategy for effort e */
  void runStrategy(Theory::Effort e);
};/* class TheoryStrings */

}/* CVC4::theory::strings namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* CVC4__THEORY__STRINGS__THEORY_STRINGS_H */
