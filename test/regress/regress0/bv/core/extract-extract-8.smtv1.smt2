(set-option :incremental false)
(meta-info :status unsat)
(set-logic QF_BV)
(declare-fun x () (_ BitVec 32))
(check-sat-assuming ( (let ((_let_0 ((_ extract 7 2) x))) (not (= ((_ extract 5 0) _let_0) _let_0))) ))
