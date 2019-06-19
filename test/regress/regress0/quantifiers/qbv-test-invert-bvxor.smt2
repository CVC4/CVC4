; COMMAND-LINE: --cbqi-bv --cbqi-bv-ineq=keep --no-cbqi-full
; EXPECT: unsat
(set-logic BV)
(declare-fun a () (_ BitVec 8))

(assert (forall ((x (_ BitVec 8))) (not (= (bvxor x a) (bvmul a a)))))

(check-sat)
