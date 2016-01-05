/*********************                                                        */
/*! \file smt2_quote_string.cpp
 ** \verbatim
 ** Original author: Tim King
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2014  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Quotes a string if necessary for smt2.
 **
 ** Quotes a string if necessary for smt2.
 **/

#include "util/smt2_quote_string.h"

#include <string>

namespace CVC4 {

/**
 * SMT-LIB 2 quoting for symbols
 */
std::string quoteSymbol(const std::string& s){
  if(s.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~!@$%^&*_-+=<>.?/") == std::string::npos) {
    // simple unquoted symbol
    return s;
  } else {
    std::string tmp(s);
    // must quote the symbol, but it cannot contain | or \, we turn those into _
    size_t p;
    while((p = tmp.find_first_of("\\|")) != std::string::npos) {
      tmp = tmp.replace(p, 1, "_");
    }
    return "|" + tmp + "|";
  }
}

}/* CVC4 namespace */
