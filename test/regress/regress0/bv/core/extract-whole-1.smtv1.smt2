(set-option :incremental false)
(meta-info :status unsat)
(set-logic QF_BV)
(declare-fun x () (_ BitVec 32))
(check-sat-assuming ( (not (= (concat (concat (concat (concat ((_ extract 31 31) x) ((_ extract 30 20) x)) ((_ extract 19 10) x)) ((_ extract 9 1) x)) ((_ extract 0 0) x)) x)) ))
