(set-option :incremental false)
(set-info :status sat)
(set-logic QF_AUFBV)
(declare-fun v0 () (_ BitVec 16))
(declare-fun v1 () (_ BitVec 1))
(declare-fun a4 () (Array (_ BitVec 1) (_ BitVec 7)))
(check-sat-assuming ( (and (bvsle (select a4 v1) (_ bv0 7)) (not (= (_ bv0 16) (ite (= v1 (_ bv0 1)) (_ bv0 16) v0)))) ))
