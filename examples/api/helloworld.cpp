/*********************                                                        */
/*! \file helloworld.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Aina Niemetz, Tim King
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief A very simple CVC4 example
 **
 ** A very simple CVC4 tutorial example.
 **/

#include <cvc5/cvc5.h>

#include <iostream>

using namespace cvc5::api;

int main()
{
  Solver slv;
  Term helloworld = slv.mkVar(slv.getBooleanSort(), "Hello World!");
  std::cout << helloworld << " is " << slv.checkEntailed(helloworld)
            << std::endl;
  return 0;
}
