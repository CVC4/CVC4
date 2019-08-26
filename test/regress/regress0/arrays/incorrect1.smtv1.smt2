(set-option :incremental false)
(meta-info :status unsat)
(set-logic QF_AUF)
(declare-sort Index 0)
(declare-sort Element 0)
(declare-fun v0 () (Array Index Element))
(declare-fun v1 () Index)
(declare-fun v2 () Index)
(declare-fun v3 () Index)
(declare-fun v4 () Element)
(check-sat-assuming ( (let ((_let_0 (distinct v0 v0))) (let ((_let_1 (= v3 v2))) (let ((_let_2 (= v1 v2))) (let ((_let_3 (ite _let_1 v0 v0))) (let ((_let_4 (ite (distinct v4 v4) _let_3 v0))) (let ((_let_5 (ite (distinct v4 v4) _let_3 _let_3))) (let ((_let_6 (ite _let_2 v0 _let_5))) (let ((_let_7 (ite _let_0 v3 v1))) (let ((_let_8 (ite _let_1 v3 v1))) (let ((_let_9 (ite (distinct v4 v4) (ite _let_1 (ite (distinct v4 v4) v2 v2) (ite (distinct v4 v4) v2 v2)) _let_8))) (let ((_let_10 (ite (distinct v4 v4) (ite (distinct v4 v4) v2 v2) (ite (distinct v4 v4) v2 v2)))) (let ((_let_11 (ite _let_1 (ite (distinct v4 v4) v2 v2) _let_9))) (let ((_let_12 (ite _let_0 v4 v4))) (let ((_let_13 (distinct v2 (ite _let_1 v1 (ite _let_1 v3 _let_10))))) (let ((_let_14 (= (xor (= (= (distinct _let_9 _let_7) (and (= v2 _let_10) (xor (= (ite _let_2 v4 _let_12) (ite _let_2 v4 _let_12)) (or (= _let_11 v2) (distinct v2 (ite _let_1 v3 _let_10)))))) (= (distinct _let_9 _let_7) (and (= v2 _let_10) (xor (= (ite _let_2 v4 _let_12) (ite _let_2 v4 _let_12)) (or (= _let_11 v2) (distinct v2 (ite _let_1 v3 _let_10))))))) (ite (=> (distinct v4 v4) (= _let_5 _let_4)) (distinct _let_8 v1) (and (xor (distinct _let_7 _let_9) (distinct _let_5 (ite _let_0 _let_4 _let_5))) (= (distinct _let_9 (ite _let_2 v3 (ite (distinct v4 v4) v2 v2))) (distinct _let_10 _let_8))))) (or (not (xor (= _let_10 _let_8) (or (=> (distinct _let_10 _let_10) (= v0 _let_3)) (= v3 _let_10)))) (not (distinct _let_12 v4)))))) (let ((_let_15 (and (ite (=> (ite (and (distinct (ite _let_1 v3 _let_10) v1) (= _let_5 _let_4)) (distinct (select _let_6 _let_9) (ite _let_2 v4 _let_12)) (and (= v2 v2) (and (= _let_1 (= _let_5 _let_3)) (ite (and (distinct _let_4 _let_5) (distinct v0 _let_6)) (ite (distinct _let_6 _let_3) (distinct _let_5 _let_3) (= _let_4 v0)) (and (distinct _let_4 _let_5) (distinct v0 _let_6)))))) _let_2) (ite (ite (= _let_11 (ite _let_1 v1 (ite _let_1 v3 _let_10))) (or (distinct v1 _let_11) (= v1 _let_10)) (and (or (= (= (select _let_6 _let_9) v4) (distinct _let_10 (ite _let_1 (ite (distinct v4 v4) v2 v2) (ite (distinct v4 v4) v2 v2)))) (= (ite _let_2 v4 _let_12) (ite (distinct v4 v4) v4 v4))) (xor (and (= _let_7 _let_11) (= (ite (distinct v4 v4) v2 v2) (ite _let_1 v1 (ite _let_1 v3 _let_10)))) (distinct _let_3 v0)))) (or _let_0 (ite (or (not (distinct (ite _let_1 _let_12 _let_12) (ite _let_1 _let_12 _let_12))) (= (ite _let_1 v1 (ite _let_1 v3 _let_10)) _let_11)) (xor (distinct _let_10 _let_11) (distinct _let_10 _let_11)) (distinct _let_5 _let_6))) (distinct _let_9 _let_11)) (ite (ite (= _let_11 (ite _let_1 v1 (ite _let_1 v3 _let_10))) (or (distinct v1 _let_11) (= v1 _let_10)) (and (or (= (= (select _let_6 _let_9) v4) (distinct _let_10 (ite _let_1 (ite (distinct v4 v4) v2 v2) (ite (distinct v4 v4) v2 v2)))) (= (ite _let_2 v4 _let_12) (ite (distinct v4 v4) v4 v4))) (xor (and (= _let_7 _let_11) (= (ite (distinct v4 v4) v2 v2) (ite _let_1 v1 (ite _let_1 v3 _let_10)))) (distinct _let_3 v0)))) (or _let_0 (ite (or (not (distinct (ite _let_1 _let_12 _let_12) (ite _let_1 _let_12 _let_12))) (= (ite _let_1 v1 (ite _let_1 v3 _let_10)) _let_11)) (xor (distinct _let_10 _let_11) (distinct _let_10 _let_11)) (distinct _let_5 _let_6))) (distinct _let_9 _let_11))) (ite (=> (ite (and (distinct (ite _let_1 v3 _let_10) v1) (= _let_5 _let_4)) (distinct (select _let_6 _let_9) (ite _let_2 v4 _let_12)) (and (= v2 v2) (and (= _let_1 (= _let_5 _let_3)) (ite (and (distinct _let_4 _let_5) (distinct v0 _let_6)) (ite (distinct _let_6 _let_3) (distinct _let_5 _let_3) (= _let_4 v0)) (and (distinct _let_4 _let_5) (distinct v0 _let_6)))))) _let_2) (ite (ite (= _let_11 (ite _let_1 v1 (ite _let_1 v3 _let_10))) (or (distinct v1 _let_11) (= v1 _let_10)) (and (or (= (= (select _let_6 _let_9) v4) (distinct _let_10 (ite _let_1 (ite (distinct v4 v4) v2 v2) (ite (distinct v4 v4) v2 v2)))) (= (ite _let_2 v4 _let_12) (ite (distinct v4 v4) v4 v4))) (xor (and (= _let_7 _let_11) (= (ite (distinct v4 v4) v2 v2) (ite _let_1 v1 (ite _let_1 v3 _let_10)))) (distinct _let_3 v0)))) (or _let_0 (ite (or (not (distinct (ite _let_1 _let_12 _let_12) (ite _let_1 _let_12 _let_12))) (= (ite _let_1 v1 (ite _let_1 v3 _let_10)) _let_11)) (xor (distinct _let_10 _let_11) (distinct _let_10 _let_11)) (distinct _let_5 _let_6))) (distinct _let_9 _let_11)) (ite (ite (= _let_11 (ite _let_1 v1 (ite _let_1 v3 _let_10))) (or (distinct v1 _let_11) (= v1 _let_10)) (and (or (= (= (select _let_6 _let_9) v4) (distinct _let_10 (ite _let_1 (ite (distinct v4 v4) v2 v2) (ite (distinct v4 v4) v2 v2)))) (= (ite _let_2 v4 _let_12) (ite (distinct v4 v4) v4 v4))) (xor (and (= _let_7 _let_11) (= (ite (distinct v4 v4) v2 v2) (ite _let_1 v1 (ite _let_1 v3 _let_10)))) (distinct _let_3 v0)))) (or _let_0 (ite (or (not (distinct (ite _let_1 _let_12 _let_12) (ite _let_1 _let_12 _let_12))) (= (ite _let_1 v1 (ite _let_1 v3 _let_10)) _let_11)) (xor (distinct _let_10 _let_11) (distinct _let_10 _let_11)) (distinct _let_5 _let_6))) (distinct _let_9 _let_11)))))) (and (or _let_15 _let_15) (not (and (and (=> (= _let_13 _let_13) (distinct (ite (distinct v4 v4) v2 v2) v3)) (= _let_14 _let_14)) (not (not (distinct _let_8 _let_11)))))))))))))))))))))) ))
