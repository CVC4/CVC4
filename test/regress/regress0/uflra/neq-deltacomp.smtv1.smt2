(set-option :incremental false)
(meta-info :status sat)
(set-logic QF_UFLRA)
(declare-fun v2 () Real)
(declare-fun v1 () Real)
(declare-fun v0 () Real)
(check-sat-assuming ( (<= (ite (>= (- v1) v0) v1 1) v2) ))
