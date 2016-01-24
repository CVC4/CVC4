/*********************                                                        */
/*! \file bv_subtheory_bitblast.h
 ** \verbatim
 ** Original author: Dejan Jovanovic
 ** Major contributors: Liana Hadarean
 ** Minor contributors (to current version): Morgan Deters, Andrew Reynolds, Clark Barrett
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2014  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Algebraic solver.
 **
 ** Algebraic solver.
 **/

#include "cvc4_private.h"

#pragma once

#include "theory/bv/bitblaster_template.h"
#include "theory/bv/bv_subtheory.h"

namespace CVC4 {
namespace theory {
namespace bv {

class LazyBitblaster;
class AbstractionModule;
class BVQuickCheck;
class QuickXPlain;

/**
 * BitblastSolver
 */
class BitblastSolver : public SubtheorySolver {
  struct Statistics {
    IntStat d_numCallstoCheck;
    IntStat d_numBBLemmas;
    Statistics();
    ~Statistics();
  };
  /** Bitblaster */
  TLazyBitblaster* d_bitblaster;

  /** Nodes that still need to be bit-blasted */
  context::CDQueue<TNode> d_bitblastQueue;
  Statistics d_statistics;

  typedef std::hash_map<Node, Node, NodeHashFunction> NodeMap;
  NodeMap d_modelCache;
  context::CDO<bool> d_validModelCache;

  /** Queue for bit-blasting lemma atoms only in full check if we are sat */
  context::CDQueue<TNode> d_lemmaAtomsQueue;
  bool  d_useSatPropagation;
  AbstractionModule* d_abstractionModule;
  BVQuickCheck* d_quickCheck;
  QuickXPlain* d_quickXplain;
  //  Node getModelValueRec(TNode node);
  void setConflict(TNode conflict);
public:
  BitblastSolver(context::Context* c, TheoryBV* bv);
  ~BitblastSolver();

  void  preRegister(TNode node);
  bool  check(Theory::Effort e);
  void  explain(TNode literal, std::vector<TNode>& assumptions);
  EqualityStatus getEqualityStatus(TNode a, TNode b);
  void collectModelInfo(TheoryModel* m, bool fullModel);
  Node getModelValue(TNode node);
  bool isComplete() { return true; }
  void bitblastQueue();
  void setAbstraction(AbstractionModule* module); 
  uint64_t computeAtomWeight(TNode atom); 
  void setProofLog( BitVectorProof * bvp );
};

// taking : namespaces
}
}
}
