/*********************                                                        */
/*! \file equality_engine.h
 ** \verbatim
 ** Original author: dejan
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 prototype.
 ** Copyright (c) 2009, 2010, 2011  The Analysis of Computer Systems Group (ACSys)
 ** Courant Institute of Mathematical Sciences
 ** New York University
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "cvc4_private.h"

#pragma once

#include <queue>
#include <vector>
#include <ext/hash_map>

#include "expr/node.h"
#include "expr/kind_map.h"
#include "context/cdo.h"
#include "util/output.h"
#include "util/stats.h"
#include "theory/rewriter.h"
#include "theory/theory.h"

#include "theory/uf/equality_engine_types.h"

namespace CVC4 {
namespace theory {
namespace eq {

/**
 * Interface for equality engine notifications. All the notifications
 * are safe as TNodes, but not necessarily for negations.
 */
class EqualityEngineNotify {

  friend class EqualityEngine;

public:

  virtual ~EqualityEngineNotify() {};

  /**
   * Notifies about a trigger equality that became true or false.
   *
   * @param eq the equality that became true or false
   * @param value the value of the equality
   */
  virtual bool eqNotifyTriggerEquality(TNode equality, bool value) = 0;

  /**
   * Notifies about a trigger predicate that became true or false.
   *
   * @param predicate the trigger predicate that bacame true or false
   * @param value the value of the predicate
   */
  virtual bool eqNotifyTriggerPredicate(TNode predicate, bool value) = 0;

  /**
   * Notifies about the merge of two trigger terms.
   *
   * @param tag the theory that both triggers were tagged with
   * @param t1 a term marked as trigger
   * @param t2 a term marked as trigger
   * @param value true if equal, false if dis-equal
   */
  virtual bool eqNotifyTriggerTermEquality(TheoryId tag, TNode t1, TNode t2, bool value) = 0;

  /**
   * Notifies about the merge of two constant terms.
   *
   * @param t1 a constant term
   * @param t2 a constnat term
   */
  virtual bool eqNotifyConstantTermMerge(TNode t1, TNode t2) = 0;
};

/**
 * Implementation of the notification interface that ignores all the
 * notifications.
 */
class EqualityEngineNotifyNone : public EqualityEngineNotify {
public:
  bool eqNotifyTriggerEquality(TNode equality, bool value) { return true; }
  bool eqNotifyTriggerPredicate(TNode predicate, bool value) { return true; }
  bool eqNotifyTriggerTermEquality(TheoryId tag, TNode t1, TNode t2, bool value) { return true; }
  bool eqNotifyConstantTermMerge(TNode t1, TNode t2) { return true; }
};


/**
 * Class for keeping an incremental congurence closure over a set of terms. It provides
 * notifications via an EqualityEngineNotify object.
 */
class EqualityEngine : public context::ContextNotifyObj {

  /** Default implementation of the notification object */
  static EqualityEngineNotifyNone s_notifyNone;

public:

  /** Statistics about the equality engine instance */
  struct Statistics {
    /** Total number of merges */
    IntStat mergesCount;
    /** Number of terms managed by the system */
    IntStat termsCount;
    /** Number of function terms managed by the system */
    IntStat functionTermsCount;
    /** Number of constant terms managed by the system */
    IntStat constantTermsCount;

    Statistics(std::string name)
    : mergesCount(name + "::mergesCount", 0),
      termsCount(name + "::termsCount", 0),
      functionTermsCount(name + "::functionTermsCount", 0),
      constantTermsCount(name + "::constantTermsCount", 0)
    {
      StatisticsRegistry::registerStat(&mergesCount);
      StatisticsRegistry::registerStat(&termsCount);
      StatisticsRegistry::registerStat(&functionTermsCount);
      StatisticsRegistry::registerStat(&constantTermsCount);
    }

    ~Statistics() {
      StatisticsRegistry::unregisterStat(&mergesCount);
      StatisticsRegistry::unregisterStat(&termsCount);
      StatisticsRegistry::unregisterStat(&functionTermsCount);
      StatisticsRegistry::unregisterStat(&constantTermsCount);
    }
  };

  /**
   * Store the application lookup, with enough information to backtrack
   */
  void storeApplicationLookup(FunctionApplication& funNormalized, EqualityNodeId funId);

private:

  /** The context we are using */
  context::Context* d_context;

  /** If we are done, we don't except any new assertions */
  context::CDO<bool> d_done;

  /** Whether to notify or not (temporarily disabled on equality checks) */
  bool d_performNotify;

  /** The class to notify when a representative changes for a term */
  EqualityEngineNotify& d_notify;

  /** The map of kinds to be treated as function applications */
  KindMap d_congruenceKinds;

  /** Map from nodes to their ids */
  __gnu_cxx::hash_map<TNode, EqualityNodeId, TNodeHashFunction> d_nodeIds;

  /** Map from function applications to their ids */
  typedef __gnu_cxx::hash_map<FunctionApplication, EqualityNodeId, FunctionApplicationHashFunction> ApplicationIdsMap;

  /**
   * A map from a pair (a', b') to a function application f(a, b), where a' and b' are the current representatives
   * of a and b.
   */
  ApplicationIdsMap d_applicationLookup;

  /** Application lookups in order, so that we can backtrack. */
  std::vector<FunctionApplication> d_applicationLookups;

  /** Number of application lookups, for backtracking.  */
  context::CDO<DefaultSizeType> d_applicationLookupsCount;

  /** Map from ids to the nodes (these need to be nodes as we pick-up the opreators) */
  std::vector<Node> d_nodes;

  /** A context-dependents count of nodes */
  context::CDO<DefaultSizeType> d_nodesCount;

  /**
   * At time of addition a function application can already normalize to something, so
   * we keep both the original, and the normalized version.
   */
  struct FunctionApplicationPair {
    FunctionApplication original;
    FunctionApplication normalized;
    FunctionApplicationPair() {}
    FunctionApplicationPair(const FunctionApplication& original, const FunctionApplication& normalized)
    : original(original), normalized(normalized) {}
    bool isNull() const {
      return !original.isApplication();
    }
  };
  /** Map from ids to the applications */
  std::vector<FunctionApplicationPair> d_applications;

  /** Map from ids to the equality nodes */
  std::vector<EqualityNode> d_equalityNodes;

  /** Number of asserted equalities we have so far */
  context::CDO<DefaultSizeType> d_assertedEqualitiesCount;

  /** Memory for the use-list nodes */
  std::vector<UseListNode> d_useListNodes;

  /**
   * We keep a list of asserted equalities. Not among original terms, but
   * among the class representatives.
   */
  struct Equality {
    /** Left hand side of the equality */
    EqualityNodeId lhs;
    /** Right hand side of the equality */
    EqualityNodeId rhs;
    /** Equality constructor */
    Equality(EqualityNodeId lhs = null_id, EqualityNodeId rhs = null_id)
    : lhs(lhs), rhs(rhs) {}
  };

  /** The ids of the classes we have merged */
  std::vector<Equality> d_assertedEqualities;

  /** The reasons for the equalities */

  /**
   * An edge in the equality graph. This graph is an undirected graph (both edges added)
   * containing the actual asserted equalities.
   */
  class EqualityEdge {

    // The id of the RHS of this equality
    EqualityNodeId d_nodeId;
    // The next edge
    EqualityEdgeId d_nextId;
    // Type of reason for this equality
    MergeReasonType d_mergeType;
    // Reason of this equality
    TNode d_reason;

  public:

    EqualityEdge():
      d_nodeId(null_edge), d_nextId(null_edge), d_mergeType(MERGED_THROUGH_CONGRUENCE) {}

    EqualityEdge(EqualityNodeId nodeId, EqualityNodeId nextId, MergeReasonType type, TNode reason):
      d_nodeId(nodeId), d_nextId(nextId), d_mergeType(type), d_reason(reason) {}

    /** Returns the id of the next edge */
    EqualityEdgeId getNext() const { return d_nextId; }

    /** Returns the id of the target edge node */
    EqualityNodeId getNodeId() const { return d_nodeId; }

    /** The reason of this edge */
    MergeReasonType getReasonType() const { return d_mergeType; }

    /** The reason of this edge */
    TNode getReason() const { return d_reason; }
};

  /**
   * All the equality edges (twice as many as the number of asserted equalities. If an equality
   * t1 = t2 is asserted, the edges added are -> t2, -> t1 (in this order). Hance, having the index
   * of one of the edges you can reconstruct the original equality.
   */
  std::vector<EqualityEdge> d_equalityEdges;

  /**
   * Returns the string representation of the edges.
   */
  std::string edgesToString(EqualityEdgeId edgeId) const;

  /**
   * Map from a node to it's first edge in the equality graph. Edges are added to the front of the
   * list which makes the insertion/backtracking easy.
   */
  std::vector<EqualityEdgeId> d_equalityGraph;

  /** Add an edge to the equality graph */
  void addGraphEdge(EqualityNodeId t1, EqualityNodeId t2, MergeReasonType type, TNode reason);

  /** Returns the equality node of the given node */
  EqualityNode& getEqualityNode(TNode node);

  /** Returns the equality node of the given node */
  const EqualityNode& getEqualityNode(TNode node) const;

  /** Returns the equality node of the given node */
  EqualityNode& getEqualityNode(EqualityNodeId nodeId);

  /** Returns the equality node of the given node */
  const EqualityNode& getEqualityNode(EqualityNodeId nodeId) const;

  /** Returns the id of the node */
  EqualityNodeId getNodeId(TNode node) const;

  /**
   * Merge the class2 into class1
   * @return true if ok, false if to break out
   */
  bool merge(EqualityNode& class1, EqualityNode& class2, std::vector<TriggerId>& triggers);

  /** Undo the mereg of class2 into class1 */
  void undoMerge(EqualityNode& class1, EqualityNode& class2, EqualityNodeId class2Id);

  /** Backtrack the information if necessary */
  void backtrack();

  /**
   * Trigger that will be updated
   */
  struct Trigger {
    /** The current class id of the LHS of the trigger */
    EqualityNodeId classId;
    /** Next trigger for class */
    TriggerId nextTrigger;

    Trigger(EqualityNodeId classId = null_id, TriggerId nextTrigger = null_trigger)
    : classId(classId), nextTrigger(nextTrigger) {}
  };

  /**
   * Vector of triggers. Triggers come in pairs for an
   * equality trigger (t1, t2): one at position 2k for t1, and one at position 2k + 1 for t2. When
   * updating triggers we always know where the other one is (^1).
   */
  std::vector<Trigger> d_equalityTriggers;

  struct TriggerInfo {
    /** The trigger itself */
    Node trigger;
    /** Polarity of the trigger */
    bool polarity;
    TriggerInfo() {}
    TriggerInfo(Node trigger, bool polarity)
    : trigger(trigger), polarity(polarity) {}
  };

  /**
   * Vector of original equalities of the triggers.
   */
  std::vector<TriggerInfo> d_equalityTriggersOriginal;

  /**
   * Context dependent count of triggers
   */
  context::CDO<DefaultSizeType> d_equalityTriggersCount;

  /**
   * Trigger lists per node. The begin id changes as we merge, but the end always points to
   * the actual end of the triggers for this node.
   */
  std::vector<TriggerId> d_nodeTriggers;

  /**
   * Map from ids to wheather they are constants (constants are always 
   * representatives of their class.
   */
  std::vector<bool> d_isConstant;

  /**
   * Map from ids to wheather they are Boolean.
   */
  std::vector<bool> d_isBoolean;

  /**
   * Adds the trigger with triggerId to the beginning of the trigger list of the node with id nodeId.
   */
  void addTriggerToList(EqualityNodeId nodeId, TriggerId triggerId);

  /** Statistics */
  Statistics d_stats;

  /** Add a new function application node to the database, i.e APP t1 t2 */
  EqualityNodeId newApplicationNode(TNode original, EqualityNodeId t1, EqualityNodeId t2, bool isEquality);

  /** Add a new node to the database */
  EqualityNodeId newNode(TNode t);

  /** Propagation queue */
  std::queue<MergeCandidate> d_propagationQueue;

  /** Enqueue to the propagation queue */
  void enqueue(const MergeCandidate& candidate);

  /** Do the propagation */
  void propagate();

  /**
   * Get an explanation of the equality t1 = t2. Returns the asserted equalities that
   * imply t1 = t2. Returns TNodes as the assertion equalities should be hashed somewhere
   * else. 
   */
  void getExplanation(EqualityEdgeId t1Id, EqualityNodeId t2Id, std::vector<TNode>& equalities) const;

  /**
   * Print the equality graph.
   */
  void debugPrintGraph() const;

  /** The true node */
  Node d_true;
  /** True node id */
  EqualityNodeId d_trueId;
  
  /** The false node */
  Node d_false;
  /** False node id */
  EqualityNodeId d_falseId;

  /**
   * Adds an equality of terms t1 and t2 to the database.
   */
  void assertEqualityInternal(TNode t1, TNode t2, TNode reason);

  /**
   * Adds a trigger equality to the database with the trigger node and polarity for notification.
   */
  void addTriggerEqualityInternal(TNode t1, TNode t2, TNode trigger, bool polarity);

  /**
   * This method gets called on backtracks from the context manager.
   */
  void contextNotifyPop() {
    backtrack();
  }

  /**
   * Constructor initialization stuff.
   */
  void init();

  /** Set of trigger terms */
  struct TriggerTermSet {
    /** Set of theories in this set */
    Theory::Set tags;
    /** The trigger terms */
    EqualityNodeId triggers[0];
    /** Returns the theory tags */
    Theory::Set hasTrigger(TheoryId tag) const { return Theory::setContains(tag, tags); }
    /** Returns a trigger by tag */
    EqualityNodeId getTrigger(TheoryId tag) const {
      return triggers[Theory::setIndex(tag, tags)];
    }
  };

  /** Internal tags for creating a new set */
  Theory::Set d_newSetTags;
  
  /** Internal triggers for creating a new set */
  EqualityNodeId d_newSetTriggers[THEORY_LAST];
  
  /** Size of the internal triggers array */
  unsigned d_newSetTriggersSize; 

  /** The information about trigger terms is stored in this easily maintained memory. */
  char* d_triggerDatabase;

  /** Allocated size of the trigger term database */
  DefaultSizeType d_triggerDatabaseAllocatedSize ;

  /** Reference for the trigger terms set */
  typedef DefaultSizeType TriggerTermSetRef;

  /** Null reference */
  static const TriggerTermSetRef null_set_id = (TriggerTermSetRef)(-1);

  /** Create new trigger term set based on the internally set information */
  TriggerTermSetRef newTriggerTermSet();

  /** Get the trigger set give a reference */
  TriggerTermSet& getTriggerTermSet(TriggerTermSetRef ref) {
    Assert(ref < d_triggerDatabaseSize);
    return *(reinterpret_cast<TriggerTermSet*>(d_triggerDatabase + ref));
  }

  /** Get the trigger set give a reference */
  const TriggerTermSet& getTriggerTermSet(TriggerTermSetRef ref) const {
    Assert(ref < d_triggerDatabaseSize);
    return *(reinterpret_cast<const TriggerTermSet*>(d_triggerDatabase + ref));
  }

  /** Used part of the trigger term database */
  context::CDO<DefaultSizeType> d_triggerDatabaseSize;

  struct TriggerSetUpdate {
    EqualityNodeId classId;
    TriggerTermSetRef oldValue;
    TriggerSetUpdate(EqualityNodeId classId = null_id, TriggerTermSetRef oldValue = null_set_id) 
    : classId(classId), oldValue(oldValue) {}
  };

  /**
   * List of trigger updates for backtracking.
   */
  std::vector<TriggerSetUpdate> d_triggerTermSetUpdates;

  /**
   * Size of the individual triggers list.
   */
  context::CDO<unsigned> d_triggerTermSetUpdatesSize;

  /**
   * Map from ids to the individual trigger set representatives.
   */
  std::vector<TriggerTermSetRef> d_nodeIndividualTrigger;

public:

  /**
   * Initialize the equality engine, given the notification class. 
   */
  EqualityEngine(EqualityEngineNotify& notify, context::Context* context, std::string name);

  /**
   * Initialize the equality engine with no notification class. 
   */
  EqualityEngine(context::Context* context, std::string name);

  /**
   * Just a destructor.
   */
  virtual ~EqualityEngine() throw(AssertionException);

  /**
   * Adds a term to the term database.
   */
  void addTerm(TNode t);

  /**
   * Add a kind to treat as function applications.
   */
  void addFunctionKind(Kind fun) {
    d_congruenceKinds |= fun;
  }

  /**
   * Returns true if this kind is used for congruence closure.
   */
  bool isFunctionKind(Kind fun) {
    return d_congruenceKinds.tst(fun);
  }

  /**
   * Adds a function application term to the database.
   */

  /**
   * Check whether the node is already in the database.
   */
  bool hasTerm(TNode t) const;

  /**
   * Adds a predicate p with given polarity. The predicate asserted
   * should be in the coungruence closure kinds (otherwise it's 
   * useless.
   *
   * @param p the (non-negated) predicate
   * @param polarity true if asserting the predicate, false if 
   *                 asserting the negated predicate
   * @param the reason to keep for building explanations
   */
  void assertPredicate(TNode p, bool polarity, TNode reason);

  /**
   * Adds an equality eq with the given polarity to the database.
   *
   * @param eq the (non-negated) equality
   * @param polarity true if asserting the equality, false if 
   *                 asserting the negated equality
   * @param the reason to keep for building explanations
   */
  void assertEquality(TNode eq, bool polarity, TNode reason);

  /**
   * Returns the current representative of the term t.
   */
  TNode getRepresentative(TNode t) const;

  /**
   * Add all the terms where the given term appears as a first child 
   * (directly or implicitly).
   */
  void getUseListTerms(TNode t, std::set<TNode>& output);

  /**
   * Returns true if the two nodes are in the same equivalence class.
   */
  bool areEqual(TNode t1, TNode t2) const;

  /**
   * Get an explanation of the equality t1 = t2 begin true of false. 
   * Returns the reasons (added when asserting) that imply it
   * in the assertions vector.
   */
  void explainEquality(TNode t1, TNode t2, bool polarity, std::vector<TNode>& assertions);

  /**
   * Get an explanation of the predicate being true or false. 
   * Returns the reasons (added when asserting) that imply imply it
   * in the assertions vector.
   */
  void explainPredicate(TNode p, bool polarity, std::vector<TNode>& assertions);

  /**
   * Add term to the set of trigger terms with a corresponding tag. The notify class will get
   * notified when two trigger terms with the same tag become equal or dis-equal. The notification
   * will not happen on all the terms, but only on the ones that are represent the class. Note that
   * a term can be added more than once with different tags, and each tag apperance will merit
   * it's own notification.
   *
   * @param t the trigger term
   * @param tag tag for this trigger (do NOT use THEORY_LAST)
   */
  void addTriggerTerm(TNode t, TheoryId theoryTag);

  /**
   * Returns true if t is a trigger term or in the same equivalence 
   * class as some other trigger term.
   */
  bool isTriggerTerm(TNode t, TheoryId theoryTag) const;

  /**
   * Returns the representative trigger term of the given term.
   *
   * @param t the term to check where isTriggerTerm(t) should be true
   */
  TNode getTriggerTermRepresentative(TNode t, TheoryId theoryTag) const;

  /**
   * Adds a notify trigger for equality. When equality becomes true eqNotifyTriggerEquality
   * will be called with value = true, and when equality becomes false eqNotifyTriggerEquality
   * will be called with value = false.
   */
  void addTriggerEquality(TNode equality);

  /**
   * Adds a notify trigger for the predicate p. When the predicate becomes true
   * eqNotifyTriggerPredicate will be called with value = true, and when equality becomes false
   * eqNotifyTriggerPredicate will be called with value = false.
   */
  void addTriggerPredicate(TNode predicate);

  /**
   * Check whether the two terms are equal.
   */
  bool areEqual(TNode t1, TNode t2);

  /**
   * Check whether the two term are dis-equal.
   */
  bool areDisequal(TNode t1, TNode t2);

  /**
   * Return the number of nodes in the equivalence class containing t
   * Adds t if not already there.
   */
  size_t getSize(TNode t);

};

} // Namespace uf
} // Namespace theory
} // Namespace CVC4

