(set-option :incremental false)
(meta-info :status sat)
(set-logic QF_BV)
(declare-fun v2 () (_ BitVec 14))
(check-sat-assuming ( (bvuge (bvmul (_ bv2692 12) (bvsub (_ bv2692 12) ((_ sign_extend 11) (ite (bvugt (_ bv1 14) v2) (_ bv1 1) (_ bv0 1))))) (_ bv1 12)) ))
