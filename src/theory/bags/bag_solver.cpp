/*********************                                                        */
/*! \file bag_solver.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Mudathir Mohamed
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of the theory of bags.
 **
 ** Implementation of the theory of bags.
 **/

#include "theory/bags/bag_solver.h"

#include "theory/bags/inference_generator.h"

using namespace std;
using namespace CVC4::context;
using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace bags {

BagSolver::BagSolver(SolverState& s, InferenceManager& im, TermRegistry& tr)
    : d_state(s), d_im(im), d_termReg(tr)
{
  d_zero = NodeManager::currentNM()->mkConst(Rational(0));
  d_one = NodeManager::currentNM()->mkConst(Rational(1));
  d_true = NodeManager::currentNM()->mkConst(true);
  d_false = NodeManager::currentNM()->mkConst(false);
}

BagSolver::~BagSolver() {}

void BagSolver::postCheck()
{
  for (std::pair<const TypeNode, std::vector<Node>>& t : d_state.getBags())
  {
    for (Node& n : t.second)
    {
      std::cout << n << std::endl;
      Kind k = n.getKind();
      switch (k)
      {
        case kind::DIFFERENCE_SUBTRACT:
          for (Node& e : d_state.getElements(t.first.getBagElementType()))
          {
            InferenceGenerator ig(NodeManager::currentNM());
            InferInfo i = ig.differenceSubtract(n, e);
            i.d_im = &d_im;
            i.process(&d_im, true);
          }
        default: break;
      }
    }
  }
}

}  // namespace bags
}  // namespace theory
}  // namespace CVC4
