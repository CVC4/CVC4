/*********************                                                        */
/*! \file omt_optimizer.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Yancheng Ou
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The base class for optimizers of individual CVC4 type
 **/

#include "omt/omt_optimizer.h"

#include "omt/bitvector_optimizer.h"
#include "omt/integer_optimizer.h"
#include "options/smt_options.h"
#include "smt/smt_engine.h"
#include "theory/quantifiers/quantifiers_attributes.h"
#include "theory/smt_engine_subsolver.h"

using namespace CVC4::theory;
using namespace CVC4::smt;
namespace CVC4::omt {

std::unique_ptr<OMTOptimizer> OMTOptimizer::getOptimizerForNode(Node targetNode,
                                                                bool isSigned)
{
  // the datatype of the target node
  TypeNode objectiveType = targetNode.getType(true);
  if (objectiveType.isInteger())
  {
    // integer type: use OMTOptimizerInteger
    return std::unique_ptr<OMTOptimizer>(new OMTOptimizerInteger());
  }
  else if (objectiveType.isBitVector())
  {
    // bitvector type: use OMTOptimizerBitVector
    return std::unique_ptr<OMTOptimizer>(new OMTOptimizerBitVector(isSigned));
  }
  else
  {
    return nullptr;
  }
}

std::unique_ptr<SmtEngine> OMTOptimizer::createOptCheckerWithTimeout(
    SmtEngine* parentSMTSolver, bool needsTimeout, unsigned long timeout)
{
  std::unique_ptr<SmtEngine> optChecker;
  // initializeSubSolver will copy the options and theories enabled
  // from the current solver to optChecker and adds timeout
  CVC4::theory::initializeSubsolver(optChecker, needsTimeout, timeout);
  // we need to be in incremental mode for multiple objectives since we need to
  // push pop we need to produce models to inrement on our objective
  optChecker->setOption("incremental", "true");
  optChecker->setOption("produce-models", "true");
  // Move assertions from the parent solver to the subsolver
  std::vector<Node> p_assertions = parentSMTSolver->getExpandedAssertions();
  for (const Node& e : p_assertions)
  {
    optChecker->assertFormula(e);
  }
  return optChecker;
}

}  // namespace CVC4::omt
