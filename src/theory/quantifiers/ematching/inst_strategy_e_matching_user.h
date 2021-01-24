/*********************                                                        */
/*! \file inst_strategy_e_matching_user.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Morgan Deters, Andrew Reynolds, Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The user-provided E matching instantiation strategy
 **/

#include "cvc4_private.h"

#ifndef CVC4__INST_STRATEGY_E_MATCHING_USER_H
#define CVC4__INST_STRATEGY_E_MATCHING_USER_H

#include <map>
#include "expr/node.h"
#include "theory/quantifiers/ematching/trigger.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

/**
 * This class is responsible for processing user-provided triggers.
 */
class InstStrategyUserPatterns : public InstStrategy{
 public:
  InstStrategyUserPatterns( QuantifiersEngine* ie ) :
      InstStrategy( ie ){}
  ~InstStrategyUserPatterns(){}
  /** add pattern */
  void addUserPattern( Node q, Node pat );
  /** get num patterns */
  size_t getNumUserGenerators(Node q) { return d_user_gen[q].size(); }
  /** get user pattern */
  inst::Trigger* getUserGenerator(Node q, size_t i) { return d_user_gen[q][i]; }
  /** identify */
  std::string identify() const override { return std::string("UserPatterns"); }
private:
  /** reset instantiation round for the given effort */
  void processResetInstantiationRound(Theory::Effort effort) override;
  /** Process quantified formula q at the given effort */
  int process(Node f, Theory::Effort effort, int e) override;
  /** explicitly provided patterns */
  std::map< Node, std::vector< inst::Trigger* > > d_user_gen;
  /** waiting to be generated patterns */
  std::map< Node, std::vector< std::vector< Node > > > d_user_gen_wait;
  /** counter for quantifiers */
  std::map< Node, int > d_counter;
};/* class InstStrategyUserPatterns */

}
}/* CVC4::theory namespace */
}/* CVC4 namespace */

#endif
