(set-option :incremental false)
(set-logic QF_UFLIA)
(declare-fun f (Int) Int)
(declare-fun x1 () Int)
(declare-fun y1 () Int)
(declare-fun x2 () Int)
(declare-fun y2 () Int)
(declare-fun a () Int)
(declare-fun b () Int)
(assert (= x1 x2))
(assert (= (f x1) (f y1)))
(assert (not (= x2 y2)))
(assert (= y1 y2))
(check-sat-assuming ( true ))
