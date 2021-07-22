; COMMAND-LINE: --dump-unsat-cores-full
; EXPECT: unsat
; EXPECT: (
; EXPECT: (and (= x y) (< x y))
; EXPECT: )
(set-logic QF_LIA)
(set-info :status unsat)
(declare-fun x () Int)
(declare-fun y () Int)
(assert (and (= x y) (< x y)))
(check-sat)
