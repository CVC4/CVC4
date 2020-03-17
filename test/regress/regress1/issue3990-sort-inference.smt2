; COMMAND-LINE: --sort-inference --no-check-unsat-cores
; EXPECT: unsat
(set-logic ABV)
(set-option :sort-inference true)
(set-info :status unsat)
(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(declare-const v5 Bool)
(declare-const v6 Bool)
(declare-const v7 Bool)
(declare-const v8 Bool)
(declare-const v9 Bool)
(declare-const v10 Bool)
(declare-const v11 Bool)
(declare-const v12 Bool)
(declare-const v13 Bool)
(assert (= v13 v3 v10 v5 v7 v8))
(declare-const bv_10-0 (_ BitVec 10))
(declare-const v14 Bool)
(assert v8)
(declare-const v15 Bool)
(declare-const v16 Bool)
(declare-const bv_25-0 (_ BitVec 25))
(declare-const bv_4-0 (_ BitVec 4))
(declare-const v17 Bool)
(declare-const v18 Bool)
(assert (not (not v13)))
(assert (and v9 v11 v5 v4 v11 (and (= v13 v3 v10 v5 v7 v8) v9) v4 v4 v9 (xor v8 v6 (not v13) (xor (= v13 v3 v10 v5 v7 v8) v10) v12) (distinct v15 v13 v13 v2 v15 v7)))
(declare-const v19 Bool)
(assert (= v13 v3 v10 v5 v7 v8))
(assert (forall ((q0 (_ BitVec 4)) (q1 (_ BitVec 4)) (q2 (_ BitVec 25)) (q3 (_ BitVec 4))) (=> (distinct bv_25-0 (bvsmod bv_25-0 bv_25-0) bv_25-0) (bvsle bv_4-0 bv_4-0))))
(declare-const bv_13-0 (_ BitVec 13))
(assert (exists ((q4 (_ BitVec 1))) v7))
(check-sat)
