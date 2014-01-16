/*********************                                                        */
/*! \file abstraction.h
 ** \verbatim
 ** Original author: Liana Hadarean
 ** Major contributors: none.
 ** Minor contributors (to current version): none.
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Bitvector theory.
 **
 ** Bitvector theory.
 **/

#include "cvc4_private.h"
#include <ext/hash_map>
#include <ext/hash_set>
#include "expr/node.h"

#ifndef __CVC4__THEORY__BV__ABSTRACTION_H
#define __CVC4__THEORY__BV__ABSTRACTION_H


namespace CVC4 {
namespace theory {
namespace bv {


typedef std::vector<TNode> ArgsVec;

class AbstractionModule {

  class ArgsTableEntry {
    std::vector<ArgsVec> d_data;
    unsigned d_arity;
  public:
    ArgsTableEntry(unsigned n)
      : d_arity(n)
    {}
    ArgsTableEntry()
      : d_arity(0)
    {}
    void addArguments(const ArgsVec& args);
    typedef std::vector<ArgsVec>::iterator iterator;

    iterator begin() { return d_data.begin(); }
    iterator end() { return d_data.end(); }
    unsigned getArity() { return d_arity; }
    unsigned getNumEntries() { return d_data.size(); }
    ArgsVec& getEntry(unsigned i ) { Assert (i < d_data.size()); return d_data[i]; }
  }; 

  class ArgsTable {
    __gnu_cxx::hash_map<TNode, ArgsTableEntry, TNodeHashFunction > d_data;
    bool hasEntry(TNode signature) const; 
  public:
    typedef __gnu_cxx::hash_map<TNode, ArgsTableEntry, TNodeHashFunction >::iterator iterator;
    ArgsTable() {}
    void addEntry(TNode signature, const ArgsVec& args);
    ArgsTableEntry& getEntry(TNode signature);
    iterator begin() { return d_data.begin(); }
    iterator end() { return d_data.end(); }
  };

  class PatternMatcher {
  public:
    /** 
     * Checks if one pattern is a generalization of the other
     * 
     * @param s 
     * @param t 
     * 
     * @return 1 if s :> t, 2 if s <: t, 0 if they equivalent and -1 if they are incomparable
     */
    static int comparePatterns(TNode s, TNode t);
  }; 
  
  typedef __gnu_cxx::hash_map<Node, std::vector<Node>, NodeHashFunction> NodeVecMap;
  typedef __gnu_cxx::hash_map<Node, TNode, NodeHashFunction> NodeTNodeMap;
  typedef __gnu_cxx::hash_map<TNode, TNode, TNodeHashFunction> TNodeTNodeMap;
  typedef __gnu_cxx::hash_map<Node, Node, NodeHashFunction> NodeNodeMap;
  typedef __gnu_cxx::hash_set<TNode, TNodeHashFunction> TNodeSet;
  typedef __gnu_cxx::hash_map<unsigned, Node> IntNodeMap; 
  typedef std::multiset<Node> NodeMultiSet;
  typedef __gnu_cxx::hash_map<unsigned, unsigned> IndexMap;
  typedef __gnu_cxx::hash_map<unsigned, std::vector<Node> > SkolemMap;
  typedef __gnu_cxx::hash_map<TNode, unsigned, TNodeHashFunction > SignatureMap;
  class DomainMaker {
    TNodeSet d_variables;
    TNodeSet d_constants;
    AbstractionModule& d_module;
    bool d_canMakeDomain; 
  public:
    DomainMaker(AbstractionModule& a)
      : d_variables()
      , d_constants()
      , d_module(a)
      , d_canMakeDomain(true)
    {}
    /** 
     * Process an assertion of the form
     * (or (= var_0 const) ... (= var_k const))
     * 
     * @param constnat 
     * @param set 
     */
    void add(TNode constnat, TNodeSet& variables);
    void finalize(); 
  };

  ArgsTable d_argsTable; 
  DomainMaker d_domainMaker; 
  
  // map from constant upper bound to domain skolem
  NodeNodeMap d_upperBoundToDomain; 
  
  // map from domain skolem to elements of domain
  NodeVecMap d_domainsEnum;
  // map from variables to their domain skolem 
  TNodeTNodeMap d_varToDomain;

  // mapping between signature and uninterpreted function symbol used to
  // abstract the signature
  NodeNodeMap d_signatureToFunc;
  NodeNodeMap d_funcToSignature; 

  NodeNodeMap d_assertionToSignature; 
  SignatureMap d_signatures;
  NodeNodeMap d_sigToGeneralization; 
  TNodeSet d_skolems;

  // skolems maps
  IndexMap d_signatureIndices;
  SkolemMap d_signatureSkolems;
  
  void inferDomains(TNode assertion);
 
  // stores the upper bound of node and creates a new domain for it if it
  // does not have one already
  void storeUpperBound(TNode node, TNode bound);
  
  /** 
   * Returns the domain skolem corresponding to var if
   * one is assigned and just var otherwise. 
   * 
   * @param var 
   * 
   * @return 
   */
  
  TNode getDomainSkolem(TNode var);
  Node makeEnumDomain(TNodeSet& values); 
  void collectArgumentTypes(TNode sig, std::vector<TypeNode>& types, TNodeSet& seen);
  void collectArguments(TNode node, TNode sig, std::vector<Node>& args, TNodeSet& seen);
  void finalizeSignatures();
  Node abstractSignatures(TNode assertion);
  Node computeSignature(TNode node);
  void storeDomain(Node var, Node domain_skolem);
  bool isDomainSkolem(TNode sk);


  TNode getGeneralization(TNode term);
  void storeGeneralization(TNode s, TNode t);
  // signature skolem stuff
  Node getGeneralizedSignature(Node node);
  Node getSignatureSkolem(TNode node);
  unsigned getBitwidthIndex(unsigned bitwidth); 
  void resetSignatureIndex();
  Node computeSignatureRec(TNode, NodeNodeMap&);
  void storeSignature(Node signature, TNode assertion);

  Node substituteArguments(TNode signature, TNode apply, unsigned& i, TNodeTNodeMap& seen);

  // crazy instantiation methods
  Node mkInstantiationLemma(const std::vector<ArgsVec>& instantiation,
                            const std::vector<TNode>& functions,
                            TNode conflict);
  bool isConsistent(const std::vector<ArgsVec>& instantiation,
                    const std::vector<TNode>& funcs);
  void generateInstantiations(unsigned current,
                              std::vector<ArgsTableEntry>& matches, 
                              std::vector<std::vector<ArgsVec> >& instantiations,
                              std::vector<std::vector<ArgsVec> >& new_instantiations);

  void getMatches(TNode node, ArgsTableEntry& matches);

  void skolemizeArguments(std::vector<Node>& assertions);

  Node reverseAbstraction(Node assertion, NodeNodeMap& seen);
public:
  AbstractionModule()
    : d_argsTable()
    , d_domainMaker(*this)
    , d_upperBoundToDomain()
    , d_domainsEnum()
    , d_varToDomain()
    , d_signatureToFunc()
    , d_funcToSignature()
    , d_assertionToSignature()
    , d_signatures()
    , d_sigToGeneralization()
    , d_skolems()
    , d_signatureIndices()
    , d_signatureSkolems()
  {}
  /** 
   * returns true if there are new uninterepreted functions symbols in the output
   * 
   * @param assertions 
   * @param new_assertions 
   * 
   * @return 
   */
  bool applyAbstraction(const std::vector<Node>& assertions, std::vector<Node>& new_assertions);
  /** 
   * Returns true if the node represents an abstraction predicate. 
   * 
   * @param node 
   * 
   * @return 
   */
  bool isAbstraction(TNode node);
  /** 
   * Returns the interpretation of the abstraction predicate. 
   * 
   * @param node 
   * 
   * @return 
   */
  Node getInterpretation(TNode node);
  Node simplifyConflict(TNode conflict); 
  void generalizeConflict(TNode conflict, std::vector<Node>& lemmas); 
};

}
}
}

#endif
