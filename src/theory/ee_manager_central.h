/******************************************************************************
 * Top contributors (to current version):
 *   Andrew Reynolds
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Equality engine manager for central equality engine architecture
 */

#include "cvc5_private.h"

#ifndef CVC5__THEORY__EE_MANAGER_CENTRAL__H
#define CVC5__THEORY__EE_MANAGER_CENTRAL__H

#include <map>
#include <memory>

#include "theory/ee_manager.h"
#include "theory/uf/equality_engine.h"

namespace cvc5 {
namespace theory {

/**
 * The (distributed) equality engine manager. This encapsulates an architecture
 * in which all theories maintain their own copy of an equality engine.
 *
 * This class is not responsible for actually initializing equality engines in
 * theories (since this class does not have access to the internals of Theory).
 * Instead, it is only responsible for the construction of the equality
 * engine objects themselves. TheoryEngine is responsible for querying this
 * class during finishInit() to determine the equality engines to pass to each
 * theories based on getEeTheoryInfo.
 *
 * This class is also responsible for setting up the master equality engine,
 * which is used as a special communication channel to quantifiers engine (e.g.
 * for ensuring quantifiers E-matching is aware of terms from all theories).
 */
class EqEngineManagerCentral : public EqEngineManager
{
 public:
  EqEngineManagerCentral(TheoryEngine& te,
                         SharedSolver& shs,
                         ProofNodeManager* pnm);
  ~EqEngineManagerCentral();
  /**
   * Initialize theories. This method allocates unique equality engines
   * per theories and connects them to a master equality engine.
   */
  void initializeTheories() override;
  /**
   * Notify this class that we are building the model.
   */
  void notifyBuildingModel();

 private:
  /** notify class for master equality engine */
  class MasterNotifyClass : public theory::eq::EqualityEngineNotify
  {
   public:
    MasterNotifyClass(QuantifiersEngine* qe) : d_quantEngine(qe) {}
    /**
     * Called when a new equivalence class is created in the master equality
     * engine.
     */
    void eqNotifyNewClass(TNode t) override;

    bool eqNotifyTriggerPredicate(TNode predicate, bool value) override
    {
      return true;
    }
    bool eqNotifyTriggerTermEquality(TheoryId tag,
                                     TNode t1,
                                     TNode t2,
                                     bool value) override
    {
      return true;
    }
    void eqNotifyConstantTermMerge(TNode t1, TNode t2) override {}
    void eqNotifyMerge(TNode t1, TNode t2) override {}
    void eqNotifyDisequal(TNode t1, TNode t2, TNode reason) override {}

   private:
    /** Pointer to quantifiers engine */
    QuantifiersEngine* d_quantEngine;
  };
  /** The master equality engine notify class */
  std::unique_ptr<MasterNotifyClass> d_masterEENotify;
  /** The master equality engine. */
  eq::EqualityEngine* d_masterEqualityEngine;
  std::unique_ptr<eq::EqualityEngine> d_masterEqualityEngineAlloc;

  // ============================ central

  /**
   * Notify class for central equality engine. This class dispatches
   * notifications from the central equality engine to the appropriate
   * theory(s).
   */
  class CentralNotifyClass : public theory::eq::EqualityEngineNotify
  {
   public:
    CentralNotifyClass(EqEngineManagerCentral& eemc);
    bool eqNotifyTriggerPredicate(TNode predicate, bool value) override;
    bool eqNotifyTriggerTermEquality(TheoryId tag,
                                     TNode t1,
                                     TNode t2,
                                     bool value) override;
    void eqNotifyConstantTermMerge(TNode t1, TNode t2) override;
    void eqNotifyNewClass(TNode t) override;
    void eqNotifyMerge(TNode t1, TNode t2) override;
    void eqNotifyDisequal(TNode t1, TNode t2, TNode reason) override;
    /** Parent */
    EqEngineManagerCentral& d_eemc;
    /** List of notify classes that need new class notification */
    std::vector<eq::EqualityEngineNotify*> d_newClassNotify;
    /** List of notify classes that need merge notification */
    std::vector<eq::EqualityEngineNotify*> d_mergeNotify;
    /** List of notify classes that need disequality notification */
    std::vector<eq::EqualityEngineNotify*> d_disequalNotify;
    /** The model notify class */
    eq::EqualityEngineNotify* d_mNotify;
    /** The quantifiers engine */
    QuantifiersEngine* d_quantEngine;
  };
  /** Notification when predicate gets value in central equality engine */
  bool eqNotifyTriggerPredicate(TNode predicate, bool value);
  bool eqNotifyTriggerTermEquality(TheoryId tag,
                                   TNode t1,
                                   TNode t2,
                                   bool value);
  /** Notification when constants are merged in central equality engine */
  void eqNotifyConstantTermMerge(TNode t1, TNode t2);
  /** The central equality engine notify class */
  CentralNotifyClass d_centralEENotify;
  /** The central equality engine. */
  eq::EqualityEngine d_centralEqualityEngine;
  /** The proof equality engine for the central equality engine */
  std::unique_ptr<eq::ProofEqEngine> d_centralPfee;
  /**
   * A table of from theory IDs to notify classes.
   */
  eq::EqualityEngineNotify* d_theoryNotify[theory::THEORY_LAST];
  /**
   * The state objects of each of the theories that use the central
   * equality engine.
   */
  std::vector<Theory*> d_centralThs;
  /** Whether we are building the model */
  context::CDO<bool> d_buildingModel;
  /** set in conflict */
  void notifyInConflict();
};

}  // namespace theory
}  // namespace cvc5

#endif /* CVC5__THEORY__EE_MANAGER_CENTRAL__H */
