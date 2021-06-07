/******************************************************************************
 * Top contributors (to current version):
 *   Tim King, Gereon Kremer, Andrew Reynolds
 *
 * This file is part of the cvc5 project.
 *
 * Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 * in the top-level source directory and their institutional affiliations.
 * All rights reserved.  See the file COPYING in the top-level source
 * directory for licensing information.
 * ****************************************************************************
 *
 * Definitions of public facing interface functions for Options.
 *
 * These are all one line wrappers for accessing the internal option data.
 */

#include "options_public.h"

#include <fstream>
#include <ostream>
#include <string>
#include <vector>

#include "base/listener.h"
#include "base/modal_exception.h"
#include "options/base_options.h"
#include "options/language.h"
#include "options/main_options.h"
#include "options/option_exception.h"
#include "options/options.h"
#include "options/printer_modes.h"
#include "options/printer_options.h"
#include "options/smt_options.h"
#include "options/uf_options.h"

namespace cvc5::options {

InstFormatMode getInstFormatMode(const Options& opts)
{
  return opts.printer.instFormatMode;
}
bool getUfHo(const Options& opts) { return opts.uf.ufHo; }
bool getIncrementalSolving(const Options& opts)
{
  return opts.smt.incrementalSolving;
}
const std::string& getBinaryName(const Options& opts)
{
  return opts.base.binary_name;
}

bool wasSetByUserIncrementalSolving(const Options& opts)
{
  return opts.smt.incrementalSolvingWasSetByUser;
}

}  // namespace cvc5::options
