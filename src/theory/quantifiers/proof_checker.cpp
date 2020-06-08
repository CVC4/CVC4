/*********************                                                        */
/*! \file proof_checker.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of quantifiers proof checker
 **/

#include "theory/quantifiers/proof_checker.h"

#include "expr/skolem_manager.h"
#include "theory/builtin/proof_checker.h"

using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace quantifiers {

void QuantifiersProofRuleChecker::registerTo(ProofChecker* pc)
{
  // add checkers
  pc->registerChecker(PfRule::WITNESS_INTRO, this);
  pc->registerChecker(PfRule::EXISTS_INTRO, this);
  pc->registerChecker(PfRule::SKOLEMIZE, this);
  pc->registerChecker(PfRule::INSTANTIATE, this);
}

Node QuantifiersProofRuleChecker::checkInternal(
    PfRule id, const std::vector<Node>& children, const std::vector<Node>& args)
{
  NodeManager* nm = NodeManager::currentNM();
  // compute what was proven
  if (id == PfRule::WITNESS_INTRO || id == PfRule::EXISTS_INTRO)
  {
    Assert(children.size() == 1);
    Assert(args.size() == 1);
    SkolemManager* sm = nm->getSkolemManager();
    Node p = SkolemManager::getSkolemForm(children[0]);
    Node t = SkolemManager::getSkolemForm(args[0]);
    Node exists = sm->mkExistential(t, p);
    if (id == PfRule::EXISTS_INTRO)
    {
      return SkolemManager::getWitnessForm(exists);
    }
    std::vector<Node> skolems;
    sm->mkSkolemize(exists, skolems, "k");
    Assert (skolems.size()==1);
    return SkolemManager::getWitnessForm(skolems[0]);
  }
  else if (id == PfRule::SKOLEMIZE)
  {
    Assert(children.size() == 1);
    Assert(args.empty());
    if (children[0].getKind() != NOT || children[0][0].getKind() != FORALL)
    {
      return Node::null();
    }
    SkolemManager* sm = nm->getSkolemManager();
    std::vector<Node> echildren;
    echildren.insert(
        echildren.begin(), children[0][0].begin(), children[0][0].end());
    Node exists = nm->mkNode(EXISTS, echildren);
    exists= SkolemManager::getSkolemForm(exists);
    std::vector<Node> skolems;
    Node res = sm->mkSkolemize(exists, skolems, "k");
    res = SkolemManager::getWitnessForm(res);
    return nm->mkNode(OR, children[0].notNode(), res);
  }
  else if (id == PfRule::INSTANTIATE)
  {
    Assert(children.size() == 1);
    if (children[0].getKind() != FORALL
        || args.size() != children[0][0].getNumChildren())
    {
      return Node::null();
    }
    Node body = SkolemManager::getSkolemForm(children[0][1]);
    std::vector<Node> vars;
    std::vector<Node> subs;
    for (unsigned i = 0, nargs = args.size(); i < nargs; i++)
    {
      vars.push_back(children[0][0][i]);
      subs.push_back(SkolemManager::getSkolemForm(args[i]));
    }
    Node inst =
        body.substitute(vars.begin(), vars.end(), subs.begin(), subs.end());
    inst = SkolemManager::getWitnessForm(inst);
    return nm->mkNode(OR, children[0].notNode(), inst);
  }

  // no rule
  return Node::null();
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC4
