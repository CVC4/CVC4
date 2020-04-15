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
 ** \brief Term registry for the theory of strings.
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__STRINGS__TERM_REGISTRY_H
#define CVC4__THEORY__STRINGS__TERM_REGISTRY_H

#include "context/cdhashset.h"
#include "context/cdlist.h"
#include "theory/strings/skolem_cache.h"
#include "theory/uf/equality_engine.h"

namespace CVC4 {
namespace theory {
namespace strings {

/** 
 * This class manages all the (pre)registration tasks for terms. These tasks
 * include:
 * (1) Constructing preregistration lemmas for terms,
 * (2) Add terms to the equality engine,
 * (3) Maintaining a list of terms (for theory combination).
 */
class TermRegistry
{
  typedef context::CDHashSet<Node, NodeHashFunction> NodeSet;
  typedef context::CDHashSet<TypeNode, TypeNodeHashFunction> TypeNodeSet;
 public:
  TermRegistry(context::Context* c, context::UserContext* u, eq::EqualityEngine& ee,
                   OutputChannel& out,
                   SequencesStatistics& statistics);
  ~TermRegistry();
  /** 
   * Preregister term, called when TheoryStrings::preRegisterTerm(n) is called.
   */
  void preRegisterTerm(TNode n);
  /** Register term
   *
   * This performs SAT-context-independent registration for a term n, which
   * may cause lemmas to be sent on the output channel that involve
   * "initial refinement lemmas" for n. This includes introducing proxy
   * variables for string terms and asserting that str.code terms are within
   * proper bounds.
   *
   * Effort is one of the following (TODO make enum #1881):
   * 0 : upon preregistration or internal assertion
   * 1 : upon occurrence in length term
   * 2 : before normal form computation
   * 3 : called on normal form terms
   *
   * Based on the strategy, we may choose to add these initial refinement
   * lemmas at one of the following efforts, where if it is not the given
   * effort, the call to this method does nothing.
   */
  void registerTerm(Node n, int effort);
  /** register term
    *
    * This method is called on non-constant string terms n. It returns a lemma
    * that should be sent on the output channel of theory of strings upon
    * registration of this term, or null if no lemma is necessary.
    *
    * If n is an atomic term, the method registerTermAtomic is called for n
    * and s = LENGTH_SPLIT and no lemma is returned.
    */
  Node registerTerm(Node n);
  /** register length
    *
    * This method is called on non-constant string terms n that are "atomic"
    * with respect to length. That is, the rewritten form of len(n) is itself.
    *
    * It sends a lemma on the output channel that ensures that the length n
    * satisfies its assigned status (given by argument s).
    *
    * If the status is LENGTH_ONE, we send the lemma len( n ) = 1.
    *
    * If the status is LENGTH_GEQ, we send a lemma n != "" ^ len( n ) > 0.
    *
    * If the status is LENGTH_SPLIT, we send a send a lemma of the form:
    *   ( n = "" ^ len( n ) = 0 ) OR len( n ) > 0
    * This method also ensures that, when applicable, the left branch is taken
    * first via calls to requirePhase.
    *
    * If the status is LENGTH_IGNORE, then no lemma is sent. This status is used
    * e.g. when the length of n is already implied by other constraints.
    *
    * In contrast to the above functions, it makes immediate calls to the output
    * channel instead of adding them to pending lists.
    */
  void registerTermAtomic(Node n, LengthStatus s);
  /** Get the skolem cache of this object */
  SkolemCache* getSkolemCache();
  //---------------------------- proxy variables and length elaboration
  /** Get symbolic definition
   *
   * This method returns the "symbolic definition" of n, call it n', and
   * populates the vector exp with an explanation such that exp => n = n'.
   *
   * The symbolic definition of n is the term where (maximal) subterms of n
   * are replaced by their proxy variables. For example, if we introduced
   * proxy variable v for x ++ y, then given input x ++ y = w, this method
   * returns v = w and adds v = x ++ y to exp.
   */
  Node getSymbolicDefinition(Node n, std::vector<Node>& exp) const;
  /** Get proxy variable
   *
   * If this method returns the proxy variable for (string) term n if one
   * exists, otherwise it returns null.
   */
  Node getProxyVariableFor(Node n) const;
  //---------------------------- end proxy variables and length elaboration
 private:
  /** Reference to equality engine of the theory of strings. */
  eq::EqualityEngine& d_ee;
  /** Reference to the output channel of the theory of strings. */
  OutputChannel& d_out;
  /** Reference to the statistics for the theory of strings/sequences. */
  SequencesStatistics& d_statistics;
  /** The cache of all skolems, which is owned by this class. */
  SkolemCache d_sk_cache;
  /** All function terms that the theory has seen in the current SAT context */
  context::CDList<TNode> d_functionsTerms;
  /** The types that we have preregistered */
  TypeNodeSet d_registeredTypesCache;
  /** The user-context dependent cache of terms that have been preregistered */
  NodeSet d_pregistered_terms_cache;
  /** The user-context dependent cache of terms that have been registered */
  NodeSet d_registered_terms_cache;
  /**
   * Map string terms to their "proxy variables". Proxy variables are used are
   * intermediate variables so that length information can be communicated for
   * constants. For example, to communicate that "ABC" has length 3, we
   * introduce a proxy variable v_{"ABC"} for "ABC", and assert:
   *   v_{"ABC"} = "ABC" ^ len( v_{"ABC"} ) = 3
   * Notice this is required since we cannot directly write len( "ABC" ) = 3,
   * which rewrites to 3 = 3.
   * In the above example, we store "ABC" -> v_{"ABC"} in this map.
   */
  NodeNodeMap d_proxyVar;
  /**
   * Map from proxy variables to their normalized length. In the above example,
   * we store "ABC" -> 3.
   */
  NodeNodeMap d_proxyVarToLength;
  /** List of terms that we have register length for */
  NodeSet d_lengthLemmaTermsCache;
  /** Register type
   *
   * Ensures the theory solver is setup to handle string-like type tn. In
   * particular, this includes:
   * - Calling preRegisterTerm on the empty word for tn
   */
  void registerType(TypeNode tn);
  /** infer substitution proxy vars
   *
   * This method attempts to (partially) convert the formula n into a
   * substitution of the form:
   *   v1 -> s1, ..., vn -> sn
   * where s1 ... sn are proxy variables and v1 ... vn are either variables
   * or constants.
   *
   * This method ensures that P ^ v1 = s1 ^ ... ^ vn = sn ^ unproc is equivalent
   * to P ^ n, where P is the conjunction of equalities corresponding to the
   * definition of all proxy variables introduced by the theory of strings.
   *
   * For example, say that v1 was introduced as a proxy variable for "ABC", and
   * v2 was introduced as a proxy variable for "AA".
   *
   * Given the input n := v1 = "ABC" ^ v2 = x ^ x = "AA", this method sets:
   * vars = { x },
   * subs = { v2 },
   * unproc = {}.
   * In particular, this says that the information content of n is essentially
   * x = v2. The first and third conjunctions can be dropped from the
   * explanation since these equalities simply correspond to definitions
   * of proxy variables.
   *
   * This method is used as a performance heuristic. It can infer when the
   * explanation of a fact depends only trivially on equalities corresponding
   * to definitions of proxy variables, which can be omitted since they are
   * assumed to hold globally.
   */
  void inferSubstitutionProxyVars(Node n,
                                  std::vector<Node>& vars,
                                  std::vector<Node>& subs,
                                  std::vector<Node>& unproc) const;
};

}/* CVC4::theory::strings namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* CVC4__THEORY__STRINGS__TERM_REGISTRY_H */
