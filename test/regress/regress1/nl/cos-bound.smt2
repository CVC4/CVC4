; COMMAND-LINE: --nl-ext
; EXPECT: unsat
(set-logic QF_UFNRAT)
(declare-fun x () Real)
(assert (> (cos x) 1.0))
(check-sat)
