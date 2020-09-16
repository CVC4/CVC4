; COMMAND-LINE: --solve-bv-as-int=sum --bvand-integer-granularity=1   --no-check-unsat-cores
; EXPECT: unsat
(set-logic QF_BV)
(declare-fun T1_31078 () (_ BitVec 8))
(assert (let ((?v_0 ((_ zero_extend 8) T1_31078))) (and true (= ?v_0 (_ bv123 16)) (not (= (_ bv123 16) ?v_0)))))
(check-sat)
(exit)
