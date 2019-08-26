(set-option :incremental false)
(meta-info :status sat)
(set-logic QF_UFLIA)
(declare-fun arg1 () Int)
(declare-fun adr_lo () Int)
(declare-fun select_format (Int) Int)
(declare-fun x () Int)
(check-sat-assuming ( (and (= (select_format arg1) adr_lo) (and (or (= 0 x) (or (= adr_lo (select_format 4)) (or (= adr_lo (select_format 3)) (or (= adr_lo (select_format 2)) (or (= 1 (select_format 0)) (= 0 (select_format 1))))))) (and (= adr_lo 3) (and (< arg1 4) (>= arg1 0))))) ))
