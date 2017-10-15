; COMMAND-LINE: --cbqi-bv --bv-div-zero-const
; EXPECT: sat
(set-logic BV)
(set-info :status sat)
(declare-fun a () (_ BitVec 32))
(declare-fun b () (_ BitVec 32))

(assert (forall ((x (_ BitVec 32))) (not (= (bvurem a x) b))))

(check-sat)
