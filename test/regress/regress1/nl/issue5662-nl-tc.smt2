(set-logic NRA)
(set-info :status sat)
(declare-fun a () Real)
(declare-fun b () Real)
(declare-fun c () Real)
(declare-fun d () Real)
(declare-fun j () Real)
(declare-fun e () Real)
(declare-fun f () Real)
(declare-fun k () Real)
(declare-fun g () Real)
(declare-fun h () Real)
(assert (forall ((i Real)) (xor (and (or (and (or (and (or (and (or
  (and (> 0.0 (* a e) (* c e)) (>= 0 k)) (<= g 0)) (= (* b j)
  2.0))))) (> f k)) (>= 0.0 k))) (>= 0 k))))
(assert (distinct a (* d h)))
(check-sat)
