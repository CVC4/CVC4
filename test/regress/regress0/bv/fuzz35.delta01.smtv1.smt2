(set-option :incremental false)
(meta-info :status unsat)
(set-logic QF_BV)
(declare-fun v0 () (_ BitVec 4))
(check-sat-assuming ( (let ((_let_0 (bvmul v0 (bvsub (_ bv4 4) (_ bv12 4))))) (bvsgt (bvadd _let_0 _let_0) (_ bv0 4))) ))
