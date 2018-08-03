/*********************                                                        */
/*! \file expr_miner.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief expr_miner
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__QUANTIFIERS__EXPRESSION_MINER_H
#define __CVC4__THEORY__QUANTIFIERS__EXPRESSION_MINER_H

#include "theory/quantifiers/sygus_sampler.h"
#include "theory/quantifiers/candidate_rewrite_database.h"
#include "theory/quantifiers/query_generator.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

/** ExpressionMiner
 * 
 */
class ExpressionMiner
{
 public:
  ExpressionMiner();
  ~ExpressionMiner(){}
  /**  Initialize this class
   *
   */
  void initialize(bool doRewSynth,
                  bool doQueryGen,
                  ExtendedRewriter* er,
                  TypeNode tn,
                  std::vector<Node>& vars,
                  unsigned nsamples,
                  bool unique_type_ids = false);
  /**  Initialize this class
   *
   */
  void initializeSygus(bool doRewSynth,
                       bool doQueryGen,
                       QuantifiersEngine* qe,
                       Node f,
                       unsigned nsamples,
                       bool useSygusType);
  /** add term
   */
  bool addTerm(Node sol, std::ostream& out, bool& rew_print);
  bool addTerm(Node sol, std::ostream& out);
 private:
  /** whether we are doing rewrite synthesis */
  bool d_do_rew_synth;
  /** whether we are doing query generation */
  bool d_do_query_gen;
  /** whether we are using sygus types */
  bool d_use_sygus_type;
  /** candidate rewrite database */
  CandidateRewriteDatabase d_crd;
  /** query generator */
  QueryGenerator d_qg;
  /** sygus sampler object */
  SygusSampler d_sampler;
};

} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */

#endif /* __CVC4__THEORY__QUANTIFIERS__EXPRESSION_MINER_H */
