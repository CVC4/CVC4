(set-logic QF_AUFNIRA)
(set-info :smt-lib-version 2.0)
(set-info :category "crafted")
(set-info :status unsat)
(declare-fun v1 () Int)
(declare-fun v2 () Int)
(declare-fun v3 () Int)
(declare-fun v4 () Int)
(declare-fun v5 () Real)
(assert (= (* v1 v2) (+ (* v3 v4) v5)))
(assert (< v5 1))
(assert (> v5 0))
(check-sat)
(exit)
