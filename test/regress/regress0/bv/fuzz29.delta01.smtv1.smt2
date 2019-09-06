(set-option :incremental false)
(set-info :status sat)
(set-logic QF_BV)
(declare-fun v1 () (_ BitVec 4))
(declare-fun v0 () (_ BitVec 4))
(declare-fun v2 () (_ BitVec 4))
(check-sat-assuming ( (let ((_let_0 (bvnand v1 (bvashr v0 v1)))) (let ((_let_1 ((_ extract 2 1) v1))) (let ((_let_2 ((_ sign_extend 3) (ite (bvsge v0 v1) (_ bv1 1) (_ bv0 1))))) (let ((_let_3 (ite (bvsgt (bvor v1 (bvashr v0 v1)) (bvor v1 (bvashr v0 v1))) (_ bv1 1) (_ bv0 1)))) (let ((_let_4 (bvugt (_ bv1 1) _let_3))) (and (or false (bvsge v2 v1) (bvsgt _let_0 v0)) (or (bvugt _let_1 _let_1) (not (bvule _let_1 ((_ sign_extend 1) (ite (bvsge v0 v1) (_ bv1 1) (_ bv0 1))))) (not (distinct (bvxnor (_ bv0 4) (bvor v1 (bvashr v0 v1))) ((_ sign_extend 3) (ite (bvsge _let_2 (_ bv0 4)) (_ bv1 1) (_ bv0 1)))))) (bvsgt v0 (_ bv1 4)) (or (not (bvule (bvnor v2 _let_0) (bvor v1 (bvashr v0 v1)))) _let_4 _let_4) (or false (distinct (_ bv1 4) _let_0) (bvuge ((_ rotate_right 2) _let_0) (_ bv1 4))) (or false (not (bvult _let_2 (_ bv1 4))) (not (bvule (_ bv1 1) _let_3))))))))) ))
