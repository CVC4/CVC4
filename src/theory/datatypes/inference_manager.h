/*********************                                                        */
/*! \file inference_manager.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Datatypes inference manager
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__DATATYPES__INFERENCE_MANAGER_H
#define CVC4__THEORY__DATATYPES__INFERENCE_MANAGER_H

#include "context/cdhashmap.h"
#include "expr/node.h"
#include "theory/datatypes/inference.h"
#include "theory/inference_manager_buffered.h"
#include "util/statistics_registry.h"

namespace CVC4 {
namespace theory {
namespace datatypes {

/**
 * The datatypes inference manager, which uses the above class for
 * inferences.
 */
class InferenceManager : public InferenceManagerBuffered
{
  friend class DatatypesInference;

 public:
  InferenceManager(Theory& t, TheoryState& state, ProofNodeManager* pnm);
  ~InferenceManager();
  /**
   * Add pending inference, which may be processed as either a fact or
   * a lemma based on mustCommunicateFact in DatatypesInference above.
   *
   * @param conc The conclusion of the inference
   * @param exp The explanation of the inference
   * @param forceLemma Whether this inference *must* be processed as a lemma.
   * Otherwise, it may be processed as a fact or lemma based on
   * mustCommunicateFact.
   * @param i The inference, used for stats and as a hint for constructing
   * the proof of (conc => exp)
   */
  void addPendingInference(Node conc,
                           Node exp,
                           bool forceLemma = false,
                           Inference i = Inference::NONE);
  /**
   * Process the current lemmas and facts. This is a custom method that can
   * be seen as overriding the behavior of calling both doPendingLemmas and
   * doPendingFacts. It determines whether facts should be sent as lemmas
   * or processed internally.
   */
  void process();
  /**
   * Send lemmas with property NONE on the output channel immediately.
   * Returns true if any lemma was sent.
   */
  bool sendLemmas(const std::vector<Node>& lemmas);

 private:
  /**
   * Process datatype inference di. We send a lemma if asLemma is true, and
   * send an internal fact if asLemma is false.
   */
  bool processDtInference(DatatypesInference& di, bool asLemma);
  /**
   * Counts the number of applications of each type of inference processed by
   * the above method.
   */
  HistogramStat<Inference> d_inferences;
};

}  // namespace datatypes
}  // namespace theory
}  // namespace CVC4

#endif
