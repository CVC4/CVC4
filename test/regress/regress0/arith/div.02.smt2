; COMMAND-LINE: -q
; EXPECT: sat
(set-logic QF_NIA)
(set-info :smt-lib-version 2.0)
(set-info :status sat)
(declare-fun n () Int)

(assert (distinct (div n n) 1))

(check-sat)
