; COMMAND-LINE: --theoryof-mode=term --nl-ext --nl-icp
; EXPECT: unknown
(set-logic QF_NRA)
(declare-fun x () Real)
(declare-fun y () Real)
(assert (and (< 1 y) (= y (+ x (* x x)))))
(check-sat)
