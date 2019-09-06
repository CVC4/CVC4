(set-option :incremental false)
(set-info :status unsat)
(set-logic QF_AUF)
(declare-sort Index 0)
(declare-sort Element 0)
(declare-fun v0 () (Array Index Element))
(declare-fun v1 () Index)
(declare-fun v2 () Index)
(declare-fun v3 () Index)
(declare-fun v4 () Element)
(declare-fun v5 () Element)
(declare-fun v6 () Element)
(check-sat-assuming ( (let ((_let_0 (distinct v3 v2))) (let ((_let_1 (= v6 v4))) (let ((_let_2 (ite (= v1 v2) v0 v0))) (let ((_let_3 (ite _let_0 _let_2 (ite (= v0 v0) v0 v0)))) (let ((_let_4 (ite (distinct v5 v5) (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0)) (ite (= v1 v2) _let_2 (ite (= v0 v0) v0 v0))))) (let ((_let_5 (ite (= v2 v2) _let_4 _let_3))) (let ((_let_6 (ite _let_1 _let_4 _let_3))) (let ((_let_7 (ite _let_1 v2 v3))) (let ((_let_8 (ite _let_0 v2 (ite (distinct v1 v2) v3 v1)))) (let ((_let_9 (ite (= v0 v0) v3 (ite (distinct v5 v5) v2 (ite (distinct v1 v2) v3 v1))))) (let ((_let_10 (ite (= v2 v2) v5 v5))) (let ((_let_11 (ite _let_0 v6 _let_10))) (let ((_let_12 (ite (= v0 v0) (ite (distinct v1 v2) v4 _let_10) v4))) (let ((_let_13 (ite (distinct v1 v2) _let_10 _let_11))) (let ((_let_14 (ite (= v1 v2) v5 _let_11))) (let ((_let_15 (ite (distinct v5 v5) _let_10 v4))) (let ((_let_16 (ite _let_0 v6 v4))) (let ((_let_17 (ite (distinct v5 v5) _let_11 _let_12))) (let ((_let_18 (ite (= v2 v2) v4 v6))) (let ((_let_19 (select (store (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0)) v2 _let_12) v2))) (let ((_let_20 (= (ite (= v2 v2) _let_8 (ite (distinct v1 v2) v3 v1)) _let_9))) (let ((_let_21 (distinct _let_15 v5))) (let ((_let_22 (and (xor (and (= _let_2 _let_6) (= (ite (distinct v1 v2) _let_3 v0) v0)) (ite (or (or (=> (= _let_19 v4) (distinct _let_19 v6)) (distinct _let_10 _let_16)) (or (not (distinct (ite (= v1 v2) _let_2 (ite (= v0 v0) v0 v0)) _let_2)) (not (xor (distinct v1 v2) (= _let_13 (ite _let_1 _let_11 _let_12)))))) (or (or (=> (= _let_19 v4) (distinct _let_19 v6)) (distinct _let_10 _let_16)) (or (not (distinct (ite (= v1 v2) _let_2 (ite (= v0 v0) v0 v0)) _let_2)) (not (xor (distinct v1 v2) (= _let_13 (ite _let_1 _let_11 _let_12)))))) (=> (ite (= _let_12 _let_17) (not (=> (not (and (distinct _let_10 v4) (ite (= _let_19 (ite (distinct v1 v2) v4 _let_10)) _let_1 (=> (not (distinct _let_11 _let_10)) (= v1 v2))))) (and (xor (= _let_3 (ite (= v1 v2) _let_2 (ite (= v0 v0) v0 v0))) (distinct _let_18 _let_17)) (distinct (ite (= v0 v0) v0 v0) _let_6)))) (not (= v1 _let_7))) (xor (distinct (ite (distinct v1 v2) v3 v1) v2) (= v6 _let_16))))) (or (=> (not (= (and (= _let_15 _let_19) (= _let_2 _let_6)) (xor (distinct v0 _let_4) (= _let_14 _let_16)))) (or (or (=> (= (ite (= v0 v0) v0 v0) (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0))) (= _let_4 _let_5)) (=> (= (ite (= v0 v0) v0 v0) (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0))) (= _let_4 _let_5))) (or (= v2 v2) (distinct v3 (ite (distinct v5 v5) v2 (ite (distinct v1 v2) v3 v1)))))) (=> (not (= (and (= _let_15 _let_19) (= _let_2 _let_6)) (xor (distinct v0 _let_4) (= _let_14 _let_16)))) (or (or (=> (= (ite (= v0 v0) v0 v0) (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0))) (= _let_4 _let_5)) (=> (= (ite (= v0 v0) v0 v0) (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0))) (= _let_4 _let_5))) (or (= v2 v2) (distinct v3 (ite (distinct v5 v5) v2 (ite (distinct v1 v2) v3 v1)))))))))) (xor (xor _let_22 _let_22) (and (ite _let_20 (xor (not (ite (or (= _let_21 (distinct _let_10 _let_15)) (distinct _let_15 _let_12)) (= _let_18 _let_18) (xor (ite (= (ite (= v1 v2) _let_2 (ite (= v0 v0) v0 v0)) _let_4) (= _let_3 (ite (= v0 v0) v0 v0)) (xor (distinct _let_14 _let_10) (= (ite (distinct v1 v2) v3 v1) _let_8))) (or (= (store (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0)) v2 _let_12) _let_2) (= (= v0 v0) (= (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0)) _let_6)))))) (= (xor _let_21 (xor (distinct _let_6 _let_6) (= (ite (= v1 v2) _let_9 v1) _let_9))) (= _let_13 _let_17))) _let_20) (not (or (ite (distinct v5 v5) (distinct _let_4 _let_5) (= (=> (distinct _let_5 (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0))) (ite (distinct _let_7 _let_7) (distinct (store (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0)) v2 _let_12) (store (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0)) v2 _let_12)) (= v5 _let_18))) (and (= _let_2 _let_5) (= (= v4 _let_10) (or (= (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0)) (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0))) (= _let_6 _let_4)))))) (=> (distinct v6 _let_18) (ite (and (distinct (ite (= v2 v2) (ite (= v0 v0) v0 v0) (ite (= v0 v0) v0 v0)) _let_2) (distinct v4 _let_13)) (or (distinct _let_9 _let_9) _let_0) (ite (distinct _let_19 _let_10) (= _let_19 _let_12) (distinct _let_16 _let_11))))))))))))))))))))))))))))))) ))
