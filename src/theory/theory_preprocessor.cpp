/*********************                                                        */
/*! \file theory_preprocessor.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Dejan Jovanovic, Morgan Deters, Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The theory preprocessor
 **/

#include "theory/theory_preprocessor.h"

#include "expr/lazy_proof.h"
#include "theory/logic_info.h"
#include "theory/rewriter.h"
#include "theory/theory_engine.h"
#include "expr/skolem_manager.h"

using namespace std;

namespace CVC4 {
namespace theory {

TheoryPreprocessor::TheoryPreprocessor(TheoryEngine& engine,
                                       RemoveTermFormulas& tfr,
                                       ProofNodeManager* pnm)
    : d_engine(engine),
      d_logicInfo(engine.getLogicInfo()),
      d_ppCache(),
      d_tfr(tfr),
      d_tpg(pnm ? new TConvProofGenerator(pnm) : nullptr)
{
}

TheoryPreprocessor::~TheoryPreprocessor() {}

void TheoryPreprocessor::clearCache() { 
  d_ppCache.clear(); 
  // TODO: clear rewrites from d_tpg
}

void TheoryPreprocessor::preprocess(TNode node,
                                    preprocessing::AssertionPipeline& lemmas,
                                    bool doTheoryPreprocess,
                                    LazyCDProof* lp)
{
  // Run theory preprocessing, maybe
  Node ppNode = node;
  if (doTheoryPreprocess)
  {
    // run theory preprocessing
    TrustNode trn = theoryPreprocess(node);
    ppNode = trn.getNode();
  }

  // Remove the ITEs
  Trace("te-tform-rm") << "Remove term formulas from " << ppNode << std::endl;
  lemmas.push_back(ppNode);
  lemmas.updateRealAssertionsEnd();
  // TODO: pass lp, d_tpg to run here
  d_tfr.run(lemmas.ref(), lemmas.getIteSkolemMap(), false, nullptr, nullptr);
  Trace("te-tform-rm") << "..done " << lemmas[0] << std::endl;

  // justify the preprocessing step
  if (lp != nullptr)
  {
    // currently this is a trusted step that combines theory preprocessing and
    // term formula removal.
    if (!CDProof::isSame(node, lemmas[0]))
    {
#if 0
      Node eq = node.eqNode(lemmas[0]);
      std::shared_ptr<ProofNode> ppf = d_tpg->getTranformProofFor(node,lp);
      Assert (ppf!=nullptr);
      Assert (ppf->getResult()==lemmas[0]);
#else
      // trusted big step
      std::vector<Node> pfChildren;
      pfChildren.push_back(node);
      std::vector<Node> pfArgs;
      pfArgs.push_back(lemmas[0]);
      lp->addStep(lemmas[0], PfRule::THEORY_PREPROCESS, pfChildren, pfArgs);
#endif
    }
  }

  if (Debug.isOn("lemma-ites"))
  {
    Debug("lemma-ites") << "removed ITEs from lemma: " << ppNode << endl;
    Debug("lemma-ites") << " + now have the following " << lemmas.size()
                        << " lemma(s):" << endl;
    for (std::vector<Node>::const_iterator i = lemmas.begin();
         i != lemmas.end();
         ++i)
    {
      Debug("lemma-ites") << " + " << *i << endl;
    }
    Debug("lemma-ites") << endl;
  }

  // now, rewrite the lemmas
  Node retLemma;
  for (size_t i = 0, lsize = lemmas.size(); i < lsize; ++i)
  {
    Node rewritten = Rewriter::rewrite(lemmas[i]);
    if (lp != nullptr)
    {
      if (!CDProof::isSame(rewritten, lemmas[i]))
      {
        std::vector<Node> pfChildren;
        pfChildren.push_back(lemmas[i]);
        std::vector<Node> pfArgs;
        pfArgs.push_back(rewritten);
        lp->addStep(
            rewritten, PfRule::MACRO_SR_PRED_TRANSFORM, pfChildren, pfArgs);
      }
    }
    lemmas.replace(i, rewritten);
  }
}

struct preprocess_stack_element
{
  TNode node;
  bool children_added;
  preprocess_stack_element(TNode n) : node(n), children_added(false) {}
};

TrustNode TheoryPreprocessor::theoryPreprocess(TNode assertion)
{
  Trace("theory::preprocess")
      << "TheoryPreprocessor::theoryPreprocess(" << assertion << ")" << endl;
  // spendResource();

  // Do a topological sort of the subexpressions and substitute them
  vector<preprocess_stack_element> toVisit;
  toVisit.push_back(assertion);

  while (!toVisit.empty())
  {
    // The current node we are processing
    preprocess_stack_element& stackHead = toVisit.back();
    TNode current = stackHead.node;

    Debug("theory::internal")
        << "TheoryPreprocessor::theoryPreprocess(" << assertion
        << "): processing " << current << endl;

    // If node already in the cache we're done, pop from the stack
    NodeMap::iterator find = d_ppCache.find(current);
    if (find != d_ppCache.end())
    {
      toVisit.pop_back();
      continue;
    }

    if (!d_logicInfo.isTheoryEnabled(Theory::theoryOf(current))
        && Theory::theoryOf(current) != THEORY_SAT_SOLVER)
    {
      stringstream ss;
      ss << "The logic was specified as " << d_logicInfo.getLogicString()
         << ", which doesn't include " << Theory::theoryOf(current)
         << ", but got a preprocessing-time fact for that theory." << endl
         << "The fact:" << endl
         << current;
      throw LogicException(ss.str());
    }

    // If this is an atom, we preprocess its terms with the theory ppRewriter
    if (Theory::theoryOf(current) != THEORY_BOOL)
    {
      Node ppRewritten = ppTheoryRewrite(current);
      d_ppCache[current] = ppRewritten;
      Assert(Rewriter::rewrite(d_ppCache[current]) == d_ppCache[current]);
      continue;
    }

    // Not yet substituted, so process
    if (stackHead.children_added)
    {
      // Children have been processed, so substitute
      NodeBuilder<> builder(current.getKind());
      if (current.getMetaKind() == kind::metakind::PARAMETERIZED)
      {
        builder << current.getOperator();
      }
      for (unsigned i = 0; i < current.getNumChildren(); ++i)
      {
        Assert(d_ppCache.find(current[i]) != d_ppCache.end());
        builder << d_ppCache[current[i]];
      }
      // Mark the substitution and continue
      Node result = builder;
      if (result != current)
      {
        result = Rewriter::rewrite(result);
      }
      Debug("theory::internal")
          << "TheoryPreprocessor::theoryPreprocess(" << assertion
          << "): setting " << current << " -> " << result << endl;
      d_ppCache[current] = result;
      toVisit.pop_back();
    }
    else
    {
      // Mark that we have added the children if any
      if (current.getNumChildren() > 0)
      {
        stackHead.children_added = true;
        // We need to add the children
        for (TNode::iterator child_it = current.begin();
             child_it != current.end();
             ++child_it)
        {
          TNode childNode = *child_it;
          NodeMap::iterator childFind = d_ppCache.find(childNode);
          if (childFind == d_ppCache.end())
          {
            toVisit.push_back(childNode);
          }
        }
      }
      else
      {
        // No children, so we're done
        Debug("substitution::internal")
            << "SubstitutionMap::internalSubstitute(" << assertion
            << "): setting " << current << " -> " << current << endl;
        d_ppCache[current] = current;
        toVisit.pop_back();
      }
    }
  }
  // Return the substituted version
  Node res = d_ppCache[assertion];
  return TrustNode::mkTrustRewrite(assertion, res, d_tpg.get());
}

// Recursively traverse a term and call the theory rewriter on its sub-terms
Node TheoryPreprocessor::ppTheoryRewrite(TNode term)
{
  NodeMap::iterator find = d_ppCache.find(term);
  if (find != d_ppCache.end())
  {
    return (*find).second;
  }
  unsigned nc = term.getNumChildren();
  if (nc == 0)
  {
    TrustNode trn = d_engine.theoryOf(term)->ppRewrite(term);
    if (!trn.isNull())
    {
      Node termr = trn.getNode();
      if (d_tpg!=nullptr)
      {
        if (trn.getGenerator()!=nullptr)
        {
          d_tpg->addRewriteStep(term, termr, trn.getGenerator());
        }
      }
      return termr;
    }
    return term;
  }
  Trace("theory-pp") << "ppTheoryRewrite { " << term << endl;

  Node newTerm = term;
  // do not rewrite inside quantifiers
  if (!term.isClosure())
  {
    NodeBuilder<> newNode(term.getKind());
    if (term.getMetaKind() == kind::metakind::PARAMETERIZED)
    {
      newNode << term.getOperator();
    }
    unsigned i;
    for (i = 0; i < nc; ++i)
    {
      newNode << ppTheoryRewrite(term[i]);
    }
    newTerm = Node(newNode);
  }
  newTerm = rewriteWithProof(newTerm);
  TrustNode trn = d_engine.theoryOf(newTerm)->ppRewrite(newTerm);
  if (!trn.isNull())
  {
    Node newTerm2 = trn.getNode();
    if (d_tpg!=nullptr)
    {
      if (trn.getGenerator()!=nullptr)
      {
        d_tpg->addRewriteStep(newTerm, newTerm2, trn.getGenerator());
      }
    }
    newTerm2 = rewriteWithProof(newTerm2);
    newTerm = ppTheoryRewrite(newTerm2);
  }
  d_ppCache[term] = newTerm;
  Trace("theory-pp") << "ppTheoryRewrite returning " << newTerm << "}" << endl;
  return newTerm;
}

Node TheoryPreprocessor::rewriteWithProof(Node term)
{
  Node termr = Rewriter::rewrite(term);
  // store rewrite step if tracking proofs and it rewrites
  if (d_tpg!=nullptr)
  {
    if (termr!=term)
    {
      d_tpg->addRewriteStep(term, termr, PfRule::REWRITE, {}, {term});
    }
  }
  return termr;
}

}  // namespace theory
}  // namespace CVC4
