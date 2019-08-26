(set-option :incremental false)
(meta-info :status unsat)
(set-logic QF_AUF)
(declare-sort Index 0)
(declare-sort Element 0)
(declare-fun a () (Array Index Element))
(declare-fun i1 () Index)
(declare-fun i2 () Index)
(declare-fun i3 () Index)
(check-sat-assuming ( (not (=> (and (= i1 i2) (= i2 i3)) (= (select a i1) (select a i3)))) ))
