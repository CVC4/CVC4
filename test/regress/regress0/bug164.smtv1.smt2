(set-option :incremental false)
(set-info :status unsat)
(set-logic QF_LRA)
(declare-fun x_34 () Real)
(declare-fun x_13 () Real)
(declare-fun x_30 () Real)
(declare-fun x_59 () Real)
(check-sat-assuming ( (and (not (<= x_59 0.0)) (= x_30 x_59) (= x_30 0.0) (or true (= x_13 x_34))) ))
