/*********************                                                        */
/*! \file infer_proof_cons.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of inference to proof conversion
 **/

#include "theory/strings/infer_proof_cons.h"

#include "expr/proof_skolem_cache.h"
#include "options/strings_options.h"
#include "options/smt_options.h"
#include "theory/builtin/proof_checker.h"
#include "theory/rewriter.h"
#include "theory/strings/regexp_operation.h"
#include "theory/strings/theory_strings_utils.h"

using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace strings {

InferProofCons::InferProofCons(context::Context* c,
                               ProofNodeManager* pnm,
                               SequencesStatistics& statistics,
                               bool pfEnabled)
    : d_pnm(pnm),
      d_lazyFactMap(c),
      d_psb(pnm->getChecker()),
      d_statistics(statistics),
      d_pfEnabled(pfEnabled)
{
}

void InferProofCons::notifyFact(const InferInfo& ii)
{
  Node fact = ii.d_conc;
  Trace("strings-ipc-debug")
      << "InferProofCons::notifyFact: " << ii << std::endl;
  if (d_lazyFactMap.find(fact) != d_lazyFactMap.end())
  {
    Trace("strings-ipc-debug") << "...duplicate!" << std::endl;
    return;
  }
  if (fact.getKind() == EQUAL)
  {
    Node symFact = fact[1].eqNode(fact[0]);
    if (d_lazyFactMap.find(symFact) != d_lazyFactMap.end())
    {
      Trace("strings-ipc-debug") << "...duplicate (sym)!" << std::endl;
      return;
    }
  }
  std::shared_ptr<InferInfo> iic = std::make_shared<InferInfo>(ii);
  d_lazyFactMap.insert(ii.d_conc, iic);
}

Node InferProofCons::convert(const InferInfo& ii,
                             ProofStep& ps,
                             bool& useBuffer)
{
  return convert(ii.d_id, ii.d_idRev, ii.d_conc, ii.d_ant, ps, useBuffer);
}

Node InferProofCons::convert(Inference infer,
                             bool isRev,
                             Node conc,
                             const std::vector<Node>& exp,
                             ProofStep& ps,
                             bool& useBuffer)
{
  // the conclusion is the same
  useBuffer = false;
  // Must flatten children with respect to AND to be ready to explain.
  // We store the index where each flattened vector begins, since some
  // explanations are "grouped".
  size_t expIndex = 0;
  std::map<size_t, size_t> startExpIndex;
  for (const Node& ec : exp)
  {
    if (d_pfEnabled)
    {
      // store the index in the flattened vector
      startExpIndex[expIndex] = ps.d_children.size();
      expIndex++;
    }
    utils::flattenOp(AND, ec, ps.d_children);
  }
  // only keep stats if we process it here
  d_statistics.d_inferences << infer;
  if (!d_pfEnabled)
  {
    // don't care about proofs, return now
    d_statistics.d_inferencesNoPf << infer;
    return conc;
  }
  // debug print
  if (Trace.isOn("strings-ipc-debug"))
  {
    Trace("strings-ipc-debug") << "InferProofCons::convert: " << infer
                               << (isRev ? " :rev " : " ") << conc << std::endl;
    for (const Node& ec : exp)
    {
      Trace("strings-ipc-debug") << "    e: " << ec << std::endl;
    }
  }
  // try to find a set of proof steps to incorporate into the buffer
  d_psb.clear();
  NodeManager* nm = NodeManager::currentNM();
  Node nodeIsRev = nm->mkConst(isRev);
  std::vector<Node> emptyVec;
  switch (infer)
  {
    // ========================== equal by substitution+rewriting
    case Inference::I_NORM_S:
    case Inference::I_CONST_MERGE:
    case Inference::I_NORM:
    case Inference::LEN_NORM:
    case Inference::NORMAL_FORM:
    case Inference::CODE_PROXY:
    {
      ps.d_args.push_back(conc);
      // will attempt this rule
      ps.d_rule = PfRule::MACRO_SR_PRED_INTRO;
    }
    break;
    // ========================== substitution + rewriting
    case Inference::RE_NF_CONFLICT:
    case Inference::EXTF:
    case Inference::EXTF_N:
    {
      if (conc.isConst())
      {
        std::vector<Node> exps;
        exps.insert(exps.end(), ps.d_children.begin(), ps.d_children.end() - 1);
        Node src = ps.d_children[ps.d_children.size() - 1];
        if (convertPredTransform(src, conc, exps))
        {
          useBuffer = true;
        }
      }
      else
      {
        // use the predicate version
        ps.d_args.push_back(conc);
        ps.d_rule = PfRule::MACRO_SR_PRED_INTRO;
      }
      // minor optimization: apply to LHS of equality (RHS is already reduced)
      // although notice the case above is also a valid proof.
      // ps.d_args.push_back(conc[0]);
      // ps.d_rule = PfRule::MACRO_SR_EQ_INTRO;
      // This doesn't quite work due for symbolic lemmas.
    }
    break;
    // ========================== substitution+rewriting+Boolean entailment
    case Inference::EXTF_D:
    case Inference::EXTF_D_N: break;
    // ========================== equal by substitution+rewriting+rewrite pred
    case Inference::I_CONST_CONFLICT: break;
    // ========================== rewrite pred
    case Inference::EXTF_EQ_REW:
    case Inference::INFER_EMP:
    {
      // need the "extended equality rewrite"
      MethodId ids = MethodId::SB_DEFAULT;
      MethodId idr = MethodId::RW_REWRITE_EQ_EXT;
      addMethodIds(ps.d_args, ids, idr);
      ps.d_rule = PfRule::MACRO_SR_PRED_ELIM;
    }
    break;
    // ========================== equal by substitution+rewriting+CTN_NOT_EQUAL
    case Inference::F_NCTN:
    case Inference::N_NCTN: break;
    // ========================== substitution+rewriting, CONCAT_EQ, ...
    case Inference::F_CONST:
    case Inference::F_UNIFY:
    case Inference::F_ENDPOINT_EMP:
    case Inference::F_ENDPOINT_EQ:
    case Inference::N_EQ_CONF:
    case Inference::N_CONST:
    case Inference::N_UNIFY:
    case Inference::N_ENDPOINT_EMP:
    case Inference::N_ENDPOINT_EQ:
    case Inference::SSPLIT_CST_PROP:
    case Inference::SSPLIT_VAR_PROP:
    case Inference::SSPLIT_CST:
    case Inference::SSPLIT_VAR:
    case Inference::DEQ_DISL_FIRST_CHAR_STRING_SPLIT:
    case Inference::DEQ_DISL_STRINGS_SPLIT:
    {
      Trace("strings-ipc-core") << "Generate core rule for " << infer
                                << " (rev=" << isRev << ")" << std::endl;
      // All of the above inferences have the form:
      //   <explanation for why t and s have the same prefix/suffix> ^
      //   t = s ^
      //  <length constraint>?
      // We call t=s the "main equality" below. The length constraint is
      // optional, which we split on below.
      size_t nchild = ps.d_children.size();
      size_t mainEqIndex = 0;
      bool mainEqIndexSet = false;
      // the length constraint
      std::vector<Node> lenConstraint;
      // these inferences have a length constraint as the last explain
      if (infer == Inference::N_UNIFY || infer == Inference::F_UNIFY
          || infer == Inference::SSPLIT_CST || infer == Inference::SSPLIT_VAR
          || infer == Inference::SSPLIT_VAR_PROP
          || infer == Inference::SSPLIT_CST_PROP)
      {
        if (exp.size() >= 2)
        {
          std::map<size_t, size_t>::iterator itsei =
              startExpIndex.find(exp.size() - 1);
          if (itsei != startExpIndex.end())
          {
            // The index of the "main" equality is the last equality before
            // the length explanation.
            mainEqIndex = itsei->second - 1;
            mainEqIndexSet = true;
            // the remainder is the length constraint
            lenConstraint.insert(lenConstraint.end(),
                                 ps.d_children.begin() + mainEqIndex + 1,
                                 ps.d_children.end());
          }
        }
      }
      else
      {
        if (nchild >= 1)
        {
          // The index of the main equality is the last child.
          mainEqIndex = nchild - 1;
          mainEqIndexSet = true;
        }
      }
      Node mainEq;
      if (mainEqIndexSet)
      {
        mainEq = ps.d_children[mainEqIndex];
        Trace("strings-ipc-core") << "Main equality " << mainEq << " at index "
                                  << mainEqIndex << std::endl;
      }
      if (mainEq.isNull() || mainEq.getKind() != EQUAL)
      {
        Trace("strings-ipc-core")
            << "...failed to find main equality" << std::endl;
        // Assert(false);
        break;
      }
      // apply MACRO_SR_PRED_ELIM using equalities up to the main eq
      std::vector<Node> childrenSRew;
      childrenSRew.push_back(mainEq);
      childrenSRew.insert(childrenSRew.end(),
                          ps.d_children.begin(),
                          ps.d_children.begin() + mainEqIndex);
      Node mainEqSRew =
          d_psb.tryStep(PfRule::MACRO_SR_PRED_ELIM, childrenSRew, emptyVec);
      if (CDProof::isSame(mainEqSRew, mainEq))
      {
        Trace("strings-ipc-core") << "...undo step" << std::endl;
        // not necessary
        d_psb.popStep();
      }
      else if (mainEqSRew == conc)
      {
        Trace("strings-ipc-core") << "...success after rewrite!" << std::endl;
        useBuffer = true;
        break;
      }
      Trace("strings-ipc-core")
          << "Main equality after subs+rewrite " << mainEqSRew << std::endl;
      // now, apply CONCAT_EQ to get the remainder
      std::vector<Node> childrenCeq;
      childrenCeq.push_back(mainEqSRew);
      std::vector<Node> argsCeq;
      argsCeq.push_back(nodeIsRev);
      Node mainEqCeq = d_psb.tryStep(PfRule::CONCAT_EQ, childrenCeq, argsCeq);
      Trace("strings-ipc-core")
          << "Main equality after CONCAT_EQ " << mainEqCeq << std::endl;
      if (mainEqCeq.isNull() || mainEqCeq.getKind() != EQUAL)
      {
        // fail
        break;
      }
      else if (mainEqCeq == mainEqSRew)
      {
        Trace("strings-ipc-core") << "...undo step" << std::endl;
        // not necessary, probably first component of equality
        d_psb.popStep();
      }
      // Now, mainEqCeq is an equality t ++ ... == s ++ ... where the
      // inference involved t and s.
      if (infer == Inference::N_ENDPOINT_EQ
          || infer == Inference::N_ENDPOINT_EMP
          || infer == Inference::F_ENDPOINT_EQ
          || infer == Inference::F_ENDPOINT_EMP)
      {
        // should be equal to conclusion already, or rewrite to it.
        // Notice that this step is necessary to handle the "rproc"
        // optimization in processSimpleNEq. Alternatively, this could
        // possibly be done by CONCAT_EQ with !isRev.
        std::vector<Node> cexp;
        if (convertPredTransform(mainEqCeq,
                                 conc,
                                 cexp,
                                 MethodId::SB_DEFAULT,
                                 MethodId::RW_REWRITE_EQ_EXT))
        {
          Trace("strings-ipc-core") << "Transformed to " << conc
                                    << " via pred transform" << std::endl;
          // success
          useBuffer = true;
          Trace("strings-ipc-core") << "...success!" << std::endl;
        }
        else
        {
          // TODO: EMP variants are ti = "" where t1 ++ ... ++ tn == "",
          // however, these are very rare applied, let alone for
          // 2+ children.
        }
      }
      else if (infer == Inference::N_CONST || infer == Inference::F_CONST
               || infer == Inference::N_EQ_CONF)
      {
        // should be a constant conflict
        std::vector<Node> childrenC;
        childrenC.push_back(mainEqCeq);
        std::vector<Node> argsC;
        argsC.push_back(nodeIsRev);
        Node mainEqC = d_psb.tryStep(PfRule::CONCAT_CONFLICT, childrenC, argsC);
        if (mainEqC == conc)
        {
          useBuffer = true;
          Trace("strings-ipc-core") << "...success!" << std::endl;
        }
      }
      else
      {
        std::vector<Node> tvec;
        std::vector<Node> svec;
        utils::getConcat(mainEqCeq[0], tvec);
        utils::getConcat(mainEqCeq[1], svec);
        Node t0 = tvec[isRev ? tvec.size() - 1 : 0];
        Node s0 = svec[isRev ? svec.size() - 1 : 0];
        bool applySym = false;
        // may need to apply symmetry
        if ((infer == Inference::SSPLIT_CST
             || infer == Inference::SSPLIT_CST_PROP)
            && t0.isConst())
        {
          Assert(!s0.isConst());
          applySym = true;
          std::swap(t0, s0);
        }
        if (infer == Inference::N_UNIFY || infer == Inference::F_UNIFY)
        {
          if (conc.getKind() != EQUAL)
          {
            break;
          }
          // one side should match, the other side could be a split constant
          if (conc[0] != t0 && conc[1] != s0)
          {
            applySym = true;
            std::swap(t0, s0);
          }
          Assert(conc[0].isConst() == t0.isConst());
          Assert(conc[1].isConst() == s0.isConst());
        }
        PfRule rule = PfRule::UNKNOWN;
        // the form of the required length constraint expected by the proof
        Node lenReq;
        bool lenSuccess = false;
        if (infer == Inference::N_UNIFY || infer == Inference::F_UNIFY)
        {
          // the required premise for unify is always len(x) = len(y),
          // however the explanation may not be literally this. Thus, we
          // need to reconstruct a proof from the given explanation.
          // it should be the case that lenConstraint => lenReq.
          // We use terms in the conclusion equality, not t0, s0 here.
          lenReq = nm->mkNode(STRING_LENGTH, conc[0])
                       .eqNode(nm->mkNode(STRING_LENGTH, conc[1]));
          lenSuccess = convertLengthPf(lenReq, lenConstraint);
          rule = PfRule::CONCAT_UNIFY;
        }
        else if (infer == Inference::SSPLIT_VAR)
        {
          // it should be the case that lenConstraint => lenReq
          lenReq = nm->mkNode(STRING_LENGTH, t0)
                       .eqNode(nm->mkNode(STRING_LENGTH, s0))
                       .notNode();
          lenSuccess = convertLengthPf(lenReq, lenConstraint);
          rule = PfRule::CONCAT_SPLIT;
        }
        else if (infer == Inference::SSPLIT_CST)
        {
          // it should be the case that lenConstraint => lenReq
          lenReq = nm->mkNode(STRING_LENGTH, t0)
                       .eqNode(nm->mkConst(Rational(0)))
                       .notNode();
          lenSuccess = convertLengthPf(lenReq, lenConstraint);
          rule = PfRule::CONCAT_CSPLIT;
        }
        else if (infer == Inference::SSPLIT_VAR_PROP)
        {
          // it should be the case that lenConstraint => lenReq
          for (unsigned r = 0; r < 2; r++)
          {
            lenReq = nm->mkNode(GT,
                                nm->mkNode(STRING_LENGTH, t0),
                                nm->mkNode(STRING_LENGTH, s0));
            if (convertLengthPf(lenReq, lenConstraint))
            {
              lenSuccess = true;
              break;
            }
            if (r == 0)
            {
              // may be the other direction
              applySym = true;
              std::swap(t0, s0);
            }
          }
          rule = PfRule::CONCAT_LPROP;
        }
        else if (infer == Inference::SSPLIT_CST_PROP)
        {
          // it should be the case that lenConstraint => lenReq
          lenReq = nm->mkNode(STRING_LENGTH, t0)
                       .eqNode(nm->mkConst(Rational(0)))
                       .notNode();
          lenSuccess = convertLengthPf(lenReq, lenConstraint);
          rule = PfRule::CONCAT_CPROP;
        }
        if (!lenSuccess)
        {
          Trace("strings-ipc-core")
              << "...failed due to length constraint" << std::endl;
          break;
        }
        // apply symmetry if necessary
        if (applySym)
        {
          std::vector<Node> childrenSymm;
          childrenSymm.push_back(mainEqCeq);
          // TODO: this explicit step may not be necessary
          mainEqCeq = d_psb.tryStep(PfRule::SYMM, childrenSymm, emptyVec);
          Trace("strings-ipc-core")
              << "Main equality after SYMM " << mainEqCeq << std::endl;
        }
        if (rule != PfRule::UNKNOWN)
        {
          Trace("strings-ipc-core")
              << "Core rule length requirement is " << lenReq << std::endl;
          // apply the given rule
          std::vector<Node> childrenMain;
          childrenMain.push_back(mainEqCeq);
          childrenMain.push_back(lenReq);
          std::vector<Node> argsMain;
          argsMain.push_back(nodeIsRev);
          Node mainEqMain = d_psb.tryStep(rule, childrenMain, argsMain);
          Trace("strings-ipc-core") << "Main equality after " << rule << " "
                                    << mainEqMain << std::endl;
          if (mainEqMain == mainEqCeq)
          {
            Trace("strings-ipc-core") << "...undo step" << std::endl;
            // not necessary, probably first component of equality
            d_psb.popStep();
          }
          // either equal or rewrites to it
          std::vector<Node> cexp;
          if (convertPredTransform(mainEqMain, conc, cexp))
          {
            // requires that length success is also true
            useBuffer = true;
            Trace("strings-ipc-core") << "...success" << std::endl;
          }
          else
          {
            Trace("strings-ipc-core") << "...fail" << std::endl;
          }
        }
        else
        {
          Assert(false);
        }
      }
    }
    break;
    // ========================== Boolean split
    case Inference::CARD_SP:
    case Inference::LEN_SPLIT:
    case Inference::LEN_SPLIT_EMP:
    case Inference::DEQ_DISL_EMP_SPLIT:
    case Inference::DEQ_DISL_FIRST_CHAR_EQ_SPLIT:
    case Inference::DEQ_STRINGS_EQ:
    case Inference::DEQ_LENS_EQ:
    case Inference::DEQ_LENGTH_SP:
    {
      if (conc.getKind() != OR)
      {
        Assert(false);
      }
      else
      {
        ps.d_rule = PfRule::SPLIT;
        ps.d_args.push_back(conc[0]);
      }
    }
    break;
    // ========================== Regular expression unfolding
    case Inference::RE_UNFOLD_POS:
    case Inference::RE_UNFOLD_NEG:
    {
      if (infer == Inference::RE_UNFOLD_POS)
      {
        ps.d_rule = PfRule::RE_UNFOLD_POS;
      }
      else
      {
        ps.d_rule = PfRule::RE_UNFOLD_NEG;
        // it may be an optimized form of concat simplification
        Assert(ps.d_children.size() == 1);
        Node mem = ps.d_children[0];
        Assert(mem.getKind() == NOT && mem[0].getKind() == STRING_IN_REGEXP);
        if (mem[0][1].getKind() == REGEXP_CONCAT)
        {
          unsigned index;
          Node reLen = RegExpOpr::getRegExpConcatFixed(mem[0][1], index);
          // if we can find a fixed length for a component, use the optimized
          // version
          if (!reLen.isNull())
          {
            ps.d_rule = PfRule::RE_UNFOLD_NEG_CONCAT_FIXED;
          }
        }
      }
    }
    break;
    // ========================== Reduction
    case Inference::CTN_POS:
    {
      if (ps.d_children.size() != 1)
      {
        break;
      }
      std::vector<Node> args;
      args.push_back(ps.d_children[0]);
      // variant 1 for eager reduction
      args.push_back(nm->mkConst(Rational(1)));
      Node res = d_psb.tryStep(PfRule::STRINGS_EAGER_REDUCTION, emptyVec, args);
      if (res.isNull())
      {
        break;
      }
      // contains(x,t) => x = k1 ++ t ++ k2
      std::vector<Node> tiChildren;
      tiChildren.push_back(ps.d_children[0]);
      Node ctnt = d_psb.tryStep(PfRule::TRUE_INTRO, tiChildren, emptyVec);
      if (ctnt.isNull() || ctnt.getKind() != EQUAL)
      {
        break;
      }
      std::vector<Node> tchildren;
      tchildren.push_back(ctnt);
      // apply substitution { contains(x,t) -> true } and rewrite
      if (convertPredTransform(res, conc, tchildren))
      {
        useBuffer = true;
      }
    }
    break;
    case Inference::REDUCTION:
    {
      size_t nchild = conc.getNumChildren();
      Node mainEq;
      if (conc.getKind() == EQUAL)
      {
        mainEq = conc;
      }
      else if (conc.getKind() == AND && conc[nchild - 1].getKind() == EQUAL)
      {
        mainEq = conc[nchild - 1];
      }
      if (mainEq.isNull())
      {
        Trace("strings-ipc-red") << "Bad Reduction: " << conc << std::endl;
        Assert(false);
        break;
      }
      std::vector<Node> argsRed;
      // the left hand side of the last conjunct is the term we are reducing
      argsRed.push_back(mainEq[0]);
      Node red = d_psb.tryStep(PfRule::STRINGS_REDUCTION, emptyVec, argsRed);
      Trace("strings-ipc-red") << "Reduction : " << red << std::endl;
      if (!red.isNull())
      {
        // either equal or rewrites to it
        std::vector<Node> cexp;
        if (convertPredTransform(red, conc, cexp))
        {
          Trace("strings-ipc-red") << "...success!" << std::endl;
          useBuffer = true;
        }
        else
        {
          Trace("strings-ipc-red") << "...failed to rewrite" << std::endl;
        }
      }
    }
    break;
    // ========================== Cardinality
    case Inference::CARDINALITY: break;
    // ========================== code injectivity
    case Inference::CODE_INJ: break;
    // ========================== prefix conflict
    case Inference::PREFIX_CONFLICT:
    {
      Trace("strings-ipc-prefix") << "Prefix conflict..." << std::endl;
      std::vector<Node> eqs;
      for (const Node e : ps.d_children)
      {
        Kind ek = e.getKind();
        if (ek == EQUAL)
        {
          Trace("strings-ipc-prefix") << "- equality : " << e << std::endl;
          eqs.push_back(e);
        }
        else if (ek == STRING_IN_REGEXP)
        {
          // unfold it and extract the equality
          std::vector<Node> children;
          children.push_back(e);
          std::vector<Node> args;
          Node eunf = d_psb.tryStep(PfRule::RE_UNFOLD_POS, children, args);
          Trace("strings-ipc-prefix")
              << "--- " << e << " unfolds to " << eunf << std::endl;
          if (eunf.isNull())
          {
            continue;
          }
          else if (eunf.getKind() == AND)
          {
            // equality is the last conjunct
            std::vector<Node> childrenAE;
            childrenAE.push_back(eunf);
            std::vector<Node> argsAE;
            argsAE.push_back(nm->mkConst(Rational(eunf.getNumChildren() - 1)));
            Node eunfAE = d_psb.tryStep(PfRule::AND_ELIM, childrenAE, argsAE);
            Trace("strings-ipc-prefix")
                << "--- and elim to " << eunfAE << std::endl;
            if (eunfAE.isNull() || eunfAE.getKind() != EQUAL)
            {
              Assert(false);
              continue;
            }
            Trace("strings-ipc-prefix")
                << "- equality : " << eunfAE << std::endl;
            eqs.push_back(eunfAE);
          }
          else if (eunf.getKind() == EQUAL)
          {
            Trace("strings-ipc-prefix") << "- equality : " << eunf << std::endl;
            eqs.push_back(eunf);
          }
        }
        else
        {
          // not sure how to use this assumption
          Assert(false);
        }
      }
      if (eqs.empty())
      {
        break;
      }
      // connect via transitivity?
      Node curr = eqs[0];
      for (size_t i = 1, esize = eqs.size(); i < esize; i++)
      {
        Node prev = curr;
        curr = convertTrans(curr, eqs[1]);
        if (curr.isNull())
        {
          break;
        }
        Trace("strings-ipc-prefix") << "- Via trans: " << curr << std::endl;
      }
      if (curr.isNull())
      {
        break;
      }
      Trace("strings-ipc-prefix")
          << "- Possible conflicting equality : " << curr << std::endl;
      std::vector<Node> emp;
      Node concE = convertPredElim(curr, emp);
      Trace("strings-ipc-prefix")
          << "- After pred elim: " << concE << std::endl;
      if (concE == conc)
      {
        Trace("strings-ipc-prefix") << "...success!" << std::endl;
        useBuffer = true;
      }
    }
    break;
    // ========================== regular expressions
    case Inference::RE_INTER_INCLUDE:
    case Inference::RE_INTER_CONF:
    case Inference::RE_INTER_INFER:
    {
      std::vector<Node> reiExp;
      std::vector<Node> reis;
      std::vector<Node> reiChildren;
      std::vector<Node> reiChildrenOrig;
      Node x;
      // make the regular expression intersection that summarizes all
      // memberships in the explanation
      for (const Node& c : ps.d_children)
      {
        bool polarity = c.getKind() != NOT;
        Node catom = polarity ? c : c[0];
        if (catom.getKind() != STRING_IN_REGEXP)
        {
          Assert(c.getKind() == EQUAL);
          if (c.getKind() == EQUAL)
          {
            reiExp.push_back(c);
          }
          continue;
        }
        if (x.isNull())
        {
          // just take the first LHS; others should be equated to it by exp
          x = catom[0];
        }
        Node rcurr =
            polarity ? catom[1] : nm->mkNode(REGEXP_COMPLEMENT, catom[1]);
        reis.push_back(rcurr);
        Node mem = nm->mkNode(STRING_IN_REGEXP, catom[0], rcurr);
        reiChildren.push_back(mem);
        reiChildrenOrig.push_back(c);
      }
      // go back and justify each premise
      bool successChildren = true;
      for (unsigned i = 0, nchild = reiChildren.size(); i < nchild; i++)
      {
        if (!convertPredTransform(reiChildrenOrig[i], reiChildren[i], reiExp))
        {
          Trace("strings-ipc-re")
              << "... failed to justify child " << reiChildren[i] << " from "
              << reiChildrenOrig[i] << std::endl;
          successChildren = false;
          break;
        }
      }
      if (!successChildren)
      {
        break;
      }
      Node mem = d_psb.tryStep(PfRule::RE_INTER, reiChildren, emptyVec);
      // Node rei = reis.size() == 1 ? reis[0] : nm->mkNode(REGEXP_INTER, reis);
      // Node mem = nm->mkNode(STRING_IN_REGEXP, x, rei);
      Trace("strings-ipc-re")
          << "Regular expression summary: " << mem << std::endl;
      // the conclusion is rewritable to the premises via rewriting?
      if (convertPredTransform(mem, conc, emptyVec))
      {
        Trace("strings-ipc-re") << "... success!" << std::endl;
        useBuffer = true;
      }
      else
      {
        Trace("strings-ipc-re")
            << "...failed to rewrite to conclusion" << std::endl;
      }
    }
    break;
    // ========================== unknown
    case Inference::I_CYCLE_E:
    case Inference::I_CYCLE:
    case Inference::RE_DELTA:
    case Inference::RE_DELTA_CONF:
    case Inference::RE_DERIVE:
    case Inference::FLOOP:
    case Inference::FLOOP_CONFLICT: break;

    // FIXME
    case Inference::DEQ_NORM_EMP:
    case Inference::CTN_TRANS:
    case Inference::CTN_DECOMPOSE:
    case Inference::CTN_NEG_EQUAL:
    default: break;
  }

  // now see if we would succeed with the checker-to-try
  bool success = false;
  if (ps.d_rule != PfRule::UNKNOWN)
  {
    Trace("strings-ipc") << "For " << infer << ", try proof rule " << ps.d_rule
                         << "...";
    Assert(ps.d_rule != PfRule::UNKNOWN);
    Node pconc = d_psb.tryStep(ps.d_rule, ps.d_children, ps.d_args);
    if (pconc.isNull() || pconc != conc)
    {
      Trace("strings-ipc") << "failed, pconc is " << pconc << " (expected "
                           << conc << ")" << std::endl;
      ps.d_rule = PfRule::UNKNOWN;
    }
    else
    {
      // successfully set up a single step proof in ps
      success = true;
      Trace("strings-ipc") << "success!" << std::endl;
    }
  }
  else if (useBuffer)
  {
    // successfully set up a multi step proof in d_psb
    success = true;
  }
  else
  {
    Trace("strings-ipc") << "For " << infer << " " << conc
                         << ", no proof rule, failed" << std::endl;
  }
  if (!success)
  {
    // debug print
    if (Trace.isOn("strings-ipc-fail"))
    {
      Trace("strings-ipc-fail")
          << "InferProofCons::convert: Failed " << infer
          << (isRev ? " :rev " : " ") << conc << std::endl;
      for (const Node& ec : exp)
      {
        Trace("strings-ipc-fail") << "    e: " << ec << std::endl;
      }
    }
    // untrustworthy conversion
    // argument is the conclusion
    ps.d_args.clear();
    ps.d_args.push_back(conc);
    // use the trust rule
    ps.d_rule = PfRule::TRUST;
    // add to stats
    d_statistics.d_inferencesNoPf << infer;
    if (options::proofNewPedantic())
    {
      std::stringstream serr;
      serr << "InferProofCons::convert: Failed " << infer
           << (isRev ? " :rev " : " ") << conc << std::endl;
      for (const Node& ec : exp)
      {
        serr << "    e: " << ec << std::endl;
      }
      AlwaysAssert(false) << serr.str();
    }
  }
  if (Trace.isOn("strings-ipc-debug"))
  {
    if (useBuffer)
    {
      Trace("strings-ipc-debug")
          << "InferProofCons::convert returned buffer with "
          << d_psb.getNumSteps() << " steps:" << std::endl;
      const std::vector<std::pair<Node, ProofStep>>& steps = d_psb.getSteps();
      for (const std::pair<Node, ProofStep>& step : steps)
      {
        Trace("strings-ipc-debug")
            << "- " << step.first << " via " << step.second << std::endl;
      }
    }
    else
    {
      Trace("strings-ipc-debug")
          << "InferProofCons::convert returned " << ps << std::endl;
    }
  }
  return conc;
}

bool InferProofCons::convertLengthPf(Node lenReq,
                                     const std::vector<Node>& lenExp)
{
  for (const Node& le : lenExp)
  {
    if (lenReq == le)
    {
      return true;
    }
  }
  Trace("strings-ipc-len") << "Must explain " << lenReq << " by " << lenExp
                           << std::endl;
  for (const Node& le : lenExp)
  {
    // probably rewrites to it?
    std::vector<Node> exp;
    if (convertPredTransform(le, lenReq, exp))
    {
      Trace("strings-ipc-len") << "...success by rewrite" << std::endl;
      return true;
    }
    // maybe x != "" => len(x) != 0
    std::vector<Node> children;
    children.push_back(le);
    std::vector<Node> args;
    Node res = d_psb.tryStep(PfRule::LENGTH_NON_EMPTY, children, args);
    if (res == lenReq)
    {
      Trace("strings-ipc-len") << "...success by LENGTH_NON_EMPTY" << std::endl;
      return true;
    }
  }
  Trace("strings-ipc-len") << "...failed" << std::endl;
  return false;
}

bool InferProofCons::convertPredTransform(Node src,
                                          Node tgt,
                                          const std::vector<Node>& exp,
                                          MethodId ids,
                                          MethodId idr)
{
  // symmetric equalities
  if (CDProof::isSame(src, tgt))
  {
    return true;
  }
  std::vector<Node> children;
  children.push_back(src);
  std::vector<Node> args;
  // try to prove that tgt rewrites to src
  children.insert(children.end(), exp.begin(), exp.end());
  args.push_back(tgt);
  addMethodIds(args, ids, idr);
  Node res = d_psb.tryStep(PfRule::MACRO_SR_PRED_TRANSFORM, children, args);
  if (res.isNull())
  {
    // failed to apply
    return false;
  }
  Trace("strings-ipc-debug") << "InferProofCons::convertPredTransform: success "
                             << src << " == " << tgt << " under " << exp
                             << " via " << ids << "/" << idr << std::endl;
  // should definitely have concluded tgt
  Assert(res == tgt);
  return true;
}

bool InferProofCons::convertPredIntro(Node tgt,
                                      const std::vector<Node>& exp,
                                      MethodId ids,
                                      MethodId idr)
{
  std::vector<Node> args;
  args.push_back(tgt);
  addMethodIds(args, ids, idr);
  Node res = d_psb.tryStep(PfRule::MACRO_SR_PRED_INTRO, exp, args);
  if (res.isNull())
  {
    return false;
  }
  Assert(res == tgt);
  return true;
}

Node InferProofCons::convertPredElim(Node src,
                                     const std::vector<Node>& exp,
                                     MethodId ids,
                                     MethodId idr)
{
  std::vector<Node> children;
  children.push_back(src);
  children.insert(children.end(), exp.begin(), exp.end());
  std::vector<Node> args;
  addMethodIds(args, ids, idr);
  Node srcRew = d_psb.tryStep(PfRule::MACRO_SR_PRED_ELIM, children, args);
  if (CDProof::isSame(src, srcRew))
  {
    d_psb.popStep();
    return src;
  }
  return srcRew;
}

void InferProofCons::addMethodIds(std::vector<Node>& args,
                                  MethodId ids,
                                  MethodId idr)
{
  bool ndefRewriter = (idr != MethodId::RW_REWRITE);
  if (ids != MethodId::SB_DEFAULT || ndefRewriter)
  {
    args.push_back(mkMethodId(ids));
  }
  if (ndefRewriter)
  {
    args.push_back(mkMethodId(idr));
  }
}

Node InferProofCons::convertTrans(Node eqa, Node eqb)
{
  if (eqa.getKind() != EQUAL || eqb.getKind() != EQUAL)
  {
    return Node::null();
  }
  for (unsigned i = 0; i < 2; i++)
  {
    Node eqaSym = i == 0 ? eqa[1].eqNode(eqa[0]) : eqa;
    for (unsigned j = 0; j < 2; j++)
    {
      Node eqbSym = j == 0 ? eqb : eqb[1].eqNode(eqb[1]);
      if (eqa[i] == eqb[j])
      {
        std::vector<Node> children;
        children.push_back(eqaSym);
        children.push_back(eqbSym);
        std::vector<Node> args;
        return d_psb.tryStep(PfRule::TRANS, children, args);
      }
    }
  }
  return Node::null();
}

ProofStepBuffer* InferProofCons::getBuffer() { return &d_psb; }

std::shared_ptr<ProofNode> InferProofCons::getProofFor(Node fact)
{
  // temporary proof
  CDProof pf(d_pnm);
  // get the inference
  NodeInferInfoMap::iterator it = d_lazyFactMap.find(fact);
  if (it == d_lazyFactMap.end())
  {
    if (fact.getKind() == EQUAL)
    {
      // Use the symmetric fact. There is no need to explictly make a
      // SYMM proof, as this is handled by CDProof::mkProof below.
      Node factSym = fact[1].eqNode(fact[0]);
      it = d_lazyFactMap.find(factSym);
    }
  }
  AlwaysAssert(it != d_lazyFactMap.end());
  // now go back and convert it to proof steps and add to proof
  bool useBuffer = false;
  ProofStep ps;
  convert(*(*it).second, ps, useBuffer);
  if (useBuffer)
  {
    if (!pf.addSteps(d_psb))
    {
      return nullptr;
    }
  }
  else
  {
    if (!pf.addStep(fact, ps))
    {
      return nullptr;
    }
  }
  return pf.mkProof(fact);
}

std::string InferProofCons::identify() const
{
  return "strings::InferProofCons";
}

}  // namespace strings
}  // namespace theory
}  // namespace CVC4
