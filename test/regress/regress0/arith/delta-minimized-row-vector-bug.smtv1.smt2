(set-option :incremental false)
(meta-info :status sat)
(set-logic QF_LRA)
(declare-fun x_120 () Real)
(declare-fun x_11 () Real)
(declare-fun x_102 () Real)
(check-sat-assuming ( (and (>= x_11 0.0) (or (= x_120 x_102) (<= x_102 (- x_11)) (= x_120 (+ x_102 x_11)))) ))
