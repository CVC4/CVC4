/*********************                                                        */
/*! \file optimization_solver.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Michael Chang
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The solver for optimization queries
 **/

#include "cvc4_private.h"

#ifndef CVC4__SMT__OPTIMIZATION_SOLVER_H
#define CVC4__SMT__OPTIMIZATION_SOLVER_H

#include "expr/node.h"
#include "expr/type_node.h"
#include "smt/assertions.h"
#include "util/result.h"

namespace CVC4 {
namespace smt {

/**
 * An enum for optimization queries.
 *
 * Represenets whether an objective should be minimized: 0
 * or maximized: 1
 */
enum CVC4_PUBLIC ObjectiveType
{
  OBJECTIVE_MINIMIZE,
  OBJECTIVE_MAXIMIZE,
  OBJECTIVE_UNDEFINED
};

/**
 * An enum for optimization queries.
 *
 * Represents the result of a checkopt query




 * (unimplemented) OPT_OPTIMAL: if 
 * value was found
 */
enum CVC4_PUBLIC OptResult
{
  // the original set of assertions has result UNKNOWN
  OPT_UNKNOWN,
  // the original set of assertions has result UNSAT
  OPT_UNSAT,
  // the optimization loop finished and optimal
  OPT_OPTIMAL
  // The last two values are here as a preparation for future work
  // in which partial and approximate optimizations will be supported.

   // if the optimized value is only partial
  OPT_SAT_PARTIAL,
  // if the solver halted early and value is only approximate
  OPT_SAT_APPROX,

};

class Objective
{
 public:
  Objective(Node n, ObjectiveType type);
  ~Objective(){};

  /** A getter for d_type **/
  ObjectiveType getType();
  /** A getter for d_node **/
  Node getNode();

 private:
  /** The type of objective this is, either OBJECTIVE_MAXIMIZE OR
   * OBJECTIVE_MINIMIZE  **/
  ObjectiveType d_type;
  /** The node associated to the term that was used to construc the objective.
   * **/
  Node d_node;
  };

  /**
   * A solver for optimization queries.
   *
   * This class is responsible for responding to get-interpol commands. It
   * spawns a subsolver SmtEngine for a sygus conjecture that captures the
   * interpolation query, and implements supporting utility methods such as
   * checkInterpol.
   */
  class OptimizationSolver
  {
   public:
    OptimizationSolver(SmtEngine* parent);
    ~OptimizationSolver();

    /** Runs the optimization loop for the activated objective **/
    OptResult checkOpt();
    /** Activates an objective: will be optimized for **/
    void activateObj(const Node& obj, const int& type);
    /** Gets the value of the optimized objective after checkopt is called **/
    Node objectiveGetValue(const Node& obj);

   private:
    /** The parent SMT engine **/
    SmtEngine* d_parent;
    /** The objectives to optimize for **/
    Objective d_activatedObjective;
    /** A saved value of the objective from the last sat call. **/
    Node d_savedValue;
  };

}  // namespace smt
}  // namespace CVC4

#endif /* CVC4__SMT__OPTIMIZATION_SOLVER_H */
