/*********************                                                        */
/*! \file bv_subtheory_bitblast.cpp
 ** \verbatim
 ** Original author: Dejan Jovanovic
 ** Major contributors: Liana Hadarean, Clark Barrett
 ** Minor contributors (to current version): Morgan Deters, Andrew Reynolds, Kshitij Bansal
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Algebraic solver.
 **
 ** Algebraic solver.
 **/

#include "theory/bv/bv_subtheory_bitblast.h"
#include "theory/bv/theory_bv.h"
#include "theory/bv/theory_bv_utils.h"
#include "theory/bv/lazy_bitblaster.h"
#include "theory/bv/bv_quick_check.h"
#include "theory/bv/options.h"
#include "theory/decision_attributes.h"
#include "decision/options.h"


using namespace std;
using namespace CVC4;
using namespace CVC4::context;
using namespace CVC4::theory;
using namespace CVC4::theory::bv;
using namespace CVC4::theory::bv::utils;

BitblastSolver::BitblastSolver(context::Context* c, TheoryBV* bv)
  : SubtheorySolver(c, bv),
    d_bitblaster(new TLazyBitblaster(c, bv, "lazy")),
    d_bitblastQueue(c),
    d_statistics(),
    d_validModelCache(c, true),
    d_lemmaAtomsQueue(c),
    d_useSatPropagation(options::bvPropagate()),
    d_abstractionModule(NULL),
    d_quickCheck(options::bitvectorQuickXplain() ? new BVQuickCheck("bb") : NULL),
    d_quickXplain(options::bitvectorQuickXplain() ? new QuickXPlain("bb", d_quickCheck) :  NULL)
{}

BitblastSolver::~BitblastSolver() {
  delete d_quickXplain;
  delete d_quickCheck;
  delete d_bitblaster;
}

BitblastSolver::Statistics::Statistics()
  : d_numCallstoCheck("theory::bv::BitblastSolver::NumCallsToCheck", 0)
  , d_numBBLemmas("theory::bv::BitblastSolver::NumTimesLemmasBB", 0)
{
  StatisticsRegistry::registerStat(&d_numCallstoCheck);
  StatisticsRegistry::registerStat(&d_numBBLemmas);
}
BitblastSolver::Statistics::~Statistics() {
  StatisticsRegistry::unregisterStat(&d_numCallstoCheck);
  StatisticsRegistry::unregisterStat(&d_numBBLemmas);
}

void BitblastSolver::setAbstraction(AbstractionModule* abs) {
  d_abstractionModule = abs; 
  d_bitblaster->setAbstraction(abs); 
}

void BitblastSolver::preRegister(TNode node) {
  if ((node.getKind() == kind::EQUAL ||
       node.getKind() == kind::BITVECTOR_ULT ||
       node.getKind() == kind::BITVECTOR_ULE ||
       node.getKind() == kind::BITVECTOR_SLT ||
       node.getKind() == kind::BITVECTOR_SLE) &&
      !d_bitblaster->hasBBAtom(node)) {
    CodeTimer weightComputationTime(d_bv->d_statistics.d_weightComputationTimer);
    d_bitblastQueue.push_back(node);
    if ((options::decisionUseWeight() || options::decisionThreshold() != 0) &&
        !node.hasAttribute(theory::DecisionWeightAttr())) {
      NodeSet seen;
      node.setAttribute(theory::DecisionWeightAttr(), d_bitblaster->computeAtomWeight(node, seen));
    }
  }
}

void BitblastSolver::explain(TNode literal, std::vector<TNode>& assumptions) {
  d_bitblaster->explain(literal, assumptions);
}

void BitblastSolver::bitblastQueue() {
  while (!d_bitblastQueue.empty()) {
    TNode atom = d_bitblastQueue.front();
    d_bitblastQueue.pop();
    if (options::bvAbstraction() &&
        d_abstractionModule->isLemmaAtom(atom)) {
      // don't bit-blast lemma atoms
      continue;
    }
    d_bitblaster->bbAtom(atom);
  }
}

bool BitblastSolver::check(Theory::Effort e) {
  Debug("bv-bitblast") << "BitblastSolver::check (" << e << ")\n";
  Assert(!options::bitvectorEagerBitblast()); 

  ++(d_statistics.d_numCallstoCheck);

  //// Lazy bit-blasting
  // bit-blast enqueued nodes
  bitblastQueue();

  // Processing assertions
  while (!done()) {
    TNode fact = get();
    d_validModelCache = false;
    Debug("bv-bitblast") << "  fact " << fact << ")\n";

    if (options::bvAbstraction()) {
      // skip atoms that are the result of abstraction lemmas
      if (d_abstractionModule->isLemmaAtom(fact)) {
        d_lemmaAtomsQueue.push_back(fact);
        continue; 
      }
    }
    
    if (!d_bv->inConflict() &&
        (!d_bv->wasPropagatedBySubtheory(fact) || d_bv->getPropagatingSubtheory(fact) != SUB_BITBLAST)) {
      // Some atoms have not been bit-blasted yet
      d_bitblaster->bbAtom(fact);
      // Assert to sat
      bool ok = d_bitblaster->assertToSat(fact, d_useSatPropagation);
      if (!ok) {
        std::vector<TNode> conflictAtoms;
        d_bitblaster->getConflict(conflictAtoms);
        setConflict(mkConjunction(conflictAtoms));
        return false;
      }
    }
  }

  // We need to ensure we are fully propagated, so propagate now
  if (d_useSatPropagation) {
    bool ok = d_bitblaster->propagate();
    if (!ok) {
      std::vector<TNode> conflictAtoms;
      d_bitblaster->getConflict(conflictAtoms);
      setConflict(mkConjunction(conflictAtoms));
      return false;
    }
  }

  // Solving
  if (e == Theory::EFFORT_FULL) {
    Assert(!d_bv->inConflict());
    Debug("bitvector::bitblaster") << "BitblastSolver::addAssertions solving. \n";
    bool ok = d_bitblaster->solve();
    if (!ok) {
      std::vector<TNode> conflictAtoms;
      d_bitblaster->getConflict(conflictAtoms);
      Node conflict = mkConjunction(conflictAtoms);
      setConflict(conflict);
      return false;
    }
  }

  if (options::bvAbstraction() &&
      e == Theory::EFFORT_FULL &&
      d_lemmaAtomsQueue.size()) {
    
    // bit-blast lemma atoms
    while(!d_lemmaAtomsQueue.empty()) {
      TNode lemma_atom = d_lemmaAtomsQueue.front();
      d_bitblaster->bbAtom(lemma_atom);
      d_lemmaAtomsQueue.pop();

      // Assert to sat and check for conflicts
      bool ok = d_bitblaster->assertToSat(lemma_atom, d_useSatPropagation);
      if (!ok) {
        std::vector<TNode> conflictAtoms;
        d_bitblaster->getConflict(conflictAtoms);
        setConflict(mkConjunction(conflictAtoms));
        return false;
      }
    }
    
    Assert(!d_bv->inConflict());
    bool ok = d_bitblaster->solve();
    if (!ok) {
      std::vector<TNode> conflictAtoms;
      d_bitblaster->getConflict(conflictAtoms);
      Node conflict = mkConjunction(conflictAtoms);
      setConflict(conflict);
      ++(d_statistics.d_numBBLemmas);
      return false;
    }
    
  }
  

  return true;
}

EqualityStatus BitblastSolver::getEqualityStatus(TNode a, TNode b) {
  return d_bitblaster->getEqualityStatus(a, b);
}

void BitblastSolver::collectModelInfo(TheoryModel* m, bool fullModel) {
  return d_bitblaster->collectModelInfo(m, fullModel);
}

Node BitblastSolver::getModelValue(TNode node)
{
  if (!d_validModelCache) {
    d_modelCache.clear();
    d_validModelCache = true;
  }
  return getModelValueRec(node);
}

Node BitblastSolver::getModelValueRec(TNode node)
{
  Node val;
  if (node.isConst()) {
    return node;
  }
  NodeMap::iterator it = d_modelCache.find(node);
  if (it != d_modelCache.end()) {
    val = (*it).second;
    Debug("bitvector-model") << node << " => (cached) " << val <<"\n";
    return val;
  }
  if (d_bv->isLeaf(node)) {
    val = d_bitblaster->getVarValue(node);
    if (val == Node()) {
      // If no value in model, just set to 0
      val = utils::mkConst(utils::getSize(node), (unsigned)0);
    }
  } else {
    NodeBuilder<> valBuilder(node.getKind());
    if (node.getMetaKind() == kind::metakind::PARAMETERIZED) {
      valBuilder << node.getOperator();
    }
    for (unsigned i = 0; i < node.getNumChildren(); ++i) {
      valBuilder << getModelValueRec(node[i]);
    }
    val = valBuilder;
    val = Rewriter::rewrite(val);
  }
  Assert(val.isConst());
  d_modelCache[node] = val;
  Debug("bitvector-model") << node << " => " << val <<"\n";
  return val;
}


void BitblastSolver::setConflict(TNode conflict) {
  Node final_conflict = conflict;
  if (options::bitvectorQuickXplain() &&
      conflict.getKind() == kind::AND &&
      conflict.getNumChildren() > 4) {
    // std::cout << "Original conflict " << conflict.getNumChildren() << "\n"; 
    final_conflict = d_quickXplain->minimizeConflict(conflict);
    //std::cout << "Minimized conflict " << final_conflict.getNumChildren() << "\n"; 
  }
  d_bv->setConflict(final_conflict);
}
