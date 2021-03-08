(set-logic QF_BV)
(set-info :smt-lib-version 2.6)
(set-info :category "crafted")
(set-info :status unsat)
(declare-fun v0 () (_ BitVec 16))
(assert (and
	 (bvult v0 (_ bv3 16))
	 (bvult (_ bv2 16) v0)))
(check-sat)
(exit)
