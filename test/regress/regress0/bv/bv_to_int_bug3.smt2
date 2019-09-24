; COMMAND-LINE: --solve-bv-as-int=1 --no-check-models  --no-check-unsat-cores
; EXPECT: sat
(set-logic QF_BV)
(declare-fun s () (_ BitVec 2))
(declare-fun t () (_ BitVec 2))
(declare-fun splust () (_ BitVec 2))
(declare-fun shift1 () (_ BitVec 2))
(declare-fun shift2 () (_ BitVec 2))
(declare-fun negshift1 () (_ BitVec 2))

(assert (= shift1 (bvlshr s splust)))
(assert (= shift2 (bvlshr t splust)))
(assert (= negshift1 (bvneg shift1)))
(assert (= splust (bvadd s t)))
(assert (distinct negshift1 shift2))

(check-sat)
