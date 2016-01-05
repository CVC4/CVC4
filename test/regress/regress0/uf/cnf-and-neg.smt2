(set-logic QF_UF)
(set-info :status unsat)
(declare-sort I 0)
(declare-fun a () I)
(declare-fun b () I)
(declare-fun c () I)
(declare-fun f (I) I)
(assert (not (= (f a) (f b))))
(assert (not (= (f a) (f c))))
(assert (not (and (not (= a b)) (not (= a c)))))
(check-sat)
