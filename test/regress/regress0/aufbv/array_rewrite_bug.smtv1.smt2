(set-option :incremental false)
(set-info :status sat)
(set-logic QF_AUFBV)
(declare-fun a1 () (Array (_ BitVec 14) (_ BitVec 11)))
(check-sat-assuming ( (let ((_let_0 (select a1 (_ bv0 14)))) (bvugt (_ bv1 16) ((_ zero_extend 3) ((_ sign_extend 2) (select (store (store a1 ((_ extract 13 0) (_ bv1 16)) (_ bv0 11)) (_ bv0 14) _let_0) ((_ zero_extend 3) _let_0)))))) ))
