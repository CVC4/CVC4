(set-option :incremental false)
(set-info :status sat)
(set-logic QF_UF)
(declare-sort U 0)
(declare-fun a () U)
(declare-fun b () U)
(declare-fun f (U) U)
(check-sat-assuming ( (= (f a) (f b)) ))
