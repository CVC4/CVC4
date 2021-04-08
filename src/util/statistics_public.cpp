/*********************                                                        */
/*! \file statistics_public.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Gereon Kremer
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Registration of public statistics
 **
 ** Registration and documentation for all public statistics.
 **/

#include "util/statistics_public.h"

#include "api/cpp/cvc5_kind.h"
#include "expr/kind.h"
#include "theory/inference_id.h"
#include "util/statistics_registry.h"

namespace cvc5 {

void registerPublicStatistics(StatisticsRegistry& reg)
{
  reg.registerHistogram<TypeConstant>("api::CONSTANT", false);
  reg.registerHistogram<TypeConstant>("api::VARIABLE", false);
  reg.registerHistogram<api::Kind>("api::TERM", false);

  reg.registerValue<std::string>("driver::filename", false);
  reg.registerValue<std::string>("driver::sat/unsat", false);
  reg.registerValue<double>("driver::totalTime", false);

  for (const std::string& theory : {"arith",
                                    "arrays",
                                    "bags",
                                    "bv",
                                    "datatypes",
                                    "fp",
                                    "sep",
                                    "sets",
                                    "strings",
                                    "uf"})
  {
    reg.registerHistogram<theory::InferenceId>(
        "theory::" + theory + "::inferencesConflict", false);
    reg.registerHistogram<theory::InferenceId>(
        "theory::" + theory + "::inferencesFact", false);
    reg.registerHistogram<theory::InferenceId>(
        "theory::" + theory + "::inferencesLemma", false);
  }
}

}  // namespace cvc5
