(set-option :incremental false)
(meta-info :status sat)
(set-logic QF_BV)
(declare-fun v0 () (_ BitVec 9))
(declare-fun v1 () (_ BitVec 2))
(declare-fun v2 () (_ BitVec 1))
(declare-fun v3 () (_ BitVec 3))
(check-sat-assuming ( (let ((_let_0 (bvsub (_ bv256 9) ((_ sign_extend 8) ((_ extract 0 0) v1))))) (let ((_let_1 ((_ zero_extend 1) v3))) (let ((_let_2 ((_ rotate_right 1) v1))) (let ((_let_3 (ite (bvule ((_ sign_extend 2) ((_ extract 1 1) _let_1)) v3) (_ bv1 1) (_ bv0 1)))) (let ((_let_4 ((_ rotate_left 0) v2))) (let ((_let_5 (bvnot v2))) (let ((_let_6 (bvadd (bvxnor _let_2 ((_ zero_extend 1) v2)) (bvxnor _let_2 ((_ zero_extend 1) v2))))) (let ((_let_7 (bvnand v3 ((_ sign_extend 2) v2)))) (let ((_let_8 (bvnot _let_1))) (let ((_let_9 (ite (bvslt _let_4 _let_3) (_ bv1 1) (_ bv0 1)))) (let ((_let_10 (bvand ((_ zero_extend 1) _let_4) (bvor ((_ zero_extend 1) ((_ extract 0 0) (bvadd _let_3 ((_ extract 0 0) v1)))) _let_2)))) (let ((_let_11 (ite (distinct ((_ sign_extend 1) _let_10) _let_7) (_ bv1 1) (_ bv0 1)))) (let ((_let_12 (ite (bvult (ite (bvult ((_ extract 1 0) v1) ((_ extract 1 0) v1)) (_ bv1 1) (_ bv0 1)) (ite (bvult ((_ extract 1 0) v1) ((_ extract 1 0) v1)) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)))) (let ((_let_13 (bvadd (bvxnor _let_2 ((_ zero_extend 1) v2)) ((_ sign_extend 1) _let_5)))) (let ((_let_14 ((_ sign_extend 3) _let_5))) (let ((_let_15 (ite (= (_ bv1 1) ((_ extract 0 0) (_ bv68 7))) _let_1 _let_14))) (let ((_let_16 (bvmul ((_ zero_extend 3) (bvcomp ((_ sign_extend 2) _let_2) _let_1)) _let_15))) (let ((_let_17 (ite (bvslt _let_2 ((_ zero_extend 1) ((_ extract 1 1) _let_1))) (_ bv1 1) (_ bv0 1)))) (let ((_let_18 (ite (= (_ bv1 1) ((_ extract 0 0) (bvadd _let_3 ((_ extract 0 0) v1)))) _let_7 ((_ zero_extend 2) _let_4)))) (let ((_let_19 (ite (bvslt v3 ((_ zero_extend 2) _let_9)) (_ bv1 1) (_ bv0 1)))) (let ((_let_20 ((_ rotate_right 0) _let_9))) (let ((_let_21 ((_ extract 1 1) _let_2))) (let ((_let_22 (bvcomp _let_14 _let_15))) (let ((_let_23 (bvmul ((_ sign_extend 8) ((_ extract 0 0) v1)) v0))) (let ((_let_24 ((_ sign_extend 1) _let_17))) (let ((_let_25 ((_ zero_extend 5) _let_8))) (let ((_let_26 ((_ sign_extend 5) _let_16))) (let ((_let_27 ((_ sign_extend 1) _let_12))) (let ((_let_28 ((_ zero_extend 6) _let_7))) (let ((_let_29 (and (ite (or (not (= _let_10 (bvxnor _let_2 ((_ zero_extend 1) v2)))) (or (not (xor (xor (= ((_ zero_extend 3) _let_5) _let_8) (= ((_ sign_extend 1) (bvxnor _let_2 ((_ zero_extend 1) v2))) v3)) (= (ite (bvult ((_ extract 1 0) v1) ((_ extract 1 0) v1)) (_ bv1 1) (_ bv0 1)) ((_ extract 1 1) _let_1)))) (not (xor (xor (= ((_ zero_extend 3) _let_5) _let_8) (= ((_ sign_extend 1) (bvxnor _let_2 ((_ zero_extend 1) v2))) v3)) (= (ite (bvult ((_ extract 1 0) v1) ((_ extract 1 0) v1)) (_ bv1 1) (_ bv0 1)) ((_ extract 1 1) _let_1)))))) (or (not (= _let_10 (bvxnor _let_2 ((_ zero_extend 1) v2)))) (or (not (xor (xor (= ((_ zero_extend 3) _let_5) _let_8) (= ((_ sign_extend 1) (bvxnor _let_2 ((_ zero_extend 1) v2))) v3)) (= (ite (bvult ((_ extract 1 0) v1) ((_ extract 1 0) v1)) (_ bv1 1) (_ bv0 1)) ((_ extract 1 1) _let_1)))) (not (xor (xor (= ((_ zero_extend 3) _let_5) _let_8) (= ((_ sign_extend 1) (bvxnor _let_2 ((_ zero_extend 1) v2))) v3)) (= (ite (bvult ((_ extract 1 0) v1) ((_ extract 1 0) v1)) (_ bv1 1) (_ bv0 1)) ((_ extract 1 1) _let_1)))))) (=> (ite (=> (not (or (= ((_ zero_extend 8) _let_11) (_ bv256 9)) (= _let_22 (ite (bvult ((_ extract 1 0) v1) ((_ extract 1 0) v1)) (_ bv1 1) (_ bv0 1))))) (ite (= _let_13 ((_ zero_extend 1) _let_21)) (not (= ((_ extract 0 0) v1) _let_11)) (= _let_8 ((_ zero_extend 2) _let_13)))) (ite (and (or (= (= ((_ zero_extend 3) v2) _let_1) (= _let_26 _let_0)) (not (ite (ite (and (= _let_4 (bvcomp ((_ sign_extend 2) _let_2) _let_1)) (= ((_ zero_extend 5) _let_6) (_ bv68 7))) (ite (= (ite (= ((_ zero_extend 1) _let_22) _let_6) (= _let_6 ((_ sign_extend 1) _let_21)) (= _let_23 _let_28)) (= ((_ extract 1 0) v1) v1)) (= ((_ zero_extend 3) _let_12) _let_15) (= (_ bv256 9) _let_25)) (= _let_8 ((_ sign_extend 3) _let_4))) (xor (= ((_ sign_extend 7) (bvor ((_ zero_extend 1) ((_ extract 0 0) (bvadd _let_3 ((_ extract 0 0) v1)))) _let_2)) _let_0) (= ((_ extract 1 1) _let_1) v2)) (and (= ((_ sign_extend 7) _let_13) v0) (= _let_0 ((_ zero_extend 8) _let_3)))))) (= ((_ zero_extend 2) _let_21) _let_18)) (ite (= (= _let_27 ((_ extract 1 0) v1)) (= (bvcomp ((_ sign_extend 2) _let_2) _let_1) ((_ extract 0 0) (bvadd _let_3 ((_ extract 0 0) v1))))) (= (ite (bvult ((_ extract 1 0) v1) ((_ extract 1 0) v1)) (_ bv1 1) (_ bv0 1)) (bvcomp ((_ sign_extend 2) _let_2) _let_1)) (not (ite (= ((_ sign_extend 2) _let_6) _let_8) (= _let_23 _let_23) (= _let_0 ((_ zero_extend 7) _let_6))))) (ite (= (= _let_27 ((_ extract 1 0) v1)) (= (bvcomp ((_ sign_extend 2) _let_2) _let_1) ((_ extract 0 0) (bvadd _let_3 ((_ extract 0 0) v1))))) (= (ite (bvult ((_ extract 1 0) v1) ((_ extract 1 0) v1)) (_ bv1 1) (_ bv0 1)) (bvcomp ((_ sign_extend 2) _let_2) _let_1)) (not (ite (= ((_ sign_extend 2) _let_6) _let_8) (= _let_23 _let_23) (= _let_0 ((_ zero_extend 7) _let_6)))))) (= (bvxnor _let_2 ((_ zero_extend 1) v2)) _let_10)) (= ((_ sign_extend 5) _let_15) (_ bv256 9)))) (=> (xor (not (and (= _let_20 (bvadd _let_3 ((_ extract 0 0) v1))) (ite (= _let_7 ((_ sign_extend 2) (bvcomp ((_ sign_extend 2) _let_2) _let_1))) (= _let_6 _let_27) (= _let_18 ((_ zero_extend 2) (bvadd _let_3 ((_ extract 0 0) v1))))))) (or (=> (= ((_ zero_extend 3) ((_ extract 0 0) v1)) _let_15) (= ((_ extract 0 0) (bvadd _let_3 ((_ extract 0 0) v1))) _let_21)) (= ((_ sign_extend 1) (bvcomp ((_ sign_extend 2) _let_2) _let_1)) _let_2))) (= (=> (= ((_ zero_extend 8) _let_9) _let_0) (= (_ bv68 7) ((_ zero_extend 6) (bvadd _let_3 ((_ extract 0 0) v1))))) (not (and (= ((_ sign_extend 1) _let_6) _let_18) (=> (and (and (and (=> (= ((_ sign_extend 1) _let_19) ((_ extract 1 0) v1)) (= _let_26 (_ bv256 9))) (not (= _let_8 _let_15))) (not (or (xor (= _let_24 (bvor ((_ zero_extend 1) ((_ extract 0 0) (bvadd _let_3 ((_ extract 0 0) v1)))) _let_2)) (= _let_6 (bvor ((_ zero_extend 1) ((_ extract 0 0) (bvadd _let_3 ((_ extract 0 0) v1)))) _let_2))) (= _let_16 ((_ sign_extend 2) (bvor ((_ zero_extend 1) ((_ extract 0 0) (bvadd _let_3 ((_ extract 0 0) v1)))) _let_2)))))) (or (= ((_ sign_extend 6) _let_11) (_ bv68 7)) (not (= _let_10 ((_ zero_extend 1) _let_4))))) (not (ite (and (= (ite (bvult ((_ extract 1 0) v1) ((_ extract 1 0) v1)) (_ bv1 1) (_ bv0 1)) _let_22) (= _let_8 ((_ zero_extend 1) _let_18))) (and (=> (= ((_ zero_extend 6) _let_18) _let_0) (= _let_28 _let_23)) (not (ite (= _let_9 ((_ extract 0 0) v1)) (= (bvadd _let_3 ((_ extract 0 0) v1)) (ite (bvult ((_ extract 1 0) v1) ((_ extract 1 0) v1)) (_ bv1 1) (_ bv0 1))) (= _let_9 ((_ extract 1 1) _let_1))))) (or (= _let_25 (_ bv256 9)) (= ((_ sign_extend 7) _let_2) _let_23)))))))))))) (and (and _let_29 _let_29) (or (ite (= (= (_ bv68 7) ((_ sign_extend 6) _let_4)) (xor (not (or (= _let_16 ((_ zero_extend 2) (bvor ((_ zero_extend 1) ((_ extract 0 0) (bvadd _let_3 ((_ extract 0 0) v1)))) _let_2))) (= (ite (bvult ((_ extract 1 0) v1) ((_ extract 1 0) v1)) (_ bv1 1) (_ bv0 1)) _let_20))) (xor (ite (= ((_ extract 0 0) v1) _let_4) (= _let_7 ((_ zero_extend 1) _let_2)) (= _let_4 _let_11)) (= (bvxnor _let_2 ((_ zero_extend 1) v2)) ((_ sign_extend 1) _let_11))))) (not (=> (= ((_ sign_extend 3) _let_17) _let_15) (= ((_ sign_extend 8) v2) _let_0))) (or (not (= (and (= ((_ extract 1 1) _let_1) _let_21) (= (=> (= _let_1 _let_16) (= _let_7 ((_ zero_extend 2) ((_ extract 0 0) v1)))) (= ((_ zero_extend 1) _let_12) _let_6))) (xor (xor (= _let_22 v2) (ite (= v1 _let_24) (= (_ bv68 7) ((_ zero_extend 4) ((_ extract 3 1) _let_1))) (ite (= (bvxnor _let_2 ((_ zero_extend 1) v2)) ((_ sign_extend 1) _let_4)) (= _let_20 _let_19) (= (ite (bvugt _let_10 _let_2) (_ bv1 1) (_ bv0 1)) _let_3)))) (= ((_ sign_extend 8) _let_5) _let_0)))) (not (and (=> (= _let_0 ((_ zero_extend 7) _let_10)) (= v0 ((_ sign_extend 5) _let_1))) (= _let_6 ((_ sign_extend 1) _let_20)))))) (= ((_ zero_extend 2) _let_10) _let_8))))))))))))))))))))))))))))))))) ))
