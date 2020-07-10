; COMMAND-LINE: --dump-instantiations --print-inst=num --no-print-inst-full
; EXPECT: unsat
; EXPECT: (num-instantiations
; EXPECT: (myQuantP 1)
; EXPECT: (myQuantQ 7)
; EXPECT: )

(set-logic UFLIA)
(declare-fun P (Int) Bool)
(declare-fun Q (Int) Bool)
(assert (forall ((x Int)) (! (P x) :qid |myQuantP|)))
(assert (forall ((x Int)) (! (=> (Q x) (Q (+ x 1))) :qid |myQuantQ|)))
(assert (Q 0))
(assert (or (not (P 5)) (not (Q 7))))
(check-sat)
