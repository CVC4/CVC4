/******************************************************************************
 * Top contributors (to current version):
 *   Michael Chang, Yancheng Ou, Aina Niemetz
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * The solver for optimization queries.
 */

#include "smt/optimization_solver.h"

#include "omt/omt_optimizer.h"
#include "smt/assertions.h"

using namespace cvc5::theory;
using namespace cvc5::omt;
namespace cvc5 {
namespace smt {

OptimizationResult::ResultType OptimizationSolver::checkOpt()
{
  // just a single objective optimization
  std::unique_ptr<OMTOptimizer> optimizer = OMTOptimizer::getOptimizerForNode(
      d_objectives[0].getTarget(), d_objectives[0].bvIsSigned());

  if (!optimizer) return OptimizationResult::UNSUPPORTED;

  OptimizationResult optResult;
  if (d_objectives[0].getType() == OptimizationObjective::MAXIMIZE)
  {
    optResult =
        optimizer->maximize(this->d_parent, this->d_objectives[0].getTarget());
  }
  else if (d_objectives[0].getType() == OptimizationObjective::MINIMIZE)
  {
    optResult =
        optimizer->minimize(this->d_parent, this->d_objectives[0].getTarget());
  }

  d_results[0] = optResult;
  return optResult.getType();
}

void OptimizationSolver::pushObjective(
    const Node target,
    const OptimizationObjective::ObjectiveType type,
    bool bvSigned)
{
  d_objectives.emplace_back(target, type, bvSigned);
  d_results.emplace_back(OptimizationResult::UNSUPPORTED, Node());
}

void OptimizationSolver::popObjective()
{
  d_objectives.pop_back();
  d_results.pop_back();
}

std::vector<OptimizationResult> OptimizationSolver::getValues()
{
  return d_results;
}

}  // namespace smt
}  // namespace cvc5
