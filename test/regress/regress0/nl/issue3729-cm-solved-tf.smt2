; COMMAND-LINE: --quiet --no-check-models --no-proof-new
; EXPECT: sat
(set-logic QF_NRAT)
(set-info :status sat)
(declare-fun a () Real)
(assert (= a (sin 1.0)))
(check-sat)
