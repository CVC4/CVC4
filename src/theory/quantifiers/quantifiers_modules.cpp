/*********************                                                        */
/*! \file quantifiers_modules.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Class for initializing the modules of quantifiers engine
 **/

#include "theory/quantifiers/quantifiers_modules.h"

#include "options/quantifiers_options.h"
#include "theory/quantifiers/relevant_domain.h"
#include "theory/quantifiers/term_registry.h"
#include "theory/quantifiers_engine.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

QuantifiersModules::QuantifiersModules()
    : d_rel_dom(nullptr),
      d_alpha_equiv(nullptr),
      d_inst_engine(nullptr),
      d_model_engine(nullptr),
      d_builder(nullptr),
      d_bint(nullptr),
      d_qcf(nullptr),
      d_sg_gen(nullptr),
      d_synth_e(nullptr),
      d_fs(nullptr),
      d_ipool(nullptr),
      d_i_cbqi(nullptr),
      d_qsplit(nullptr),
      d_sygus_inst(nullptr)
{
}
QuantifiersModules::~QuantifiersModules() {}
void QuantifiersModules::initialize(QuantifiersEngine* qe,
                                    QuantifiersState& qs,
                                    QuantifiersInferenceManager& qim,
                                    QuantifiersRegistry& qr,
                                    TermRegistry& tr,
                                    DecisionManager* dm,
                                    std::vector<QuantifiersModule*>& modules)
{
  // add quantifiers modules
  if (options::quantConflictFind())
  {
    d_qcf.reset(new QuantConflictFind(qe, qs, qim, qr));
    modules.push_back(d_qcf.get());
  }
  if (options::conjectureGen())
  {
    d_sg_gen.reset(new ConjectureGenerator(qe, qs, qim, qr));
    modules.push_back(d_sg_gen.get());
  }
  if (!options::finiteModelFind() || options::fmfInstEngine())
  {
    d_inst_engine.reset(new InstantiationEngine(qe, qs, qim, qr, tr));
    modules.push_back(d_inst_engine.get());
  }
  if (options::cegqi())
  {
    d_i_cbqi.reset(new InstStrategyCegqi(qe, qs, qim, qr));
    modules.push_back(d_i_cbqi.get());
    qim.getInstantiate()->addRewriter(d_i_cbqi->getInstRewriter());
  }
  if (options::sygus())
  {
    d_synth_e.reset(new SynthEngine(qe, qs, qim, qr));
    modules.push_back(d_synth_e.get());
  }
  // finite model finding
  if (options::fmfBound())
  {
    d_bint.reset(new BoundedIntegers(qe, qs, qim, qr, dm));
    modules.push_back(d_bint.get());
  }
  if (options::finiteModelFind() || options::fmfBound())
  {
    d_model_engine.reset(new ModelEngine(qe, qs, qim, qr));
    modules.push_back(d_model_engine.get());
    Trace("quant-init-debug")
        << "Initialize model engine, mbqi : " << options::mbqiMode() << " "
        << options::fmfBound() << std::endl;
    if (tr.useFmcModel())
    {
      Trace("quant-init-debug") << "...make fmc builder." << std::endl;
      d_builder.reset(new fmcheck::FullModelChecker(qs, qr, qim));
    }
    else
    {
      Trace("quant-init-debug")
          << "...make default model builder." << std::endl;
      d_builder.reset(new QModelBuilder(qs, qr, qim));
    }
  }
  if (options::quantDynamicSplit() != options::QuantDSplitMode::NONE)
  {
    d_qsplit.reset(new QuantDSplit(qe, qs, qim, qr));
    modules.push_back(d_qsplit.get());
  }
  if (options::quantAlphaEquiv())
  {
    d_alpha_equiv.reset(new AlphaEquivalence(qe));
  }
  // full saturation : instantiate from relevant domain, then arbitrary terms
  if (options::fullSaturateQuant() || options::fullSaturateInterleave())
  {
    d_rel_dom.reset(new RelevantDomain(qe, qr));
    d_fs.reset(new InstStrategyEnum(qe, qs, qim, qr, d_rel_dom.get()));
    modules.push_back(d_fs.get());
  }
  if (options::poolInst())
  {
    d_ipool.reset(new InstStrategyPool(qe, qs, qim, qr));
    modules.push_back(d_ipool.get());
  }
  if (options::sygusInst())
  {
    d_sygus_inst.reset(new SygusInst(qe, qs, qim, qr));
    modules.push_back(d_sygus_inst.get());
  }
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC4
