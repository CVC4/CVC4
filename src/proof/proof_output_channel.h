/*********************                                                        */
/*! \file proof_output_channel.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Guy Katz, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 **/

#include "cvc4_private.h"

#ifndef __CVC4__PROOF_OUTPUT_CHANNEL_H
#define __CVC4__PROOF_OUTPUT_CHANNEL_H

#include <unordered_set>

#include "expr/node.h"
#include "util/proof.h"
#include "theory/output_channel.h"

namespace CVC4 {

class ProofOutputChannel : public theory::OutputChannel {
public:
  ProofOutputChannel();
  ~ProofOutputChannel() override {}

  /** This may be called exactly once. */
  void conflict(TNode n, Proof* pf = nullptr) override;
  bool propagate(TNode x) override;
  theory::LemmaStatus lemma(TNode n, ProofRule rule, bool, bool, bool) override;
  theory::LemmaStatus splitLemma(TNode, bool) override;
  void requirePhase(TNode n, bool b) override;
  bool flipDecision() override;
  void setIncomplete() override;

  bool hasConflict() const { return !d_conflict.isNull(); }
  Node getConflict() const { return d_conflict; }
  /** Requires hasConflict() to hold. */
  Proof* getConflictProof();
  Node getLastLemma() const { return d_lemma; }

 private:
  Node d_conflict;
  Proof* d_proof;
  Node d_lemma;
  std::set<Node> d_propagations;
};/* class ProofOutputChannel */

class MyPreRegisterVisitor {
  theory::Theory* d_theory;
  std::unordered_set<TNode, TNodeHashFunction> d_visited;
public:
  typedef void return_type;
  MyPreRegisterVisitor(theory::Theory* theory);
  bool alreadyVisited(TNode current, TNode parent);
  void visit(TNode current, TNode parent);
  void start(TNode node);
  void done(TNode node);
}; /* class MyPreRegisterVisitor */

} /* CVC4 namespace */

#endif /* __CVC4__PROOF_OUTPUT_CHANNEL_H */
