; COMMAND-LINE: --cbqi-bv --cbqi-bv-ineq=keep --no-cbqi-ful
; EXPECT: unsat
(set-logic BV)
(set-info :status unsat)
(declare-fun a () (_ BitVec 8))

(assert (forall ((x (_ BitVec 8))) (not (= (bvxor x a) (bvmul a a)))))

(check-sat)
