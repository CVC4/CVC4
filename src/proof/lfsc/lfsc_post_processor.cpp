/*********************                                                        */
/*! \file lfsc_post_processor.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of the Lfsc post proccessor
 **/

#include "proof/lfsc/lfsc_post_processor.h"
#include "proof/lfsc/lfsc_printer.h"

#include "expr/lazy_proof.h"
#include "expr/proof_checker.h"
#include "expr/proof_node_algorithm.h"
#include "expr/proof_node_updater.h"
#include "options/proof_options.h"

using namespace CVC4::kind;

namespace CVC4 {
namespace proof {

LfscProofPostprocessCallback::LfscProofPostprocessCallback(
    ProofNodeManager* pnm)
    : d_pnm(pnm), d_pc(pnm->getChecker())
{
}

LfscProofPostprocess::LfscProofPostprocess(ProofNodeManager* pnm)
    : d_cb(new proof::LfscProofPostprocessCallback(pnm)), d_pnm(pnm)
{
}

bool LfscProofPostprocessCallback::shouldUpdate(std::shared_ptr<ProofNode> pn,
                                                bool& continueUpdate)
{
  return pn->getRule() != PfRule::LFSC_RULE;
}

bool LfscProofPostprocessCallback::update(Node res,
                                          PfRule id,
                                          const std::vector<Node>& children,
                                          const std::vector<Node>& args,
                                          CDProof* cdp,
                                          bool& continueUpdate)
{
  NodeManager* nm = NodeManager::currentNM();
  Assert(id != PfRule::LFSC_RULE);

  // convert children to internal form
  std::vector<Node> ics;
  for (const Node& c : children)
  {
    ics.push_back(d_tproc.convert(c));
  }

  // convert arguments to internal form
  std::vector<Node> ias;
  for (const Node& a : args)
  {
    ias.push_back(d_tproc.convert(a));
  }

  switch (id)
  {
    case PfRule::CHAIN_RESOLUTION:
    {
      // turn into binary resolution
      Node cur = children[0];
      for (size_t i = 1, size = children.size(); i < size; i++)
      {
        std::vector<Node> newChildren{cur, children[i]};
        std::vector<Node> newArgs{args[(i - 1) * 2], args[(i - 1) * 2 + 1]};
        cur = d_pc->checkDebug(PfRule::RESOLUTION, newChildren, newArgs);
        cdp->addStep(cur, PfRule::RESOLUTION, newChildren, newArgs);
      }
    }
    break;
    case PfRule::TRANS:
    {
      if (children.size() > 2)
      {
        // turn into binary
        Node cur = children[0];
        for (size_t i = 1, size = children.size(); i < size; i++)
        {
          std::vector<Node> newChildren{cur, children[i]};
          cur = d_pc->checkDebug(PfRule::TRANS, newChildren, {});
          cdp->addStep(cur, PfRule::TRANS, newChildren, {});
        }
        return true;
      }
      return false;
    }
    break;
    case PfRule::CONG:
    {
      Assert(res.getKind() == EQUAL);
      Assert(res[0].getOperator() == res[1].getOperator());
      Kind k = res[0].getKind();
      // We are proving f(t1, ..., tn) = f(s1, ..., sn), nested.
      // First, get the operator, which will be used for printing the base
      // REFL step. Notice this may be for interpreted or uninterpreted
      // function symbols.
      Node op = d_tproc.getOperatorForTerm(res[0]);
      Assert(!op.isNull());
      // initial base step is REFL
      Node opEq = op.eqNode(op);
      cdp->addStep(opEq, PfRule::REFL, {}, {op});
      size_t nchildren = children.size();
      Node nullTerm = LfscTermProcessor::getNullTerminator(k);
      // Are we doing congruence of an n-ary operator? If so, notice that op
      // is a binary operator and we must apply congruence in a special way.
      if (ExprManager::isNAryKind(k) && (nchildren >= 2 || !nullTerm.isNull()))
      {
        // get the null terminator for the kind, which may mean we are doing
        // a special kind of congruence for n-ary kinds whose base is a REFL
        // step for the null terminator.
        Node currEq;
        if (!nullTerm.isNull())
        {
          currEq = nullTerm.eqNode(nullTerm);
          // if we have a null terminator, we do a final conclusion to add
          // the null terminator to both sides
          cdp->addStep(currEq, PfRule::REFL, {}, {nullTerm});
        }
        else
        {
          currEq = children[nchildren - 1];
        }
        for (size_t i = 0; i < nchildren; i++)
        {
          size_t ii = (nchildren - 1) - i;
          Node argAppEq =
              nm->mkNode(HO_APPLY, op, children[ii][0])
                  .eqNode(nm->mkNode(HO_APPLY, op, children[ii][1]));
          addLfscRule(cdp, argAppEq, {opEq, children[ii]}, LfscRule::CONG, {});
          // now, congruence to the current equality
          Node nextEq;
          if (ii == 0)
          {
            // use final conclusion
            nextEq = res;
          }
          else
          {
            // otherwise continue to apply
            nextEq = nm->mkNode(HO_APPLY, argAppEq[0], currEq[0])
                         .eqNode(nm->mkNode(HO_APPLY, argAppEq[1], currEq[1]));
          }
          addLfscRule(cdp, nextEq, {argAppEq, currEq}, LfscRule::CONG, {});
          currEq = nextEq;
        }
      }
      else
      {
        Node curL = op;
        Node curR = op;
        Node currEq = opEq;
        for (size_t i = 0; i < nchildren; i++)
        {
          // CONG rules for each child
          Node nextEq;
          if (i + 1 == nchildren && nullTerm.isNull())
          {
            // if we are at the end, we prove the final equality, unless we
            // have a null terminator.
            nextEq = res;
          }
          else
          {
            curL = nm->mkNode(HO_APPLY, curL, children[i][0]);
            curR = nm->mkNode(HO_APPLY, curR, children[i][1]);
            nextEq = curL.eqNode(curR);
          }
          addLfscRule(cdp, nextEq, {currEq, children[i]}, LfscRule::CONG, {});
          currEq = nextEq;
        }
      }
      return true;
    }
    break;
    case PfRule::AND_ELIM:
    {
      uint32_t i;
      bool b = ProofRuleChecker::getUInt32(args[0], i);
      Assert(b);
      // we start with the n-ary AND
      Node cur = children[0];
      std::vector<Node> cchildren(cur.begin(), cur.end());
      // get its chain form
      Node curChain = mkChain(AND, cchildren);
      // currently there is assymmetry on how the first step below is handled,
      // where from n-ary AND we conclude a chained AND. This could be made
      // symmetric by an explicit, internal-only rule to conclude chained form
      // from n-ary form.
      for (uint32_t j = 0; j < i; j++)
      {
        Node cur_r = j == 0 ? curChain[1] : cur[1];
        addLfscRule(cdp, cur_r, {cur}, LfscRule::CNF_AND_POS_2, {});
        cur = cur_r;
      }
      // We always get the left child, even if we are at the end
      // (i=cchildren.size()-1) or at the beginning (i=0). For the end case,
      // we are taking F from (and F true), in the beginning case, we are
      // taking F from the original n-ary (and F ...).
      addLfscRule(cdp, cur[0], {cur}, LfscRule::CNF_AND_POS_1, {});
    }
    break;
    default: return false; break;
  }
  return true;
}

void LfscProofPostprocessCallback::addLfscRule(
    CDProof* cdp,
    Node conc,
    const std::vector<Node>& children,
    LfscRule lr,
    const std::vector<Node>& args)
{
  std::vector<Node> largs;
  largs.push_back(mkLfscRuleNode(lr));
  largs.push_back(conc);
  largs.insert(largs.end(), args.begin(), args.end());
  cdp->addStep(conc, PfRule::LFSC_RULE, children, largs);
}

Node LfscProofPostprocessCallback::mkChain(Kind k,
                                           const std::vector<Node>& children)
{
  Assert(!children.empty());
  NodeManager* nm = NodeManager::currentNM();
  size_t nchildren = children.size();
  size_t i = 0;
  // do we have a null terminator? If so, we start with it.
  Node ret = LfscTermProcessor::getNullTerminator(k);
  if (ret.isNull())
  {
    ret = children[nchildren - 1];
    i = 1;
  }
  while (i < nchildren)
  {
    ret = nm->mkNode(k, children[(nchildren - 1) - i], ret);
    i++;
  }
  return ret;
}

void LfscProofPostprocess::process(std::shared_ptr<ProofNode> pf)
{
  ProofNodeUpdater updater(d_pnm, *(d_cb.get()));
  updater.process(pf);
}

}  // namespace proof
}  // namespace CVC4
