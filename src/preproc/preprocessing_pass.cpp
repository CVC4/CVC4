/*********************                                                        */
/*! \file preprocessing_pass.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **  Justin Xu
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Preprocessing pass super class
 **
 ** Preprocessing pass super class.
 **/
#include "preproc/preprocessing_pass.h"

namespace CVC4 {
namespace preproc {

PreprocessingPassResult PreprocessingPass::apply(
    AssertionPipeline* assertionsToPreprocess) {
  TimerStat::CodeTimer codeTimer(d_timer);
  Trace("preproc") << "PRE " << d_name << std::endl;
  Chat() << d_name << "..." << std::endl;
  dumpAssertions(("pre-" + d_name).c_str(), *assertionsToPreprocess);
  PreprocessingPassResult result = applyInternal(assertionsToPreprocess);
  dumpAssertions(("post-" + d_name).c_str(), *assertionsToPreprocess);
  Trace("preproc") << "POST " << d_name << std::endl;
  return result;
}

void PreprocessingPass::dumpAssertions(const char* key,
                                       const AssertionPipeline& assertionList) {
  if (Dump.isOn("assertions") && Dump.isOn(std::string("assertions:") + key)) {
    // Push the simplified assertions to the dump output stream
    for (unsigned i = 0; i < assertionList.size(); ++i) {
      TNode n = assertionList[i];
      Dump("assertions") << AssertCommand(Expr(n.toExpr()));
    }
  }
}

PreprocessingPass::PreprocessingPass(
    PreprocessingPassContext* preprocContext, 
    const std::string& name)
    : d_name(name), d_timer("preproc::" + name) {
  d_preprocContext = preprocContext; 
  smtStatisticsRegistry()->registerStat(&d_timer);
  initInternal(preprocContext);
//  preprocessingPassRegistry->registerPass(name, this);
}

PreprocessingPass::~PreprocessingPass() {
  Assert(smt::smtEngineInScope());
  if (smtStatisticsRegistry() != NULL) {
    smtStatisticsRegistry()->unregisterStat(&d_timer);
  }
}

}  // namespace preproc
}  // namespace CVC4
