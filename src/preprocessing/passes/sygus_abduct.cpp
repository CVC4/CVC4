/*********************                                                        */
/*! \file sygus_abduct.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Sygus inference module
 **/

#include "preprocessing/passes/sygus_abduct.h"

#include "expr/node_algorithm.h"
#include "smt/smt_engine.h"
#include "smt/smt_engine_scope.h"
#include "smt/smt_statistics_registry.h"
#include "theory/quantifiers/quantifiers_attributes.h"
#include "theory/quantifiers/quantifiers_rewriter.h"
#include "theory/quantifiers/term_util.h"
#include "theory/rewriter.h"

using namespace std;
using namespace CVC4::kind;

namespace CVC4 {
namespace preprocessing {
namespace passes {

SygusAbduct::SygusAbduct(PreprocessingPassContext* preprocContext)
    : PreprocessingPass(preprocContext, "sygus-abduct"){};

PreprocessingPassResult SygusAbduct::applyInternal(
    AssertionPipeline* assertionsToPreprocess)
{
  NodeManager* nm = NodeManager::currentNM();
  Trace("sygus-abduct") << "Run sygus abduct..." << std::endl;

  Trace("sygus-abduct-debug") << "Collect symbols..." << std::endl;
  std::unordered_set<Node, NodeHashFunction> symset;
  std::vector<Node>& asserts = assertionsToPreprocess->ref();
  std::vector< Node > assumptions;
  bool usingAssumptions = (assertionsToPreprocess->getNumAssumptions()>0);
  for (unsigned i = 0, size = asserts.size(); i < size; i++)
  {
    expr::getSymbols(asserts[i], symset);
    if( usingAssumptions && i<assertionsToPreprocess->getAssumptionsStart() )
    {
      assumptions.push_back(asserts[i]);
    }
  }
  Trace("sygus-abduct-debug")
      << "...finish, got " << symset.size() << " symbols." << std::endl;

  Trace("sygus-abduct-debug") << "Setup symbols..." << std::endl;
  std::vector<Node> syms;
  std::vector<Node> vars;
  std::vector<Node> varlist;
  std::vector<TypeNode> varlistTypes;
  for (const Node& s : symset)
  {
    TypeNode tn = s.getType();
    if (tn.isFirstClass())
    {
      std::stringstream ss;
      ss << s;
      Node var = nm->mkBoundVar(tn);
      syms.push_back(s);
      vars.push_back(var);
      Node vlv = nm->mkBoundVar(ss.str(), tn);
      varlist.push_back(vlv);
      varlistTypes.push_back(tn);
    }
  }
  Trace("sygus-abduct-debug") << "...finish" << std::endl;

  Trace("sygus-abduct-debug") << "Make abduction predicate..." << std::endl;
  // make the abduction predicate to synthesize
  TypeNode abdType = varlistTypes.empty() ? nm->booleanType()
                                          : nm->mkPredicateType(varlistTypes);
  Node abd = nm->mkBoundVar("A", abdType);
  Trace("sygus-abduct-debug") << "...finish" << std::endl;

  Trace("sygus-abduct-debug") << "Make abduction predicate app..." << std::endl;
  std::vector<Node> achildren;
  achildren.push_back(abd);
  achildren.insert(achildren.end(), vars.begin(), vars.end());
  Node abdApp = vars.empty() ? abd : nm->mkNode(APPLY_UF, achildren);
  Trace("sygus-abduct-debug") << "...finish" << std::endl;
  
  Trace("sygus-abduct-debug") << "Set attributes..." << std::endl;
  // set the sygus bound variable list
  Node abvl = nm->mkNode(BOUND_VAR_LIST, varlist);
  abd.setAttribute(theory::SygusSynthFunVarListAttribute(), abvl);
  Trace("sygus-abduct-debug") << "...finish" << std::endl;

  Trace("sygus-abduct-debug") << "Make conjecture body..." << std::endl;
  Node input = asserts.size() == 1 ? asserts[0] : nm->mkNode(AND, asserts);
  input = input.substitute(syms.begin(), syms.end(), vars.begin(), vars.end());
  // A(x) => ~input( x )
  input = nm->mkNode(OR, abdApp.negate(), input.negate());
  Trace("sygus-abduct-debug") << "...finish" << std::endl;

  Trace("sygus-abduct-debug") << "Make conjecture..." << std::endl;
  Node res = input.negate();
  if (!vars.empty())
  {
    Node bvl = nm->mkNode(BOUND_VAR_LIST, vars);
    // exists x. ~( A( x ) => ~input( x ) )
    res = nm->mkNode(EXISTS, bvl, res);
  }
  // sygus attribute
  Node sygusVar = nm->mkSkolem("sygus", nm->booleanType());
  theory::SygusAttribute ca;
  sygusVar.setAttribute(ca, true);
  Node instAttr = nm->mkNode(INST_ATTRIBUTE, sygusVar);
  std::vector< Node > iplc;
  iplc.push_back(instAttr);
  if( !assumptions.empty() )
  {
    Node aconj = assumptions.size()==1 ? assumptions[0] : nm->mkNode( AND, assumptions );
    aconj = aconj.substitute(syms.begin(), syms.end(), vars.begin(), vars.end());
    Trace("sygus-abduct") << "---> Assumptions: " << aconj << std::endl;
    Node sc = nm->mkNode(AND,aconj,abdApp);
    Node vbvl = nm->mkNode(BOUND_VAR_LIST, vars);
    sc = nm->mkNode(EXISTS, vbvl, sc);
    Node sygusScVar = nm->mkSkolem("sygus_sc", nm->booleanType());
    sygusScVar.setAttribute(theory::SygusSideConditionAttribute(), sc );
    // build in the side condition
    //   exists x. A( x ) ^ input_assumptions( x )
    // as an additional annotation on the sygus conjecture
    iplc.push_back(sygusScVar);
  }
  Node instAttrList = nm->mkNode(INST_PATTERN_LIST, iplc);

  Node fbvl = nm->mkNode(BOUND_VAR_LIST, abd);

  // forall A. exists x. ~( A( x ) => ~input( x ) )
  res = nm->mkNode(FORALL, fbvl, res, instAttrList);
  Trace("sygus-abduct-debug") << "...finish" << std::endl;

  res = theory::Rewriter::rewrite(res);

  Trace("sygus-abduct") << "Generate: " << res << std::endl;

  Node trueNode = nm->mkConst(true);

  assertionsToPreprocess->replace(0, res);
  for (unsigned i = 1, size = assertionsToPreprocess->size(); i < size; ++i)
  {
    assertionsToPreprocess->replace(i, trueNode);
  }

  return PreprocessingPassResult::NO_CONFLICT;
}

}  // namespace passes
}  // namespace preprocessing
}  // namespace CVC4
