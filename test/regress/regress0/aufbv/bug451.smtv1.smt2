(set-option :incremental false)
(meta-info :status unsat)
(set-logic QF_AUFBV)
(declare-fun v0 () (_ BitVec 15))
(declare-fun a1 () (Array (_ BitVec 9) (_ BitVec 14)))
(check-sat-assuming ( (let ((_let_0 (bvxor (_ bv37005 16) (_ bv37005 16)))) (let ((_let_1 (bvmul ((_ zero_extend 15) (ite (= (_ bv37005 16) (_ bv37005 16)) (_ bv1 1) (_ bv0 1))) (_ bv37005 16)))) (let ((_let_2 (ite (bvuge _let_1 ((_ sign_extend 1) v0)) (_ bv1 1) (_ bv0 1)))) (let ((_let_3 (ite (= ((_ sign_extend 14) (ite (bvugt v0 ((_ zero_extend 4) (_ bv1274 11))) (_ bv1 1) (_ bv0 1))) v0) (_ bv1 1) (_ bv0 1)))) (let ((_let_4 ((_ sign_extend 2) (bvnand (select a1 ((_ extract 8 0) v0)) ((_ sign_extend 3) (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2))))))) (let ((_let_5 ((_ zero_extend 2) (bvnand (select a1 ((_ extract 8 0) v0)) ((_ sign_extend 3) (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2))))))) (let ((_let_6 ((_ sign_extend 5) (_ bv1274 11)))) (let ((_let_7 (and (ite (or (xor (xor (bvule _let_1 ((_ zero_extend 5) (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2)))) (bvuge (select a1 ((_ extract 8 0) v0)) ((_ sign_extend 13) _let_3))) (bvsgt (_ bv37005 16) ((_ sign_extend 15) (ite (= (_ bv37005 16) (_ bv37005 16)) (_ bv1 1) (_ bv0 1))))) (xor (xor (bvule _let_1 ((_ zero_extend 5) (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2)))) (bvuge (select a1 ((_ extract 8 0) v0)) ((_ sign_extend 13) _let_3))) (bvsgt (_ bv37005 16) ((_ sign_extend 15) (ite (= (_ bv37005 16) (_ bv37005 16)) (_ bv1 1) (_ bv0 1)))))) (=> (= (not (bvule _let_1 _let_5)) (bvsgt v0 ((_ sign_extend 14) (ite (= (_ bv37005 16) (_ bv37005 16)) (_ bv1 1) (_ bv0 1))))) (or (= (or (= _let_1 _let_5) (and (bvsle _let_1 ((_ sign_extend 15) (ite (bvugt v0 ((_ zero_extend 4) (_ bv1274 11))) (_ bv1 1) (_ bv0 1)))) (bvsge v0 ((_ zero_extend 4) (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2)))))) (bvuge ((_ zero_extend 14) (ite (bvugt v0 ((_ zero_extend 4) (_ bv1274 11))) (_ bv1 1) (_ bv0 1))) v0)) (=> (not (bvult (bvnand (select a1 ((_ extract 8 0) v0)) ((_ sign_extend 3) (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2)))) (bvnand (select a1 ((_ extract 8 0) v0)) ((_ sign_extend 3) (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2)))))) (not (bvult (bvnand (select a1 ((_ extract 8 0) v0)) ((_ sign_extend 3) (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2)))) (bvnand (select a1 ((_ extract 8 0) v0)) ((_ sign_extend 3) (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2))))))))) (or (xor (xor (bvule _let_1 ((_ zero_extend 5) (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2)))) (bvuge (select a1 ((_ extract 8 0) v0)) ((_ sign_extend 13) _let_3))) (bvsgt (_ bv37005 16) ((_ sign_extend 15) (ite (= (_ bv37005 16) (_ bv37005 16)) (_ bv1 1) (_ bv0 1))))) (xor (xor (bvule _let_1 ((_ zero_extend 5) (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2)))) (bvuge (select a1 ((_ extract 8 0) v0)) ((_ sign_extend 13) _let_3))) (bvsgt (_ bv37005 16) ((_ sign_extend 15) (ite (= (_ bv37005 16) (_ bv37005 16)) (_ bv1 1) (_ bv0 1))))))) (=> (= (= (or (bvsle _let_4 _let_0) (distinct _let_4 _let_0)) (bvsge (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2)) ((_ zero_extend 10) _let_2))) (xor (bvsgt (_ bv1274 11) ((_ zero_extend 10) _let_3)) (bvsge (_ bv37005 16) _let_6))) (bvule (bvnand (select a1 ((_ extract 8 0) v0)) ((_ sign_extend 3) (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2)))) (bvnand (select a1 ((_ extract 8 0) v0)) ((_ sign_extend 3) (bvashr (_ bv1274 11) ((_ sign_extend 10) _let_2))))))))) (and (not (xor _let_7 _let_7)) (ite (distinct _let_1 _let_1) (bvule _let_1 _let_6) (= ((_ sign_extend 14) _let_2) v0))))))))))) ))
