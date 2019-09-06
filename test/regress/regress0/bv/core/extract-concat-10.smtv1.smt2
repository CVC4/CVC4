(set-option :incremental false)
(set-info :status unsat)
(set-logic QF_BV)
(declare-fun x () (_ BitVec 32))
(declare-fun y () (_ BitVec 32))
(check-sat-assuming ( (not (= ((_ extract 60 3) (concat x y)) (concat ((_ extract 28 0) x) ((_ extract 31 3) y)))) ))
