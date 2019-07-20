/*********************                                                        */
/*! \file infer_info.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Tianyi Liang, Morgan Deters
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of inference information utility.
 **/

#include "theory/strings/infer_info.h"

using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace strings {

std::ostream& operator<<(std::ostream& out, Inference i)
{
  switch (i)
  {
    case INFER_SSPLIT_CST_PROP: out << "S-Split(CST-P)-prop"; break;
    case INFER_SSPLIT_VAR_PROP: out << "S-Split(VAR)-prop"; break;
    case INFER_LEN_SPLIT: out << "Len-Split(Len)"; break;
    case INFER_LEN_SPLIT_EMP: out << "Len-Split(Emp)"; break;
    case INFER_SSPLIT_CST_BINARY: out << "S-Split(CST-P)-binary"; break;
    case INFER_SSPLIT_CST: out << "S-Split(CST-P)"; break;
    case INFER_SSPLIT_VAR: out << "S-Split(VAR)"; break;
    case INFER_FLOOP: out << "F-Loop"; break;
    default: out << "?"; break;
  }
  return out;
}

}  // namespace strings
}  // namespace theory
}  // namespace CVC4
