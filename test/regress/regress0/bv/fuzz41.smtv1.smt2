(set-option :incremental false)
(meta-info :status sat)
(set-logic QF_BV)
(declare-fun v0 () (_ BitVec 15))
(declare-fun v1 () (_ BitVec 7))
(declare-fun v2 () (_ BitVec 13))
(declare-fun v3 () (_ BitVec 8))
(declare-fun v4 () (_ BitVec 9))
(check-sat-assuming ( (let ((_let_0 ((_ repeat 1) v3))) (let ((_let_1 (ite (bvsgt ((_ sign_extend 7) (ite (bvslt (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) ((_ sign_extend 2) v4)) (_ bv1 1) (_ bv0 1))) v3) (_ bv1 1) (_ bv0 1)))) (let ((_let_2 ((_ sign_extend 10) (ite (bvslt (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) ((_ sign_extend 2) v4)) (_ bv1 1) (_ bv0 1))))) (let ((_let_3 (bvnand ((_ zero_extend 8) (ite (bvslt (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) ((_ sign_extend 2) v4)) (_ bv1 1) (_ bv0 1))) v4))) (let ((_let_4 (bvsmod ((_ sign_extend 2) _let_3) (_ bv452 11)))) (let ((_let_5 (bvashr ((_ zero_extend 14) (ite (bvslt (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) ((_ sign_extend 2) v4)) (_ bv1 1) (_ bv0 1))) v0))) (let ((_let_6 (bvneg v4))) (let ((_let_7 ((_ extract 3 1) _let_5))) (let ((_let_8 (bvand v0 _let_5))) (let ((_let_9 (ite (= (bvnor ((_ sign_extend 2) (bvurem _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11)))) (bvsdiv ((_ zero_extend 2) (_ bv452 11)) v2)) ((_ zero_extend 4) _let_3)) (_ bv1 1) (_ bv0 1)))) (let ((_let_10 (ite (= (_ bv1 1) ((_ extract 2 2) (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11)))) ((_ sign_extend 7) _let_1) _let_0))) (let ((_let_11 (bvurem ((_ sign_extend 8) (ite (bvuge _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11))) (_ bv1 1) (_ bv0 1))) _let_6))) (let ((_let_12 ((_ extract 0 0) v4))) (let ((_let_13 (bvudiv ((_ zero_extend 2) _let_6) _let_4))) (let ((_let_14 ((_ sign_extend 0) (bvsdiv ((_ sign_extend 4) (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11))) _let_5)))) (let ((_let_15 (bvadd ((_ sign_extend 2) _let_6) (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11))))) (let ((_let_16 (bvneg (bvurem _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11)))))) (let ((_let_17 (ite (= ((_ zero_extend 6) (ite (bvslt (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) ((_ sign_extend 2) v4)) (_ bv1 1) (_ bv0 1))) v1) (_ bv1 1) (_ bv0 1)))) (let ((_let_18 ((_ sign_extend 14) _let_12))) (let ((_let_19 ((_ sign_extend 3) _let_10))) (let ((_let_20 ((_ sign_extend 6) _let_11))) (let ((_let_21 (or (xor (ite (and (xor (= (bvsdiv ((_ sign_extend 4) (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11))) _let_5) ((_ sign_extend 12) _let_7)) (bvuge _let_16 ((_ zero_extend 2) _let_3))) (bvslt v0 ((_ sign_extend 2) (bvnor ((_ sign_extend 2) (bvurem _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11)))) (bvsdiv ((_ zero_extend 2) (_ bv452 11)) v2))))) (not (or (xor (bvugt ((_ zero_extend 2) _let_6) _let_4) (and (= v2 (bvsdiv ((_ zero_extend 2) (_ bv452 11)) v2)) (bvsle ((_ sign_extend 2) v1) _let_6))) (bvule ((_ sign_extend 7) _let_0) _let_8))) (= (not (not (ite (and (bvsgt ((_ sign_extend 10) (ite (bvuge _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11))) (_ bv1 1) (_ bv0 1))) (_ bv452 11)) (= (bvslt ((_ zero_extend 10) (ite (bvuge _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11))) (_ bv1 1) (_ bv0 1))) (bvmul ((_ zero_extend 3) v3) (_ bv452 11))) (bvult (bvxnor ((_ zero_extend 2) (_ bv452 11)) v2) ((_ zero_extend 12) _let_12)))) (bvsge _let_18 _let_8) (= (xor (bvult (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) ((_ sign_extend 3) _let_0)) (bvuge ((_ sign_extend 2) _let_12) _let_7)) (= _let_12 _let_1))))) (ite (and (bvslt (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11)) ((_ rotate_left 2) _let_4)) (or (distinct ((_ zero_extend 2) (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11))) (bvxnor ((_ zero_extend 2) (_ bv452 11)) v2)) (= _let_15 _let_16))) (distinct _let_5 ((_ sign_extend 2) v2)) (not (bvsge ((_ zero_extend 4) v1) _let_16))))) (and (ite (bvslt _let_16 _let_19) (xor (bvsgt _let_18 _let_14) (bvult _let_11 v4)) (bvsgt _let_15 ((_ zero_extend 2) _let_11))) (= (bvsge (bvsdiv ((_ sign_extend 4) (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11))) _let_5) ((_ zero_extend 12) _let_7)) (bvugt ((_ zero_extend 5) _let_0) (bvsdiv ((_ zero_extend 2) (_ bv452 11)) v2))))) (and (=> (not (or (or (bvugt (bvnor ((_ sign_extend 2) (bvurem _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11)))) (bvsdiv ((_ zero_extend 2) (_ bv452 11)) v2)) ((_ sign_extend 2) (_ bv452 11))) (bvsge v2 (bvnor ((_ sign_extend 2) (bvurem _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11)))) (bvsdiv ((_ zero_extend 2) (_ bv452 11)) v2)))) (xor (bvugt _let_0 (bvxnor ((_ zero_extend 7) _let_1) _let_10)) (bvugt ((_ zero_extend 4) (bvurem _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11)))) _let_14)))) (xor (xor (bvsle ((_ sign_extend 12) _let_1) (bvxnor ((_ zero_extend 2) (_ bv452 11)) v2)) (ite (bvult _let_13 (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11))) (bvult ((_ sign_extend 14) (ite (bvslt (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) ((_ sign_extend 2) v4)) (_ bv1 1) (_ bv0 1))) _let_5) (bvule ((_ zero_extend 2) v2) (bvsdiv ((_ sign_extend 4) (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11))) _let_5)))) (xor (bvsle ((_ sign_extend 12) _let_1) (bvxnor ((_ zero_extend 2) (_ bv452 11)) v2)) (ite (bvult _let_13 (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11))) (bvult ((_ sign_extend 14) (ite (bvslt (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) ((_ sign_extend 2) v4)) (_ bv1 1) (_ bv0 1))) _let_5) (bvule ((_ zero_extend 2) v2) (bvsdiv ((_ sign_extend 4) (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11))) _let_5)))))) (and (or (and (bvugt v4 ((_ zero_extend 8) _let_17)) (bvslt _let_5 ((_ sign_extend 2) (bvsdiv ((_ zero_extend 2) (_ bv452 11)) v2)))) (ite (and (ite (distinct _let_13 _let_4) (= (ite (bvsge v0 _let_18) (= ((_ sign_extend 4) _let_16) (bvxor _let_8 ((_ zero_extend 2) (bvnor ((_ sign_extend 2) (bvurem _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11)))) (bvsdiv ((_ zero_extend 2) (_ bv452 11)) v2))))) (= ((_ sign_extend 10) _let_12) (bvmul ((_ zero_extend 3) v3) (_ bv452 11)))) (ite (bvule (bvsdiv ((_ zero_extend 2) (_ bv452 11)) v2) ((_ sign_extend 12) (ite (bvuge _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11))) (_ bv1 1) (_ bv0 1)))) (bvslt _let_8 ((_ sign_extend 4) (bvurem _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11))))) (bvsgt _let_14 ((_ sign_extend 7) _let_10)))) (bvugt _let_19 (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11)))) (= ((_ sign_extend 2) _let_11) ((_ rotate_left 1) (_ bv452 11)))) (= v1 ((_ sign_extend 6) (ite (bvuge _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11))) (_ bv1 1) (_ bv0 1)))) (=> (bvuge _let_14 ((_ zero_extend 14) _let_12)) (bvuge ((_ zero_extend 2) _let_15) v2)))) (ite (= (bvugt (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11)) (bvurem _let_2 (bvmul ((_ zero_extend 3) v3) (_ bv452 11)))) (bvsgt _let_5 _let_20)) (xor (or (bvule v0 _let_20) (bvsge ((_ sign_extend 3) v3) _let_15)) (ite (bvult _let_10 _let_0) (bvule v4 ((_ sign_extend 8) _let_1)) (not (bvugt _let_17 _let_17)))) (= (= ((_ zero_extend 14) _let_17) _let_14) (or (= (= (and (bvsgt _let_8 ((_ zero_extend 6) _let_11)) (distinct _let_4 ((_ sign_extend 10) _let_17))) (= (bvult v2 ((_ sign_extend 12) (ite (bvslt (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) ((_ sign_extend 2) v4)) (_ bv1 1) (_ bv0 1)))) (=> (= _let_1 _let_9) (= ((_ zero_extend 8) _let_9) _let_11)))) (not (bvult ((_ zero_extend 7) _let_0) v0))) (= (bvule ((_ sign_extend 6) v4) _let_14) (distinct ((_ zero_extend 1) v3) v4)))))))))) (and (and (and (and (and (and (and (and (and (xor (not (ite (and (=> (not (xor (bvuge _let_5 ((_ zero_extend 4) (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11)))) (or (= _let_9 _let_9) (bvslt _let_3 ((_ zero_extend 1) _let_10))))) (and (distinct _let_4 _let_4) (bvsge (bvsdiv ((_ sign_extend 4) (bvmul (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv452 11))) _let_5) _let_18))) (or (not (bvsgt (bvsdiv ((_ zero_extend 2) (_ bv452 11)) v2) ((_ sign_extend 10) _let_7))) (ite (bvsgt ((_ sign_extend 12) _let_7) v0) (bvult _let_13 ((_ sign_extend 3) v3)) (bvsge _let_8 ((_ sign_extend 4) _let_15))))) (not (bvuge _let_16 ((_ zero_extend 10) _let_12))) (ite (=> (= (bvugt ((_ sign_extend 3) v3) (bvmul ((_ zero_extend 3) v3) (_ bv452 11))) (bvslt _let_13 ((_ zero_extend 3) v3))) (bvslt _let_15 _let_15)) (= (bvsge (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) ((_ zero_extend 2) _let_3)) (bvule v0 (bvadd v0 v0))) (not (and (distinct ((_ sign_extend 3) v3) _let_4) (bvsgt ((_ zero_extend 3) (bvxnor ((_ zero_extend 7) _let_1) _let_10)) _let_16)))))) (xor _let_21 _let_21)) (not (= _let_4 (_ bv0 11)))) (not (= v2 (_ bv0 13)))) (not (= v2 (bvnot (_ bv0 13))))) (not (= _let_6 (_ bv0 9)))) (not (= (_ bv452 11) (_ bv0 11)))) (not (= (_ bv452 11) (bvnot (_ bv0 11))))) (not (= (bvmul ((_ zero_extend 3) v3) (_ bv452 11)) (_ bv0 11)))) (not (= _let_5 (_ bv0 15)))) (not (= _let_5 (bvnot (_ bv0 15))))))))))))))))))))))))))) ))
