(set-option :incremental false)
(set-info :status unsat)
(set-logic QF_AUFBV)
(declare-fun a1 () (Array (_ BitVec 32) (_ BitVec 8)))
(check-sat-assuming ( (let ((_let_0 (store a1 (_ bv0 32) (_ bv1 8)))) (not (= (_ bv0 1) (bvnot (ite (= _let_0 (store a1 (_ bv0 32) (select (store a1 (_ bv0 32) (select (store a1 (_ bv0 32) (select (store a1 (_ bv0 32) (select (store a1 (_ bv0 32) (select (store _let_0 (_ bv0 32) (bvand (_ bv1 8) (bvnot (bvand (select a1 (_ bv0 32)) (bvnot (_ bv1 8)))))) (_ bv0 32))) (_ bv0 32))) (_ bv0 32))) (_ bv0 32))) (_ bv0 32)))) (_ bv1 1) (_ bv0 1)))))) ))
