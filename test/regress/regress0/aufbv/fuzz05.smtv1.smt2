(set-option :incremental false)
(meta-info :status sat)
(set-logic QF_AUFBV)
(declare-fun v0 () (_ BitVec 13))
(declare-fun a1 () (Array (_ BitVec 5) (_ BitVec 5)))
(declare-fun a2 () (Array (_ BitVec 12) (_ BitVec 9)))
(declare-fun a3 () (Array (_ BitVec 8) (_ BitVec 7)))
(check-sat-assuming ( (let ((_let_0 (bvsdiv v0 v0))) (let ((_let_1 (bvurem (_ bv6 4) (_ bv6 4)))) (let ((_let_2 ((_ sign_extend 8) _let_1))) (let ((_let_3 (select (store a2 ((_ extract 11 0) v0) ((_ zero_extend 5) (_ bv6 4))) _let_2))) (let ((_let_4 (select a2 ((_ extract 11 0) _let_0)))) (let ((_let_5 (select a2 _let_2))) (let ((_let_6 (select a2 ((_ extract 12 1) v0)))) (let ((_let_7 (bvadd _let_3 ((_ zero_extend 5) _let_1)))) (let ((_let_8 (ite (bvuge _let_6 ((_ sign_extend 4) (select a1 ((_ extract 6 2) v0)))) (_ bv1 1) (_ bv0 1)))) (let ((_let_9 (bvnor (_ bv6 4) (_ bv6 4)))) (let ((_let_10 ((_ zero_extend 4) _let_5))) (let ((_let_11 (ite (bvsge _let_10 (bvmul ((_ sign_extend 4) _let_6) _let_0)) (_ bv1 1) (_ bv0 1)))) (let ((_let_12 (ite (bvuge _let_4 ((_ zero_extend 8) (bvcomp v0 ((_ zero_extend 9) (_ bv6 4))))) (_ bv1 1) (_ bv0 1)))) (let ((_let_13 (and (= (xor (bvsge ((_ sign_extend 8) _let_12) _let_3) (bvult ((_ zero_extend 3) _let_8) _let_9)) (=> (not (ite (bvugt ((_ zero_extend 1) (_ bv6 4)) (select a1 ((_ extract 6 2) v0))) (bvule _let_0 ((_ sign_extend 4) _let_4)) (bvsge _let_1 ((_ sign_extend 3) _let_12)))) (not (bvsle ((_ zero_extend 9) ((_ rotate_left 1) _let_9)) _let_0)))) (ite (ite (ite (bvsge (bvmul ((_ sign_extend 4) _let_6) _let_0) _let_10) (bvuge ((_ zero_extend 9) _let_1) v0) (bvsgt _let_4 ((_ sign_extend 8) _let_11))) (xor (and (= (select a1 ((_ extract 6 2) v0)) ((_ sign_extend 1) _let_1)) (= _let_7 ((_ zero_extend 8) _let_8))) (bvult ((_ sign_extend 5) _let_1) _let_5)) (ite (bvsge (bvmul ((_ sign_extend 4) _let_6) _let_0) _let_10) (bvuge ((_ zero_extend 9) _let_1) v0) (bvsgt _let_4 ((_ sign_extend 8) _let_11)))) (and (not (and (xor (bvsgt ((_ sign_extend 8) (bvcomp v0 ((_ zero_extend 9) (_ bv6 4)))) _let_5) (bvult _let_4 ((_ sign_extend 5) (bvnand _let_1 _let_1)))) (= _let_10 v0))) (xor (bvslt ((_ zero_extend 4) (select a1 ((_ extract 6 2) v0))) _let_5) (bvult _let_4 ((_ sign_extend 5) (_ bv6 4))))) (and (not (and (xor (bvsgt ((_ sign_extend 8) (bvcomp v0 ((_ zero_extend 9) (_ bv6 4)))) _let_5) (bvult _let_4 ((_ sign_extend 5) (bvnand _let_1 _let_1)))) (= _let_10 v0))) (xor (bvslt ((_ zero_extend 4) (select a1 ((_ extract 6 2) v0))) _let_5) (bvult _let_4 ((_ sign_extend 5) (_ bv6 4))))))))) (and (and (and (xor (or _let_13 _let_13) (not (and (bvult ((_ sign_extend 5) _let_9) _let_6) (xor (bvsge _let_3 ((_ zero_extend 8) _let_11)) (bvuge _let_6 _let_7))))) (not (= v0 (_ bv0 13)))) (not (= v0 (bvnot (_ bv0 13))))) (not (= (_ bv6 4) (_ bv0 4)))))))))))))))))) ))
