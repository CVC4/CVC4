(set-option :incremental false)
(set-info :status unsat)
(set-logic QF_BV)
(declare-fun x () (_ BitVec 32))
(check-sat-assuming ( (let ((_let_0 ((_ extract 31 0) x))) (not (= ((_ extract 31 0) _let_0) _let_0))) ))
