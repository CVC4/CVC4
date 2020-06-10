/*********************                                                        */
/*! \file eager_proof_generator.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of the abstract proof generator class
 **/

#include "theory/eager_proof_generator.h"

#include "expr/proof_node_manager.h"

namespace CVC4 {
namespace theory {

EagerProofGenerator::EagerProofGenerator(ProofNodeManager* pnm,
                                         context::Context* c)
    : d_pnm(pnm), d_proofs(c == nullptr ? &d_context : c)
{
}

void EagerProofGenerator::setProofFor(Node f, std::shared_ptr<ProofNode> pf)
{
  // pf should prove f
  Assert(pf->getResult() == f);
  d_proofs[f] = pf;
}

std::shared_ptr<ProofNode> EagerProofGenerator::getProofFor(Node f)
{
  NodeProofNodeMap::iterator it = d_proofs.find(f);
  if (it == d_proofs.end())
  {
    return nullptr;
  }
  return (*it).second;
}

bool EagerProofGenerator::hasProofFor(Node f)
{
  return d_proofs.find(f) != d_proofs.end();
}

TrustNode EagerProofGenerator::mkTrustNode(Node n,
                                           std::shared_ptr<ProofNode> pf,
                                           bool isConflict)
{
  if (pf == nullptr)
  {
    return TrustNode::null();
  }
  if (isConflict)
  {
    // this shouldnt modify the key
    Node ckey = TrustNode::getConflictProven(n);
    setProofFor(ckey, pf);
    // we can now return the trust node
    return TrustNode::mkTrustConflict(n, this);
  }
  // this shouldnt modify the key
  Node lkey = TrustNode::getLemmaProven(n);
  setProofFor(lkey, pf);
  // we can now return the trust node
  return TrustNode::mkTrustLemma(n, this);
}

TrustNode EagerProofGenerator::mkTrustNode(Node n,
                                           PfRule id,
                                           const std::vector<Node>& args,
                                           bool isConflict)
{
  std::vector<std::shared_ptr<ProofNode>> children;
  std::shared_ptr<ProofNode> pf = d_pnm->mkNode(id, children, args, n);
  return mkTrustNode(n, pf, isConflict);
}

TrustNode EagerProofGenerator::mkTrustedPropagation(
    Node n, Node exp, std::shared_ptr<ProofNode> pf)
{
  if (pf == nullptr)
  {
    return TrustNode::null();
  }
  Node pekey = TrustNode::getPropExpProven(n, exp);
  setProofFor(pekey, pf);
  return TrustNode::mkTrustPropExp(n, exp, this);
}

TrustNode EagerProofGenerator::mkTrustNodeSplit(Node f)
{
  // make the lemma
  Node lem = f.orNode(f.notNode());
  std::vector<Node> args;
  return mkTrustNode(lem, PfRule::SPLIT, args, false);
}

}  // namespace theory
}  // namespace CVC4
