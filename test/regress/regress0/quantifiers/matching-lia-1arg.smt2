; COMMAND-LINE: --purify-triggers
; EXPECT: unsat
(set-logic ALL_SUPPORTED)
(declare-fun P (Int) Bool)
(assert (forall ((x Int)) (P (* 2 x))))
(assert (not (P 38)))
(check-sat)

