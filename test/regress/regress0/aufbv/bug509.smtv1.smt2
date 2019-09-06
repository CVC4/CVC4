(set-option :incremental false)
(set-info :status sat)
(set-logic QF_AUFBV)
(declare-fun v0 () (_ BitVec 1))
(declare-fun v1 () (_ BitVec 13))
(declare-fun v2 () (_ BitVec 5))
(declare-fun a3 () (Array (_ BitVec 1) (_ BitVec 16)))
(check-sat-assuming ( (let ((_let_0 (bvnand v1 v1))) (let ((_let_1 ((_ zero_extend 11) v2))) (let ((_let_2 (select a3 (bvneg v0)))) (let ((_let_3 (select a3 ((_ extract 14 14) _let_2)))) (let ((_let_4 (ite (bvsgt (select a3 (_ bv1 1)) ((_ zero_extend 15) (bvneg v0))) (_ bv1 1) (_ bv0 1)))) (let ((_let_5 (bvnand ((_ sign_extend 12) (_ bv1 1)) _let_0))) (let ((_let_6 (ite (bvsge _let_3 (select a3 (_ bv1 1))) (_ bv1 1) (_ bv0 1)))) (let ((_let_7 (bvand _let_2 ((_ sign_extend 15) _let_4)))) (let ((_let_8 (ite (bvult ((_ sign_extend 8) v2) v1) (_ bv1 1) (_ bv0 1)))) (let ((_let_9 ((_ sign_extend 2) ((_ sign_extend 1) v1)))) (let ((_let_10 (bvurem _let_1 _let_3))) (let ((_let_11 (ite (= _let_3 _let_3) (_ bv1 1) (_ bv0 1)))) (let ((_let_12 ((_ sign_extend 15) (_ bv1 1)))) (let ((_let_13 ((_ sign_extend 15) v0))) (let ((_let_14 (or (xor (or (bvsle ((_ sign_extend 15) _let_11) (bvxor (select a3 (_ bv1 1)) ((_ zero_extend 15) ((_ rotate_right 0) (_ bv1 1))))) (bvsgt (bvxor (select a3 (_ bv1 1)) ((_ zero_extend 15) ((_ rotate_right 0) (_ bv1 1)))) ((_ zero_extend 15) (bvxnor v0 v0)))) (ite (or (bvsgt _let_3 _let_3) (bvsgt _let_3 _let_3)) (ite (ite (bvugt (select a3 (_ bv1 1)) (bvsrem _let_9 _let_2)) (=> (= (bvsge (select a3 (_ bv1 1)) ((_ zero_extend 15) _let_6)) (bvsle _let_10 (select a3 (_ bv1 1)))) (=> (bvslt (bvxor (select a3 (_ bv1 1)) ((_ zero_extend 15) ((_ rotate_right 0) (_ bv1 1)))) _let_10) (distinct ((_ sign_extend 12) _let_4) v1))) (ite (bvsge (ite (bvslt ((_ sign_extend 12) (bvneg v0)) v1) (_ bv1 1) (_ bv0 1)) (ite (bvslt ((_ sign_extend 12) (bvneg v0)) v1) (_ bv1 1) (_ bv0 1))) (bvuge _let_13 (bvxor (select a3 (_ bv1 1)) ((_ zero_extend 15) ((_ rotate_right 0) (_ bv1 1))))) (= _let_10 ((_ zero_extend 3) v1)))) (bvule (bvlshr _let_3 ((_ sign_extend 15) (ite (bvslt ((_ sign_extend 12) (bvneg v0)) v1) (_ bv1 1) (_ bv0 1)))) _let_1) (bvult ((_ sign_extend 15) _let_8) (bvashr ((_ zero_extend 3) _let_0) (bvxor (select a3 (_ bv1 1)) ((_ zero_extend 15) ((_ rotate_right 0) (_ bv1 1))))))) (not (and (bvsle v0 _let_4) (bvule _let_7 _let_3))))) (and (bvugt (select a3 (_ bv1 1)) ((_ zero_extend 15) _let_11)) (bvult (_ bv1 1) ((_ rotate_right 0) (_ bv1 1))))))) (let ((_let_15 (ite (xor (=> (or (bvule _let_10 _let_1) (distinct ((_ zero_extend 12) (bvxnor v0 v0)) v1)) (bvsgt _let_3 _let_10)) (bvule (bvlshr _let_3 ((_ sign_extend 15) (ite (bvslt ((_ sign_extend 12) (bvneg v0)) v1) (_ bv1 1) (_ bv0 1)))) _let_1)) (xor (=> (or (bvule _let_10 _let_1) (distinct ((_ zero_extend 12) (bvxnor v0 v0)) v1)) (bvsgt _let_3 _let_10)) (bvule (bvlshr _let_3 ((_ sign_extend 15) (ite (bvslt ((_ sign_extend 12) (bvneg v0)) v1) (_ bv1 1) (_ bv0 1)))) _let_1)) (bvuge (bvashr ((_ zero_extend 3) _let_0) (bvxor (select a3 (_ bv1 1)) ((_ zero_extend 15) ((_ rotate_right 0) (_ bv1 1))))) ((_ sign_extend 3) _let_0))))) (and (and (and (or (ite _let_14 (ite (xor (xor (bvslt _let_1 (select a3 (_ bv1 1))) (distinct (select a3 (_ bv1 1)) _let_10)) (and (= _let_12 _let_3) (bvuge _let_7 _let_1))) (not (and (and (not (= (bvslt (ite (bvslt ((_ sign_extend 12) (bvneg v0)) v1) (_ bv1 1) (_ bv0 1)) (bvneg v0)) (or (bvugt _let_8 _let_6) (bvsge (_ bv1 1) v0)))) (not (= (distinct _let_9 _let_1) (bvugt _let_12 (bvlshr _let_3 ((_ sign_extend 15) (ite (bvslt ((_ sign_extend 12) (bvneg v0)) v1) (_ bv1 1) (_ bv0 1)))))))) (ite (= (= (bvuge ((_ rotate_right 0) (_ bv1 1)) _let_4) (or (distinct _let_1 _let_3) (bvugt _let_2 _let_13))) (distinct _let_3 ((_ zero_extend 15) _let_4))) (not (bvslt ((_ rotate_right 0) (_ bv1 1)) _let_4)) (= (bvule _let_5 ((_ sign_extend 12) _let_8)) (bvsle ((_ zero_extend 12) _let_4) _let_5))))) (bvsgt _let_7 ((_ sign_extend 15) ((_ rotate_right 0) (_ bv1 1))))) _let_14) (xor _let_15 _let_15)) (not (= _let_3 (_ bv0 16)))) (not (= _let_2 (_ bv0 16)))) (not (= _let_2 (bvnot (_ bv0 16))))))))))))))))))))) ))
