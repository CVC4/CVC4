; COMMAND-LINE: --fmf-bound --no-cegqi -q
; EXPECT: sat
(set-logic ALL)
(assert (forall ((a Int)) (or (distinct (/ 0 0) a) (= (/ 0 a) 0))))
(check-sat)
