(set-option :incremental false)
(set-info :status unsat)
(set-logic QF_BV)
(declare-fun v2 () (_ BitVec 9))
(check-sat-assuming ( (= (_ bv0 2) ((_ zero_extend 1) (bvcomp (concat (_ bv0 6) (ite (bvult v2 v2) (_ bv1 1) (_ bv0 1))) (_ bv0 7)))) ))
