(set-option :incremental false)
(set-info :status unknown)
(set-logic QF_BV)
(declare-fun v0 () (_ BitVec 10))
(check-sat-assuming ( (let ((_let_0 (ite (bvule (_ bv30369 16) (_ bv30369 16)) (_ bv1 1) (_ bv0 1)))) (let ((_let_1 (bvcomp ((_ zero_extend 1) _let_0) (_ bv3 2)))) (let ((_let_2 ((_ sign_extend 15) _let_1))) (let ((_let_3 (bvsmod (_ bv30369 16) (_ bv30369 16)))) (let ((_let_4 ((_ sign_extend 15) _let_0))) (let ((_let_5 (or (xor (bvsgt _let_4 _let_3) (bvuge (_ bv30369 16) ((_ sign_extend 15) (ite (bvsgt _let_2 (_ bv30369 16)) (_ bv1 1) (_ bv0 1))))) (bvslt ((_ sign_extend 14) (_ bv3 2)) _let_3)))) (let ((_let_6 (not (xor (bvsge (bvsdiv v0 v0) ((_ zero_extend 9) _let_0)) (bvsge (bvsdiv v0 v0) ((_ zero_extend 9) _let_0)))))) (and (and (and (and (= (or (or (bvsgt v0 ((_ sign_extend 9) _let_0)) (bvsgt (_ bv3 2) ((_ sign_extend 1) _let_0))) (=> (bvslt ((_ zero_extend 9) _let_0) (bvsdiv v0 v0)) (bvult (_ bv3 2) (_ bv3 2)))) (= (and _let_6 _let_6) (= (= (xor (and _let_5 _let_5) (xor (bvslt _let_3 ((_ zero_extend 15) _let_1)) (bvugt _let_2 _let_3))) (not (distinct ((_ zero_extend 9) _let_1) v0))) (xor (= (not (and (bvsle (_ bv30369 16) ((_ zero_extend 14) (_ bv3 2))) (bvslt _let_0 _let_1))) (= ((_ zero_extend 8) (_ bv3 2)) v0)) (and (not (bvsgt _let_1 _let_1)) (= _let_3 _let_4)))))) (not (= v0 (_ bv0 10)))) (not (= v0 (bvnot (_ bv0 10))))) (not (= (_ bv30369 16) (_ bv0 16)))) (not (= (_ bv30369 16) (bvnot (_ bv0 16)))))))))))) ))
