(declare-sort U 0)
(declare-fun a () U)
(declare-fun P (U U) Bool)
(assert (forall ((x U)) (= x a)))
;(assert (forall ((x U)) (= (= x a) (forall ((y U)) (P x y)))))
(declare-fun b () U)
(declare-fun c () U)
(assert (not (= b c)))
(check-sat)
