; COMMAND-LINE: --unconstrained-simp
(set-logic QF_AUFBVLIA)
(set-info :smt-lib-version 2.6)
(set-info :category "crafted")
(set-info :status unsat)
(declare-fun x0 () (_ BitVec 10))
(declare-fun x1 () (_ BitVec 10))
(declare-fun x2 () (_ BitVec 10))
(declare-fun x3 () (_ BitVec 10))
(declare-fun x4 () (_ BitVec 10))
(declare-fun x5 () (_ BitVec 10))
(declare-fun x6 () (_ BitVec 10))
(declare-fun x7 () (_ BitVec 10))
(declare-fun v2 () Int)
(declare-fun a2 (Int) (_ BitVec 1024))
(declare-fun v3 () (_ BitVec 1024))
(declare-fun v4 () (_ BitVec 1024))
(declare-fun v5 () (_ BitVec 1024))
(assert (bvslt x1 (_ bv513 10)))
(assert (bvslt x0 x1))
(check-sat)
(exit)
