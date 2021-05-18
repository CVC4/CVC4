###############################################################################
# Top contributors (to current version):
#   Yoni Zohar, Ying Sheng
#
# This file is part of the cvc5 project.
#
# Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
# in the top-level source directory and their institutional affiliations.
# All rights reserved.  See the file COPYING in the top-level source
# directory for licensing information.
# #############################################################################
##

import pytest
import pycvc5
import sys

from pycvc5 import kinds


@pytest.fixture
def solver():
    return pycvc5.Solver()


def test_recoverable_exception(solver):
    solver.setOption("produce-models", "true")
    x = solver.mkConst(solver.getBooleanSort(), "x")
    solver.assertFormula(x.eqTerm(x).notTerm())
    with pytest.raises(RuntimeError):
        c = solver.getValue(x)


def test_supports_floating_point(solver):
    if solver.supportsFloatingPoint():
        solver.mkRoundingMode(pycvc5.RoundNearestTiesToEven)
    else:
        with pytest.raises(RuntimeError):
            solver.mkRoundingMode(pycvc5.RoundNearestTiesToEven)


def test_get_boolean_sort(solver):
    solver.getBooleanSort()


def test_get_integer_sort(solver):
    solver.getIntegerSort()


def test_get_real_sort(solver):
    solver.getRealSort()


def test_get_reg_exp_sort(solver):
    solver.getRegExpSort()


def test_get_string_sort(solver):
    solver.getStringSort()


def test_get_rounding_mode_sort(solver):
    if solver.supportsFloatingPoint():
        solver.getRoundingModeSort()
    else:
        with pytest.raises(RuntimeError):
            solver.getRoundingModeSort()


def test_mk_array_sort(solver):
    boolSort = solver.getBooleanSort()
    intSort = solver.getIntegerSort()
    realSort = solver.getRealSort()
    bvSort = solver.mkBitVectorSort(32)
    solver.mkArraySort(boolSort, boolSort)
    solver.mkArraySort(intSort, intSort)
    solver.mkArraySort(realSort, realSort)
    solver.mkArraySort(bvSort, bvSort)
    solver.mkArraySort(boolSort, intSort)
    solver.mkArraySort(realSort, bvSort)

    if (solver.supportsFloatingPoint()):
        fpSort = solver.mkFloatingPointSort(3, 5)
        solver.mkArraySort(fpSort, fpSort)
        solver.mkArraySort(bvSort, fpSort)

    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.mkArraySort(boolSort, boolSort)


def test_mk_bit_vector_sort(solver):
    solver.mkBitVectorSort(32)
    with pytest.raises(RuntimeError):
        solver.mkBitVectorSort(0)


def test_mk_floating_point_sort(solver):
    if solver.supportsFloatingPoint():
        solver.mkFloatingPointSort(4, 8)
        with pytest.raises(RuntimeError):
            solver.mkFloatingPointSort(0, 8)
        with pytest.raises(RuntimeError):
            solver.mkFloatingPointSort(4, 0)
    else:
        with pytest.raises(RuntimeError):
            solver.mkFloatingPointSort(4, 8)


def test_mk_datatype_sort(solver):
    dtypeSpec = solver.mkDatatypeDecl("list")
    cons = solver.mkDatatypeConstructorDecl("cons")
    cons.addSelector("head", solver.getIntegerSort())
    dtypeSpec.addConstructor(cons)
    nil = solver.mkDatatypeConstructorDecl("nil")
    dtypeSpec.addConstructor(nil)
    solver.mkDatatypeSort(dtypeSpec)

    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.mkDatatypeSort(dtypeSpec)

    throwsDtypeSpec = solver.mkDatatypeDecl("list")
    with pytest.raises(RuntimeError):
        solver.mkDatatypeSort(throwsDtypeSpec)


def test_mk_datatype_sorts(solver):
    slv = pycvc5.Solver()

    dtypeSpec1 = solver.mkDatatypeDecl("list1")
    cons1 = solver.mkDatatypeConstructorDecl("cons1")
    cons1.addSelector("head1", solver.getIntegerSort())
    dtypeSpec1.addConstructor(cons1)
    nil1 = solver.mkDatatypeConstructorDecl("nil1")
    dtypeSpec1.addConstructor(nil1)

    dtypeSpec2 = solver.mkDatatypeDecl("list2")
    cons2 = solver.mkDatatypeConstructorDecl("cons2")
    cons2.addSelector("head2", solver.getIntegerSort())
    dtypeSpec2.addConstructor(cons2)
    nil2 = solver.mkDatatypeConstructorDecl("nil2")
    dtypeSpec2.addConstructor(nil2)

    decls = [dtypeSpec1, dtypeSpec2]
    solver.mkDatatypeSorts(decls, [])

    with pytest.raises(RuntimeError):
        slv.mkDatatypeSorts(decls, [])

    throwsDtypeSpec = solver.mkDatatypeDecl("list")
    throwsDecls = [throwsDtypeSpec]
    with pytest.raises(RuntimeError):
        solver.mkDatatypeSorts(throwsDecls, [])

    # with unresolved sorts
    unresList = solver.mkUninterpretedSort("ulist")
    unresSorts = [unresList]
    ulist = solver.mkDatatypeDecl("ulist")
    ucons = solver.mkDatatypeConstructorDecl("ucons")
    ucons.addSelector("car", unresList)
    ucons.addSelector("cdr", unresList)
    ulist.addConstructor(ucons)
    unil = solver.mkDatatypeConstructorDecl("unil")
    ulist.addConstructor(unil)
    udecls = [ulist]

    solver.mkDatatypeSorts(udecls, unresSorts)
    with pytest.raises(RuntimeError):
        slv.mkDatatypeSorts(udecls, unresSorts)


def test_mk_function_sort(solver):
    funSort = solver.mkFunctionSort(solver.mkUninterpretedSort("u"),\
            solver.getIntegerSort())

    # function arguments are allowed
    solver.mkFunctionSort(funSort, solver.getIntegerSort())

    # non-first-class arguments are not allowed
    reSort = solver.getRegExpSort()
    with pytest.raises(RuntimeError):
        solver.mkFunctionSort(reSort, solver.getIntegerSort())
    with pytest.raises(RuntimeError):
        solver.mkFunctionSort(solver.getIntegerSort(), funSort)

    solver.mkFunctionSort([solver.mkUninterpretedSort("u"),\
            solver.getIntegerSort()],\
            solver.getIntegerSort())
    funSort2 = solver.mkFunctionSort(solver.mkUninterpretedSort("u"),\
            solver.getIntegerSort())

    # function arguments are allowed
    solver.mkFunctionSort([funSort2, solver.mkUninterpretedSort("u")],\
            solver.getIntegerSort())

    with pytest.raises(RuntimeError):
        solver.mkFunctionSort([solver.getIntegerSort(),\
                solver.mkUninterpretedSort("u")],\
                funSort2)

    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.mkFunctionSort(solver.mkUninterpretedSort("u"),\
                solver.getIntegerSort())
    with pytest.raises(RuntimeError):
        slv.mkFunctionSort(slv.mkUninterpretedSort("u"),\
                solver.getIntegerSort())
    sorts1 = [solver.getBooleanSort(),\
            slv.getIntegerSort(),\
            solver.getIntegerSort()]
    sorts2 = [slv.getBooleanSort(), slv.getIntegerSort()]
    slv.mkFunctionSort(sorts2, slv.getIntegerSort())
    with pytest.raises(RuntimeError):
        slv.mkFunctionSort(sorts1, slv.getIntegerSort())
    with pytest.raises(RuntimeError):
        slv.mkFunctionSort(sorts2, solver.getIntegerSort())


def test_mk_param_sort(solver):
    solver.mkParamSort("T")
    solver.mkParamSort("")


def test_mk_predicate_sort(solver):
    solver.mkPredicateSort([solver.getIntegerSort()])
    with pytest.raises(RuntimeError):
        solver.mkPredicateSort([])

    funSort = solver.mkFunctionSort(solver.mkUninterpretedSort("u"),\
            solver.getIntegerSort())
    # functions as arguments are allowed
    solver.mkPredicateSort([solver.getIntegerSort(), funSort])

    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.mkPredicateSort([solver.getIntegerSort()])


def test_mk_record_sort(solver):
    fields = [("b", solver.getBooleanSort()),\
              ("bv", solver.mkBitVectorSort(8)),\
              ("i", solver.getIntegerSort())]
    empty = []
    solver.mkRecordSort(fields)
    solver.mkRecordSort(empty)
    recSort = solver.mkRecordSort(fields)
    recSort.getDatatype()


def test_mk_set_sort(solver):
    solver.mkSetSort(solver.getBooleanSort())
    solver.mkSetSort(solver.getIntegerSort())
    solver.mkSetSort(solver.mkBitVectorSort(4))
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.mkSetSort(solver.mkBitVectorSort(4))


def test_mk_sequence_sort(solver):
    solver.mkSequenceSort(solver.getBooleanSort())
    solver.mkSequenceSort(\
            solver.mkSequenceSort(solver.getIntegerSort()))
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.mkSequenceSort(solver.getIntegerSort())


def test_mk_uninterpreted_sort(solver):
    solver.mkUninterpretedSort("u")
    solver.mkUninterpretedSort("")


def test_mk_sortConstructor_sort(solver):
    solver.mkSortConstructorSort("s", 2)
    solver.mkSortConstructorSort("", 2)
    with pytest.raises(RuntimeError):
        solver.mkSortConstructorSort("", 0)


def test_mk_tuple_sort(solver):
    solver.mkTupleSort([solver.getIntegerSort()])
    funSort = solver.mkFunctionSort(solver.mkUninterpretedSort("u"),\
                                    solver.getIntegerSort())
    with pytest.raises(RuntimeError):
        solver.mkTupleSort([solver.getIntegerSort(), funSort])

    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.mkTupleSort([solver.getIntegerSort()])


def test_mk_var(solver):
    boolSort = solver.getBooleanSort()
    intSort = solver.getIntegerSort()
    funSort = solver.mkFunctionSort(intSort, boolSort)
    solver.mkVar(boolSort)
    solver.mkVar(funSort)
    solver.mkVar(boolSort, "b")
    solver.mkVar(funSort, "")
    with pytest.raises(RuntimeError):
        solver.mkVar(pycvc5.Sort(solver))
    with pytest.raises(RuntimeError):
        solver.mkVar(pycvc5.Sort(solver), "a")
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.mkVar(boolSort, "x")


def test_mk_boolean(solver):
    solver.mkBoolean(True)
    solver.mkBoolean(False)


def test_mk_rounding_mode(solver):
    if solver.supportsFloatingPoint():
        solver.mkRoundingMode(pycvc5.RoundTowardZero)
    else:
        with pytest.raises(RuntimeError):
            solver.mkRoundingMode(pycvc5.RoundTowardZero)


def test_mk_uninterpreted_const(solver):
    solver.mkUninterpretedConst(solver.getBooleanSort(), 1)
    with pytest.raises(RuntimeError):
        solver.mkUninterpretedConst(pycvc5.Sort(solver), 1)
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.mkUninterpretedConst(solver.getBooleanSort(), 1)


def test_mk_floating_point(solver):
    t1 = solver.mkBitVector(8)
    t2 = solver.mkBitVector(4)
    t3 = solver.mkInteger(2)
    if (solver.supportsFloatingPoint()):
        solver.mkFloatingPoint(3, 5, t1)
    else:
        with pytest.raises(RuntimeError):
            solver.mkFloatingPoint(3, 5, t1)

    with pytest.raises(RuntimeError):
        solver.mkFloatingPoint(0, 5, pycvc5.Term(solver))
    with pytest.raises(RuntimeError):
        solver.mkFloatingPoint(0, 5, t1)
    with pytest.raises(RuntimeError):
        solver.mkFloatingPoint(3, 0, t1)
    with pytest.raises(RuntimeError):
        solver.mkFloatingPoint(3, 5, t2)
    with pytest.raises(RuntimeError):
        solver.mkFloatingPoint(3, 5, t2)

    if (solver.supportsFloatingPoint()):
        slv = pycvc5.Solver()
        with pytest.raises(RuntimeError):
            slv.mkFloatingPoint(3, 5, t1)


def test_mk_empty_set(solver):
    slv = pycvc5.Solver()
    s = solver.mkSetSort(solver.getBooleanSort())
    solver.mkEmptySet(pycvc5.Sort(solver))
    solver.mkEmptySet(s)
    with pytest.raises(RuntimeError):
        solver.mkEmptySet(solver.getBooleanSort())
    with pytest.raises(RuntimeError):
        slv.mkEmptySet(s)


def test_mk_empty_sequence(solver):
    slv = pycvc5.Solver()
    s = solver.mkSequenceSort(solver.getBooleanSort())
    solver.mkEmptySequence(s)
    solver.mkEmptySequence(solver.getBooleanSort())
    with pytest.raises(RuntimeError):
        slv.mkEmptySequence(s)


def test_mk_false(solver):
    solver.mkFalse()
    solver.mkFalse()


def test_mk_nan(solver):
    if (solver.supportsFloatingPoint()):
        solver.mkNaN(3, 5)
    else:
        with pytest.raises(RuntimeError):
            solver.mkNaN(3, 5)


def test_mk_neg_zero(solver):
    if (solver.supportsFloatingPoint()):
        solver.mkNegZero(3, 5)
    else:
        with pytest.raises(RuntimeError):
            solver.mkNegZero(3, 5)


def test_mk_neg_inf(solver):
    if (solver.supportsFloatingPoint()):
        solver.mkNegInf(3, 5)
    else:
        with pytest.raises(RuntimeError):
            solver.mkNegInf(3, 5)


def test_mk_pos_inf(solver):
    if (solver.supportsFloatingPoint()):
        solver.mkPosInf(3, 5)
    else:
        with pytest.raises(RuntimeError):
            solver.mkPosInf(3, 5)


def test_mk_pos_zero(solver):
    if (solver.supportsFloatingPoint()):
        solver.mkPosZero(3, 5)
    else:
        with pytest.raises(RuntimeError):
            solver.mkPosZero(3, 5)


def test_mk_pi(solver):
    solver.mkPi()


def test_mk_integer(solver):
    solver.mkInteger("123")
    with pytest.raises(RuntimeError):
        solver.mkInteger("1.23")
    with pytest.raises(RuntimeError):
        solver.mkInteger("1/23")
    with pytest.raises(RuntimeError):
        solver.mkInteger("12/3")
    with pytest.raises(RuntimeError):
        solver.mkInteger(".2")
    with pytest.raises(RuntimeError):
        solver.mkInteger("2.")
    with pytest.raises(RuntimeError):
        solver.mkInteger("")
    with pytest.raises(RuntimeError):
        solver.mkInteger("asdf")
    with pytest.raises(RuntimeError):
        solver.mkInteger("1.2/3")
    with pytest.raises(RuntimeError):
        solver.mkInteger(".")
    with pytest.raises(RuntimeError):
        solver.mkInteger("/")
    with pytest.raises(RuntimeError):
        solver.mkInteger("2/")
    with pytest.raises(RuntimeError):
        solver.mkInteger("/2")

    solver.mkReal("123")
    with pytest.raises(RuntimeError):
        solver.mkInteger("1.23")
    with pytest.raises(RuntimeError):
        solver.mkInteger("1/23")
    with pytest.raises(RuntimeError):
        solver.mkInteger("12/3")
    with pytest.raises(RuntimeError):
        solver.mkInteger(".2")
    with pytest.raises(RuntimeError):
        solver.mkInteger("2.")
    with pytest.raises(RuntimeError):
        solver.mkInteger("")
    with pytest.raises(RuntimeError):
        solver.mkInteger("asdf")
    with pytest.raises(RuntimeError):
        solver.mkInteger("1.2/3")
    with pytest.raises(RuntimeError):
        solver.mkInteger(".")
    with pytest.raises(RuntimeError):
        solver.mkInteger("/")
    with pytest.raises(RuntimeError):
        solver.mkInteger("2/")
    with pytest.raises(RuntimeError):
        solver.mkInteger("/2")

    val1 = 1
    val2 = -1
    val3 = 1
    val4 = -1
    solver.mkInteger(val1)
    solver.mkInteger(val2)
    solver.mkInteger(val3)
    solver.mkInteger(val4)
    solver.mkInteger(val4)


def test_mk_real(solver):
    solver.mkReal("123")
    solver.mkReal("1.23")
    solver.mkReal("1/23")
    solver.mkReal("12/3")
    solver.mkReal(".2")
    solver.mkReal("2.")
    with pytest.raises(RuntimeError):
        solver.mkReal("")
    with pytest.raises(RuntimeError):
        solver.mkReal("asdf")
    with pytest.raises(RuntimeError):
        solver.mkReal("1.2/3")
    with pytest.raises(RuntimeError):
        solver.mkReal(".")
    with pytest.raises(RuntimeError):
        solver.mkReal("/")
    with pytest.raises(RuntimeError):
        solver.mkReal("2/")
    with pytest.raises(RuntimeError):
        solver.mkReal("/2")

    solver.mkReal("123")
    solver.mkReal("1.23")
    solver.mkReal("1/23")
    solver.mkReal("12/3")
    solver.mkReal(".2")
    solver.mkReal("2.")
    with pytest.raises(RuntimeError):
        solver.mkReal("")
    with pytest.raises(RuntimeError):
        solver.mkReal("asdf")
    with pytest.raises(RuntimeError):
        solver.mkReal("1.2/3")
    with pytest.raises(RuntimeError):
        solver.mkReal(".")
    with pytest.raises(RuntimeError):
        solver.mkReal("/")
    with pytest.raises(RuntimeError):
        solver.mkReal("2/")
    with pytest.raises(RuntimeError):
        solver.mkReal("/2")

    val1 = 1
    val2 = -1
    val3 = 1
    val4 = -1
    solver.mkReal(val1)
    solver.mkReal(val2)
    solver.mkReal(val3)
    solver.mkReal(val4)
    solver.mkReal(val4)
    solver.mkReal(val1, val1)
    solver.mkReal(val2, val2)
    solver.mkReal(val3, val3)
    solver.mkReal(val4, val4)


def test_mk_regexp_empty(solver):
    strSort = solver.getStringSort()
    s = solver.mkConst(strSort, "s")
    solver.mkTerm(kinds.StringInRegexp, s, solver.mkRegexpEmpty())


def test_mk_regexp_sigma(solver):
    strSort = solver.getStringSort()
    s = solver.mkConst(strSort, "s")
    solver.mkTerm(kinds.StringInRegexp, s, solver.mkRegexpSigma())


def test_mk_sep_nil(solver):
    solver.mkSepNil(solver.getBooleanSort())
    with pytest.raises(RuntimeError):
        solver.mkSepNil(pycvc5.Sort(solver))
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.mkSepNil(solver.getIntegerSort())


def test_mk_true(solver):
    solver.mkTrue()
    solver.mkTrue()


def test_mk_universe_set(solver):
    solver.mkUniverseSet(solver.getBooleanSort())
    with pytest.raises(RuntimeError):
        solver.mkUniverseSet(pycvc5.Sort(solver))
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.mkUniverseSet(solver.getBooleanSort())


def test_mk_const(solver):
    boolSort = solver.getBooleanSort()
    intSort = solver.getIntegerSort()
    funSort = solver.mkFunctionSort(intSort, boolSort)
    solver.mkConst(boolSort)
    solver.mkConst(funSort)
    solver.mkConst(boolSort, "b")
    solver.mkConst(intSort, "i")
    solver.mkConst(funSort, "f")
    solver.mkConst(funSort, "")
    with pytest.raises(RuntimeError):
        solver.mkConst(pycvc5.Sort(solver))
    with pytest.raises(RuntimeError):
        solver.mkConst(pycvc5.Sort(solver), "a")

    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.mkConst(boolSort)


def test_mk_const_array(solver):
    intSort = solver.getIntegerSort()
    arrSort = solver.mkArraySort(intSort, intSort)
    zero = solver.mkInteger(0)
    constArr = solver.mkConstArray(arrSort, zero)

    solver.mkConstArray(arrSort, zero)
    with pytest.raises(RuntimeError):
        solver.mkConstArray(pycvc5.Sort(solver), zero)
    with pytest.raises(RuntimeError):
        solver.mkConstArray(arrSort, pycvc5.Term(solver))
    with pytest.raises(RuntimeError):
        solver.mkConstArray(arrSort, solver.mkBitVector(1, 1))
    with pytest.raises(RuntimeError):
        solver.mkConstArray(intSort, zero)
    slv = pycvc5.Solver()
    zero2 = slv.mkInteger(0)
    arrSort2 = slv.mkArraySort(slv.getIntegerSort(), slv.getIntegerSort())
    with pytest.raises(RuntimeError):
        slv.mkConstArray(arrSort2, zero)
    with pytest.raises(RuntimeError):
        slv.mkConstArray(arrSort, zero2)


def test_declare_fun(solver):
    bvSort = solver.mkBitVectorSort(32)
    funSort = solver.mkFunctionSort(solver.mkUninterpretedSort("u"),\
                                    solver.getIntegerSort())
    solver.declareFun("f1", [], bvSort)
    solver.declareFun("f3", [bvSort, solver.getIntegerSort()], bvSort)
    with pytest.raises(RuntimeError):
        solver.declareFun("f2", [], funSort)
    # functions as arguments is allowed
    solver.declareFun("f4", [bvSort, funSort], bvSort)
    with pytest.raises(RuntimeError):
        solver.declareFun("f5", [bvSort, bvSort], funSort)
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.declareFun("f1", [], bvSort)


def test_declare_sort(solver):
    solver.declareSort("s", 0)
    solver.declareSort("s", 2)
    solver.declareSort("", 2)


def test_define_fun(solver):
    bvSort = solver.mkBitVectorSort(32)
    funSort1 = solver.mkFunctionSort([bvSort, bvSort], bvSort)
    funSort2 = solver.mkFunctionSort(solver.mkUninterpretedSort("u"),\
                                     solver.getIntegerSort())
    b1 = solver.mkVar(bvSort, "b1")
    b11 = solver.mkVar(bvSort, "b1")
    b2 = solver.mkVar(solver.getIntegerSort(), "b2")
    b3 = solver.mkVar(funSort2, "b3")
    v1 = solver.mkConst(bvSort, "v1")
    v2 = solver.mkConst(solver.getIntegerSort(), "v2")
    v3 = solver.mkConst(funSort2, "v3")
    f1 = solver.mkConst(funSort1, "f1")
    f2 = solver.mkConst(funSort2, "f2")
    f3 = solver.mkConst(bvSort, "f3")
    solver.defineFun("f", [], bvSort, v1)
    solver.defineFun("ff", [b1, b2], bvSort, v1)
    solver.defineFun(f1, [b1, b11], v1)
    with pytest.raises(RuntimeError):
        solver.defineFun("ff", [v1, b2], bvSort, v1)
    with pytest.raises(RuntimeError):
        solver.defineFun("fff", [b1], bvSort, v3)
    with pytest.raises(RuntimeError):
        solver.defineFun("ffff", [b1], funSort2, v3)
    # b3 has function sort, which is allowed as an argument
    solver.defineFun("fffff", [b1, b3], bvSort, v1)
    with pytest.raises(RuntimeError):
        solver.defineFun(f1, [v1, b11], v1)
    with pytest.raises(RuntimeError):
        solver.defineFun(f1, [b1], v1)
    with pytest.raises(RuntimeError):
        solver.defineFun(f1, [b1, b11], v2)
    with pytest.raises(RuntimeError):
        solver.defineFun(f1, [b1, b11], v3)
    with pytest.raises(RuntimeError):
        solver.defineFun(f2, [b1], v2)
    with pytest.raises(RuntimeError):
        solver.defineFun(f3, [b1], v1)

    slv = pycvc5.Solver()
    bvSort2 = slv.mkBitVectorSort(32)
    v12 = slv.mkConst(bvSort2, "v1")
    b12 = slv.mkVar(bvSort2, "b1")
    b22 = slv.mkVar(slv.getIntegerSort(), "b2")
    with pytest.raises(RuntimeError):
        slv.defineFun("f", [], bvSort, v12)
    with pytest.raises(RuntimeError):
        slv.defineFun("f", [], bvSort2, v1)
    with pytest.raises(RuntimeError):
        slv.defineFun("ff", [b1, b22], bvSort2, v12)
    with pytest.raises(RuntimeError):
        slv.defineFun("ff", [b12, b2], bvSort2, v12)
    with pytest.raises(RuntimeError):
        slv.defineFun("ff", [b12, b22], bvSort, v12)
    with pytest.raises(RuntimeError):
        slv.defineFun("ff", [b12, b22], bvSort2, v1)


def test_define_fun_rec(solver):
    bvSort = solver.mkBitVectorSort(32)
    funSort1 = solver.mkFunctionSort([bvSort, bvSort], bvSort)
    funSort2 = solver.mkFunctionSort(solver.mkUninterpretedSort("u"),\
                                     solver.getIntegerSort())
    b1 = solver.mkVar(bvSort, "b1")
    b11 = solver.mkVar(bvSort, "b1")
    b2 = solver.mkVar(solver.getIntegerSort(), "b2")
    b3 = solver.mkVar(funSort2, "b3")
    v1 = solver.mkConst(bvSort, "v1")
    v2 = solver.mkConst(solver.getIntegerSort(), "v2")
    v3 = solver.mkConst(funSort2, "v3")
    f1 = solver.mkConst(funSort1, "f1")
    f2 = solver.mkConst(funSort2, "f2")
    f3 = solver.mkConst(bvSort, "f3")
    solver.defineFunRec("f", [], bvSort, v1)
    solver.defineFunRec("ff", [b1, b2], bvSort, v1)
    solver.defineFunRec(f1, [b1, b11], v1)
    with pytest.raises(RuntimeError):
        solver.defineFunRec("fff", [b1], bvSort, v3)
    with pytest.raises(RuntimeError):
        solver.defineFunRec("ff", [b1, v2], bvSort, v1)
    with pytest.raises(RuntimeError):
        solver.defineFunRec("ffff", [b1], funSort2, v3)
    # b3 has function sort, which is allowed as an argument
    solver.defineFunRec("fffff", [b1, b3], bvSort, v1)
    with pytest.raises(RuntimeError):
        solver.defineFunRec(f1, [b1], v1)
    with pytest.raises(RuntimeError):
        solver.defineFunRec(f1, [b1, b11], v2)
    with pytest.raises(RuntimeError):
        solver.defineFunRec(f1, [b1, b11], v3)
    with pytest.raises(RuntimeError):
        solver.defineFunRec(f2, [b1], v2)
    with pytest.raises(RuntimeError):
        solver.defineFunRec(f3, [b1], v1)

    slv = pycvc5.Solver()
    bvSort2 = slv.mkBitVectorSort(32)
    v12 = slv.mkConst(bvSort2, "v1")
    b12 = slv.mkVar(bvSort2, "b1")
    b22 = slv.mkVar(slv.getIntegerSort(), "b2")
    slv.defineFunRec("f", [], bvSort2, v12)
    slv.defineFunRec("ff", [b12, b22], bvSort2, v12)
    with pytest.raises(RuntimeError):
        slv.defineFunRec("f", [], bvSort, v12)
    with pytest.raises(RuntimeError):
        slv.defineFunRec("f", [], bvSort2, v1)
    with pytest.raises(RuntimeError):
        slv.defineFunRec("ff", [b1, b22], bvSort2, v12)
    with pytest.raises(RuntimeError):
        slv.defineFunRec("ff", [b12, b2], bvSort2, v12)
    with pytest.raises(RuntimeError):
        slv.defineFunRec("ff", [b12, b22], bvSort, v12)
    with pytest.raises(RuntimeError):
        slv.defineFunRec("ff", [b12, b22], bvSort2, v1)


def test_define_fun_rec_wrong_logic(solver):
    solver.setLogic("QF_BV")
    bvSort = solver.mkBitVectorSort(32)
    funSort = solver.mkFunctionSort([bvSort, bvSort], bvSort)
    b = solver.mkVar(bvSort, "b")
    v = solver.mkConst(bvSort, "v")
    f = solver.mkConst(funSort, "f")
    with pytest.raises(RuntimeError):
        solver.defineFunRec("f", [], bvSort, v)
    with pytest.raises(RuntimeError):
        solver.defineFunRec(f, [b, b], v)


def test_uf_iteration(solver):
    intSort = solver.getIntegerSort()
    funSort = solver.mkFunctionSort([intSort, intSort], intSort)
    x = solver.mkConst(intSort, "x")
    y = solver.mkConst(intSort, "y")
    f = solver.mkConst(funSort, "f")
    fxy = solver.mkTerm(kinds.ApplyUf, f, x, y)

    # Expecting the uninterpreted function to be one of the children
    expected_children = [f, x, y]
    idx = 0
    for c in fxy:
        assert idx < 3
        assert c == expected_children[idx]
        idx = idx + 1


def test_get_info(solver):
    solver.getInfo("name")
    with pytest.raises(RuntimeError):
        solver.getInfo("asdf")

# TODO getAbduct

# TODO getInterpolant

# TODO check if the declarePool should assert non-null sort
def test_declare_pool(solver):
    intSort = solver.getIntegerSort()
    setSort = solver.mkSetSort(intSort)
    zero = solver.mkInteger(0)
    x = solver.mkConst(intSort, "x")
    y = solver.mkConst(intSort, "y")
    # declare a pool with initial value { 0, x, y }
    p = solver.declarePool("p", intSort, [zero, x, y])
    # pool should have the same sort
    assert p.getSort() == setSort
    # cannot pass null sort
    nullSort = None
#    with pytest.raises(RuntimeError):
#        solver.declarePool("i", nullSort, [])

def test_get_op(solver):
    bv32 = solver.mkBitVectorSort(32)
    a = solver.mkConst(bv32, "a")
    ext = solver.mkOp(kinds.BVExtract, 2, 1)
    exta = solver.mkTerm(ext, a)

    assert not a.hasOp()
    with pytest.raises(RuntimeError):
        a.getOp()
    assert exta.hasOp()
    assert exta.getOp() == ext

    # Test Datatypes -- more complicated
    consListSpec = solver.mkDatatypeDecl("list")
    cons = solver.mkDatatypeConstructorDecl("cons")
    cons.addSelector("head", solver.getIntegerSort())
    cons.addSelectorSelf("tail")
    consListSpec.addConstructor(cons)
    nil = solver.mkDatatypeConstructorDecl("nil")
    consListSpec.addConstructor(nil)
    consListSort = solver.mkDatatypeSort(consListSpec)
    consList = consListSort.getDatatype()

    consTerm = consList.getConstructorTerm("cons")
    nilTerm = consList.getConstructorTerm("nil")
    headTerm = consList["cons"].getSelectorTerm("head")

    listnil = solver.mkTerm(kinds.ApplyConstructor, nilTerm)
    listcons1 = solver.mkTerm(
        kinds.ApplyConstructor, consTerm, solver.mkInteger(1), listnil)
    listhead = solver.mkTerm(kinds.ApplySelector, headTerm, listcons1)

    assert listnil.hasOp()
    assert listcons1.hasOp()
    assert listhead.hasOp()

def test_get_option(solver):
    solver.getOption("incremental")
    with pytest.raises(RuntimeError):
        solver.getOption("asdf")

def test_get_unsat_assumptions1(solver):
    solver.setOption("incremental", "false")
    solver.checkSatAssuming(solver.mkFalse())
    with pytest.raises(RuntimeError):
        solver.getUnsatAssumptions()

def test_get_unsat_assumptions2(solver):
    solver.setOption("incremental", "true")
    solver.setOption("produce-unsat-assumptions", "false")
    solver.checkSatAssuming(solver.mkFalse())
    with pytest.raises(RuntimeError):
        solver.getUnsatAssumptions()

def test_get_unsat_assumptions3(solver):
    solver.setOption("incremental", "true")
    solver.setOption("produce-unsat-assumptions", "true")
    solver.checkSatAssuming(solver.mkFalse())
    solver.getUnsatAssumptions()
    solver.checkSatAssuming(solver.mkTrue())
    with pytest.raises(RuntimeError):
        solver.getUnsatAssumptions()

def test_get_unsat_core1(solver):
    solver.setOption("incremental", "false")
    solver.assertFormula(solver.mkFalse())
    solver.checkSat()
    with pytest.raises(RuntimeError):
        solver.getUnsatCore()

def test_get_unsat_core2(solver):
    solver.setOption("incremental", "false")
    solver.setOption("produce-unsat-cores", "false")
    solver.assertFormula(solver.mkFalse())
    solver.checkSat()
    with pytest.raises(RuntimeError):
        solver.getUnsatCore()

def test_get_unsat_core3(solver):
    solver.setOption("incremental", "true")
    solver.setOption("produce-unsat-cores", "true")

    uSort = solver.mkUninterpretedSort("u")
    intSort = solver.getIntegerSort()
    boolSort = solver.getBooleanSort()
    uToIntSort = solver.mkFunctionSort(uSort, intSort)
    intPredSort = solver.mkFunctionSort(intSort, boolSort)

    x = solver.mkConst(uSort, "x")
    y = solver.mkConst(uSort, "y")
    f = solver.mkConst(uToIntSort, "f")
    p = solver.mkConst(intPredSort, "p")
    zero = solver.mkInteger(0)
    one = solver.mkInteger(1)
    f_x = solver.mkTerm(kinds.ApplyUf, f, x)
    f_y = solver.mkTerm(kinds.ApplyUf, f, y)
    summ = solver.mkTerm(kinds.Plus, f_x, f_y)
    p_0 = solver.mkTerm(kinds.ApplyUf, p, zero)
    p_f_y = solver.mkTerm(kinds.ApplyUf, p, f_y)
    solver.assertFormula(solver.mkTerm(kinds.Gt, zero, f_x))
    solver.assertFormula(solver.mkTerm(kinds.Gt, zero, f_y))
    solver.assertFormula(solver.mkTerm(kinds.Gt, summ, one))
    solver.assertFormula(p_0)
    solver.assertFormula(p_f_y.notTerm())
    assert solver.checkSat().isUnsat()

    unsat_core = solver.getUnsatCore()

    solver.resetAssertions()
    for t in unsat_core:
        solver.assertFormula(t)
    res = solver.checkSat()
    assert res.isUnsat()

def test_get_value1(solver):
    solver.setOption("produce-models", "false")
    t = solver.mkTrue()
    solver.assertFormula(t)
    solver.checkSat()
    with pytest.raises(RuntimeError):
        solver.getValue(t)

def test_get_value2(solver):
    solver.setOption("produce-models", "true")
    t = solver.mkFalse()
    solver.assertFormula(t)
    solver.checkSat()
    with pytest.raises(RuntimeError):
        solver.getValue(t)

def test_get_value3(solver):
    solver.setOption("produce-models", "true")
    uSort = solver.mkUninterpretedSort("u")
    intSort = solver.getIntegerSort()
    boolSort = solver.getBooleanSort()
    uToIntSort = solver.mkFunctionSort(uSort, intSort)
    intPredSort = solver.mkFunctionSort(intSort, boolSort)

    x = solver.mkConst(uSort, "x")
    y = solver.mkConst(uSort, "y")
    z = solver.mkConst(uSort, "z")
    f = solver.mkConst(uToIntSort, "f")
    p = solver.mkConst(intPredSort, "p")
    zero = solver.mkInteger(0)
    one = solver.mkInteger(1)
    f_x = solver.mkTerm(kinds.ApplyUf, f, x)
    f_y = solver.mkTerm(kinds.ApplyUf, f, y)
    summ = solver.mkTerm(kinds.Plus, f_x, f_y)
    p_0 = solver.mkTerm(kinds.ApplyUf, p, zero)
    p_f_y = solver.mkTerm(kinds.ApplyUf, p, f_y)

    solver.assertFormula(solver.mkTerm(kinds.Leq, zero, f_x))
    solver.assertFormula(solver.mkTerm(kinds.Leq, zero, f_y))
    solver.assertFormula(solver.mkTerm(kinds.Leq, summ, one))
    solver.assertFormula(p_0.notTerm())
    solver.assertFormula(p_f_y)
    assert solver.checkSat().isSat()
    solver.getValue(x)
    solver.getValue(y)
    solver.getValue(z)
    solver.getValue(summ)
    solver.getValue(p_f_y)

    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.getValue(x)

# TODO: no api getQuantifierElimination
def test_get_quantifier_elimination(solver):
    x = solver.mkVar(solver.getBooleanSort(), "x")
    forall = solver.mkTerm(kinds.Forall,\
                      solver.mkTerm(kinds.BoundVarList, x),\
                      solver.mkTerm(kinds.Or, x, solver.mkTerm(kinds.Not, x)))
#    with pytest.raises(RuntimeError):
#        solver.getQuantifierElimination(pycvc5.Term(solver))
#  ASSERT_THROW(d_solver.getQuantifierElimination(Solver().mkBoolean(false)),
#               CVC5ApiException);
#  ASSERT_NO_THROW(d_solver.getQuantifierElimination(forall));

# TODO: no api
#def test_get_quantifier_elimination_disjunct(solver):

def test_declare_separation_heap(solver):
    solver.setLogic("ALL_SUPPORTED")
    integer = solver.getIntegerSort()
    solver.declareSeparationHeap(integer, integer)
    # cannot declare separation logic heap more than once
    with pytest.raises(RuntimeError):
        solver.declareSeparationHeap(integer, integer)

# Helper function for test_get_separation_{heap,nil}_termX. Asserts and checks
# some simple separation logic constraints.
def checkSimpleSeparationConstraints(slv):
    integer = slv.getIntegerSort()
    # declare the separation heap
    slv.declareSeparationHeap(integer, integer)
    x = slv.mkConst(integer, "x")
    p = slv.mkConst(integer, "p")
    heap = slv.mkTerm(kinds.SepPto, p, x)
    slv.assertFormula(heap)
    nil = slv.mkSepNil(integer)
    slv.assertFormula(nil.eqTerm(slv.mkReal(5)))
    slv.checkSat()

def test_get_separation_heap_term1(solver):
    solver.setLogic("QF_BV")
    solver.setOption("incremental", "false")
    solver.setOption("produce-models", "true")
    t = solver.mkTrue()
    solver.assertFormula(t)
    with pytest.raises(RuntimeError):
        solver.getSeparationHeap()


def test_get_separation_heap_term2(solver):
    solver.setLogic("ALL_SUPPORTED")
    solver.setOption("incremental", "false")
    solver.setOption("produce-models", "false")
    checkSimpleSeparationConstraints(solver)
    with pytest.raises(RuntimeError):
        solver.getSeparationHeap()

def test_get_separation_heap_term3(solver):
    solver.setLogic("ALL_SUPPORTED")
    solver.setOption("incremental", "false")
    solver.setOption("produce-models", "true")
    t = solver.mkFalse()
    solver.assertFormula(t)
    solver.checkSat()
    with pytest.raises(RuntimeError):
        solver.getSeparationHeap()

def test_get_separation_heap_term4(solver):
    solver.setLogic("ALL_SUPPORTED")
    solver.setOption("incremental", "false")
    solver.setOption("produce-models", "true")
    t = solver.mkTrue()
    solver.assertFormula(t)
    solver.checkSat()
    with pytest.raises(RuntimeError):
        solver.getSeparationHeap()

def test_get_separation_heap_term5(solver):
    solver.setLogic("ALL_SUPPORTED")
    solver.setOption("incremental", "false")
    solver.setOption("produce-models", "true")
    checkSimpleSeparationConstraints(solver)
    solver.getSeparationHeap()

def test_get_separation_nil_term1(solver):
    solver.setLogic("QF_BV")
    solver.setOption("incremental", "false")
    solver.setOption("produce-models", "true")
    t = solver.mkTrue()
    solver.assertFormula(t)
    with pytest.raises(RuntimeError):
        solver.getSeparationNilTerm()

def test_get_separation_nil_term2(solver):
    solver.setLogic("ALL_SUPPORTED")
    solver.setOption("incremental", "false")
    solver.setOption("produce-models", "false")
    checkSimpleSeparationConstraints(solver)
    with pytest.raises(RuntimeError):
        solver.getSeparationNilTerm()

def test_get_separation_nil_term3(solver):
    solver.setLogic("ALL_SUPPORTED")
    solver.setOption("incremental", "false")
    solver.setOption("produce-models", "true")
    t = solver.mkFalse()
    solver.assertFormula(t)
    solver.checkSat()
    with pytest.raises(RuntimeError):
        solver.getSeparationNilTerm()

def test_get_separation_nil_term4(solver):
    solver.setLogic("ALL_SUPPORTED")
    solver.setOption("incremental", "false")
    solver.setOption("produce-models", "true")
    t = solver.mkTrue()
    solver.assertFormula(t)
    solver.checkSat()
    with pytest.raises(RuntimeError):
        solver.getSeparationNilTerm()

def test_get_separation_nil_term5(solver):
    solver.setLogic("ALL_SUPPORTED")
    solver.setOption("incremental", "false")
    solver.setOption("produce-models", "true")
    checkSimpleSeparationConstraints(solver)
    solver.getSeparationNilTerm()

def test_push1(solver):
    solver.setOption("incremental", "true")
    solver.push(1)
    with pytest.raises(RuntimeError):
        solver.setOption("incremental", "false")
    with pytest.raises(RuntimeError):
        solver.setOption("incremental", "true")

def test_push2(solver):
    solver.setOption("incremental", "false")
    with pytest.raises(RuntimeError):
        solver.push(1)

def test_pop1(solver):
    solver.setOption("incremental", "false")
    with pytest.raises(RuntimeError):
        solver.pop(1)

def test_pop2(solver):
    solver.setOption("incremental", "true")
    with pytest.raises(RuntimeError):
        solver.pop(1)

def test_pop3(solver):
    solver.setOption("incremental", "true")
    solver.push(1)
    solver.pop(1)
    with pytest.raises(RuntimeError):
        solver.pop(1)

# TODO: no api blockModel()
def test_blockModel1(solver):
    solver.setOption("produce-models", "true")
    x = solver.mkConst(solver.getBooleanSort(), "x")
    solver.assertFormula(x.eqTerm(x))
    solver.checkSat()
#    with pytest.raises(RuntimeError):
#        solver.blockModel()


# TODO: no api blockModel()
#TEST_F(TestApiBlackSolver, blockModel2)
#{
#  d_solver.setOption("block-models", "literals");
#  Term x = d_solver.mkConst(d_solver.getBooleanSort(), "x");
#  d_solver.assertFormula(x.eqTerm(x));
#  d_solver.checkSat();
#  ASSERT_THROW(d_solver.blockModel(), CVC5ApiException);
#}


# TODO: no api blockModel()
#TEST_F(TestApiBlackSolver, blockModel3)
#{
#  d_solver.setOption("produce-models", "true");
#  d_solver.setOption("block-models", "literals");
#  Term x = d_solver.mkConst(d_solver.getBooleanSort(), "x");
#  d_solver.assertFormula(x.eqTerm(x));
#  ASSERT_THROW(d_solver.blockModel(), CVC5ApiException);
#}
#
# TODO: no api blockModel()
#TEST_F(TestApiBlackSolver, blockModel4)
#{
#  d_solver.setOption("produce-models", "true");
#  d_solver.setOption("block-models", "literals");
#  Term x = d_solver.mkConst(d_solver.getBooleanSort(), "x");
#  d_solver.assertFormula(x.eqTerm(x));
#  d_solver.checkSat();
#  ASSERT_NO_THROW(d_solver.blockModel());
#}


# TODO: no api blockModelValues()
#TEST_F(TestApiBlackSolver, blockModelValues1)
#{
#  d_solver.setOption("produce-models", "true");
#  d_solver.setOption("block-models", "literals");
#  Term x = d_solver.mkConst(d_solver.getBooleanSort(), "x");
#  d_solver.assertFormula(x.eqTerm(x));
#  d_solver.checkSat();
#  ASSERT_THROW(d_solver.blockModelValues({}), CVC5ApiException);
#  ASSERT_THROW(d_solver.blockModelValues({Term()}), CVC5ApiException);
#  ASSERT_THROW(d_solver.blockModelValues({Solver().mkBoolean(false)}),
#               CVC5ApiException);
#}
#
## TODO: no api blockModelValues()
#TEST_F(TestApiBlackSolver, blockModelValues2)
#{
#  d_solver.setOption("produce-models", "true");
#  Term x = d_solver.mkConst(d_solver.getBooleanSort(), "x");
#  d_solver.assertFormula(x.eqTerm(x));
#  d_solver.checkSat();
#  ASSERT_THROW(d_solver.blockModelValues({x}), CVC5ApiException);
#}
#
## TODO: no api blockModelValues()
#TEST_F(TestApiBlackSolver, blockModelValues3)
#{
#  d_solver.setOption("block-models", "literals");
#  Term x = d_solver.mkConst(d_solver.getBooleanSort(), "x");
#  d_solver.assertFormula(x.eqTerm(x));
#  d_solver.checkSat();
#  ASSERT_THROW(d_solver.blockModelValues({x}), CVC5ApiException);
#}
#
## TODO: no api blockModelValues()
#TEST_F(TestApiBlackSolver, blockModelValues4)
#{
#  d_solver.setOption("produce-models", "true");
#  d_solver.setOption("block-models", "literals");
#  Term x = d_solver.mkConst(d_solver.getBooleanSort(), "x");
#  d_solver.assertFormula(x.eqTerm(x));
#  ASSERT_THROW(d_solver.blockModelValues({x}), CVC5ApiException);
#}
#
## TODO: no api blockModelValues()
#TEST_F(TestApiBlackSolver, blockModelValues5)
#{
#  d_solver.setOption("produce-models", "true");
#  d_solver.setOption("block-models", "literals");
#  Term x = d_solver.mkConst(d_solver.getBooleanSort(), "x");
#  d_solver.assertFormula(x.eqTerm(x));
#  d_solver.checkSat();
#  ASSERT_NO_THROW(d_solver.blockModelValues({x}));
#}


def test_setInfo(solver):
    with pytest.raises(RuntimeError):
        solver.setInfo("cvc5-lagic", "QF_BV")
    with pytest.raises(RuntimeError):
        solver.setInfo("cvc2-logic", "QF_BV")
    with pytest.raises(RuntimeError):
        solver.setInfo("cvc5-logic", "asdf")

    solver.setInfo("source", "asdf")
    solver.setInfo("category", "asdf")
    solver.setInfo("difficulty", "asdf")
    solver.setInfo("filename", "asdf")
    solver.setInfo("license", "asdf")
    solver.setInfo("name", "asdf")
    solver.setInfo("notes", "asdf")

    solver.setInfo("smt-lib-version", "2")
    solver.setInfo("smt-lib-version", "2.0")
    solver.setInfo("smt-lib-version", "2.5")
    solver.setInfo("smt-lib-version", "2.6")
    with pytest.raises(RuntimeError):
        solver.setInfo("smt-lib-version", ".0")

    solver.setInfo("status", "sat")
    solver.setInfo("status", "unsat")
    solver.setInfo("status", "unknown")
    with pytest.raises(RuntimeError):
        solver.setInfo("status", "asdf")


def test_simplify(solver):
    with pytest.raises(RuntimeError):
        solver.simplify(pycvc5.Term(solver))

    bvSort = solver.mkBitVectorSort(32)
    uSort = solver.mkUninterpretedSort("u")
    funSort1 = solver.mkFunctionSort([bvSort, bvSort], bvSort)
    funSort2 = solver.mkFunctionSort(uSort, solver.getIntegerSort())
    consListSpec = solver.mkDatatypeDecl("list")
    cons = solver.mkDatatypeConstructorDecl("cons")
    cons.addSelector("head", solver.getIntegerSort())
    cons.addSelectorSelf("tail")
    consListSpec.addConstructor(cons)
    nil = solver.mkDatatypeConstructorDecl("nil")
    consListSpec.addConstructor(nil)
    consListSort = solver.mkDatatypeSort(consListSpec)

    x = solver.mkConst(bvSort, "x")
    solver.simplify(x)
    a = solver.mkConst(bvSort, "a")
    solver.simplify(a)
    b = solver.mkConst(bvSort, "b")
    solver.simplify(b)
    x_eq_x = solver.mkTerm(kinds.Equal, x, x)
    solver.simplify(x_eq_x)
    assert solver.mkTrue() != x_eq_x
    assert solver.mkTrue() == solver.simplify(x_eq_x)
    x_eq_b = solver.mkTerm(kinds.Equal, x, b)
    solver.simplify(x_eq_b)
    assert solver.mkTrue() != x_eq_b
    assert solver.mkTrue() != solver.simplify(x_eq_b)
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.simplify(x)

    i1 = solver.mkConst(solver.getIntegerSort(), "i1")
    solver.simplify(i1)
    i2 = solver.mkTerm(kinds.Mult, i1, solver.mkInteger("23"))
    solver.simplify(i2)
    assert i1 != i2
    assert i1 != solver.simplify(i2)
    i3 = solver.mkTerm(kinds.Plus, i1, solver.mkInteger(0))
    solver.simplify(i3)
    assert i1 != i3
    assert i1 == solver.simplify(i3)

    consList = consListSort.getDatatype()
    dt1 = solver.mkTerm(\
        kinds.ApplyConstructor,\
        consList.getConstructorTerm("cons"),\
        solver.mkInteger(0),\
        solver.mkTerm(kinds.ApplyConstructor, consList.getConstructorTerm("nil")))
    solver.simplify(dt1)
    dt2 = solver.mkTerm(\
      kinds.ApplySelector, consList["cons"].getSelectorTerm("head"), dt1)
    solver.simplify(dt2)

    b1 = solver.mkVar(bvSort, "b1")
    solver.simplify(b1)
    b2 = solver.mkVar(bvSort, "b1")
    solver.simplify(b2)
    b3 = solver.mkVar(uSort, "b3")
    solver.simplify(b3)
    v1 = solver.mkConst(bvSort, "v1")
    solver.simplify(v1)
    v2 = solver.mkConst(solver.getIntegerSort(), "v2")
    solver.simplify(v2)
    f1 = solver.mkConst(funSort1, "f1")
    solver.simplify(f1)
    f2 = solver.mkConst(funSort2, "f2")
    solver.simplify(f2)
    solver.defineFunsRec([f1, f2], [[b1, b2], [b3]], [v1, v2])
    solver.simplify(f1)
    solver.simplify(f2)

def test_assert_formula(solver):
    solver.assertFormula(solver.mkTrue())
    with pytest.raises(RuntimeError):
        solver.assertFormula(pycvc5.Term(solver))
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.assertFormula(solver.mkTrue())

def test_check_entailed(solver):
    solver.setOption("incremental", "false")
    solver.checkEntailed(solver.mkTrue())
    with pytest.raises(RuntimeError):
        solver.checkEntailed(solver.mkTrue())
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.checkEntailed(solver.mkTrue())

def test_check_entailed1(solver):
    boolSort = solver.getBooleanSort()
    x = solver.mkConst(boolSort, "x")
    y = solver.mkConst(boolSort, "y")
    z = solver.mkTerm(kinds.And, x, y)
    solver.setOption("incremental", "true")
    solver.checkEntailed(solver.mkTrue())
    with pytest.raises(RuntimeError):
        solver.checkEntailed(pycvc5.Term(solver))
    solver.checkEntailed(solver.mkTrue())
    solver.checkEntailed(z)
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.checkEntailed(solver.mkTrue())

def test_check_entailed2(solver):
    solver.setOption("incremental", "true")

    uSort = solver.mkUninterpretedSort("u")
    intSort = solver.getIntegerSort()
    boolSort = solver.getBooleanSort()
    uToIntSort = solver.mkFunctionSort(uSort, intSort)
    intPredSort = solver.mkFunctionSort(intSort, boolSort)

    n = pycvc5.Term(solver)
    # Constants
    x = solver.mkConst(uSort, "x")
    y = solver.mkConst(uSort, "y")
    # Functions
    f = solver.mkConst(uToIntSort, "f")
    p = solver.mkConst(intPredSort, "p")
    # Values
    zero = solver.mkInteger(0)
    one = solver.mkInteger(1)
    # Terms
    f_x = solver.mkTerm(kinds.ApplyUf, f, x)
    f_y = solver.mkTerm(kinds.ApplyUf, f, y)
    summ = solver.mkTerm(kinds.Plus, f_x, f_y)
    p_0 = solver.mkTerm(kinds.ApplyUf, p, zero)
    p_f_y = solver.mkTerm(kinds.ApplyUf, p, f_y)
    # Assertions
    assertions =\
        solver.mkTerm(kinds.And,\
                      [solver.mkTerm(kinds.Leq, zero, f_x),  # 0 <= f(x)
                       solver.mkTerm(kinds.Leq, zero, f_y),  # 0 <= f(y)
                       solver.mkTerm(kinds.Leq, summ, one),  # f(x) + f(y) <= 1
                       p_0.notTerm(),                        # not p(0)
                       p_f_y                                 # p(f(y))
                      ])

    solver.checkEntailed(solver.mkTrue())
    solver.assertFormula(assertions)
    solver.checkEntailed(solver.mkTerm(kinds.Distinct, x, y))
    solver.checkEntailed(\
        [solver.mkFalse(), solver.mkTerm(kinds.Distinct, x, y)])
    with pytest.raises(RuntimeError):
        solver.checkEntailed(n)
    with pytest.raises(RuntimeError):
        solver.checkEntailed([n, solver.mkTerm(kinds.Distinct, x, y)])
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.checkEntailed(solver.mkTrue())

def test_check_sat(solver):
    solver.setOption("incremental", "false")
    solver.checkSat()
    with pytest.raises(RuntimeError):
        solver.checkSat()

def test_check_sat_assuming(solver):
    solver.setOption("incremental", "false")
    solver.checkSatAssuming(solver.mkTrue())
    with pytest.raises(RuntimeError):
        solver.checkSatAssuming(solver.mkTrue())
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.checkSatAssuming(solver.mkTrue())

def test_check_sat_assuming1(solver):
    boolSort = solver.getBooleanSort()
    x = solver.mkConst(boolSort, "x")
    y = solver.mkConst(boolSort, "y")
    z = solver.mkTerm(kinds.And, x, y)
    solver.setOption("incremental", "true")
    solver.checkSatAssuming(solver.mkTrue())
    with pytest.raises(RuntimeError):
        solver.checkSatAssuming(pycvc5.Term(solver))
    solver.checkSatAssuming(solver.mkTrue())
    solver.checkSatAssuming(z)
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.checkSatAssuming(solver.mkTrue())


def test_check_sat_assuming2(solver):
    solver.setOption("incremental", "true")

    uSort = solver.mkUninterpretedSort("u")
    intSort = solver.getIntegerSort()
    boolSort = solver.getBooleanSort()
    uToIntSort = solver.mkFunctionSort(uSort, intSort)
    intPredSort = solver.mkFunctionSort(intSort, boolSort)

    n = pycvc5.Term(solver)
    # Constants
    x = solver.mkConst(uSort, "x")
    y = solver.mkConst(uSort, "y")
    # Functions
    f = solver.mkConst(uToIntSort, "f")
    p = solver.mkConst(intPredSort, "p")
    # Values
    zero = solver.mkInteger(0)
    one = solver.mkInteger(1)
    # Terms
    f_x = solver.mkTerm(kinds.ApplyUf, f, x)
    f_y = solver.mkTerm(kinds.ApplyUf, f, y)
    summ = solver.mkTerm(kinds.Plus, f_x, f_y)
    p_0 = solver.mkTerm(kinds.ApplyUf, p, zero)
    p_f_y = solver.mkTerm(kinds.ApplyUf, p, f_y)
    # Assertions
    assertions =\
        solver.mkTerm(kinds.And,\
                      [solver.mkTerm(kinds.Leq, zero, f_x),  # 0 <= f(x)
                       solver.mkTerm(kinds.Leq, zero, f_y),  # 0 <= f(y)
                       solver.mkTerm(kinds.Leq, summ, one),  # f(x) + f(y) <= 1
                       p_0.notTerm(),                        # not p(0)
                       p_f_y                                 # p(f(y))
                      ])

    solver.checkSatAssuming(solver.mkTrue())
    solver.assertFormula(assertions)
    solver.checkSatAssuming(solver.mkTerm(kinds.Distinct, x, y))
    solver.checkSatAssuming(
        [solver.mkFalse(), solver.mkTerm(kinds.Distinct, x, y)])
    with pytest.raises(RuntimeError):
        solver.checkSatAssuming(n)
    with pytest.raises(RuntimeError):
        solver.checkSatAssuming([n, solver.mkTerm(kinds.Distinct, x, y)])
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.checkSatAssuming(solver.mkTrue())


def test_set_logic(solver):
    solver.setLogic("AUFLIRA")
    with pytest.raises(RuntimeError):
        solver.setLogic("AF_BV")
    solver.assertFormula(solver.mkTrue())
    with pytest.raises(RuntimeError):
        solver.setLogic("AUFLIRA")

def test_set_option(solver):
    solver.setOption("bv-sat-solver", "minisat")
    with pytest.raises(RuntimeError):
        solver.setOption("bv-sat-solver", "1")
    solver.assertFormula(solver.mkTrue())
    with pytest.raises(RuntimeError):
        solver.setOption("bv-sat-solver", "minisat")

def test_reset_assertions(solver):
    solver.setOption("incremental", "true")

    bvSort = solver.mkBitVectorSort(4)
    one = solver.mkBitVector(4, 1)
    x = solver.mkConst(bvSort, "x")
    ule = solver.mkTerm(kinds.BVUle, x, one)
    srem = solver.mkTerm(kinds.BVSrem, one, x)
    solver.push(4)
    slt = solver.mkTerm(kinds.BVSlt, srem, one)
    solver.resetAssertions()
    solver.checkSatAssuming([slt, ule])

# TODO: no api getNullSort()
def test_mk_sygus_var(solver):
    boolSort = solver.getBooleanSort()
    intSort = solver.getIntegerSort()
    funSort = solver.mkFunctionSort(intSort, boolSort)

    solver.mkSygusVar(boolSort)
    solver.mkSygusVar(funSort)
    solver.mkSygusVar(boolSort, "b")
    solver.mkSygusVar(funSort, "")
    with pytest.raises(RuntimeError):
        solver.mkSygusVar(pycvc5.Sort(solver))
#    with pytest.raises(RuntimeError):
#        solver.mkSygusVar(solver.getNullSort(), "a")
    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.mkSygusVar(boolSort)


def test_mk_sygus_grammar(solver):
    nullTerm = pycvc5.Term(solver)
    boolTerm = solver.mkBoolean(True)
    boolVar = solver.mkVar(solver.getBooleanSort())
    intVar = solver.mkVar(solver.getIntegerSort())

    solver.mkSygusGrammar([], [intVar])
    solver.mkSygusGrammar([boolVar], [intVar])
    with pytest.raises(RuntimeError):
        solver.mkSygusGrammar([], [])
    with pytest.raises(RuntimeError):
        solver.mkSygusGrammar([], [nullTerm])
    with pytest.raises(RuntimeError):
        solver.mkSygusGrammar([], [boolTerm])
    with pytest.raises(RuntimeError):
        solver.mkSygusGrammar([boolTerm], [intVar])
    slv = pycvc5.Solver()
    boolVar2 = slv.mkVar(slv.getBooleanSort())
    intVar2 = slv.mkVar(slv.getIntegerSort())
    slv.mkSygusGrammar([boolVar2], [intVar2])
    with pytest.raises(RuntimeError):
        slv.mkSygusGrammar([boolVar], [intVar2])
    with pytest.raises(RuntimeError):
        slv.mkSygusGrammar([boolVar2], [intVar])


def test_synth_fun(solver):
#    null = solver.getNullSort()
    boolean = solver.getBooleanSort()
    integer = solver.getIntegerSort()

    nullTerm = pycvc5.Term(solver)
    x = solver.mkVar(boolean)

    start1 = solver.mkVar(boolean)
    start2 = solver.mkVar(integer)

    g1 = solver.mkSygusGrammar([x], [start1])
    g1.addRule(start1, solver.mkBoolean(False))

    g2 = solver.mkSygusGrammar([x], [start2])
    g2.addRule(start2, solver.mkInteger(0))

    solver.synthFun("", [], boolean)
    solver.synthFun("f1", [x], boolean)
    solver.synthFun("f2", [x], boolean, g1)

    with pytest.raises(RuntimeError):
        solver.synthFun("f3", [nullTerm], boolean)
#    with pytest.raises(RuntimeError):
#        solver.synthFun("f4", [], null)
    with pytest.raises(RuntimeError):
        solver.synthFun("f6", [x], boolean, g2)
    slv = pycvc5.Solver()
    x2 = slv.mkVar(slv.getBooleanSort())
    slv.synthFun("f1", [x2], slv.getBooleanSort())
    with pytest.raises(RuntimeError):
        slv.synthFun("", [], solver.getBooleanSort())
    with pytest.raises(RuntimeError):
        slv.synthFun("f1", [x], solver.getBooleanSort())


def test_synth_inv(solver):
    boolean = solver.getBooleanSort()
    integer = solver.getIntegerSort()

    nullTerm = pycvc5.Term(solver)
    x = solver.mkVar(boolean)

    start1 = solver.mkVar(boolean)
    start2 = solver.mkVar(integer)

    g1 = solver.mkSygusGrammar([x], [start1])
    g1.addRule(start1, solver.mkBoolean(False))

    g2 = solver.mkSygusGrammar([x], [start2])
    g2.addRule(start2, solver.mkInteger(0))

    solver.synthInv("", [])
    solver.synthInv("i1", [x])
    solver.synthInv("i2", [x], g1)

    with pytest.raises(RuntimeError):
        solver.synthInv("i3", [nullTerm])
    with pytest.raises(RuntimeError):
        solver.synthInv("i4", [x], g2)


def test_add_sygus_constraint(solver):
    nullTerm = pycvc5.Term(solver)
    boolTerm = solver.mkBoolean(True)
    intTerm = solver.mkInteger(1)

    solver.addSygusConstraint(boolTerm)
    with pytest.raises(RuntimeError):
        solver.addSygusConstraint(nullTerm)
    with pytest.raises(RuntimeError):
        solver.addSygusConstraint(intTerm)

    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.addSygusConstraint(boolTerm)


def test_add_sygus_inv_constraint(solver):
    boolean = solver.getBooleanSort()
    real = solver.getRealSort()

    nullTerm = pycvc5.Term(solver)
    intTerm = solver.mkInteger(1)

    inv = solver.declareFun("inv", [real], boolean)
    pre = solver.declareFun("pre", [real], boolean)
    trans = solver.declareFun("trans", [real, real], boolean)
    post = solver.declareFun("post", [real], boolean)

    inv1 = solver.declareFun("inv1", [real], real)

    trans1 = solver.declareFun("trans1", [boolean, real], boolean)
    trans2 = solver.declareFun("trans2", [real, boolean], boolean)
    trans3 = solver.declareFun("trans3", [real, real], real)

    solver.addSygusInvConstraint(inv, pre, trans, post)

    with pytest.raises(RuntimeError):
        solver.addSygusInvConstraint(nullTerm, pre, trans, post)
    with pytest.raises(RuntimeError):
        solver.addSygusInvConstraint(inv, nullTerm, trans, post)
    with pytest.raises(RuntimeError):
        solver.addSygusInvConstraint(inv, pre, nullTerm, post)
    with pytest.raises(RuntimeError):
        solver.addSygusInvConstraint(inv, pre, trans, nullTerm)

    with pytest.raises(RuntimeError):
        solver.addSygusInvConstraint(intTerm, pre, trans, post)

    with pytest.raises(RuntimeError):
        solver.addSygusInvConstraint(inv1, pre, trans, post)

    with pytest.raises(RuntimeError):
        solver.addSygusInvConstraint(inv, trans, trans, post)

    with pytest.raises(RuntimeError):
        solver.addSygusInvConstraint(inv, pre, intTerm, post)
    with pytest.raises(RuntimeError):
        solver.addSygusInvConstraint(inv, pre, pre, post)
    with pytest.raises(RuntimeError):
        solver.addSygusInvConstraint(inv, pre, trans1, post)
    with pytest.raises(RuntimeError):
        solver.addSygusInvConstraint(inv, pre, trans2, post)
    with pytest.raises(RuntimeError):
        solver.addSygusInvConstraint(inv, pre, trans3, post)

    with pytest.raises(RuntimeError):
        solver.addSygusInvConstraint(inv, pre, trans, trans)
    slv = pycvc5.Solver()
    boolean2 = slv.getBooleanSort()
    real2 = slv.getRealSort()
    inv22 = slv.declareFun("inv", [real2], boolean2)
    pre22 = slv.declareFun("pre", [real2], boolean2)
    trans22 = slv.declareFun("trans", [real2, real2], boolean2)
    post22 = slv.declareFun("post", [real2], boolean2)
    slv.addSygusInvConstraint(inv22, pre22, trans22, post22)
    with pytest.raises(RuntimeError):
        slv.addSygusInvConstraint(inv, pre22, trans22, post22)
    with pytest.raises(RuntimeError):
        slv.addSygusInvConstraint(inv22, pre, trans22, post22)
    with pytest.raises(RuntimeError):
        slv.addSygusInvConstraint(inv22, pre22, trans, post22)
    with pytest.raises(RuntimeError):
        slv.addSygusInvConstraint(inv22, pre22, trans22, post)

def test_get_synth_solution(solver):
    solver.setOption("lang", "sygus2")
    solver.setOption("incremental", "false")

    nullTerm = pycvc5.Term(solver)
    x = solver.mkBoolean(False)
    f = solver.synthFun("f", [], solver.getBooleanSort())

    with pytest.raises(RuntimeError):
        solver.getSynthSolution(f)

    solver.checkSynth()

    solver.getSynthSolution(f)
    solver.getSynthSolution(f)

    with pytest.raises(RuntimeError):
        solver.getSynthSolution(nullTerm)
    with pytest.raises(RuntimeError):
        solver.getSynthSolution(x)

    slv = pycvc5.Solver()
    with pytest.raises(RuntimeError):
        slv.getSynthSolution(f)

# TODO: no api getSynthSolutions 
def test_get_synth_solutions(solver):
    solver.setOption("lang", "sygus2")
    solver.setOption("incremental", "false")

    nullTerm = pycvc5.Term(solver)
    x = solver.mkBoolean(False)
    f = solver.synthFun("f", [], solver.getBooleanSort())

#    with pytest.raises(RuntimeError):
#        solver.getSynthSolutions([])
#    with pytest.raises(RuntimeError):
#        solver.getSynthSolutions([f])

    solver.checkSynth();
#
#  ASSERT_NO_THROW(d_solver.getSynthSolutions({f}));
#  ASSERT_NO_THROW(d_solver.getSynthSolutions({f, f}));
#
#  ASSERT_THROW(d_solver.getSynthSolutions({}), CVC5ApiException);
#  ASSERT_THROW(d_solver.getSynthSolutions({nullTerm}), CVC5ApiException);
#  ASSERT_THROW(d_solver.getSynthSolutions({x}), CVC5ApiException);
#
#  Solver slv;
#  ASSERT_THROW(slv.getSynthSolutions({x}), CVC5ApiException);
#}


# TODO: no api mkTuple
def test_tuple_project(solver):
    sorts = [solver.getBooleanSort(),\
             solver.getIntegerSort(),\
             solver.getStringSort(),\
             solver.mkSetSort(solver.getStringSort())]
    elements = [solver.mkBoolean(True),\
                solver.mkInteger(3),\
                solver.mkString("C"),\
                solver.mkTerm(kinds.Singleton, solver.mkString("Z"))]

#    tupl = solver.mkTuple(sorts, elements)
#
#  std::vector<uint32_t> indices1 = {};
#  std::vector<uint32_t> indices2 = {0};
#  std::vector<uint32_t> indices3 = {0, 1};
#  std::vector<uint32_t> indices4 = {0, 0, 2, 2, 3, 3, 0};
#  std::vector<uint32_t> indices5 = {4};
#  std::vector<uint32_t> indices6 = {0, 4};
#
#  ASSERT_NO_THROW(
#      d_solver.mkTerm(d_solver.mkOp(TUPLE_PROJECT, indices1), tuple));
#  ASSERT_NO_THROW(
#      d_solver.mkTerm(d_solver.mkOp(TUPLE_PROJECT, indices2), tuple));
#  ASSERT_NO_THROW(
#      d_solver.mkTerm(d_solver.mkOp(TUPLE_PROJECT, indices3), tuple));
#  ASSERT_NO_THROW(
#      d_solver.mkTerm(d_solver.mkOp(TUPLE_PROJECT, indices4), tuple));
#
#  ASSERT_THROW(d_solver.mkTerm(d_solver.mkOp(TUPLE_PROJECT, indices5), tuple),
#               CVC5ApiException);
#  ASSERT_THROW(d_solver.mkTerm(d_solver.mkOp(TUPLE_PROJECT, indices6), tuple),
#               CVC5ApiException);
#
#  std::vector<uint32_t> indices = {0, 3, 2, 0, 1, 2};
#
#  Op op = d_solver.mkOp(TUPLE_PROJECT, indices);
#  Term projection = d_solver.mkTerm(op, tuple);
#
#  Datatype datatype = tuple.getSort().getDatatype();
#  DatatypeConstructor constructor = datatype[0];
#
#  for (size_t i = 0; i < indices.size(); i++)
#  {
#    Term selectorTerm = constructor[indices[i]].getSelectorTerm();
#    Term selectedTerm = d_solver.mkTerm(APPLY_SELECTOR, selectorTerm, tuple);
#    Term simplifiedTerm = d_solver.simplify(selectedTerm);
#    ASSERT_EQ(elements[indices[i]], simplifiedTerm);
#  }
#
#  ASSERT_EQ(
#      "((_ tuple_project 0 3 2 0 1 2) (mkTuple true 3 \"C\" (singleton "
#      "\"Z\")))",
#      projection.toString());
#}
