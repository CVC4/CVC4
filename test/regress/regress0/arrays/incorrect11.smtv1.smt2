(set-option :incremental false)
(meta-info :status unsat)
(set-logic QF_AUF)
(declare-sort Index 0)
(declare-sort Element 0)
(declare-fun v0 () (Array Index Element))
(declare-fun v1 () Index)
(declare-fun v2 () Element)
(declare-fun v3 () Element)
(declare-fun v4 () Element)
(check-sat-assuming ( (let ((_let_0 (= v2 v3))) (let ((_let_1 (ite (distinct v2 v4) v1 v1))) (let ((_let_2 (ite (distinct v0 v0) _let_1 v1))) (let ((_let_3 (ite _let_0 v1 _let_2))) (let ((_let_4 (ite _let_0 v4 v2))) (let ((_let_5 (ite (distinct v1 v1) v3 v3))) (let ((_let_6 (select (ite (distinct v1 v1) (ite (distinct v0 v0) v0 v0) v0) v1))) (let ((_let_7 (distinct v3 v3))) (let ((_let_8 (ite (xor (= v3 v2) (= _let_5 (ite (distinct v2 v4) _let_4 _let_4))) _let_0 (distinct (ite (distinct v1 v1) (ite (distinct v0 v0) v0 v0) v0) (ite (distinct v2 v4) v0 (ite _let_0 v0 v0)))))) (let ((_let_9 (and (and (= (ite (distinct v0 v0) v3 v3) v2) (distinct v2 v4)) (and (= (ite (distinct v0 v0) v3 v3) v2) (distinct v2 v4))))) (let ((_let_10 (ite (xor (ite (xor (and (and (= v4 v4) (= (ite (distinct v2 v4) _let_4 _let_4) _let_5)) (or (= v3 v3) (distinct v1 _let_2))) _let_7) (xor (and (and (= v4 v4) (= (ite (distinct v2 v4) _let_4 _let_4) _let_5)) (or (= v3 v3) (distinct v1 _let_2))) _let_7) (xor (and (and (= v4 v4) (= (ite (distinct v2 v4) _let_4 _let_4) _let_5)) (or (= v3 v3) (distinct v1 _let_2))) _let_7)) (distinct v0 v0)) (or (distinct v3 (ite (distinct v0 v0) v3 v3)) (xor (= (= _let_2 _let_3) (distinct v2 v4)) (xor (= (ite (distinct v2 v4) v0 (ite _let_0 v0 v0)) v0) (= _let_5 _let_4)))) (xor (= (not (distinct v4 _let_4)) (xor (distinct (ite _let_0 v0 v0) (ite (distinct v2 v4) (ite _let_0 v0 v0) (ite _let_0 v0 v0))) (=> (= (ite (distinct v1 v1) _let_1 _let_2) v1) (= (ite (distinct v2 v4) _let_4 _let_4) _let_4)))) (= (not (distinct v4 _let_4)) (xor (distinct (ite _let_0 v0 v0) (ite (distinct v2 v4) (ite _let_0 v0 v0) (ite _let_0 v0 v0))) (=> (= (ite (distinct v1 v1) _let_1 _let_2) v1) (= (ite (distinct v2 v4) _let_4 _let_4) _let_4)))))))) (and (= (ite _let_10 _let_10 (not (ite (xor (distinct (ite _let_0 v0 v0) (ite _let_0 v0 v0)) (or _let_8 _let_8)) (distinct v2 _let_4) (and (= (ite (distinct v0 v0) v3 v3) v3) (xor (distinct _let_4 v4) (distinct v1 v1)))))) (= (= (not (= v0 (ite (distinct v0 v0) v0 v0))) (xor _let_7 (or (= _let_5 _let_5) (and (distinct _let_6 _let_6) (= _let_3 v1))))) (and (distinct _let_1 v1) (distinct v2 v2)))) (or _let_9 _let_9))))))))))))) ))
