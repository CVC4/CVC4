; COMMAND-LINE: --hol
(set-logic AUFBV)
(set-info :status unsat)
(set-option :fmf-bound-int true)
(declare-fun _substvar_20_ () Bool)
(declare-sort S4 0)
(assert (forall ((q15 S4) (q16 (_ BitVec 20)) (q17 (_ BitVec 13))) (xor (= (_ bv0 13) (_ bv0 13) q17 (bvsrem (_ bv0 13) (_ bv0 13)) q17) _substvar_20_ true)))
(check-sat)
(exit)
