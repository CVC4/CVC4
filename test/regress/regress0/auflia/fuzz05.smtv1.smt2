(set-option :incremental false)
(meta-info :status sat)
(set-logic QF_AUFLIA)
(declare-fun f0 (Int Int) Int)
(declare-fun f1 ((Array Int Int) (Array Int Int) (Array Int Int)) (Array Int Int))
(declare-fun p0 (Int) Bool)
(declare-fun p1 ((Array Int Int) (Array Int Int) (Array Int Int)) Bool)
(declare-fun v0 () Int)
(declare-fun v1 () Int)
(declare-fun v2 () Int)
(declare-fun v3 () (Array Int Int))
(check-sat-assuming ( (let ((_let_0 (- v2 v2))) (let ((_let_1 (ite (p0 v2) 1 0))) (let ((_let_2 (> (* (f0 v1 v0) 2) v1))) (let ((_let_3 (<= _let_1 (f0 v1 v0)))) (let ((_let_4 (p0 _let_1))) (let ((_let_5 (p0 _let_0))) (let ((_let_6 (ite _let_3 v3 (f1 v3 v3 v3)))) (let ((_let_7 (ite _let_2 (ite _let_5 _let_6 v3) _let_6))) (let ((_let_8 (ite (>= v2 v0) (f1 v3 v3 v3) (f1 v3 v3 v3)))) (let ((_let_9 (ite _let_3 v3 _let_8))) (let ((_let_10 (ite (p1 (f1 v3 v3 v3) v3 v3) _let_6 v3))) (let ((_let_11 (ite _let_5 _let_0 v0))) (let ((_let_12 (ite _let_3 v1 (f0 v1 v0)))) (let ((_let_13 (ite _let_2 (f0 v1 v0) v0))) (let ((_let_14 (ite _let_5 _let_13 _let_11))) (let ((_let_15 (ite _let_3 v1 v0))) (let ((_let_16 (ite (p1 (f1 v3 v3 v3) v3 v3) (f0 v1 v0) _let_11))) (let ((_let_17 (ite _let_4 v2 _let_16))) (let ((_let_18 (ite _let_3 _let_1 _let_1))) (let ((_let_19 (store v3 _let_15 (* (f0 v1 v0) 2)))) (let ((_let_20 (f1 (f1 v3 v3 v3) _let_9 _let_7))) (let ((_let_21 (f1 _let_7 _let_9 _let_8))) (let ((_let_22 (f1 _let_19 _let_7 _let_20))) (let ((_let_23 (ite (p0 _let_15) 1 0))) (let ((_let_24 (* (- 2) _let_17))) (let ((_let_25 (f0 _let_14 _let_23))) (let ((_let_26 (or (not (not (distinct (+ _let_11 _let_14) _let_11))) (ite (not (=> (and (= (or (<= (- _let_18) _let_11) (p0 _let_16)) (< (f0 (f0 v1 v0) (- _let_16)) _let_11)) (= (not (p0 (* 2 (* (f0 v1 v0) 2)))) (<= (f0 (ite _let_3 (* (f0 v1 v0) 2) _let_13) _let_14) _let_17))) (and (= (or (<= (- _let_18) _let_11) (p0 _let_16)) (< (f0 (f0 v1 v0) (- _let_16)) _let_11)) (= (not (p0 (* 2 (* (f0 v1 v0) 2)))) (<= (f0 (ite _let_3 (* (f0 v1 v0) 2) _let_13) _let_14) _let_17))))) (and (xor (distinct _let_18 _let_16) (not (p0 (ite _let_3 (* (f0 v1 v0) 2) _let_13)))) (or (> _let_1 _let_0) (= (p1 _let_9 _let_20 (f1 _let_7 (ite _let_4 _let_6 v3) _let_10)) _let_2))) (and (p1 _let_19 _let_22 _let_7) (ite (p1 (f1 v3 v3 v3) v3 v3) (=> (xor (and _let_4 (p1 v3 _let_7 (f1 _let_6 _let_19 (f1 (f1 v3 v3 v3) (ite _let_5 _let_6 v3) v3)))) (xor (> _let_12 (f0 (f0 v1 v0) (- _let_16))) (distinct _let_25 _let_1))) (xor (and _let_4 (p1 v3 _let_7 (f1 _let_6 _let_19 (f1 (f1 v3 v3 v3) (ite _let_5 _let_6 v3) v3)))) (xor (> _let_12 (f0 (f0 v1 v0) (- _let_16))) (distinct _let_25 _let_1)))) (and (p1 _let_22 _let_8 (f1 v3 v3 v3)) (p1 _let_22 _let_8 (f1 v3 v3 v3))))))))) (let ((_let_27 (and (not (=> (ite (= (not (>= (select _let_9 _let_17) (f0 (f0 (ite _let_3 (* (f0 v1 v0) 2) _let_13) _let_14) v0))) (ite (xor (not (p1 _let_9 _let_20 (ite _let_4 _let_6 v3))) (or (<= _let_25 (+ (select _let_9 _let_17) v2)) (ite (= (ite (p0 _let_12) 1 0) (f0 _let_1 v0)) (= (= _let_15 (+ _let_13 _let_0)) (or (< (- (ite (>= v2 v0) _let_11 _let_12)) (* (f0 v1 v0) 2)) (>= _let_14 (- (+ _let_11 _let_14) (ite _let_3 (* (f0 v1 v0) 2) _let_13))))) (distinct (- _let_16) v2)))) (distinct v2 (f0 v1 v0)) (xor (or (p1 (ite _let_5 _let_6 v3) (f1 v3 v3 v3) _let_21) (p1 (f1 v3 v3 v3) _let_19 _let_8)) (ite (p1 (f1 _let_10 _let_10 v3) (ite _let_4 _let_6 v3) _let_20) (and (p1 _let_22 (f1 (f1 v3 v3 v3) (ite _let_5 _let_6 v3) v3) (f1 _let_7 (ite _let_4 _let_6 v3) _let_10)) (<= v0 v0)) (p0 (f0 _let_1 v0)))))) (or (and (=> (p0 v2) (distinct (- (f0 _let_1 v0)) _let_24)) (or (= (f0 (f0 (ite _let_3 (* (f0 v1 v0) 2) _let_13) _let_14) v0) (- _let_16)) (p1 _let_6 (f1 v3 v3 v3) _let_6))) (or (and (= (p1 (f1 _let_10 _let_10 v3) _let_20 v3) (p1 _let_7 _let_7 _let_20)) (=> (>= v2 v0) (< (ite (>= v2 v0) _let_11 _let_12) _let_0))) (= (= (=> (= (+ _let_13 _let_0) (ite (p0 v1) 1 0)) (or (>= (+ (select _let_9 _let_17) v2) _let_23) (p1 _let_6 _let_21 _let_8))) (=> _let_5 (= _let_24 _let_18))) (xor (or (xor (<= v1 (f0 _let_16 _let_12)) (>= (+ _let_11 _let_14) (- _let_18))) (and (distinct _let_11 (* (f0 v1 v0) 2)) (p1 (f1 _let_6 _let_19 (f1 (f1 v3 v3 v3) (ite _let_5 _let_6 v3) v3)) _let_7 (f1 _let_7 (ite _let_4 _let_6 v3) _let_10)))) (> _let_13 _let_18))))) (or (and (=> (p0 v2) (distinct (- (f0 _let_1 v0)) _let_24)) (or (= (f0 (f0 (ite _let_3 (* (f0 v1 v0) 2) _let_13) _let_14) v0) (- _let_16)) (p1 _let_6 (f1 v3 v3 v3) _let_6))) (or (and (= (p1 (f1 _let_10 _let_10 v3) _let_20 v3) (p1 _let_7 _let_7 _let_20)) (=> (>= v2 v0) (< (ite (>= v2 v0) _let_11 _let_12) _let_0))) (= (= (=> (= (+ _let_13 _let_0) (ite (p0 v1) 1 0)) (or (>= (+ (select _let_9 _let_17) v2) _let_23) (p1 _let_6 _let_21 _let_8))) (=> _let_5 (= _let_24 _let_18))) (xor (or (xor (<= v1 (f0 _let_16 _let_12)) (>= (+ _let_11 _let_14) (- _let_18))) (and (distinct _let_11 (* (f0 v1 v0) 2)) (p1 (f1 _let_6 _let_19 (f1 (f1 v3 v3 v3) (ite _let_5 _let_6 v3) v3)) _let_7 (f1 _let_7 (ite _let_4 _let_6 v3) _let_10)))) (> _let_13 _let_18)))))) (and (p1 _let_21 _let_10 _let_22) (xor (<= _let_25 _let_18) _let_3)))) (not (=> (ite (= (not (>= (select _let_9 _let_17) (f0 (f0 (ite _let_3 (* (f0 v1 v0) 2) _let_13) _let_14) v0))) (ite (xor (not (p1 _let_9 _let_20 (ite _let_4 _let_6 v3))) (or (<= _let_25 (+ (select _let_9 _let_17) v2)) (ite (= (ite (p0 _let_12) 1 0) (f0 _let_1 v0)) (= (= _let_15 (+ _let_13 _let_0)) (or (< (- (ite (>= v2 v0) _let_11 _let_12)) (* (f0 v1 v0) 2)) (>= _let_14 (- (+ _let_11 _let_14) (ite _let_3 (* (f0 v1 v0) 2) _let_13))))) (distinct (- _let_16) v2)))) (distinct v2 (f0 v1 v0)) (xor (or (p1 (ite _let_5 _let_6 v3) (f1 v3 v3 v3) _let_21) (p1 (f1 v3 v3 v3) _let_19 _let_8)) (ite (p1 (f1 _let_10 _let_10 v3) (ite _let_4 _let_6 v3) _let_20) (and (p1 _let_22 (f1 (f1 v3 v3 v3) (ite _let_5 _let_6 v3) v3) (f1 _let_7 (ite _let_4 _let_6 v3) _let_10)) (<= v0 v0)) (p0 (f0 _let_1 v0)))))) (or (and (=> (p0 v2) (distinct (- (f0 _let_1 v0)) _let_24)) (or (= (f0 (f0 (ite _let_3 (* (f0 v1 v0) 2) _let_13) _let_14) v0) (- _let_16)) (p1 _let_6 (f1 v3 v3 v3) _let_6))) (or (and (= (p1 (f1 _let_10 _let_10 v3) _let_20 v3) (p1 _let_7 _let_7 _let_20)) (=> (>= v2 v0) (< (ite (>= v2 v0) _let_11 _let_12) _let_0))) (= (= (=> (= (+ _let_13 _let_0) (ite (p0 v1) 1 0)) (or (>= (+ (select _let_9 _let_17) v2) _let_23) (p1 _let_6 _let_21 _let_8))) (=> _let_5 (= _let_24 _let_18))) (xor (or (xor (<= v1 (f0 _let_16 _let_12)) (>= (+ _let_11 _let_14) (- _let_18))) (and (distinct _let_11 (* (f0 v1 v0) 2)) (p1 (f1 _let_6 _let_19 (f1 (f1 v3 v3 v3) (ite _let_5 _let_6 v3) v3)) _let_7 (f1 _let_7 (ite _let_4 _let_6 v3) _let_10)))) (> _let_13 _let_18))))) (or (and (=> (p0 v2) (distinct (- (f0 _let_1 v0)) _let_24)) (or (= (f0 (f0 (ite _let_3 (* (f0 v1 v0) 2) _let_13) _let_14) v0) (- _let_16)) (p1 _let_6 (f1 v3 v3 v3) _let_6))) (or (and (= (p1 (f1 _let_10 _let_10 v3) _let_20 v3) (p1 _let_7 _let_7 _let_20)) (=> (>= v2 v0) (< (ite (>= v2 v0) _let_11 _let_12) _let_0))) (= (= (=> (= (+ _let_13 _let_0) (ite (p0 v1) 1 0)) (or (>= (+ (select _let_9 _let_17) v2) _let_23) (p1 _let_6 _let_21 _let_8))) (=> _let_5 (= _let_24 _let_18))) (xor (or (xor (<= v1 (f0 _let_16 _let_12)) (>= (+ _let_11 _let_14) (- _let_18))) (and (distinct _let_11 (* (f0 v1 v0) 2)) (p1 (f1 _let_6 _let_19 (f1 (f1 v3 v3 v3) (ite _let_5 _let_6 v3) v3)) _let_7 (f1 _let_7 (ite _let_4 _let_6 v3) _let_10)))) (> _let_13 _let_18)))))) (and (p1 _let_21 _let_10 _let_22) (xor (<= _let_25 _let_18) _let_3))))))) (= (= _let_26 _let_26) (not (and _let_27 _let_27))))))))))))))))))))))))))))))) ))
