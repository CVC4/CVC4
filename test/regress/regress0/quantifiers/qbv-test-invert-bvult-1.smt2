; COMMAND-LINE: --cbqi-bv --cbqi-bv-ineq=keep --no-cbqi-ful
; EXPECT: sat
(set-logic BV)
(set-info :status sat)
(declare-fun a () (_ BitVec 8))

(assert (forall ((x (_ BitVec 8))) (not (bvult a x))))

(check-sat)
