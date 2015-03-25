; EXIT: 1
; EXPECT: (error "Array theory solver does not yet support write-chains connecting two different constant arrays")
(set-logic QF_ALIA)
(set-info :status unsat)
(declare-const all1 (Array Int Int))
(declare-const all2 (Array Int Int))
(declare-const aa (Array Int Int))
(declare-const bb (Array Int Int))
(declare-const a Int)
(declare-const i Int)
(assert (= all1 ((as const (Array Int Int)) 1)))
(assert (= aa (store all1 i 0)))
(assert (= all2 ((as const (Array Int Int)) 2)))
(assert (= bb (store all2 i 0)))
(assert (= aa bb))
(check-sat)
