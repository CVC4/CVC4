/*********************                                                        */
/*! \file quantifiers_engine.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Morgan Deters, Haniel Barbosa
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Theory instantiator, Instantiation Engine classes
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__QUANTIFIERS_ENGINE_H
#define __CVC4__THEORY__QUANTIFIERS_ENGINE_H

#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>

#include "context/cdhashset.h"
#include "context/cdlist.h"
#include "expr/attribute.h"
#include "options/quantifiers_modes.h"
#include "theory/quantifiers/inst_match.h"
#include "theory/quantifiers/quant_util.h"
#include "theory/theory.h"
#include "util/hash.h"
#include "util/statistics_registry.h"

namespace CVC4 {

class TheoryEngine;

namespace theory {

class QuantifiersEngine;

namespace quantifiers {
  //TODO: organize this more/review this, github issue #1163
  //TODO: include these instead of doing forward declarations? #1307
  //utilities
  class TermDb;
  class TermDbSygus;
  class TermUtil;
  class Instantiate;
  class Skolemize;
  class TermEnumeration;
  class FirstOrderModel;
  class QuantAttributes;
  class QuantEPR;
  class QuantRelevance;
  class RelevantDomain;
  class BvInverter;
  class InstPropagator;
  class EqualityInference;
  class EqualityQueryQuantifiersEngine;
  //modules, these are "subsolvers" of the quantifiers theory.
  class InstantiationEngine;
  class ModelEngine;
  class BoundedIntegers;
  class QuantConflictFind;
  class RewriteEngine;
  class QModelBuilder;
  class ConjectureGenerator;
  class CegInstantiation;
  class LtePartialInst;
  class AlphaEquivalence;
  class InstStrategyEnum;
  class InstStrategyCbqi;
  class InstStrategyCegqi;
  class QuantDSplit;
  class QuantAntiSkolem;
  class EqualityInference;
}/* CVC4::theory::quantifiers */

namespace inst {
  class TriggerTrie;
}/* CVC4::theory::inst */


class QuantifiersEngine {
  // TODO: remove these github issue #1163
  friend class quantifiers::InstantiationEngine;
  friend class quantifiers::InstStrategyCbqi;
  friend class quantifiers::InstStrategyCegqi;
  friend class quantifiers::ModelEngine;
  friend class quantifiers::RewriteEngine;
  friend class quantifiers::QuantConflictFind;
  friend class inst::InstMatch;
  typedef context::CDHashMap< Node, bool, NodeHashFunction > BoolMap;
  typedef context::CDList<Node> NodeList;
  typedef context::CDList<bool> BoolList;
  typedef context::CDHashSet<Node, NodeHashFunction> NodeSet;
private:
  /** reference to theory engine object */
  TheoryEngine* d_te;
  /** vector of utilities for quantifiers */
  std::vector< QuantifiersUtil* > d_util;
  /** vector of modules for quantifiers */
  std::vector< QuantifiersModule* > d_modules;
  //------------- quantifiers utilities
  /** equality query class */
  std::unique_ptr<quantifiers::EqualityQueryQuantifiersEngine> d_eq_query;
  /** equality inference class */
  std::unique_ptr<quantifiers::EqualityInference> d_eq_inference;
  /** quantifiers instantiation propagtor */
  std::unique_ptr<quantifiers::InstPropagator > d_inst_prop;
  /** all triggers will be stored in this trie */
  std::unique_ptr<inst::TriggerTrie> d_tr_trie;
  /** extended model object */
  std::unique_ptr<quantifiers::FirstOrderModel> d_model;
  /** for computing relevance of quantifiers */
  std::unique_ptr<quantifiers::QuantRelevance> d_quant_rel;
  /** relevant domain */
  std::unique_ptr<quantifiers::RelevantDomain> d_rel_dom;
  /** inversion utility for BV instantiation */
  std::unique_ptr<quantifiers::BvInverter > d_bv_invert;
  /** model builder */
  std::unique_ptr<quantifiers::QModelBuilder> d_builder;
  /** utility for effectively propositional logic */
  std::unique_ptr<quantifiers::QuantEPR> d_qepr;
  /** term utilities */
  std::unique_ptr<quantifiers::TermUtil> d_term_util;
  /** term database */
  std::unique_ptr<quantifiers::TermDb> d_term_db;
  /** sygus term database */
  std::unique_ptr<quantifiers::TermDbSygus> d_sygus_tdb;
  /** quantifiers attributes */
  std::unique_ptr<quantifiers::QuantAttributes> d_quant_attr;
  /** instantiate utility */
  std::unique_ptr<quantifiers::Instantiate> d_instantiate;
  /** skolemize utility */
  std::unique_ptr<quantifiers::Skolemize> d_skolemize;
  /** term enumeration utility */
  std::unique_ptr<quantifiers::TermEnumeration> d_term_enum;
  //------------- end quantifiers utilities
  //------------- quantifiers modules
  /** alpha equivalence */
  std::unique_ptr<quantifiers::AlphaEquivalence > d_alpha_equiv;
  /** instantiation engine */
  std::unique_ptr<quantifiers::InstantiationEngine> d_inst_engine;
  /** model engine */
  std::unique_ptr<quantifiers::ModelEngine> d_model_engine;
  /** bounded integers utility */
  std::unique_ptr<quantifiers::BoundedIntegers > d_bint;
  /** Conflict find mechanism for quantifiers */
  std::unique_ptr<quantifiers::QuantConflictFind> d_qcf;
  /** rewrite rules utility */
  std::unique_ptr<quantifiers::RewriteEngine > d_rr_engine;
  /** subgoal generator */
  std::unique_ptr<quantifiers::ConjectureGenerator > d_sg_gen;
  /** ceg instantiation */
  std::unique_ptr<quantifiers::CegInstantiation > d_ceg_inst;
  /** lte partial instantiation */
  std::unique_ptr<quantifiers::LtePartialInst > d_lte_part_inst;
  /** full saturation */
  std::unique_ptr<quantifiers::InstStrategyEnum> d_fs;
  /** counterexample-based quantifier instantiation */
  std::unique_ptr<quantifiers::InstStrategyCbqi > d_i_cbqi;
  /** quantifiers splitting */
  std::unique_ptr<quantifiers::QuantDSplit > d_qsplit;
  /** quantifiers anti-skolemization */
  std::unique_ptr<quantifiers::QuantAntiSkolem > d_anti_skolem;
  //------------- end quantifiers modules
 private:  //this information is reset during check
    /** current effort level */
  QuantifiersModule::QEffort d_curr_effort_level;
  /** are we in conflict */
  bool d_conflict;
  context::CDO<bool> d_conflict_c;
  /** has added lemma this round */
  bool d_hasAddedLemma;
  /** whether to use model equality engine */
  bool d_useModelEe;
 private:
  /** list of all quantifiers seen */
  std::map< Node, bool > d_quants;
  /** quantifiers pre-registered */
  NodeSet d_quants_prereg;
  /** quantifiers reduced */
  BoolMap d_quants_red;
  std::map< Node, Node > d_quants_red_lem;
  /** list of all lemmas produced */
  //std::map< Node, bool > d_lemmas_produced;
  BoolMap d_lemmas_produced_c;
  /** lemmas waiting */
  std::vector< Node > d_lemmas_waiting;
  /** phase requirements waiting */
  std::map< Node, bool > d_phase_req_waiting;
  /** inst round counters TODO: make context-dependent? */
  context::CDO< int > d_ierCounter_c;
  int d_ierCounter;
  int d_ierCounter_lc;
  int d_ierCounterLastLc;
  int d_inst_when_phase;
  /** has presolve been called */
  context::CDO< bool > d_presolve;
  /** presolve cache */
  NodeSet d_presolve_in;
  NodeList d_presolve_cache;
  BoolList d_presolve_cache_wq;
  BoolList d_presolve_cache_wic;

public:
  QuantifiersEngine(context::Context* c, context::UserContext* u, TheoryEngine* te);
  ~QuantifiersEngine();
  /** get theory engine */
  TheoryEngine* getTheoryEngine()const { return d_te; }
  /** get equality query */
  EqualityQuery* getEqualityQuery()const;
  /** get the equality inference */
  quantifiers::EqualityInference* getEqualityInference() const{ return d_eq_inference.get(); }
  /** get default sat context for quantifiers engine */
  context::Context* getSatContext();
  /** get default sat context for quantifiers engine */
  context::UserContext* getUserContext();
  /** get default output channel for the quantifiers engine */
  OutputChannel& getOutputChannel();
  /** get default valuation for the quantifiers engine */
  Valuation& getValuation();
  /** get the logic info for the quantifiers engine */
  const LogicInfo& getLogicInfo() const;
  /** get relevant domain */
  quantifiers::RelevantDomain* getRelevantDomain() const{ return d_rel_dom.get(); }
  /** get the BV inverter utility */
  quantifiers::BvInverter * getBvInverter()const { return d_bv_invert.get(); }
  /** get quantifier relevance */
  quantifiers::QuantRelevance* getQuantifierRelevance()const { return d_quant_rel.get(); }
  /** get the model builder */
  quantifiers::QModelBuilder* getModelBuilder()const { return d_builder.get(); }
  /** get utility for EPR */
  quantifiers::QuantEPR* getQuantEPR() const{ return d_qepr.get(); }
 public:  // modules
  /** get instantiation engine */
  quantifiers::InstantiationEngine* getInstantiationEngine() const{ return d_inst_engine.get(); }
  /** get model engine */
  quantifiers::ModelEngine* getModelEngine() const{ return d_model_engine.get(); }
  /** get bounded integers utility */
  quantifiers::BoundedIntegers * getBoundedIntegers() const{ return d_bint.get(); }
  /** Conflict find mechanism for quantifiers */
  quantifiers::QuantConflictFind* getConflictFind() const{ return d_qcf.get(); }
  /** rewrite rules utility */
  quantifiers::RewriteEngine * getRewriteEngine() const{ return d_rr_engine.get(); }
  /** subgoal generator */
  quantifiers::ConjectureGenerator * getConjectureGenerator() const{ return d_sg_gen.get(); }
  /** ceg instantiation */
  quantifiers::CegInstantiation * getCegInstantiation() const{ return d_ceg_inst.get(); }
  /** local theory ext partial inst */
  quantifiers::LtePartialInst * getLtePartialInst() const{ return d_lte_part_inst.get(); }
  /** get full saturation */
  quantifiers::InstStrategyEnum* getInstStrategyEnum() const{ return d_fs.get(); }
  /** get inst strategy cbqi */
  quantifiers::InstStrategyCbqi * getInstStrategyCbqi() const{ return d_i_cbqi.get(); }
  /** get quantifiers splitting */
  quantifiers::QuantDSplit * getQuantDSplit() const{ return d_qsplit.get(); }
  /** get quantifiers anti-skolemization */
  quantifiers::QuantAntiSkolem * getQuantAntiSkolem() const{ return d_anti_skolem.get(); }
private:
  /** owner of quantified formulas */
  std::map< Node, QuantifiersModule * > d_owner;
  std::map< Node, int > d_owner_priority;
public:
  /** get owner */
  QuantifiersModule * getOwner( Node q );
  /** set owner */
  void setOwner( Node q, QuantifiersModule * m, int priority = 0 );
  /** considers */
  bool hasOwnership( Node q, QuantifiersModule * m = NULL );
  /** is finite bound */
  bool isFiniteBound( Node q, Node v );
public:
  /** presolve */
  void presolve();
  /** notify preprocessed assertion */
  void ppNotifyAssertions(const std::vector<Node>& assertions);
  /** check at level */
  void check( Theory::Effort e );
  /** notify that theories were combined */
  void notifyCombineTheories();
  /** preRegister quantifier
   *
   * This function is called after registerQuantifier for quantified formulas
   * that are pre-registered to the quantifiers theory.
   */
  void preRegisterQuantifier(Node q);
  /** register quantifier */
  void registerPattern( std::vector<Node> & pattern);
  /** assert universal quantifier */
  void assertQuantifier( Node q, bool pol );
  /** get next decision request */
  Node getNextDecisionRequest( unsigned& priority );
private:
 /** (context-indepentent) register quantifier internal
  *
  * This is called when a quantified formula q is pre-registered to the
  * quantifiers theory, and updates the modules in this class with
  * context-independent information about how to handle q. This includes basic
  * information such as which module owns q.
  */
 void registerQuantifierInternal(Node q);
 /** reduceQuantifier, return true if reduced */
 bool reduceQuantifier(Node q);
 /** flush lemmas */
 void flushLemmas();

public:
  /** add lemma lem */
  bool addLemma( Node lem, bool doCache = true, bool doRewrite = true );
  /** remove pending lemma */
  bool removeLemma( Node lem );
  /** add require phase */
  void addRequirePhase( Node lit, bool req );
  /** add EPR axiom */
  bool addEPRAxiom( TypeNode tn );
  /** mark relevant quantified formula, this will indicate it should be checked before the others */
  void markRelevant( Node q );
  /** has added lemma */
  bool hasAddedLemma() { return !d_lemmas_waiting.empty() || d_hasAddedLemma; }
  /** is in conflict */
  bool inConflict() { return d_conflict; }
  /** set conflict */
  void setConflict();
  /** get current q effort */
  QuantifiersModule::QEffort getCurrentQEffort() { return d_curr_effort_level; }
  /** get number of waiting lemmas */
  unsigned getNumLemmasWaiting() { return d_lemmas_waiting.size(); }
  /** get needs check */
  bool getInstWhenNeedsCheck( Theory::Effort e );
  /** get user pat mode */
  quantifiers::UserPatMode getInstUserPatMode();
public:
  /** get model */
  quantifiers::FirstOrderModel* getModel() { return d_model.get(); }
  /** get term database */
  quantifiers::TermDb* getTermDatabase() { return d_term_db.get(); }
  /** get term database sygus */
  quantifiers::TermDbSygus * getTermDatabaseSygus() { return d_sygus_tdb.get(); }
  /** get term utilities */
  quantifiers::TermUtil* getTermUtil() { return d_term_util.get(); }
  /** get quantifiers attributes */
  quantifiers::QuantAttributes* getQuantAttributes() {
    return d_quant_attr.get();
  }
  /** get instantiate utility */
  quantifiers::Instantiate* getInstantiate() { return d_instantiate.get(); }
  /** get skolemize utility */
  quantifiers::Skolemize* getSkolemize() { return d_skolemize.get(); }
  /** get term enumeration utility */
  quantifiers::TermEnumeration* getTermEnumeration()
  {
    return d_term_enum.get();
  }
  /** get trigger database */
  inst::TriggerTrie* getTriggerDatabase() { return d_tr_trie.get(); }
  /** add term to database */
  void addTermToDatabase( Node n, bool withinQuant = false, bool withinInstClosure = false );
  /** notification when master equality engine is updated */
  void eqNotifyNewClass(TNode t);
  void eqNotifyPreMerge(TNode t1, TNode t2);
  void eqNotifyPostMerge(TNode t1, TNode t2);
  void eqNotifyDisequal(TNode t1, TNode t2, TNode reason);
  /** get the master equality engine */
  eq::EqualityEngine* getMasterEqualityEngine();
  /** get the active equality engine */
  eq::EqualityEngine* getActiveEqualityEngine();
  /** use model equality engine */
  bool usingModelEqualityEngine() const { return d_useModelEe; }
  /** debug print equality engine */
  void debugPrintEqualityEngine( const char * c );
  /** get internal representative */
  Node getInternalRepresentative( Node a, Node q, int index );

 public:
  //----------user interface for instantiations (see quantifiers/instantiate.h)
  /** print instantiations */
  void printInstantiations(std::ostream& out);
  /** print solution for synthesis conjectures */
  void printSynthSolution(std::ostream& out);
  /** get list of quantified formulas that were instantiated */
  void getInstantiatedQuantifiedFormulas(std::vector<Node>& qs);
  /** get instantiations */
  void getInstantiations(Node q, std::vector<Node>& insts);
  void getInstantiations(std::map<Node, std::vector<Node> >& insts);
  /** get instantiation term vectors */
  void getInstantiationTermVectors(Node q,
                                   std::vector<std::vector<Node> >& tvecs);
  void getInstantiationTermVectors(
      std::map<Node, std::vector<std::vector<Node> > >& insts);
  /** get instantiated conjunction */
  Node getInstantiatedConjunction(Node q);
  /** get unsat core lemmas */
  bool getUnsatCoreLemmas(std::vector<Node>& active_lemmas);
  bool getUnsatCoreLemmas(std::vector<Node>& active_lemmas,
                          std::map<Node, Node>& weak_imp);
  /** get explanation for instantiation lemmas */
  void getExplanationForInstLemmas(const std::vector<Node>& lems,
                                   std::map<Node, Node>& quant,
                                   std::map<Node, std::vector<Node> >& tvec);

  /** get synth solutions
   *
   * This function adds entries to sol_map that map functions-to-synthesize with
   * their solutions, for all active conjectures. This should be called
   * immediately after the solver answers unsat for sygus input.
   *
   * For details on what is added to sol_map, see
   * CegConjecture::getSynthSolutions.
   */
  void getSynthSolutions(std::map<Node, Node>& sol_map);

  //----------end user interface for instantiations

  /** statistics class */
  class Statistics {
  public:
    TimerStat d_time;
    TimerStat d_qcf_time;
    TimerStat d_ematching_time;
    IntStat d_num_quant;
    IntStat d_instantiation_rounds;
    IntStat d_instantiation_rounds_lc;
    IntStat d_triggers;
    IntStat d_simple_triggers;
    IntStat d_multi_triggers;
    IntStat d_multi_trigger_instantiations;
    IntStat d_red_alpha_equiv;
    IntStat d_instantiations_user_patterns;
    IntStat d_instantiations_auto_gen;
    IntStat d_instantiations_guess;
    IntStat d_instantiations_qcf;
    IntStat d_instantiations_qcf_prop;
    IntStat d_instantiations_fmf_exh;
    IntStat d_instantiations_fmf_mbqi;
    IntStat d_instantiations_cbqi;
    IntStat d_instantiations_rr;
    Statistics();
    ~Statistics();
  };/* class QuantifiersEngine::Statistics */
  Statistics d_statistics;
};/* class QuantifiersEngine */

}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif /* __CVC4__THEORY__QUANTIFIERS_ENGINE_H */
