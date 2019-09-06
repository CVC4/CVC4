(set-option :incremental false)
(set-info :status sat)
(set-logic QF_BV)
(declare-fun v2 () (_ BitVec 4))
(declare-fun v1 () (_ BitVec 4))
(check-sat-assuming ( (= (_ bv1 1) (bvsub (_ bv1 1) (bvand (ite (bvsgt (_ bv0 4) ((_ sign_extend 3) (ite (bvslt (_ bv0 4) v1) (_ bv1 1) (_ bv0 1)))) (_ bv1 1) (_ bv0 1)) (ite (bvsle (_ bv0 1) (ite (= v2 (_ bv0 4)) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1))))) ))
