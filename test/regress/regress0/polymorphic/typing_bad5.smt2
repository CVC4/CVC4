(set-logic UF)
(set-info :smt-lib-version 2.0)
(set-info :category "crafted")
(declare-sort A 1)
(declare-sort B 1)
(declare-sort I1 0)
(declare-sort I2 0)
(declare-fun i1 () I1)

(declare-fun mk  (par (a) (a) (A a)))
(declare-fun get (par (a) ((B a)) a))

(assert (forall ((x I1)) (= x (get (mk x)))))
(assert (not (= i1 (get (mk i1)))))

(check-sat)
(exit)

; EXPECT-ERROR: (error "Parse Error: typing_bad5.smt2:13.42: cannot apply this polymorphic function on these arguments")
; EXIT: 1
