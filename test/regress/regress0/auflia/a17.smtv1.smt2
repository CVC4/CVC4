(set-option :incremental false)
(set-info :status sat)
(set-logic QF_AUFLIA)
(declare-fun a () (Array Int Int))
(declare-fun x1 () Int)
(declare-fun y1 () Int)
(declare-fun z0 () Int)
(declare-fun p ((Array Int Int)) Bool)
(check-sat-assuming ( (let ((_let_0 (store a (+ x1 z0) 1))) (and (>= (select _let_0 x1) 1) (p a) (p _let_0) (p (store _let_0 y1 1)) (>= x1 1) (>= z0 0) (<= z0 0) (<= y1 1) (>= y1 1))) ))
