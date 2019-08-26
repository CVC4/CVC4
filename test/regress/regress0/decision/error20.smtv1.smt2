(set-option :incremental false)
(meta-info :status sat)
(set-logic QF_AUFBV)
(declare-fun v0 () (_ BitVec 16))
(declare-fun v1 () (_ BitVec 1))
(declare-fun a2 () (Array (_ BitVec 12) (_ BitVec 10)))
(declare-fun a3 () (Array (_ BitVec 14) (_ BitVec 11)))
(declare-fun a4 () (Array (_ BitVec 1) (_ BitVec 7)))
(check-sat-assuming ( (let ((_let_0 (ite (= (_ bv1 1) ((_ extract 0 0) v1)) ((_ sign_extend 15) (_ bv1 1)) v0))) (let ((_let_1 (select a4 v1))) (let ((_let_2 ((_ zero_extend 6) v1))) (let ((_let_3 ((_ rotate_right 0) v1))) (let ((_let_4 ((_ rotate_right 3) _let_1))) (let ((_let_5 (ite (bvsge (select (store (store a4 ((_ extract 4 4) v0) ((_ extract 9 3) v0)) (_ bv1 1) ((_ extract 12 6) v0)) v1) _let_2) (_ bv1 1) (_ bv0 1)))) (let ((_let_6 ((_ repeat 5) _let_3))) (let ((_let_7 (bvsdiv ((_ sign_extend 9) _let_4) _let_0))) (let ((_let_8 (bvmul ((_ zero_extend 15) (_ bv1 1)) v0))) (let ((_let_9 ((_ sign_extend 4) (_ bv1 1)))) (let ((_let_10 (or (ite (and (or (bvsgt (select (store (store a4 ((_ extract 4 4) v0) ((_ extract 9 3) v0)) (_ bv1 1) ((_ extract 12 6) v0)) v1) ((_ zero_extend 6) _let_5)) (= (bvsle _let_7 ((_ zero_extend 15) _let_5)) (bvsgt v0 _let_0))) (not (or (= ((_ zero_extend 15) (_ bv1 1)) _let_7) (bvult ((_ zero_extend 11) _let_6) _let_0)))) (and (or (bvsgt (select (store (store a4 ((_ extract 4 4) v0) ((_ extract 9 3) v0)) (_ bv1 1) ((_ extract 12 6) v0)) v1) ((_ zero_extend 6) _let_5)) (= (bvsle _let_7 ((_ zero_extend 15) _let_5)) (bvsgt v0 _let_0))) (not (or (= ((_ zero_extend 15) (_ bv1 1)) _let_7) (bvult ((_ zero_extend 11) _let_6) _let_0)))) (= (or (and (bvsgt _let_4 _let_2) (= (bvult (_ bv1 1) (_ bv1 1)) (bvsle ((_ sign_extend 6) v1) _let_4))) (=> (bvugt _let_8 ((_ zero_extend 15) v1)) (bvsle _let_6 ((_ zero_extend 4) _let_3)))) (xor (bvugt (select (store (store a4 ((_ extract 4 4) v0) ((_ extract 9 3) v0)) (_ bv1 1) ((_ extract 12 6) v0)) v1) ((_ sign_extend 2) _let_6)) (= (or (bvugt ((_ zero_extend 9) _let_1) _let_8) (bvule _let_4 ((_ zero_extend 6) _let_3))) (bvsgt _let_9 _let_6))))) (ite (bvslt _let_3 _let_5) (distinct (select (store (store a4 ((_ extract 4 4) v0) ((_ extract 9 3) v0)) (_ bv1 1) ((_ extract 12 6) v0)) v1) (select (store (store a4 ((_ extract 4 4) v0) ((_ extract 9 3) v0)) (_ bv1 1) ((_ extract 12 6) v0)) v1)) (bvule _let_6 _let_9))))) (and (and (and (not (bvsle _let_1 _let_4)) (= _let_10 _let_10)) (not (= _let_0 (_ bv0 16)))) (not (= _let_0 (bvnot (_ bv0 16)))))))))))))))) ))
