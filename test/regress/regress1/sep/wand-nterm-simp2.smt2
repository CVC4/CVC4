; COMMAND-LINE: --no-check-models
; EXPECT: sat
(set-logic QF_ALL_SUPPORTED)
(set-info :status sat)
(declare-heap (Int Int))
(declare-fun x () Int)
(assert (wand (pto x 1) (_ emp Int Int)))
(check-sat)
