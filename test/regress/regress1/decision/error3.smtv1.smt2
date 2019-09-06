(set-option :incremental false)
(set-info :status unsat)
(set-logic QF_AUFBV)
(declare-fun v0 () (_ BitVec 15))
(declare-fun v1 () (_ BitVec 3))
(declare-fun a2 () (Array (_ BitVec 13) (_ BitVec 3)))
(declare-fun a3 () (Array (_ BitVec 16) (_ BitVec 5)))
(check-sat-assuming ( (let ((_let_0 (ite (bvslt ((_ sign_extend 2) (ite (bvsgt v1 v1) (_ bv1 1) (_ bv0 1))) v1) (_ bv1 1) (_ bv0 1)))) (let ((_let_1 (ite (bvuge v0 ((_ zero_extend 14) _let_0)) (_ bv1 1) (_ bv0 1)))) (let ((_let_2 (bvmul ((_ zero_extend 10) (bvnot _let_0)) (_ bv228 11)))) (let ((_let_3 (select a2 ((_ zero_extend 12) _let_0)))) (let ((_let_4 (select a2 ((_ zero_extend 2) (_ bv228 11))))) (let ((_let_5 (ite (bvuge _let_2 ((_ zero_extend 10) (ite (bvsgt v1 v1) (_ bv1 1) (_ bv0 1)))) (_ bv1 1) (_ bv0 1)))) (let ((_let_6 (bvshl ((_ sign_extend 2) _let_0) (select a2 ((_ extract 13 1) v0))))) (let ((_let_7 (bvudiv ((_ sign_extend 2) _let_0) _let_6))) (let ((_let_8 (bvlshr v0 ((_ zero_extend 4) (bvsmod ((_ zero_extend 8) _let_6) _let_2))))) (let ((_let_9 ((_ rotate_left 5) (_ bv228 11)))) (let ((_let_10 (ite (= _let_0 (ite (bvsgt v1 v1) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)))) (let ((_let_11 (ite (bvsgt _let_9 _let_2) (_ bv1 1) (_ bv0 1)))) (let ((_let_12 (bvor _let_9 ((_ sign_extend 10) _let_5)))) (let ((_let_13 ((_ repeat 14) _let_1))) (let ((_let_14 (ite (bvsge (select a2 ((_ extract 13 1) v0)) ((_ sign_extend 2) _let_5)) (_ bv1 1) (_ bv0 1)))) (let ((_let_15 (bvshl _let_4 ((_ zero_extend 2) (ite (bvsgt v1 v1) (_ bv1 1) (_ bv0 1)))))) (let ((_let_16 ((_ zero_extend 3) (bvsmod ((_ zero_extend 8) _let_6) _let_2)))) (let ((_let_17 (bvxnor _let_11 _let_1))) (let ((_let_18 (concat v1 _let_1))) (let ((_let_19 (bvsmod ((_ sign_extend 4) (bvxnor (bvsmod ((_ zero_extend 8) _let_6) _let_2) ((_ sign_extend 10) (bvnot _let_0)))) _let_8))) (let ((_let_20 (ite (bvult _let_3 ((_ sign_extend 1) ((_ repeat 2) (bvnot _let_0)))) (_ bv1 1) (_ bv0 1)))) (let ((_let_21 ((_ zero_extend 8) (select a2 ((_ extract 13 1) v0))))) (let ((_let_22 ((_ sign_extend 2) _let_10))) (let ((_let_23 (bvugt _let_0 (ite (bvsgt v1 v1) (_ bv1 1) (_ bv0 1))))) (let ((_let_24 (bvuge (bvsmod ((_ zero_extend 8) _let_6) _let_2) ((_ zero_extend 8) _let_7)))) (let ((_let_25 (ite (not (or (bvult _let_12 ((_ zero_extend 9) ((_ repeat 2) (bvnot _let_0)))) (bvult _let_12 ((_ zero_extend 9) ((_ repeat 2) (bvnot _let_0)))))) (and (bvult _let_9 _let_21) (or (bvuge _let_22 (select a2 ((_ extract 13 1) v0))) (bvule _let_3 ((_ zero_extend 2) _let_14)))) (=> (or (distinct (bvxnor (bvsmod ((_ zero_extend 8) _let_6) _let_2) ((_ sign_extend 10) (bvnot _let_0))) ((_ zero_extend 10) _let_11)) (bvugt _let_20 (bvnot _let_0))) (= ((_ zero_extend 4) (bvsmod ((_ zero_extend 8) _let_6) _let_2)) v0))))) (and (and (and (and (and (ite (not (=> _let_25 _let_25)) (not (or (and (xor (bvuge ((_ sign_extend 14) _let_14) _let_19) (bvslt ((_ zero_extend 2) _let_5) (select a2 ((_ extract 13 1) v0)))) (not (=> (ite (= (bvule _let_13 ((_ sign_extend 13) (bvnot _let_0))) (xor (bvslt v0 ((_ zero_extend 4) (bvxnor (bvsmod ((_ zero_extend 8) _let_6) _let_2) ((_ sign_extend 10) (bvnot _let_0))))) (or (bvult ((_ sign_extend 1) _let_19) ((_ sign_extend 15) _let_5)) (bvsge _let_9 ((_ sign_extend 8) _let_15))))) (bvsgt v0 ((_ zero_extend 1) _let_16)) (=> (= (xor (bvsgt _let_18 ((_ sign_extend 1) v1)) (ite (bvuge _let_21 _let_9) (bvugt v1 ((_ sign_extend 2) _let_17)) (or (or (not (= _let_16 _let_16)) (or (=> (bvsge _let_16 ((_ zero_extend 13) (ite (bvsgt v1 v1) (_ bv1 1) (_ bv0 1)))) (= _let_6 _let_22)) (xor (bvsge ((_ zero_extend 2) _let_11) _let_6) (bvsgt ((_ sign_extend 12) _let_15) _let_19)))) (=> (not (and (bvsgt (bvxnor (bvsmod ((_ zero_extend 8) _let_6) _let_2) ((_ sign_extend 10) (bvnot _let_0))) (bvsmod ((_ zero_extend 8) _let_6) _let_2)) (bvule (select a2 ((_ extract 13 1) v0)) ((_ sign_extend 2) (bvnot _let_0))))) (= ((_ sign_extend 4) (_ bv228 11)) _let_19))))) (or (bvugt (bvxnor (bvsmod ((_ zero_extend 8) _let_6) _let_2) ((_ sign_extend 10) (bvnot _let_0))) _let_21) (= (bvsle ((_ sign_extend 2) _let_11) _let_3) (bvult _let_16 ((_ sign_extend 11) (select a2 ((_ extract 13 1) v0))))))) (bvult _let_8 ((_ zero_extend 1) _let_13)))) (distinct ((_ sign_extend 4) (bvsmod ((_ zero_extend 8) _let_6) _let_2)) _let_19)))) (= (=> (bvslt _let_15 ((_ sign_extend 2) _let_20)) (= (= (bvugt v0 ((_ zero_extend 14) _let_14)) (bvule _let_19 ((_ zero_extend 12) _let_7))) (or (bvule _let_6 (select a2 ((_ extract 13 1) v0))) (bvuge _let_2 ((_ zero_extend 7) _let_18))))) (bvsle _let_4 ((_ zero_extend 2) _let_17))))) (=> (or (=> _let_24 _let_24) (or (= (bvsgt _let_11 _let_1) (= _let_0 (bvnot _let_0))) (ite _let_23 (= (bvsle ((_ zero_extend 10) _let_0) (_ bv228 11)) (bvsle ((_ sign_extend 14) _let_10) _let_19)) _let_23))) (and (bvsge (ite (bvsgt v1 v1) (_ bv1 1) (_ bv0 1)) _let_0) (bvugt _let_12 ((_ sign_extend 8) _let_7))))) (not (= _let_8 (_ bv0 15)))) (not (= _let_8 (bvnot (_ bv0 15))))) (not (= _let_2 (_ bv0 11)))) (not (= _let_2 (bvnot (_ bv0 11))))) (not (= _let_6 (_ bv0 3)))))))))))))))))))))))))))))) ))
