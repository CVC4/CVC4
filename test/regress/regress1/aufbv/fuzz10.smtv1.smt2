(set-option :incremental false)
(set-info :status unsat)
(set-logic QF_AUFBV)
(declare-fun v0 () (_ BitVec 11))
(declare-fun v1 () (_ BitVec 8))
(declare-fun a2 () (Array (_ BitVec 15) (_ BitVec 1)))
(declare-fun a3 () (Array (_ BitVec 6) (_ BitVec 8)))
(declare-fun a4 () (Array (_ BitVec 11) (_ BitVec 10)))
(check-sat-assuming ( (let ((_let_0 (bvcomp ((_ zero_extend 3) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1)) v0))) (let ((_let_1 (select (store (store a3 ((_ extract 12 7) (bvsrem (_ bv4728 14) (_ bv4728 14))) v1) ((_ extract 6 1) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1)) v1) ((_ zero_extend 2) (_ bv8 4))))) (let ((_let_2 (bvudiv ((_ sign_extend 13) (select a2 ((_ zero_extend 7) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1)))) (bvsrem (_ bv4728 14) (_ bv4728 14))))) (let ((_let_3 (bvsdiv ((_ zero_extend 10) (select a2 ((_ zero_extend 7) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1)))) v0))) (let ((_let_4 ((_ zero_extend 13) (select a2 ((_ zero_extend 7) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1)))))) (let ((_let_5 (ite (bvsle _let_2 _let_4) (_ bv1 1) (_ bv0 1)))) (let ((_let_6 (ite (bvuge ((_ sign_extend 6) v1) (bvsrem (_ bv4728 14) (_ bv4728 14))) (_ bv1 1) (_ bv0 1)))) (let ((_let_7 (ite (bvugt _let_3 ((_ zero_extend 3) _let_1)) (_ bv1 1) (_ bv0 1)))) (let ((_let_8 (bvneg (_ bv4728 14)))) (let ((_let_9 (bvshl _let_8 _let_4))) (let ((_let_10 (ite (= (_ bv1 1) ((_ extract 0 0) (select a2 ((_ zero_extend 7) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1))))) (bvlshr ((_ zero_extend 10) (_ bv8 4)) (bvsrem (_ bv4728 14) (_ bv4728 14))) _let_8))) (let ((_let_11 (bvcomp (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1) v1))) (let ((_let_12 ((_ zero_extend 3) _let_3))) (let ((_let_13 (bvsmod _let_12 _let_2))) (let ((_let_14 (bvsub (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1) ((_ sign_extend 7) _let_5)))) (let ((_let_15 (bvnor ((_ zero_extend 13) _let_0) _let_10))) (let ((_let_16 ((_ zero_extend 13) _let_6))) (let ((_let_17 ((_ sign_extend 13) _let_0))) (let ((_let_18 ((_ sign_extend 13) _let_7))) (let ((_let_19 ((_ zero_extend 7) _let_11))) (let ((_let_20 ((_ sign_extend 3) v0))) (let ((_let_21 ((_ zero_extend 6) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1)))) (let ((_let_22 ((_ sign_extend 13) _let_6))) (let ((_let_23 ((_ sign_extend 6) _let_1))) (let ((_let_24 ((_ zero_extend 6) v1))) (let ((_let_25 (ite (ite (or (ite (and (bvsgt _let_2 _let_17) (distinct ((_ zero_extend 7) (select a2 ((_ zero_extend 7) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1)))) _let_14)) (not (and (or (bvule _let_2 _let_21) (= (bvsle _let_17 (bvlshr ((_ zero_extend 10) (_ bv8 4)) (bvsrem (_ bv4728 14) (_ bv4728 14)))) (bvugt _let_0 (select a2 ((_ zero_extend 7) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1)))))) (xor (bvsgt _let_14 _let_19) (bvuge _let_10 _let_13)))) (or (bvugt _let_17 _let_13) (= (select a2 ((_ zero_extend 7) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1))) (select a2 ((_ zero_extend 7) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1)))))) (ite (bvult ((_ zero_extend 13) _let_7) _let_9) (= (=> (distinct _let_8 (bvsrem (_ bv4728 14) (_ bv4728 14))) (bvsle (bvlshr ((_ zero_extend 10) (_ bv8 4)) (bvsrem (_ bv4728 14) (_ bv4728 14))) _let_2)) (bvsle _let_10 ((_ sign_extend 13) _let_5))) (or (xor (bvugt _let_19 _let_14) (bvsgt _let_5 _let_0)) (bvult _let_0 _let_0)))) (or (or (ite (or (and (= _let_16 (bvsrem (_ bv4728 14) (_ bv4728 14))) (not (= (bvsge _let_16 _let_13) (bvule _let_5 _let_0)))) (or (not (bvugt _let_10 _let_20)) (ite (bvslt _let_24 _let_13) (bvsle _let_14 ((_ zero_extend 7) _let_6)) (not (ite (= (_ bv8 4) (_ bv8 4)) (bvugt (bvsrem (_ bv4728 14) (_ bv4728 14)) _let_22) (bvuge ((_ zero_extend 3) v0) _let_2)))))) (xor (= (or (distinct ((_ sign_extend 13) (select a2 ((_ zero_extend 7) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1)))) _let_15) (xor (= (distinct v0 ((_ sign_extend 10) _let_5)) (bvsge _let_3 ((_ sign_extend 10) _let_11))) (bvslt _let_8 ((_ zero_extend 13) _let_5)))) (or (bvslt _let_7 _let_0) (bvsle _let_10 ((_ sign_extend 6) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1))))) (xor (bvult _let_19 (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1)) (bvugt _let_13 ((_ zero_extend 13) _let_11)))) (not (and (= _let_15 (bvlshr ((_ zero_extend 10) (_ bv8 4)) (bvsrem (_ bv4728 14) (_ bv4728 14)))) (or (bvuge _let_8 _let_18) (= _let_8 _let_23))))) (ite (bvslt (bvsrem (_ bv4728 14) (_ bv4728 14)) (_ bv4728 14)) (bvult ((_ sign_extend 6) v1) _let_8) (bvsgt ((_ sign_extend 6) v1) _let_13))) (and (=> (= (ite (bvult ((_ zero_extend 10) _let_7) v0) (distinct ((_ sign_extend 6) v1) (_ bv4728 14)) (ite (bvslt _let_15 _let_21) (= _let_15 _let_12) (bvsle _let_15 ((_ zero_extend 6) _let_14)))) (not (bvugt _let_8 _let_15))) (= (ite (bvult ((_ sign_extend 13) _let_11) _let_8) (not (bvult (_ bv4728 14) _let_20)) (bvsle _let_22 _let_9)) (bvsgt _let_15 _let_13))) (bvugt _let_9 _let_17))) (= (=> (or (=> (bvslt ((_ zero_extend 3) _let_1) _let_3) (bvslt ((_ zero_extend 3) _let_1) _let_3)) (bvult _let_10 _let_18)) (ite (= _let_19 _let_14) (= _let_9 _let_10) (not (bvsle (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1) ((_ sign_extend 7) _let_7))))) (=> (xor (bvsge _let_15 _let_24) (xor (bvule ((_ sign_extend 13) (select a2 ((_ zero_extend 7) (bvsdiv ((_ sign_extend 4) (_ bv8 4)) v1)))) (bvsrem (_ bv4728 14) (_ bv4728 14))) (bvsgt _let_11 _let_11))) (=> (bvsge _let_23 _let_10) (bvsgt v0 ((_ sign_extend 10) _let_6)))))) (bvugt _let_11 _let_5) (not (bvugt _let_9 _let_12))))) (let ((_let_26 (bvnot (_ bv0 14)))) (and (and (and (and (and (and (and (and (and (and (= _let_15 _let_8) (and _let_25 _let_25)) (not (= (bvsrem (_ bv4728 14) (_ bv4728 14)) (_ bv0 14)))) (not (= v0 (_ bv0 11)))) (not (= v0 (bvnot (_ bv0 11))))) (not (= v1 (_ bv0 8)))) (not (= v1 (bvnot (_ bv0 8))))) (not (= _let_2 (_ bv0 14)))) (not (= _let_2 _let_26))) (not (= (_ bv4728 14) (_ bv0 14)))) (not (= (_ bv4728 14) _let_26)))))))))))))))))))))))))))))) ))
