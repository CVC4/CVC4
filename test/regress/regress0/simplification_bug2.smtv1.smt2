(set-option :incremental false)
(meta-info :status unsat)
(set-logic QF_UF)
(declare-sort U 0)
(declare-fun b () Bool)
(check-sat-assuming ( (and b (or false false)) ))
