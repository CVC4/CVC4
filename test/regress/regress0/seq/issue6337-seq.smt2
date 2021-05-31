(set-logic QF_SLIA)
(declare-fun x () (Seq (Seq Int)))
(declare-fun y () (Seq (Seq Int)))
(declare-fun z () (Seq Int))
(declare-fun w () (Seq Int))
(declare-fun j () Int)
(assert (= w (seq.unit j)))
(assert (not (= x (seq.unit z))))
(assert (not (= y (seq.unit w))))
(assert (not (= x y)))
(set-info :status sat)
(check-sat)
