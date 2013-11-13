/*********************                                                        */
/*! \file bitblaster.h
 ** \verbatim
 ** Original author: Liana Hadarean
 ** Major contributors: none
 ** Minor contributors (to current version): lianah, Morgan Deters, Dejan Jovanovic
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Wrapper around the SAT solver used for bitblasting
 **
 ** Wrapper around the SAT solver used for bitblasting.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__BITBLASTER_H
#define __CVC4__BITBLASTER_H


#include "expr/node.h"
#include <vector>
#include <list>
#include <iostream>
#include <math.h>
#include <ext/hash_map>

#include "context/cdo.h"
#include "context/cdhashset.h"
#include "context/cdlist.h"

#include "theory/theory.h"
#include "theory_bv_utils.h"
#include "util/statistics_registry.h"
#include "bitblast_strategies.h"

#include "prop/sat_solver.h"

namespace CVC4 {

// forward declarations
namespace prop {
class CnfStream;
class BVSatSolverInterface;
}

namespace theory {

class OutputChannel;
class TheoryModel;

namespace bv {

typedef std::vector<Node> Bits;

std::string toString (const Bits& bits);

class TheoryBV;

typedef __gnu_cxx::hash_set<TNode, TNodeHashFunction> VarSet;

/**
 * The Bitblaster that manages the mapping between Nodes
 * and their bitwise definition
 *
 */

class Bitblaster {
  typedef __gnu_cxx::hash_map <Node, Bits, TNodeHashFunction>  TermDefMap;
  typedef __gnu_cxx::hash_set<TNode, TNodeHashFunction>        AtomSet;

  typedef void   (*TermBBStrategy) (TNode, Bits&, Bitblaster*);
  typedef Node   (*AtomBBStrategy) (TNode, Bitblaster*);

  // caches and mappings
  TermDefMap                   d_termCache;
  AtomSet                      d_bitblastedAtoms;

  void initAtomBBStrategies();
  void initTermBBStrategies();
protected:
  /// function tables for the various bitblasting strategies indexed by node kind
  TermBBStrategy d_termBBStrategies[kind::LAST_KIND];
  AtomBBStrategy d_atomBBStrategies[kind::LAST_KIND];
  
public:
  Bitblaster(); 
  virtual ~Bitblaster() {}
  virtual void bbAtom(TNode node) = 0; 
  virtual void bbTerm(TNode node, Bits&  bits) = 0;
  virtual void storeVariable(TNode node) = 0;

  bool hasBBAtom(TNode atom) const;
  bool hasBBTerm(TNode node) const;
  void getBBTerm(TNode node, Bits& bits) const;
  void storeBBAtom(TNode atom);
  void storeBBTerm(TNode term, const Bits& bits); 

}; 

class LazyBitblaster :  public Bitblaster {

  /** This class gets callbacks from minisat on propagations */
  class MinisatNotify : public prop::BVSatSolverInterface::Notify {
    prop::CnfStream* d_cnf;
    TheoryBV *d_bv;
  public:
    MinisatNotify(prop::CnfStream* cnf, TheoryBV *bv)
    : d_cnf(cnf)
    , d_bv(bv)
    {}
    bool notify(prop::SatLiteral lit);
    void notify(prop::SatClause& clause);
    void safePoint();
  };


  TheoryBV *d_bv;

  // sat solver used for bitblasting and associated CnfStream
  theory::OutputChannel*             d_bvOutput;
  prop::BVSatSolverInterface*        d_satSolver;
  prop::CnfStream*                   d_cnfStream;

  context::CDList<prop::SatLiteral>  d_assertedAtoms; /**< context dependent list storing the atoms
                                                       currently asserted by the DPLL SAT solver. */
  VarSet d_variables;
  // returns a node that might be easier to bitblast
  Node bbOptimize(TNode node);

  void addAtom(TNode atom);
  bool hasValue(TNode a);
public:
  void bbTerm(TNode node, Bits&  bits);
  void bbAtom(TNode node);

  LazyBitblaster(context::Context* c, bv::TheoryBV* bv);
  ~LazyBitblaster();
  bool assertToSat(TNode node, bool propagate = true);
  bool propagate();
  bool solve(bool quick_solve = false);
  void getConflict(std::vector<TNode>& conflict);
  void explain(TNode atom, std::vector<TNode>& explanation);

  EqualityStatus getEqualityStatus(TNode a, TNode b);
  /**
   * Return a constant Node representing the value of a variable
   * in the current model.
   * @param a
   *
   * @return
   */
  Node getVarValue(TNode a, bool fullModel=true);
  /**
   * Adds a constant value for each bit-blasted variable in the model.
   *
   * @param m the model
   * @param fullModel whether to create a "full model," i.e., add
   * constants to equivalence classes that don't already have them
   */
  void collectModelInfo(TheoryModel* m, bool fullModel);
  /**
   * Stores the variable (or non-bv term) and its corresponding bits.
   *
   * @param var
   */
  void storeVariable(TNode var) {
    d_variables.insert(var);
  }

  bool isSharedTerm(TNode node);
  uint64_t computeAtomWeight(TNode node);

private:

  class Statistics {
  public:
    IntStat d_numTermClauses, d_numAtomClauses;
    IntStat d_numTerms, d_numAtoms;
    TimerStat d_bitblastTimer;
    Statistics();
    ~Statistics();
  };

  Statistics d_statistics;
};



} /* bv namespace */

} /* theory namespace */

} /* CVC4 namespace */

#endif /* __CVC4__BITBLASTER_H */
