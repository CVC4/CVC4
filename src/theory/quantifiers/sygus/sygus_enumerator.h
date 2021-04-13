/******************************************************************************
 * Top contributors (to current version):
 *   Andrew Reynolds, Mathias Preiner
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * sygus_enumerator
 */

#include "cvc4_private.h"

#ifndef CVC5__THEORY__QUANTIFIERS__SYGUS_ENUMERATOR_H
#define CVC5__THEORY__QUANTIFIERS__SYGUS_ENUMERATOR_H

#include <map>
#include <unordered_set>
#include "expr/node.h"
#include "expr/type_node.h"
#include "theory/quantifiers/sygus/synth_conjecture.h"
#include "theory/quantifiers/sygus/term_database_sygus.h"

namespace cvc5 {
namespace theory {
namespace quantifiers {

class SynthConjecture;
class SygusPbe;

/** SygusEnumerator
 *
 * This class is used for enumerating all terms of a sygus datatype type. At
 * a high level, it is used as an alternative approach to sygus datatypes
 * solver as a candidate generator in a synthesis loop. It filters terms based
 * on redundancy criteria, for instance, it does not generate two terms whose
 * builtin terms (TermDb::sygusToBuiltin) can be shown to be equivalent via
 * rewriting. It enumerates terms in order of sygus term size
 * (TermDb::getSygusTermSize).
 *
 * It also can be configured to enumerates sygus terms with free variables,
 * (as opposed to variables bound in the formal arguments list of the
 * function-to-synthesize), where each free variable appears in exactly one
 * subterm. For grammar:
 *   S -> 0 | 1 | x | S+S
 * this enumerator will generate the stream:
 *   z1, C_0, C_1, C_x, C_+(z1, z2), C_+(z1, C_1), C_+(C_1, C_1) ...
 * and so on, where z1 and z2 are variables of sygus datatype type S. We call
 * these "shapes". This feature can be enabled by setting enumShapes to true
 * in the constructor below.
 */
class SygusEnumerator : public EnumValGenerator
{
 public:
  SygusEnumerator(TermDbSygus* tds,
                  SynthConjecture* p,
                  SygusStatistics& s,
                  bool enumShapes = false);
  ~SygusEnumerator() {}
  /** initialize this class with enumerator e */
  void initialize(Node e) override;
  /** Inform this generator that abstract value v was enumerated. */
  void addValue(Node v) override;
  /** increment */
  bool increment() override;
  /** Get the next concrete value generated by this class. */
  Node getCurrent() override;
  /** Are we enumerating shapes? */
  bool isEnumShapes() const;

 private:
  /** pointer to term database sygus */
  TermDbSygus* d_tds;
  /** pointer to the synth conjecture that owns this enumerator */
  SynthConjecture* d_parent;
  /** reference to the statistics of parent */
  SygusStatistics& d_stats;
  /** Whether we are enumerating shapes */
  bool d_enumShapes;
  /** Term cache
   *
   * This stores a list of terms for a given sygus type. The key features of
   * this data structure are that terms are stored in order of size,
   * indices can be recorded that indicate where terms of size n begin for each
   * natural number n, and redundancy criteria are used for discarding terms
   * that are not relevant. This includes discarding terms whose builtin version
   * is the same up to T-rewriting with another, or is equivalent under
   * examples, if the conjecture in question is in examples form and
   * sygusSymBreakPbe is enabled.
   *
   * This class also computes static information about sygus types that is
   * relevant for enumeration. Primarily, this includes mapping constructors
   * to "constructor classes". Two sygus constructors can be placed in the same
   * constructor class if their constructor weight is equal, and the tuple
   * of their argument types are the same. For example, for:
   *   A -> A+B | A-B | A*B | A+A | A | x
   * The first three constructors above can be placed in the same constructor
   * class, assuming they have identical weights. Constructor classes are used
   * as an optimization when enumerating terms, since they expect the same
   * tuple of argument terms for constructing a term of a fixed size.
   *
   * Constructor classes are allocated such that the constructor weights are
   * in ascending order. This allows us to avoid constructors whose weight
   * is greater than n while we are trying to enumerate terms of sizes strictly
   * less than n.
   *
   * Constructor class 0 is reserved for nullary operators with weight 0. This
   * is an optimization so that such constructors can be skipped for sizes
   * greater than 0, since we know all terms generated by these constructors
   * have size 0.
   */
  class TermCache
  {
   public:
    TermCache();
    /** initialize this cache */
    void initialize(SygusStatistics* s,
                    Node e,
                    TypeNode tn,
                    TermDbSygus* tds,
                    ExampleEvalCache* ece = nullptr);
    /** get last constructor class index for weight
     *
     * This returns a minimal index n such that all constructor classes at
     * index < n have weight at most w.
     */
    unsigned getLastConstructorClassIndexForWeight(unsigned w) const;
    /** get num constructor classes */
    unsigned getNumConstructorClasses() const;
    /** get the constructor indices for constructor class i */
    void getConstructorClass(unsigned i, std::vector<unsigned>& cclass) const;
    /** get argument types for constructor class i */
    void getTypesForConstructorClass(unsigned i,
                                     std::vector<TypeNode>& types) const;
    /** get constructor weight for constructor class i */
    unsigned getWeightForConstructorClass(unsigned i) const;

    /**
     * Add sygus term n to this cache, return true if the term was unique based
     * on the redundancy criteria used by this class.
     */
    bool addTerm(Node n);
    /**
     * Indicate to this cache that we are finished enumerating terms of the
     * current size.
     */
    void pushEnumSizeIndex();
    /** Get the current size of terms that we are enumerating */
    unsigned getEnumSize() const;
    /** get the index at which size s terms start, where s <= getEnumSize() */
    unsigned getIndexForSize(unsigned s) const;
    /** get the index^th term successfully added to this cache */
    Node getTerm(unsigned index) const;
    /** get the number of terms successfully added to this cache */
    unsigned getNumTerms() const;
    /** are we finished enumerating terms? */
    bool isComplete() const;
    /** set that we are finished enumerating terms */
    void setComplete();

   private:
    /** reference to the statistics of parent */
    SygusStatistics* d_stats;
    /** the enumerator this cache is for */
    Node d_enum;
    /** the sygus type of terms in this cache */
    TypeNode d_tn;
    /** pointer to term database sygus */
    TermDbSygus* d_tds;
    /**
     * Pointer to the example evaluation cache utility (used for symmetry
     * breaking).
     */
    ExampleEvalCache* d_eec;
    //-------------------------static information about type
    /** is d_tn a sygus type? */
    bool d_isSygusType;
    /** number of constructor classes */
    unsigned d_numConClasses;
    /** Map from weights to the starting constructor class for that weight. */
    std::map<unsigned, unsigned> d_weightToCcIndex;
    /** Information for each constructor class */
    class ConstructorClass
    {
     public:
      ConstructorClass() : d_weight(0) {}
      ~ConstructorClass() {}
      /** The indices of the constructors in this constructor class */
      std::vector<unsigned> d_cons;
      /** The argument types of the constructor class */
      std::vector<TypeNode> d_types;
      /** Constructor weight */
      unsigned d_weight;
    };
    std::map<unsigned, ConstructorClass> d_cclass;
    /** constructor to indices */
    std::map<unsigned, std::vector<unsigned>> d_cToCIndices;
    //-------------------------end static information about type

    /** the list of sygus terms we have enumerated */
    std::vector<Node> d_terms;
    /** the set of builtin terms corresponding to the above list */
    std::unordered_set<Node, NodeHashFunction> d_bterms;
    /**
     * The index of first term whose size is greater than or equal to that size,
     * if it exists.
     */
    std::map<unsigned, unsigned> d_sizeStartIndex;
    /** the maximum size of terms we have stored in this cache so far */
    unsigned d_sizeEnum;
    /** whether this term cache is complete */
    bool d_isComplete;
    /** sampler (for --sygus-rr-verify) */
    quantifiers::SygusSampler d_samplerRrV;
    /** is the above sampler initialized? */
    bool d_sampleRrVInit;
  };
  /** above cache for each sygus type */
  std::map<TypeNode, TermCache> d_tcache;
  /** initialize term cache for type tn */
  void initializeTermCache(TypeNode tn);

  /** virtual class for term enumerators */
  class TermEnum
  {
   public:
    TermEnum();
    virtual ~TermEnum() {}
    /** get the current size of terms we are enumerating */
    unsigned getCurrentSize();
    /** get the current term of the enumerator */
    virtual Node getCurrent() = 0;
    /** increment the enumerator, return false if the enumerator is finished */
    virtual bool increment() = 0;

   protected:
    /** pointer to the sygus enumerator class */
    SygusEnumerator* d_se;
    /** the (sygus) type of terms we are enumerating */
    TypeNode d_tn;
    /** the current size of terms we are enumerating */
    unsigned d_currSize;
  };
  class TermEnumMaster;
  /** A "slave" enumerator
   *
   * A slave enumerator simply iterates over an index in a given term cache,
   * and relies on a pointer to a "master" enumerator to generate new terms
   * whenever necessary.
   *
   * This class maintains the following invariants, for tc=d_se->d_tcache[d_tn]:
   * (1) d_index < tc.getNumTerms(),
   * (2) d_currSize is the term size of tc.getTerm( d_index ),
   * (3) d_hasIndexNextEnd is (d_currSize < tc.getEnumSize()),
   * (4) If d_hasIndexNextEnd is true, then
   *       d_indexNextEnd = tc.getIndexForSize(d_currSize+1), and
   *       d_indexNextEnd > d_index.
   *
   * Intuitively, these invariants say (1) d_index is a valid index in the
   * term cache, (2) d_currSize is the sygus term size of getCurrent(), (3)
   * d_hasIndexNextEnd indicates whether d_indexNextEnd is valid, and (4)
   * d_indexNextEnd is the index in the term cache where terms of the current
   * size end, if such an index exists.
   */
  class TermEnumSlave : public TermEnum
  {
   public:
    TermEnumSlave();
    /**
     * Initialize this enumerator to enumerate terms of type tn whose size is in
     * the range [sizeMin, sizeMax], inclusive. If this function returns true,
     * then getCurrent() will return the first term in the stream, which is
     * non-empty. Further terms are generated by increment()/getCurrent().
     */
    bool initialize(SygusEnumerator* se,
                    TypeNode tn,
                    unsigned sizeMin,
                    unsigned sizeMax);
    /** get the current term of the enumerator */
    Node getCurrent() override;
    /** increment the enumerator */
    bool increment() override;

   private:
    /** the maximum size of terms this enumerator should enumerate */
    unsigned d_sizeLim;
    /** the current index in the term cache we are considering */
    unsigned d_index;
    /** the index in the term cache where terms of the current size end */
    unsigned d_indexNextEnd;
    /** whether d_indexNextEnd refers to a valid index */
    bool d_hasIndexNextEnd;
    /** pointer to the master enumerator of type d_tn */
    TermEnum* d_master;
    /** validate invariants on d_index, d_indexNextEnd, d_hasIndexNextEnd */
    bool validateIndex();
    /** validate invariants on  d_indexNextEnd, d_hasIndexNextEnd  */
    void validateIndexNextEnd();
  };
  /** Class for "master" enumerators
   *
   * This enumerator is used to generate new terms of a given type d_tn. It
   * generates all terms of type d_tn that are not redundant based on the
   * current criteria.
   *
   * To enumerate terms, this class performs the following steps as necessary
   * during a call to increment():
   * - Fix the size of terms "d_currSize" we are currently enumerating,
   * - Set the constructor class "d_consClassNum" whose constructors are the top
   * symbol of the current term we are constructing. All constructors from this
   * class have the same weight, which we store in d_ccWeight,
   * - Initialize the current children "d_children" so that the sum of their
   * current sizes is exactly (d_currSize - d_ccWeight),
   * - Increment the current set of children until a new tuple of terms is
   * considered,
   * - Set the constructor "d_consNum" from within the constructor class,
   * - Build the current term and check whether it is not redundant according
   * to the term cache of its type.
   *
   * We say this enumerator is active if initialize(...) returns true, and the
   * last call (if any) to increment returned true.
   *
   * This class maintains the following invariants, for tc=d_se->d_tcache[d_tn],
   * if it is active:
   * (1) getCurrent() is tc.getTerm(tc.getNumTerms()-1),
   * (2) tc.pushEnumSizeIndex() has been called by this class exactly
   * getCurrentSize() times.
   * In other words, (1) getCurrent() is always the last term in the term cache
   * of the type of this enumerator tc, and (2) the size counter of tc is in
   * sync with the current size of this enumerator, that is, the master
   * enumerator is responsible for communicating size boundaries to its term
   * cache.
   */
  class TermEnumMaster : public TermEnum
  {
   public:
    TermEnumMaster();
    /**
     * Initialize this enumerator to enumerate (all) terms of type tn, modulo
     * the redundancy criteria used by this class.
     */
    bool initialize(SygusEnumerator* se, TypeNode tn);
    /** get the current term of the enumerator */
    Node getCurrent() override;
    /** increment the enumerator
     *
     * Returns true if there are more terms to enumerate, in which case a
     * subsequent call to getCurrent() returns the next enumerated term. This
     * method returns false if the last call to increment() has yet to
     * terminate. This can happen for recursive datatypes where a slave
     * enumerator may request that this class increment the next term. We avoid
     * an infinite loop by guarding this with the d_isIncrementing flag and
     * return false.
     */
    bool increment() override;

   private:
    /** pointer to term database sygus */
    TermDbSygus* d_tds;
    /** are we enumerating shapes? */
    bool d_enumShapes;
    /** have we initialized the shape enumeration? */
    bool d_enumShapesInit;
    /** are we currently inside a increment() call? */
    bool d_isIncrementing;
    /** cache for getCurrent() */
    Node d_currTerm;
    /** is d_currTerm set */
    bool d_currTermSet;
    //----------------------------- current constructor class information
    /** the next constructor class we are using */
    unsigned d_consClassNum;
    /** the constructors in the current constructor class */
    std::vector<unsigned> d_ccCons;
    /** the types of the current constructor class */
    std::vector<TypeNode> d_ccTypes;
    /** the operator weight for the constructor class */
    unsigned d_ccWeight;
    //----------------------------- end current constructor class information
    /** If >0, 1 + the index in d_ccCons we are considering */
    unsigned d_consNum;
    /** the child enumerators for this enumerator */
    std::map<unsigned, TermEnumSlave> d_children;
    /** the current sum of current sizes of the enumerators in d_children */
    unsigned d_currChildSize;
    /**
     * The number of indices, starting from zero, in d_children that point to
     * initialized slave enumerators.
     */
    unsigned d_childrenValid;
    /** initialize children
     *
     * Initialize all the remaining uninitialized children of this enumerator.
     * If this method returns true, then each of d_children are
     * initialized to be slave enumerators of the argument types indicated by
     * d_ccTypes, and the sum of their current sizes (d_currChildSize) is
     * exactly (d_currSize - d_ccWeight). If this method returns false, then
     * it was not possible to initialize the children such that they meet the
     * size requirements, and such that the assignments from children to size
     * has not already been considered. In this case, all updates to d_children
     * are reverted and d_currChildSize and d_childrenValid are reset to their
     * values prior to this call.
     */
    bool initializeChildren();
    /** initialize child
     *
     * Initialize child i to enumerate terms whose size is at least sizeMin,
     * and whose maximum size is the largest size such that we can still
     * construct terms for the given constructor class and the current children
     * whose size is not more than the current size d_currSize. Additionally,
     * if i is the last child, we modify sizeMin such that it is exactly the
     * size of terms needed for the children to sum up to size d_currSize.
     */
    bool initializeChild(unsigned i, unsigned sizeMin);
    /** increment internal, helper for increment() */
    bool incrementInternal();
    /**
     * The vector children is a set of terms given to
     *    NodeManager::mkNode(APPLY_CONSTRUCTOR, children)
     * This converts children so that all sygus free variables are unique. Note
     * that the first child is a constructor operator and should be skipped.
     */
    void childrenToShape(std::vector<Node>& children);
    /**
     * Convert n into shape based on the variable counters. For example if
     * vcounter is { Int -> 7 }, then (+ x1 x2) is converted to (+ x7 x8) and
     * vouncter is updated to { Int -> 9 }.
     */
    Node convertShape(Node n, std::map<TypeNode, int>& vcounter);
  };
  /** an interpreted value enumerator
   *
   * This enumerator uses the builtin type enumerator for a given type. It
   * is used to fill in concrete holes into "any constant" constructors
   * when sygus-repair-const is not enabled. The number of terms of size n
   * is m^n, where m is configurable via --sygus-active-gen-enum-cfactor=m.
   */
  class TermEnumMasterInterp : public TermEnum
  {
   public:
    TermEnumMasterInterp(TypeNode tn);
    /** initialize this enumerator */
    bool initialize(SygusEnumerator* se, TypeNode tn);
    /** get the current term of the enumerator */
    Node getCurrent() override;
    /** increment the enumerator */
    bool increment() override;

   private:
    /** the type enumerator */
    TypeEnumerator d_te;
    /** the current number of terms we are enumerating for the given size */
    unsigned d_currNumConsts;
    /** the next end threshold */
    unsigned d_nextIndexEnd;
  };
  /** a free variable enumerator
   *
   * This enumerator enumerates canonical free variables for a given type.
   * The n^th variable in this stream is assigned size n. This enumerator is
   * used in conjunction with sygus-repair-const to generate solutions with
   * constant holes. In this approach, free variables are arguments to
   * any-constant constructors. This is required so that terms with holes are
   * unique up to rewriting when appropriate.
   */
  class TermEnumMasterFv : public TermEnum
  {
   public:
    TermEnumMasterFv();
    /** initialize this enumerator */
    bool initialize(SygusEnumerator* se, TypeNode tn);
    /** get the current term of the enumerator */
    Node getCurrent() override;
    /** increment the enumerator */
    bool increment() override;
  };
  /** the master enumerator for each sygus type */
  std::map<TypeNode, TermEnumMaster> d_masterEnum;
  /** the master enumerator for each non-sygus type */
  std::map<TypeNode, TermEnumMasterFv> d_masterEnumFv;
  /** the master enumerator for each non-sygus type */
  std::map<TypeNode, std::unique_ptr<TermEnumMasterInterp>> d_masterEnumInt;
  /** the sygus enumerator this class is for */
  Node d_enum;
  /** the type of d_enum */
  TypeNode d_etype;
  /** pointer to the master enumerator of type d_etype */
  TermEnum* d_tlEnum;
  /** the abort size, caches the value of --sygus-abort-size */
  int d_abortSize;
  /** get master enumerator for type tn */
  TermEnum* getMasterEnumForType(TypeNode tn);
  //-------------------------------- externally specified symmetry breaking
  /** set of constructors we disallow at top level
   *
   * A constructor C is disallowed at the top level if a symmetry breaking
   * lemma that entails ~is-C( d_enum ) was registered to
   * TermDbSygus::registerSymBreakLemma.
   */
  std::unordered_set<Node, NodeHashFunction> d_sbExcTlCons;
  //-------------------------------- end externally specified symmetry breaking
};

}  // namespace quantifiers
}  // namespace theory
}  // namespace cvc5

#endif /* CVC5__THEORY__QUANTIFIERS__SYGUS_ENUMERATOR_H */
