/*********************                                                        */
/*! \file expr_miner_manager.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of expression miner manager.
 **/

#include "theory/quantifiers/expr_miner_manager.h"
#include "theory/quantifiers_engine.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

ExpressionMinerManager::ExpressionMinerManager()
    : d_doRewSynth(false),
      d_doQueryGen(false),
      d_use_sygus_type(false),
      d_qe(nullptr),
      d_tds(nullptr)
{
}

void ExpressionMinerManager::initialize(const std::vector<Node>& vars,
                                        TypeNode tn,
                                        unsigned nsamples,
                                        bool unique_type_ids)
{
  d_doRewSynth = false;
  d_doQueryGen = false;
  d_sygus_fun = Node::null();
  d_use_sygus_type = false;
  d_qe = nullptr;
  d_tds = nullptr;
  // initialize the sampler
  d_sampler.initialize(tn, vars, nsamples, unique_type_ids);
}

void ExpressionMinerManager::initializeSygus(QuantifiersEngine* qe,
                                             Node f,
                                             unsigned nsamples,
                                             bool useSygusType)
{
  d_doRewSynth = false;
  d_doQueryGen = false;
  d_sygus_fun = f;
  d_use_sygus_type = useSygusType;
  d_qe = qe;
  d_tds = qe->getTermDatabaseSygus();
  // initialize the sampler
  d_sampler.initializeSygus(d_tds, f, nsamples, useSygusType);
}

void ExpressionMinerManager::enableRewriteRuleSynth()
{
  if (d_doRewSynth)
  {
    // already enabled
    return;
  }
  d_doRewSynth = true;
  std::vector<Node> vars;
  d_sampler.getVariables(vars);
  // initialize the candidate rewrite database
  if (!d_sygus_fun.isNull())
  {
    Assert(d_qe != nullptr);
    d_crd.initializeSygus(vars, d_qe, d_sygus_fun, &d_sampler);
  }
  else
  {
    d_crd.initialize(vars, &d_sampler);
  }
  d_crd.setExtendedRewriter(&d_ext_rew);
  d_crd.setSilent(false);
}

void ExpressionMinerManager::enableQueryGeneration(unsigned deqThresh)
{
  if (d_doQueryGen)
  {
    // already enabled
    return;
  }
  d_doQueryGen = true;
  std::vector<Node> vars;
  d_sampler.getVariables(vars);
  // must also enable rewrite rule synthesis
  if (!d_do_rew_synth)
  {
    // initialize the candidate rewrite database, in silent mode
    enableRewriteRuleSynth();
    d_crd.setSilent(true);
  }
  // initialize the query generator
  d_qg.initialize(vars, &d_sampler);
  d_qg.setThreshold(deqThresh);
}

bool ExpressionMinerManager::addTerm(Node sol,
                                     std::ostream& out,
                                     bool& rew_print)
{
  bool ret = d_crd.addTerm(sol, out, rew_print);
  if (ret && d_doQueryGen)
  {
    // always use the builtin version
    Node solb = sol;
    if (d_use_sygus_type)
    {
      solb = d_tds->sygusToBuiltin(sol);
    }
    // a unique term, let's try the query generator
    d_qg.addTerm(solb, out);
  }
  return ret;
}

bool ExpressionMinerManager::addTerm(Node sol, std::ostream& out)
{
  bool rew_print = false;
  return addTerm(sol, out, rew_print);
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC4
