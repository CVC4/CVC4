/********************                                                        */
/*! \file sygus_enumerator.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief sygus_enumerator
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__QUANTIFIERS__SYGUS_ENUMERATOR_H
#define __CVC4__THEORY__QUANTIFIERS__SYGUS_ENUMERATOR_H

#include <map>
#include <unordered_set>
#include "expr/node.h"
#include "expr/type_node.h"
#include "theory/quantifiers/sygus/synth_conjecture.h"
#include "theory/quantifiers/sygus/term_database_sygus.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

class SynthConjecture;
class SygusPbe;

/** SygusEnumerator
 *
 */
class SygusEnumerator : public EnumValGenerator
{
 public:
  SygusEnumerator(TermDbSygus* tds, SynthConjecture* p);
  ~SygusEnumerator() {}
  /** initialize this class with enumerator e */
  void initialize(Node e) override;
  /** Inform this generator that abstract value v was enumerated. */
  void addValue(Node v) override;
  /** Get the next concrete value generated by this class. */
  Node getNext() override;

 private:
  /** pointer to term database sygus */
  TermDbSygus* d_tds;
  /** pointer to the synth conjecture that owns this enumerator */
  SynthConjecture* d_parent;
  /** Term cache
   *
   * This stores a list of terms for a given sygus type. The key features of
   * this data structure are that terms are stored in order of size,
   * indices can be recorded that indicate where terms of size n begin for each
   * natural number n, and redundancy criteria are used for discarding terms
   * that are not relevant. This includes discarding terms whose builtin version
   * is the same up to T-rewriting with another, or is equivalent under
   * examples, if the conjecture in question is in PBE form and sygusSymBreakPbe
   * is enabled.
   *
   * This class also computes static information about sygus types that is
   * relevant for enumeration. Primarily, this includes mapping constructors
   * to "constructor classes". Two sygus constructors can be placed in the same
   * constructor class if their constructor weight is equal, and the multisets
   * of their argument types are the same. For example, for:
   *   A -> A+B | B-A | B+A | A+A | A | x
   * The first three constructors above can be placed in the same constructor
   * class, assuming they have identical weights. Constructor classes are used
   * as an optimization when enumerating terms, since they expect the same
   * tuples of argument terms for constructing a term of a fixed size.
   */
  class TermCache
  {
   public:
    TermCache();
    /** initialize this cache */
    void initialize(Node e, TypeNode tn, TermDbSygus* tds, SygusPbe* pbe=nullptr);
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
    /** get child indices for constructor
     * 
     * These are the argument indices of constructor i with respect to
     * the argument types of its constructor class. Add these to indices.
     * 
     * Let types be the argument types for the constructor class of constructor
     * i. Then, we have that the j^th argument of constructor i has type
     * types[indices[j]].
     * 
     * For example, for:
     *   A -> A+B | B-A | ...
     * Assume we have that these constructors are in the same constructor class
     * n, and getTypesForConstructorClass(n,types) returns types = { A, B }.
     * We have that getChildIndicesForConstructor(i,indices) returns 
     * indices = { 0, 1 } for i=0 and indices = { 1, 0 } for i=1.
     */
    void getChildIndicesForConstructor(unsigned i,
                                       std::vector<unsigned>& cindices) const;

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

   private:
    /** the enumerator this cache is for */
    Node d_enum;
    /** the sygus type of terms in this cache */
    TypeNode d_tn;
    /** pointer to term database sygus */
    TermDbSygus* d_tds;
    /** pointer to the PBE utility (used for symmetry breaking) */
    SygusPbe* d_pbe;
    //-------------------------static information about type
    /** is d_tn a sygus type? */
    bool d_isSygusType;
    /** number of constructor classes */
    unsigned d_numConClasses;
    /** Map from weights to the starting constructor class for that weight. */
    std::map<unsigned, unsigned> d_weightToCcIndex;
    /** constructor classes */
    std::map<unsigned, std::vector<unsigned>> d_ccToCons;
    /** maps constructor classes to children types */
    std::map<unsigned, std::vector<TypeNode>> d_ccToTypes;
    /** maps constructor classes to constructor weight */
    std::map<unsigned, unsigned> d_ccToWeight;
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
    /** increment the enumerator */
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
   */
  class TermEnumSlave : public TermEnum
  {
   public:
    TermEnumSlave();
    /** initialize this enumerator */
    bool initialize(SygusEnumerator* se,
                    TypeNode tn,
                    unsigned sizeMin,
                    unsigned sizeMax);
    /** get the current term of the enumerator */
    Node getCurrent() override;
    /** increment the enumerator */
    bool increment() override;
   private:
    //------------------------------------------- for non-master enumerators
    /** the maximum size of terms this enumerator should enumerator */
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
    //------------------------------------------- end for non-master enumerators
  };
  /** Class for "master" enumerators 
   * 
   */
  class TermEnumMaster : public TermEnum
  {
   public:
    TermEnumMaster();
    /** initialize this enumerator */
    bool initialize(SygusEnumerator* se, TypeNode tn);
    /** get the current term of the enumerator */
    Node getCurrent() override;
    /** increment the enumerator */
    bool increment() override;
   private:
    /** are we currently inside a increment() call? */
    bool d_isIncrementing;
    /** the last term we enumerated */
    Node d_currTerm;
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
    /** the current sum of child sizes */
    unsigned d_currChildSize;
    /** children valid */
    unsigned d_childrenValid;
    /** the last enumerated term size */
    unsigned d_lastSize;
    /** initialize children */
    bool initializeChildren();
    /** initialize child */
    bool initializeChild(unsigned i, unsigned sizeMin);
    /** increment internal */
    bool incrementInternal();
  };
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
  };
  /** the master enumerator for each sygus type */
  std::map<TypeNode, TermEnumMaster> d_masterEnum;
  /** the master enumerator for each non-sygus type */
  std::map<TypeNode, std::unique_ptr<TermEnumMasterInterp>> d_masterEnumInt;
  /** the enumerator this class is for */
  Node d_enum;
  /** the top-level type */
  TypeNode d_etype;
  /** pointer to the top-level enumerator */
  TermEnum* d_tlEnum;
  /** abort size */
  int d_abortSize;
  /** get master enumerator for type */
  TermEnum* getMasterEnumForType(TypeNode tn);
};

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC4

#endif /* __CVC4__THEORY__QUANTIFIERS__SYGUS_ENUMERATOR_H */
