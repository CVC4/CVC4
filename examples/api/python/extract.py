#!/usr/bin/env python3

#####################
#! \file extract.py
 ## \verbatim
 ## Top contributors (to current version):
 ##   Makai Mann
 ## This file is part of the CVC4 project.
 ## Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ## in the top-level source directory) and their institutional affiliations.
 ## All rights reserved.  See the file COPYING in the top-level source
 ## directory for licensing information.\endverbatim
 ##
 ## \brief A simple demonstration of the solving capabilities of the CVC4
 ## bit-vector solver through the Python API. This is a direct translation
 ## of extract-new.cpp.

from pycvc4 import Solver
from pycvc4.kinds import BVExtractOp, BVExtract, Equal

if __name__ == "__main__":
    slv = Solver()
    slv.setLogic("QF_BV")

    bitvector32 = slv.mkBitVectorSort(32)

    x = slv.mkVar(bitvector32, "a")

    ext_31_1 = slv.mkOpTerm(BVExtractOp, 31, 1)
    x_31_1 = slv.mkTerm(BVExtract, ext_31_1, x)

    ext_30_0 = slv.mkOpTerm(BVExtractOp, 30, 0)
    x_30_0 = slv.mkTerm(BVExtract, ext_30_0, x)

    ext_31_31 = slv.mkOpTerm(BVExtractOp, 31, 31)
    x_31_31 = slv.mkTerm(BVExtract, ext_31_31, x)

    ext_0_0 = slv.mkOpTerm(BVExtractOp, 0, 0)
    x_0_0 = slv.mkTerm(BVExtract, ext_0_0, x)

    eq = slv.mkTerm(Equal, x_31_1, x_30_0)
    print("Asserting:", eq)
    slv.assertFormula(eq)

    eq2 = slv.mkTerm(Equal, x_31_31, x_0_0)
    print("Check validity assuming:", eq2)
    print("Expect valid")
    print("CVC4:", slv.checkValidAssuming(eq2))
