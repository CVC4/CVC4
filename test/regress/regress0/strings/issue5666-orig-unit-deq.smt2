(set-logic ALL)
(set-info :status sat)
(declare-fun i5 () Int)
(declare-fun seq2 () (Seq Int))
(assert (< 1 i5))
(assert (xor true true (seq.prefixof seq2 seq2) (seq.suffixof (seq.unit 2) (seq.unit i5))))
(check-sat)
