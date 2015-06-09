(set-logic UF)
(set-info :smt-lib-version 2.0)
(set-info :category "crafted")
(set-info :status unsat)
(declare-sort A 1)
(declare-sort I1 0)
(declare-fun i1 () I1)

(declare-fun mk  (par (a) (a) (A a)))
(declare-fun get (par (a) ((A a)) a))

(assert (forall ((x I1)) (= x (get (mk x)))))
(assert (not (= i1 (get (mk i1)))))



(check-sat)
(exit)
