(set-logic QF_AX)
(set-info :smt-lib-version 2.0)
(set-info :status unsat)
(declare-sort Index 0)
(declare-sort Element 0)
(declare-fun a1 () (Array Index Element))
(declare-fun a2 () (Array Index Element))
(declare-fun i1 () Index)
(declare-fun i2 () Index)
(declare-fun e1 () Element)
(assert (not (= (select (store (store a1 i2 e1) i2 (select a1 i2)) i1) (select (store a1 i2 (select a1 i2)) i1))))
(check-sat)
(exit)
