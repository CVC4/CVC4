/*********************                                                        */
/*! \file preprocessing_pass_registry.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **  Justin Xu
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Preprocessing pass registry
 **
 ** Preprocessing pass registry that registers the preprocessing passes.
 **/
#include "preproc/preprocessing_pass_registry.h"
#include <utility>
#include "base/output.h"
#include "preproc/preprocessing_pass.h"

namespace CVC4 {
namespace preproc {

void PreprocessingPassRegistry::init(PreprocessingPassAPI* api) {
  for (std::pair<std::string, PreprocessingPass*>&& element :
       d_stringToPreprocessingPass) {
    element.second->init(api);
  }
}

void PreprocessingPassRegistry::registerPass(
    const std::string& ppName, PreprocessingPass* preprocessingPass) {
  Debug("pp-registry") << "Registering pass " << ppName << std::endl;
  d_stringToPreprocessingPass[ppName] = preprocessingPass;
}

PreprocessingPass* PreprocessingPassRegistry::getPass(
    const std::string& ppName) {
  assert(d_stringToPreprocessingPass.find(ppName) !=
         d_stringToPreprocessingPass.end());
  return d_stringToPreprocessingPass[ppName];
}

PreprocessingPassRegistry::~PreprocessingPassRegistry() {
  for (std::pair<std::string, PreprocessingPass*>&& element :
       d_stringToPreprocessingPass) {
    delete element.second;
  }
}

}  // namespace preproc
}  // namespace CVC4
