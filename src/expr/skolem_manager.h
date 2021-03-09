/*********************                                                        */
/*! \file skolem_manager.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Skolem manager utility
 **/

#include "cvc4_private.h"

#ifndef CVC4__EXPR__SKOLEM_MANAGER_H
#define CVC4__EXPR__SKOLEM_MANAGER_H

#include <string>

#include "expr/node.h"

namespace CVC4 {

class ProofGenerator;

/**
 * A manager for skolems that can be used in proofs. This is designed to be
 * a trusted interface to NodeManager::mkSkolem, where one
 * must provide a definition for the skolem they create in terms of a
 * predicate that the introduced variable is intended to witness.
 *
 * It is implemented by mapping terms to an attribute corresponding to their
 * "witness form" as described below. Hence, this class does not impact the
 * reference counting of skolem variables which may be deleted if they are not
 * used.
 */
class SkolemManager
{
 public:
  SkolemManager() {}
  ~SkolemManager() {}
  /**
   * This makes a skolem of same type as bound variable v, (say its type is T),
   * whose definition is (witness ((v T)) pred). This definition is maintained
   * by this class.
   *
   * Notice that (exists ((v T)) pred) should be a valid formula. This fact
   * captures the reason for why the returned Skolem was introduced.
   *
   * Take as an example extensionality in arrays:
   *
   * (declare-fun a () (Array Int Int))
   * (declare-fun b () (Array Int Int))
   * (assert (not (= a b)))
   *
   * To witness the index where the arrays a and b are disequal, it is intended
   * we call this method on:
   *   Node k = mkSkolem( x, F )
   * where F is:
   *   (=> (not (= a b)) (not (= (select a x) (select b x))))
   * and x is a fresh bound variable of integer type. Internally, this will map
   * k to the term:
   *   (witness ((x Int)) (=> (not (= a b))
   *                          (not (= (select a x) (select b x)))))
   * A lemma generated by the array solver for extensionality may safely use
   * the skolem k in the standard way:
   *   (=> (not (= a b)) (not (= (select a k) (select b k))))
   * Furthermore, notice that the following lemma does not involve fresh
   * skolem variables and is valid according to the theory of arrays extended
   * with support for witness:
   *   (let ((w (witness ((x Int)) (=> (not (= a b))
   *                                   (not (= (select a x) (select b x)))))))
   *     (=> (not (= a b)) (not (= (select a w) (select b w)))))
   * This version of the lemma, which requires no explicit tracking of free
   * Skolem variables, can be obtained by a call to getWitnessForm(...)
   * below. We call this the "witness form" of the lemma above.
   *
   * @param v The bound variable of the same type of the Skolem to create.
   * @param pred The desired property of the Skolem to create, in terms of bound
   * variable v.
   * @param prefix The prefix of the name of the Skolem
   * @param comment Debug information about the Skolem
   * @param flags The flags for the Skolem (see NodeManager::mkSkolem)
   * @param pg The proof generator for this skolem. If non-null, this proof
   * generator must respond to a call to getProofFor(exists v. pred) during
   * the lifetime of the current node manager.
   * @param retWitness Whether we wish to return the witness term for the
   * given Skolem, which notice is of the form (witness v. pred), where pred
   * is in Skolem form. A typical use case of setting this flag to true
   * is preprocessing passes that eliminate terms. Using a witness term
   * instead of its corresponding Skolem indicates that the body of the witness
   * term needs to be added as an assertion, e.g. by the term formula remover.
   * @return The skolem whose witness form is registered by this class.
   */
  Node mkSkolem(Node v,
                Node pred,
                const std::string& prefix,
                const std::string& comment = "",
                int flags = NodeManager::SKOLEM_DEFAULT,
                ProofGenerator* pg = nullptr,
                bool retWitness = false);
  /**
   * Make skolemized form of existentially quantified formula q, and store its
   * Skolems into the argument skolems.
   *
   * For example, calling this method on:
   *   (exists ((x Int) (y Int)) (P x y))
   * returns:
   *   (P w1 w2)
   * where w1 and w2 are skolems with witness forms:
   *   (witness ((x Int)) (exists ((y' Int)) (P x y')))
   *   (witness ((y Int)) (P w1 y))
   * respectively. Additionally, this method will add { w1, w2 } to skolems.
   * Notice that y is renamed to y' in the witness form of w1 to avoid variable
   * shadowing.
   *
   * In contrast to mkSkolem, the proof generator is for the *entire*
   * existentially quantified formula q, which may have multiple variables in
   * its prefix.
   *
   * @param q The existentially quantified formula to skolemize,
   * @param skolems Vector to add Skolems of q to,
   * @param prefix The prefix of the name of each of the Skolems
   * @param comment Debug information about each of the Skolems
   * @param flags The flags for the Skolem (see NodeManager::mkSkolem)
   * @param pg The proof generator for this skolem. If non-null, this proof
   * generator must respond to a call to getProofFor(q) during
   * the lifetime of the current node manager.
   * @return The skolemized form of q.
   */
  Node mkSkolemize(Node q,
                   std::vector<Node>& skolems,
                   const std::string& prefix,
                   const std::string& comment = "",
                   int flags = NodeManager::SKOLEM_DEFAULT,
                   ProofGenerator* pg = nullptr);
  /**
   * Same as above, but for special case of (witness ((x T)) (= x t))
   * where T is the type of t. This skolem is unique for each t, which we
   * implement via an attribute on t. This attribute is used to ensure to
   * associate a unique skolem for each t.
   *
   * Notice that a purification skolem is trivial to justify, and hence it
   * does not require a proof generator.
   */
  Node mkPurifySkolem(Node t,
                      const std::string& prefix,
                      const std::string& comment = "",
                      int flags = NodeManager::SKOLEM_DEFAULT);
  /**
   * Make Boolean term variable for term t. This is a special case of
   * mkPurifySkolem above, where the returned term has kind
   * BOOLEAN_TERM_VARIABLE.
   */
  Node mkBooleanTermVariable(Node t);
  /**
   * Get proof generator for existentially quantified formula q. This returns
   * the proof generator that was provided in a call to mkSkolem above.
   */
  ProofGenerator* getProofGenerator(Node q) const;
  /**
   * Convert to witness form, where notice this recursively replaces *all*
   * skolems in n by their corresponding witness term. This is intended to be
   * used by the proof checker only.
   *
   * @param n The term or formula to convert to witness form described above
   * @return n in witness form.
   */
  static Node getWitnessForm(Node n);
  /**
   * Convert to Skolem form, which recursively replaces all witness terms in n
   * by their corresponding Skolems.
   *
   * @param n The term or formula to convert to Skolem form described above
   * @return n in Skolem form.
   */
  static Node getSkolemForm(Node n);
  /** convert to witness form vector */
  static void convertToWitnessFormVec(std::vector<Node>& vec);
  /** convert to Skolem form vector */
  static void convertToSkolemFormVec(std::vector<Node>& vec);

 private:
  /**
   * Mapping from witness terms to proof generators.
   */
  std::map<Node, ProofGenerator*> d_gens;
  /** Convert to witness or skolem form */
  static Node convertInternal(Node n, bool toWitness);
  /** Get or make skolem attribute for witness term w */
  static Node getOrMakeSkolem(Node w,
                              const std::string& prefix,
                              const std::string& comment,
                              int flags);
  /**
   * Skolemize the first variable of existentially quantified formula q.
   * For example, calling this method on:
   *   (exists ((x Int) (y Int)) (P x y))
   * will return:
   *   (witness ((x Int)) (exists ((y Int)) (P x y)))
   * If q is not an existentially quantified formula, then null is
   * returned and an assertion failure is thrown.
   *
   * This method additionally updates qskolem to be the skolemized form of q.
   * In the above example, this is set to:
   *   (exists ((y Int)) (P (witness ((x Int)) (exists ((y' Int)) (P x y'))) y))
   */
  Node skolemize(Node q,
                 Node& qskolem,
                 const std::string& prefix,
                 const std::string& comment = "",
                 int flags = NodeManager::SKOLEM_DEFAULT);
};

}  // namespace CVC4

#endif /* CVC4__EXPR__PROOF_SKOLEM_CACHE_H */
