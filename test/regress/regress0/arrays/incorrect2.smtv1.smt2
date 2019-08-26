(set-option :incremental false)
(meta-info :status unsat)
(set-logic QF_AUF)
(declare-sort Index 0)
(declare-sort Element 0)
(declare-fun v0 () (Array Index Element))
(declare-fun v1 () Index)
(declare-fun v2 () Index)
(declare-fun v3 () Index)
(declare-fun v4 () Index)
(declare-fun v5 () Element)
(check-sat-assuming ( (let ((_let_0 (distinct v4 v3))) (let ((_let_1 (distinct v4 v4))) (let ((_let_2 (distinct v1 v2))) (let ((_let_3 (= v5 v5))) (let ((_let_4 (ite (= v2 v3) (ite (distinct v3 v2) v0 v0) (ite (distinct v3 v2) v0 v0)))) (let ((_let_5 (ite _let_2 (ite (= v3 v3) (ite (= v4 v2) v0 (ite (distinct v3 v2) v0 v0)) _let_4) (ite (= v3 v3) (ite (= v4 v2) v0 (ite (distinct v3 v2) v0 v0)) _let_4)))) (let ((_let_6 (ite (distinct v3 v2) _let_4 (ite (= v3 v3) (ite (= v4 v2) v0 (ite (distinct v3 v2) v0 v0)) _let_4)))) (let ((_let_7 (ite _let_3 (ite (= v3 v3) (ite (= v4 v2) v0 (ite (distinct v3 v2) v0 v0)) _let_4) _let_4))) (let ((_let_8 (ite (distinct v3 v2) (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4)) (ite (distinct v3 v2) v0 v0)))) (let ((_let_9 (ite _let_0 (ite (distinct v3 v2) v0 v0) (ite (distinct v3 v2) v0 v0)))) (let ((_let_10 (ite (= v2 v3) _let_4 (ite (= v3 v3) (ite (= v4 v2) v0 (ite (distinct v3 v2) v0 v0)) _let_4)))) (let ((_let_11 (ite (distinct v2 v2) _let_7 (ite (distinct v4 v2) _let_4 (ite (distinct v3 v2) v0 v0))))) (let ((_let_12 (ite (distinct v2 v2) v3 v4))) (let ((_let_13 (ite (= v2 v3) _let_12 v4))) (let ((_let_14 (ite (= v2 v3) _let_13 v1))) (let ((_let_15 (ite _let_3 (ite _let_2 v3 (ite _let_1 v1 v2)) v2))) (let ((_let_16 (ite (= v2 v3) (ite _let_2 v3 (ite _let_1 v1 v2)) (ite _let_1 v1 v2)))) (let ((_let_17 (ite (distinct v3 v2) _let_15 v4))) (let ((_let_18 (ite (= v3 v3) _let_13 _let_12))) (let ((_let_19 (ite (distinct v4 v2) _let_13 v1))) (let ((_let_20 (ite (= v4 v2) _let_13 v2))) (let ((_let_21 (ite (= v0 v0) _let_17 _let_15))) (let ((_let_22 (ite _let_0 _let_20 v3))) (let ((_let_23 (ite (distinct v2 v2) v5 v5))) (let ((_let_24 (ite (= v2 v3) v5 v5))) (let ((_let_25 (ite (= v4 v2) v5 _let_24))) (let ((_let_26 (ite (distinct v3 v2) _let_24 _let_25))) (let ((_let_27 (ite (distinct v4 v2) _let_24 v5))) (let ((_let_28 (ite (= v0 v0) v5 _let_25))) (let ((_let_29 (ite (distinct v3 v2) _let_25 _let_27))) (let ((_let_30 (ite (distinct v3 v2) (ite _let_2 (ite (= v0 v0) _let_24 (ite _let_0 _let_23 _let_24)) _let_23) (ite _let_1 _let_24 _let_23)))) (let ((_let_31 (ite _let_1 _let_23 (ite _let_1 _let_24 _let_23)))) (let ((_let_32 (ite (= v0 v0) (ite (= v0 v0) _let_24 (ite _let_0 _let_23 _let_24)) (ite _let_2 (ite _let_1 _let_24 _let_23) v5)))) (let ((_let_33 (ite (= v2 v3) (ite _let_2 (ite _let_1 _let_24 _let_23) v5) _let_27))) (let ((_let_34 (and (not (xor (= (distinct _let_4 v0) (distinct _let_5 (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4)))) (and (distinct v3 v2) (= (ite _let_2 (ite (= v0 v0) _let_24 (ite _let_0 _let_23 _let_24)) _let_23) (ite _let_0 _let_23 _let_24))))) (and (not (= (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4)) _let_11)) (distinct (ite _let_3 _let_24 _let_27) _let_27))))) (let ((_let_35 (=> (not (and (= (ite (distinct v3 v2) _let_21 (ite _let_2 v3 (ite _let_1 v1 v2))) (ite _let_2 v3 (ite _let_1 v1 v2))) (distinct _let_5 (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4))))) (not (ite (or (= (ite (distinct v4 v2) _let_4 (ite (distinct v3 v2) v0 v0)) (ite _let_1 _let_4 v0)) (= _let_13 _let_13)) (not (= _let_7 (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4)))) (distinct _let_13 _let_13)))))) (let ((_let_36 (= (or (= _let_8 _let_9) (not (and (= (distinct _let_13 (ite _let_2 v3 (ite _let_1 v1 v2))) (= _let_14 _let_18)) (xor (= _let_31 (ite (= v3 v3) (ite (= v2 v3) (ite _let_2 (ite _let_1 _let_24 _let_23) v5) _let_28) _let_27)) (= (ite (= v0 v0) (ite (distinct v3 v2) v0 v0) (ite (distinct v3 v2) v0 v0)) _let_9))))) (=> (xor (=> (distinct _let_33 _let_29) (=> (distinct (ite (distinct v3 v2) v0 v0) _let_9) (= (distinct _let_32 (ite _let_0 _let_26 _let_30)) (= _let_8 _let_4)))) (distinct _let_15 _let_14)) (and (distinct _let_9 (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4))) (distinct (ite (distinct v3 v2) v0 v0) (ite _let_1 _let_4 v0))))))) (xor (not (not (not (and (and (and (= _let_16 _let_12) (xor (= _let_15 v4) (=> (= (= _let_5 (ite (distinct v3 v2) v0 v0)) (distinct _let_27 (ite _let_1 _let_24 _let_23))) (or (= _let_6 _let_10) (or (distinct v4 (ite (distinct v3 v2) _let_21 (ite _let_2 v3 (ite _let_1 v1 v2)))) (distinct v0 (ite (distinct v4 v2) _let_4 (ite (distinct v3 v2) v0 v0)))))))) (ite (distinct _let_13 (ite (distinct v3 v2) _let_21 (ite _let_2 v3 (ite _let_1 v1 v2)))) (distinct (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4)) _let_8) (not _let_2))) (=> (not (= (ite _let_0 _let_26 _let_30) (ite _let_2 (ite (= v0 v0) _let_24 (ite _let_0 _let_23 _let_24)) _let_23))) (not (= _let_19 v2))))))) (=> (=> (=> (= _let_13 _let_18) (ite (distinct v0 (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4))) (= _let_19 _let_21) (= (ite (= v3 v3) (ite (= v4 v2) v0 (ite (distinct v3 v2) v0 v0)) _let_4) _let_6))) (= (=> (=> (distinct v3 (ite _let_1 v1 v2)) (not (= v2 _let_14))) (not (and (distinct _let_19 (ite _let_2 v3 (ite _let_1 v1 v2))) (and (= _let_21 _let_14) (= _let_5 (ite (distinct v4 v2) _let_4 (ite (distinct v3 v2) v0 v0))))))) (xor (= (= _let_19 _let_18) (and (= _let_21 _let_20) (= _let_11 _let_10))) (=> (= _let_22 _let_20) (distinct _let_22 _let_20))))) (= (or (= (ite (=> (= (ite (distinct v3 v2) _let_21 (ite _let_2 v3 (ite _let_1 v1 v2))) v2) (= (ite (distinct v4 v2) _let_4 (ite (distinct v3 v2) v0 v0)) _let_10)) (not (= (distinct (ite _let_1 _let_24 _let_23) _let_28) (= (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) _let_11))) _let_0) (xor (distinct _let_20 _let_20) (ite (= (ite (= v2 v3) (ite _let_2 (ite _let_1 _let_24 _let_23) v5) _let_28) (ite _let_1 _let_24 _let_23)) (= (ite (distinct v4 v2) _let_4 (ite (distinct v3 v2) v0 v0)) (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4))) (or (= _let_11 _let_6) (= v4 v2))))) (and (and (= (= (= _let_16 _let_20) (= (ite (distinct v3 v2) v0 v0) (ite _let_1 _let_4 v0))) (and (= v0 _let_4) (or (= (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4)) _let_9) (or (distinct _let_14 _let_18) (= v2 v3))))) (not (not (xor (ite (and (distinct (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4)) _let_11) (= (ite _let_1 _let_24 _let_23) (ite _let_1 _let_24 _let_23))) (and (=> (distinct _let_8 (ite (= v3 v3) (ite (= v4 v2) v0 (ite (distinct v3 v2) v0 v0)) _let_4)) (= v3 v3)) (not (or (= (ite (= v0 v0) (ite (distinct v3 v2) v0 v0) (ite (distinct v3 v2) v0 v0)) (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4))) (distinct _let_6 _let_4)))) (distinct v1 v3)) (=> (=> (=> (distinct (ite _let_0 _let_26 _let_30) _let_25) (distinct _let_18 _let_15)) (xor (or (= _let_24 (ite _let_0 _let_23 _let_24)) (= _let_4 _let_8)) (distinct v4 v2))) (=> (not (distinct (ite (distinct v4 v2) _let_4 (ite (distinct v3 v2) v0 v0)) v0)) _let_1)))))) (=> _let_36 _let_36))) (= (= _let_34 _let_34) (or (and _let_35 _let_35) (=> (and (not (ite (= (ite _let_2 (ite _let_1 _let_24 _let_23) v5) _let_29) (ite (= (distinct _let_9 _let_9) (ite (distinct (ite (distinct v4 v2) _let_4 (ite (distinct v3 v2) v0 v0)) _let_7) (= _let_17 _let_18) (distinct _let_23 (ite _let_1 _let_24 _let_23)))) (or (= _let_9 _let_10) (= v0 v0)) (and (not (= (ite (distinct v4 v2) _let_4 (ite (distinct v3 v2) v0 v0)) (ite (distinct v4 v2) _let_4 (ite (distinct v3 v2) v0 v0)))) (xor (= _let_12 v1) (xor (distinct _let_11 _let_8) (= v2 _let_12))))) (or (distinct _let_33 (ite _let_2 (ite (= v0 v0) _let_24 (ite _let_0 _let_23 _let_24)) _let_23)) (= (= _let_4 (ite (distinct v4 v2) _let_4 (ite (distinct v3 v2) v0 v0))) (= _let_10 _let_4))))) (ite (or (ite (or (= (=> (or (not (distinct v3 v2)) (or (distinct _let_32 _let_31) (= (ite (= v3 v3) (ite (= v4 v2) v0 (ite (distinct v3 v2) v0 v0)) _let_4) (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4))))) (not (or (distinct (ite (= v0 v0) _let_24 (ite _let_0 _let_23 _let_24)) _let_32) (= (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4)) v0)))) (not (distinct (ite _let_2 v3 (ite _let_1 v1 v2)) v3))) (xor (ite (ite (xor (distinct (ite _let_0 _let_23 _let_24) _let_26) (distinct _let_12 v1)) (distinct v2 v2) (distinct (ite (distinct v4 v2) _let_4 (ite (distinct v3 v2) v0 v0)) _let_4)) (=> (= _let_30 _let_23) (distinct _let_4 (ite (distinct v3 v2) v0 v0))) (= _let_21 v1)) (and (= (ite (= v4 v2) v0 (ite (distinct v3 v2) v0 v0)) (ite (= v3 v3) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4) (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4))) (= v4 v4)))) (= _let_31 _let_23) (or (xor (and (= _let_6 _let_8) (xor (distinct v5 _let_30) (= _let_12 _let_12))) (distinct _let_5 _let_8)) (xor (= (ite _let_1 _let_24 _let_23) _let_28) (distinct _let_11 _let_7)))) (distinct v0 _let_11)) (xor (=> (= v0 _let_11) (= v2 v3)) (distinct _let_10 (ite _let_1 (ite (distinct v3 v2) v0 v0) _let_4))) (or (= (xor (= _let_5 _let_10) (distinct _let_11 (ite (= v3 v3) (ite (= v4 v2) v0 (ite (distinct v3 v2) v0 v0)) _let_4))) (and (xor (= (= v1 _let_15) (= _let_30 _let_30)) (not (distinct _let_15 _let_18))) (xor (= _let_10 v0) (distinct _let_24 (select _let_7 _let_14))))) (= _let_30 _let_25)))) (=> (=> (distinct (ite (= v3 v3) (ite (= v4 v2) v0 (ite (distinct v3 v2) v0 v0)) _let_4) _let_7) _let_3) (or (= (=> (not (distinct (ite (= v3 v3) (ite (= v4 v2) v0 (ite (distinct v3 v2) v0 v0)) _let_4) _let_8)) (= (ite (distinct v3 v2) v0 v0) _let_7)) _let_3) (or (= v2 _let_15) (= _let_9 _let_7))))))))))))))))))))))))))))))))))))))))))))))) ))
