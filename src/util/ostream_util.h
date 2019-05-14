/*********************                                                        */
/*! \file ostream_util.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Utilities for using ostreams.
 **
 ** Utilities for using ostreams.
 **/

#include "cvc4_private.h"

#ifndef CVC4__UTIL__OSTREAM_UTIL_H
#define CVC4__UTIL__OSTREAM_UTIL_H

#include <ios>
#include <ostream>

namespace CVC4 {

// Saves the formatting of an ostream and restores the previous settings on
// destruction. Example usage:
//   void Foo::Print(std::ostream& out) {
//     StreamFormatScope format_scope(out);
//     out << std::setprecision(6) << bar();
//   }
class StreamFormatScope
{
 public:
  // `out` must outlive StreamFormatScope.
  StreamFormatScope(std::ostream& out);
  ~StreamFormatScope();

 private:
  // Does not own the memory of d_out
  std::ostream& d_out;
  std::ios_base::fmtflags d_format_flags;
  std::streamsize d_precision;
};

}  // namespace CVC4

#endif /* CVC4__UTIL__OSTREAM_UTIL_H */
