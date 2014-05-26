/*********************                                                        */
/*! \file theory_bv.cpp
** \verbatim
** Original author: Dejan Jovanovic
** Major contributors: Morgan Deters, Liana Hadarean
** Minor contributors (to current version): Tim King, Kshitij Bansal, Clark Barrett, Andrew Reynolds
** This file is part of the CVC4 project.
** Copyright (c) 2009-2013  New York University and The University of Iowa
** See the file COPYING in the top-level source directory for licensing
** information.\endverbatim
**
** \brief [[ Add one-line brief description here ]]
**
** [[ Add lengthier description here ]]
** \todo document this file
**/

#include "theory/bv/theory_bv.h"
#include "theory/bv/theory_bv_utils.h"
#include "theory/bv/slicer.h"
#include "theory/valuation.h"
#include "theory/bv/options.h"
#include "theory/bv/theory_bv_rewrite_rules_normalization.h"
#include "theory/bv/theory_bv_rewrite_rules_simplification.h"
#include "theory/bv/bv_subtheory_core.h"
#include "theory/bv/bv_subtheory_inequality.h"
#include "theory/bv/bv_subtheory_algebraic.h"
#include "theory/bv/bv_subtheory_bitblast.h"
#include "theory/bv/bv_eager_solver.h"
#include "theory/bv/theory_bv_rewriter.h"
#include "theory/theory_model.h"
#include "theory/bv/abstraction.h"

using namespace CVC4;
using namespace CVC4::theory;
using namespace CVC4::theory::bv;
using namespace CVC4::context;

using namespace std;
using namespace CVC4::theory::bv::utils;

TheoryBV::TheoryBV(context::Context* c, context::UserContext* u, OutputChannel& out, Valuation valuation, const LogicInfo& logicInfo)
  : Theory(THEORY_BV, c, u, out, valuation, logicInfo),
    d_context(c),
    d_alreadyPropagatedSet(c),
    d_sharedTermsSet(c),
    d_subtheories(),
    d_subtheoryMap(),
    d_statistics(),
    d_lemmasAdded(c, false),
    d_conflict(c, false),
    d_literalsToPropagate(c),
    d_literalsToPropagateIndex(c, 0),
    d_propagatedBy(c),
    d_eagerSolver(NULL),
    d_abstractionModule(new AbstractionModule()),
    d_isCoreTheory(options::bitvectorEqualitySlicer()),
    d_calledPreregister(false)
{

  if (options::bitblastMode() == theory::bv::BITBLAST_MODE_EAGER) {
    d_eagerSolver = new EagerBitblastSolver();
    return; 
  }

  if (options::bitvectorEqualitySolver()) {
    SubtheorySolver* core_solver = new CoreSolver(c, this);
    d_subtheories.push_back(core_solver);
    d_subtheoryMap[SUB_CORE] = core_solver;
  }
  
  if (options::bitvectorInequalitySolver()) {
    SubtheorySolver* ineq_solver = new InequalitySolver(c, this);
    d_subtheories.push_back(ineq_solver);
    d_subtheoryMap[SUB_INEQUALITY] = ineq_solver;
  }

  if (options::bitvectorAlgebraicSolver()) {
    SubtheorySolver* alg_solver = new AlgebraicSolver(c, this);
    d_subtheories.push_back(alg_solver);
    d_subtheoryMap[SUB_ALGEBRAIC] = alg_solver;
  }

  BitblastSolver* bb_solver = new BitblastSolver(c, this);
  if (options::bvAbstraction()) {
    bb_solver->setAbstraction(d_abstractionModule);
  }
  d_subtheories.push_back(bb_solver);
  d_subtheoryMap[SUB_BITBLAST] = bb_solver;
}


TheoryBV::~TheoryBV() {
  for (unsigned i = 0; i < d_subtheories.size(); ++i) {
    delete d_subtheories[i];
  }
}

void TheoryBV::setMasterEqualityEngine(eq::EqualityEngine* eq) {
  if (options::bitblastMode() == theory::bv::BITBLAST_MODE_EAGER) {
    return; 
  }
  if (options::bitvectorEqualitySolver()) {
    dynamic_cast<CoreSolver*>(d_subtheoryMap[SUB_CORE])->setMasterEqualityEngine(eq);
  }
}

TheoryBV::Statistics::Statistics():
  d_avgConflictSize("theory::bv::AvgBVConflictSize"),
  d_solveSubstitutions("theory::bv::NumberOfSolveSubstitutions", 0),
  d_solveTimer("theory::bv::solveTimer"),
  d_numCallsToCheckFullEffort("theory::bv::NumberOfFullCheckCalls", 0),
  d_numCallsToCheckStandardEffort("theory::bv::NumberOfStandardCheckCalls", 0),
  d_weightComputationTimer("theory::bv::weightComputationTimer"),
  d_numMultSlice("theory::bv::NumMultSliceApplied", 0)
{
  StatisticsRegistry::registerStat(&d_avgConflictSize);
  StatisticsRegistry::registerStat(&d_solveSubstitutions);
  StatisticsRegistry::registerStat(&d_solveTimer);
  StatisticsRegistry::registerStat(&d_numCallsToCheckFullEffort);
  StatisticsRegistry::registerStat(&d_numCallsToCheckStandardEffort);
  StatisticsRegistry::registerStat(&d_weightComputationTimer);
  StatisticsRegistry::registerStat(&d_numMultSlice);
}

TheoryBV::Statistics::~Statistics() {
  StatisticsRegistry::unregisterStat(&d_avgConflictSize);
  StatisticsRegistry::unregisterStat(&d_solveSubstitutions);
  StatisticsRegistry::unregisterStat(&d_solveTimer);
  StatisticsRegistry::unregisterStat(&d_numCallsToCheckFullEffort);
  StatisticsRegistry::unregisterStat(&d_numCallsToCheckStandardEffort);
  StatisticsRegistry::unregisterStat(&d_weightComputationTimer);
  StatisticsRegistry::unregisterStat(&d_numMultSlice);
}

Node TheoryBV::getBVDivByZero(Kind k, unsigned width) {
  NodeManager* nm = NodeManager::currentNM();
  if (k == kind::BITVECTOR_UDIV) {
    if (d_BVDivByZero.find(width) == d_BVDivByZero.end()) {
      // lazily create the function symbols
      ostringstream os;
      os << "BVUDivByZero_" << width;
      Node divByZero = nm->mkSkolem(os.str(),
                                    nm->mkFunctionType(nm->mkBitVectorType(width), nm->mkBitVectorType(width)),
                                    "partial bvudiv", NodeManager::SKOLEM_EXACT_NAME);
      d_BVDivByZero[width] = divByZero;
    }
    return d_BVDivByZero[width];
  }
  else if (k == kind::BITVECTOR_UREM) {
    if (d_BVRemByZero.find(width) == d_BVRemByZero.end()) {
      ostringstream os;
      os << "BVURemByZero_" << width;
      Node divByZero = nm->mkSkolem(os.str(),
                                    nm->mkFunctionType(nm->mkBitVectorType(width), nm->mkBitVectorType(width)),
                                    "partial bvurem", NodeManager::SKOLEM_EXACT_NAME);
      d_BVRemByZero[width] = divByZero;
    }
    return d_BVRemByZero[width];
  }

  Unreachable();
}


Node TheoryBV::expandDefinition(LogicRequest &logicRequest, Node node) {
  Debug("bitvector-expandDefinition") << "TheoryBV::expandDefinition(" << node << ")" << std::endl;

  switch (node.getKind()) {
  case kind::BITVECTOR_SDIV:
  case kind::BITVECTOR_SREM:
  case kind::BITVECTOR_SMOD:
    return TheoryBVRewriter::eliminateBVSDiv(node);
    break;

  case kind::BITVECTOR_UDIV:
  case kind::BITVECTOR_UREM: {
    NodeManager* nm = NodeManager::currentNM();
    unsigned width = node.getType().getBitVectorSize();

    if (options::bitvectorDivByZeroConst()) {
      Kind kind = node.getKind() == kind::BITVECTOR_UDIV ? kind::BITVECTOR_UDIV_TOTAL : kind::BITVECTOR_UREM_TOTAL;
      return nm->mkNode(kind, node[0], node[1]); 
    }

    Node divByZero = getBVDivByZero(node.getKind(), width);
    TNode num = node[0], den = node[1];
    Node den_eq_0 = nm->mkNode(kind::EQUAL, den, nm->mkConst(BitVector(width, Integer(0))));
    Node divByZeroNum = nm->mkNode(kind::APPLY_UF, divByZero, num);
    Node divTotalNumDen = nm->mkNode(node.getKind() == kind::BITVECTOR_UDIV ? kind::BITVECTOR_UDIV_TOTAL :
				     kind::BITVECTOR_UREM_TOTAL, num, den);
    node = nm->mkNode(kind::ITE, den_eq_0, divByZeroNum, divTotalNumDen);
    logicRequest.widenLogic(THEORY_UF);
    return node;
  }
    break;

  default:
    return node;
    break;
  }

  Unreachable();
}


void TheoryBV::preRegisterTerm(TNode node) {
  d_calledPreregister = true;
  Debug("bitvector-preregister") << "TheoryBV::preRegister(" << node << ")" << std::endl;
 
  if (options::bitblastMode() == theory::bv::BITBLAST_MODE_EAGER) {
    // the aig bit-blaster option is set heuristically
    // if bv abstraction is not used
    if (!d_eagerSolver->isInitialized()) {
      d_eagerSolver->initialize();
    }

    if (node.getKind() == kind::BITVECTOR_EAGER_ATOM) {
      Node formula = node[0]; 
      d_eagerSolver->assertFormula(formula);
    }
    // nothing to do for the other terms
    return; 
  }
  
  for (unsigned i = 0; i < d_subtheories.size(); ++i) {
    d_subtheories[i]->preRegister(node);
  }
}

void TheoryBV::sendConflict() {
  Assert(d_conflict);
  if (d_conflictNode.isNull()) {
    return;
  } else {
    Debug("bitvector") << indent() << "TheoryBV::check(): conflict " << d_conflictNode;
    d_out->conflict(d_conflictNode);
    d_statistics.d_avgConflictSize.addEntry(d_conflictNode.getNumChildren());
    d_conflictNode = Node::null();
  }
}

void TheoryBV::checkForLemma(TNode fact) {
  if (fact.getKind() == kind::EQUAL) {
    if (fact[0].getKind() == kind::BITVECTOR_UREM_TOTAL) {
      TNode urem = fact[0];
      TNode result = fact[1];
      TNode divisor = urem[1];
      Node result_ult_div = mkNode(kind::BITVECTOR_ULT, result, divisor);
      Node divisor_eq_0 = mkNode(kind::EQUAL,
                                 divisor,
                                 mkConst(BitVector(getSize(divisor), 0u)));
      Node split = utils::mkNode(kind::OR, divisor_eq_0, mkNode(kind::NOT, fact), result_ult_div);
      lemma(split);
    }
    if (fact[1].getKind() == kind::BITVECTOR_UREM_TOTAL) {
      TNode urem = fact[1];
      TNode result = fact[0];
      TNode divisor = urem[1];
      Node result_ult_div = mkNode(kind::BITVECTOR_ULT, result, divisor);
      Node divisor_eq_0 = mkNode(kind::EQUAL,
                                 divisor,
                                 mkConst(BitVector(getSize(divisor), 0u)));
      Node split = utils::mkNode(kind::OR, divisor_eq_0, mkNode(kind::NOT, fact), result_ult_div);
      lemma(split);
    }
  }
}


void TheoryBV::check(Effort e)
{
  Debug("bitvector") << "TheoryBV::check(" << e << ")" << std::endl;

  // if we are using the eager solver
  if (options::bitblastMode() == theory::bv::BITBLAST_MODE_EAGER) {
    // this can only happen on an empty benchmark
    if (!d_eagerSolver->isInitialized()) {
      d_eagerSolver->initialize();
    }
    if (!Theory::fullEffort(e))
      return;

    std::vector<TNode> assertions; 
    while (!done()) {
      TNode fact = get().assertion;
      Assert (fact.getKind() == kind::BITVECTOR_EAGER_ATOM);
      assertions.push_back(fact); 
    }
    Assert (d_eagerSolver->hasAssertions(assertions)); 
    
    bool ok = d_eagerSolver->checkSat();
    if (!ok) {
      if (assertions.size() == 1) {
        d_out->conflict(assertions[0]);
        return; 
      }
      Node conflict = NodeManager::currentNM()->mkNode(kind::AND, assertions);
      d_out->conflict(conflict);
      return; 
    }
    return;
  }
  
  
  if (Theory::fullEffort(e)) {
    ++(d_statistics.d_numCallsToCheckFullEffort);
  } else {
    ++(d_statistics.d_numCallsToCheckStandardEffort);
  }
  // if we are already in conflict just return the conflict
  if (inConflict()) {
    sendConflict();
    return;
  }

  while (!done()) {
    TNode fact = get().assertion;

    checkForLemma(fact);

    for (unsigned i = 0; i < d_subtheories.size(); ++i) {
      d_subtheories[i]->assertFact(fact);
    }
  }

  bool ok = true;
  bool complete = false;
  for (unsigned i = 0; i < d_subtheories.size(); ++i) {
    Assert (!inConflict());
    ok = d_subtheories[i]->check(e);
    complete = d_subtheories[i]->isComplete();

    if (!ok) {
      // if we are in a conflict no need to check with other theories
      Assert (inConflict());
      sendConflict();
      return;
    }
    if (complete) {
      // if the last subtheory was complete we stop
      return;
    }
  }
}

void TheoryBV::collectModelInfo( TheoryModel* m, bool fullModel ){
  Assert(!inConflict());
  //  Assert (fullModel); // can only query full model
  for (unsigned i = 0; i < d_subtheories.size(); ++i) {
    if (d_subtheories[i]->isComplete()) {
      d_subtheories[i]->collectModelInfo(m, fullModel);
      return;
    }
  }
}

Node TheoryBV::getModelValue(TNode var) {
  Assert(!inConflict());
  for (unsigned i = 0; i < d_subtheories.size(); ++i) {
    if (d_subtheories[i]->isComplete()) {
      return d_subtheories[i]->getModelValue(var);
    }
  }
  Unreachable();
}

void TheoryBV::propagate(Effort e) {
  Debug("bitvector") << indent() << "TheoryBV::propagate()" << std::endl;
  if (options::bitblastMode() == theory::bv::BITBLAST_MODE_EAGER) {
    return; 
  }

  if (inConflict()) {
    return;
  }

  // go through stored propagations
  bool ok = true;
  for (; d_literalsToPropagateIndex < d_literalsToPropagate.size() && ok; d_literalsToPropagateIndex = d_literalsToPropagateIndex + 1) {
    TNode literal = d_literalsToPropagate[d_literalsToPropagateIndex];
    // temporary fix for incremental bit-blasting
    if (d_valuation.isSatLiteral(literal)) {
      Debug("bitvector::propagate") << "TheoryBV:: propagating " << literal <<"\n";
      ok = d_out->propagate(literal);
    }
  }

  if (!ok) {
    Debug("bitvector::propagate") << indent() << "TheoryBV::propagate(): conflict from theory engine" << std::endl;
    setConflict();
  }
}


Theory::PPAssertStatus TheoryBV::ppAssert(TNode in, SubstitutionMap& outSubstitutions) {
  switch(in.getKind()) {
  case kind::EQUAL:
    {
      if (in[0].isVar() && !in[1].hasSubterm(in[0])) {
        ++(d_statistics.d_solveSubstitutions);
        outSubstitutions.addSubstitution(in[0], in[1]);
        return PP_ASSERT_STATUS_SOLVED;
      }
      if (in[1].isVar() && !in[0].hasSubterm(in[1])) {
        ++(d_statistics.d_solveSubstitutions);
        outSubstitutions.addSubstitution(in[1], in[0]);
        return PP_ASSERT_STATUS_SOLVED;
      }
      Node node = Rewriter::rewrite(in); 
      if ((node[0].getKind() == kind::BITVECTOR_EXTRACT && node[1].isConst()) ||
          (node[1].getKind() == kind::BITVECTOR_EXTRACT && node[0].isConst())) {
        Node extract = node[0].isConst() ? node[1] : node[0];
        if (extract[0].getKind() == kind::VARIABLE) {
          Node c = node[0].isConst() ? node[0] : node[1];
        
          unsigned high = utils::getExtractHigh(extract);
          unsigned low = utils::getExtractLow(extract);
          unsigned var_bitwidth = utils::getSize(extract[0]);
          std::vector<Node> children;
        
          if (low == 0) {
            Assert (high != var_bitwidth - 1);
            unsigned skolem_size = var_bitwidth - high - 1;
            Node skolem = utils::mkVar(skolem_size);
            children.push_back(skolem); 
            children.push_back(c);
          } else if (high == var_bitwidth - 1) {
            unsigned skolem_size = low;
            Node skolem = utils::mkVar(skolem_size);
            children.push_back(c);
            children.push_back(skolem); 
          } else {
            unsigned skolem1_size = low;
            unsigned skolem2_size = var_bitwidth - high - 1;
            Node skolem1 = utils::mkVar(skolem1_size);
            Node skolem2 = utils::mkVar(skolem2_size);
            children.push_back(skolem2);
            children.push_back(c);
            children.push_back(skolem1);
          }
          Node concat = utils::mkNode(kind::BITVECTOR_CONCAT, children);
          Assert (utils::getSize(concat) == utils::getSize(extract[0])); 
          outSubstitutions.addSubstitution(extract[0], concat);
          return PP_ASSERT_STATUS_SOLVED;
        }
      }
    }
    break;
  case kind::BITVECTOR_ULT:
  case kind::BITVECTOR_SLT:
  case kind::BITVECTOR_ULE:
  case kind::BITVECTOR_SLE:
    
  default:
    // TODO other predicates
    break;
  }
  return PP_ASSERT_STATUS_UNSOLVED;
}

Node TheoryBV::ppRewrite(TNode t)
{
  Node res = t;
  if (RewriteRule<BitwiseEq>::applies(t)) {
    Node result = RewriteRule<BitwiseEq>::run<false>(t);
    res = Rewriter::rewrite(result);
  } else if (d_isCoreTheory && t.getKind() == kind::EQUAL) {
    std::vector<Node> equalities;
    Slicer::splitEqualities(t, equalities);
    res = utils::mkAnd(equalities);
  }

  // if(t.getKind() == kind::EQUAL &&
  //    ((t[0].getKind() == kind::BITVECTOR_MULT && t[1].getKind() == kind::BITVECTOR_PLUS) ||
  //     (t[1].getKind() == kind::BITVECTOR_MULT && t[0].getKind() == kind::BITVECTOR_PLUS))) {
  //   // if we have an equality between a multiplication and addition
  //   // try to express multiplication in terms of addition
  //   Node mult = t[0].getKind() == kind::BITVECTOR_MULT? t[0] : t[1];
  //   Node add = t[0].getKind() == kind::BITVECTOR_PLUS? t[0] : t[1];
  //   if (RewriteRule<MultSlice>::applies(mult)) {
  //     Node new_mult = RewriteRule<MultSlice>::run<false>(mult);
  //     Node new_eq = Rewriter::rewrite(utils::mkNode(kind::EQUAL, new_mult, add));

  //     // the simplification can cause the formula to blow up
  //     // only apply if formula reduced
  //     if (d_subtheoryMap.find(SUB_BITBLAST) != d_subtheoryMap.end()) {
  //       BitblastSolver* bv = (BitblastSolver*)d_subtheoryMap[SUB_BITBLAST];
  //       uint64_t old_size = bv->computeAtomWeight(t);
  //       Assert (old_size);
  //       uint64_t new_size = bv->computeAtomWeight(new_eq);
  //       double ratio = ((double)new_size)/old_size;
  //       if (ratio <= 0.4) {
  //         ++(d_statistics.d_numMultSlice);
  //         return new_eq;
  //       }
  //     }
      
  //     if (new_eq.getKind() == kind::CONST_BOOLEAN) {
  //       ++(d_statistics.d_numMultSlice);
  //       return new_eq;
  //     }
  //   }
  // }
  
  if (options::bvAbstraction() && t.getType().isBoolean()) {
    d_abstractionModule->addInputAtom(res); 
  }
  return res;
}

void TheoryBV::presolve() {
  Debug("bitvector") << "TheoryBV::presolve" << endl;
}

static int prop_count = 0; 

bool TheoryBV::storePropagation(TNode literal, SubTheory subtheory)
{
  Debug("bitvector::propagate") << indent() << getSatContext()->getLevel() << " " << "TheoryBV::storePropagation(" << literal << ", " << subtheory << ")" << std::endl;
  prop_count++; 
  
  // If already in conflict, no more propagation
  if (d_conflict) {
    Debug("bitvector::propagate") << indent() << "TheoryBV::storePropagation(" << literal << ", " << subtheory << "): already in conflict" << std::endl;
    return false;
  }

  // If propagated already, just skip
  PropagatedMap::const_iterator find = d_propagatedBy.find(literal);
  if (find != d_propagatedBy.end()) {
    return true;
  } else {
    bool polarity = literal.getKind() != kind::NOT;
    Node negatedLiteral = polarity ? literal.notNode() : (Node) literal[0];
    find = d_propagatedBy.find(negatedLiteral);
    if (find != d_propagatedBy.end() && (*find).second != subtheory) {
      // Safe to ignore this one, subtheory should produce a conflict
      return true;
    }

    d_propagatedBy[literal] = subtheory;
  }

  // Propagate differs depending on the subtheory
  // * bitblaster needs to be left alone until it's done, otherwise it doesn't know how to explain
  // * equality engine can propagate eagerly
  bool ok = true;
  if (subtheory == SUB_CORE) {
    d_out->propagate(literal);
    if (!ok) {
      setConflict();
    }
  } else {
    d_literalsToPropagate.push_back(literal);
  }
  return ok;

}/* TheoryBV::propagate(TNode) */


void TheoryBV::explain(TNode literal, std::vector<TNode>& assumptions) {
  Assert (wasPropagatedBySubtheory(literal));
  SubTheory sub = getPropagatingSubtheory(literal);
  d_subtheoryMap[sub]->explain(literal, assumptions);
}


Node TheoryBV::explain(TNode node) {
  Debug("bitvector::explain") << "TheoryBV::explain(" << node << ")" << std::endl;
  std::vector<TNode> assumptions;

  // Ask for the explanation
  explain(node, assumptions);
  // this means that it is something true at level 0
  if (assumptions.size() == 0) {
    return utils::mkTrue();
  }
  // return the explanation
  Node explanation = utils::mkAnd(assumptions);
  Debug("bitvector::explain") << "TheoryBV::explain(" << node << ") => " << explanation << std::endl;
  return explanation;
}


void TheoryBV::addSharedTerm(TNode t) {
  Debug("bitvector::sharing") << indent() << "TheoryBV::addSharedTerm(" << t << ")" << std::endl;
  d_sharedTermsSet.insert(t);
  if (options::bitvectorEqualitySolver()) {
    for (unsigned i = 0; i < d_subtheories.size(); ++i) {
      d_subtheories[i]->addSharedTerm(t);
    }
  }
}


EqualityStatus TheoryBV::getEqualityStatus(TNode a, TNode b)
{
  Assert (options::bitblastMode() == theory::bv::BITBLAST_MODE_LAZY); 
  for (unsigned i = 0; i < d_subtheories.size(); ++i) {
    EqualityStatus status = d_subtheories[i]->getEqualityStatus(a, b);
    if (status != EQUALITY_UNKNOWN) {
      return status;
    }
  }
  return EQUALITY_UNKNOWN; ;
}


void TheoryBV::enableCoreTheorySlicer() {
  Assert (!d_calledPreregister);
  d_isCoreTheory = true;
  if (d_subtheoryMap.find(SUB_CORE) != d_subtheoryMap.end()) {
    CoreSolver* core = (CoreSolver*)d_subtheoryMap[SUB_CORE];
    core->enableSlicer(); 
  }
}


void TheoryBV::ppStaticLearn(TNode in, NodeBuilder<>& learned) {}

bool TheoryBV::applyAbstraction(const std::vector<Node>& assertions, std::vector<Node>& new_assertions) {
  bool changed = d_abstractionModule->applyAbstraction(assertions, new_assertions);
  if (changed &&
      options::bitblastMode() == theory::bv::BITBLAST_MODE_EAGER &&
      options::bitvectorAig()) {
    // disable AIG mode
    AlwaysAssert (!d_eagerSolver->isInitialized());
    d_eagerSolver->turnOffAig();
    d_eagerSolver->initialize();
  }
  return changed;
}

void TheoryBV::setConflict(Node conflict) {
  if (options::bvAbstraction()) {
    Node new_conflict = d_abstractionModule->simplifyConflict(conflict);
      
    std::vector<Node> lemmas;
    lemmas.push_back(new_conflict); 
    d_abstractionModule->generalizeConflict(new_conflict, lemmas);
    for (unsigned i = 0; i < lemmas.size(); ++i) {
      lemma(utils::mkNode(kind::NOT, lemmas[i])); 
    }
  }
  d_conflict = true;
  d_conflictNode = conflict;
}
