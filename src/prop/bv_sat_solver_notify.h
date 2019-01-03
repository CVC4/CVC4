/*********************                                                        */
/*! \file sat_solver_notify.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Liana Hadarean, Dejan Jovanovic, Morgan Deters
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The interface for things that want to recieve notification from the
 **        SAT solver
 **/

#include "cvc4_private.h"

#ifndef __CVC4__PROP__BVSATSOLVERNOTIFY_H
#define __CVC4__PROP__BVSATSOLVERNOTIFY_H

#include "prop/sat_solver_types.h"

namespace CVC4 {
namespace prop {

class BVSatSolverNotify {
public:

  virtual ~BVSatSolverNotify() {};

  /**
   * If the notify returns false, the solver will break out of whatever it's currently doing
   * with an "unknown" answer.
   */
  virtual bool notify(SatLiteral lit) = 0;

  /**
   * Notify about a learnt clause.
   */
  virtual void notify(SatClause& clause) = 0;
  virtual void spendResource(unsigned amount) = 0;
  virtual void safePoint(unsigned amount) = 0;

};/* class BVSatSolverInterface::Notify */

}
}

#endif
