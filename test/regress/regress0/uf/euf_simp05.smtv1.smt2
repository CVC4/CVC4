(set-option :incremental false)
(set-info :status unsat)
(set-info :difficulty "unknown")
(set-info :category "crafted")
(set-logic QF_UF)
(declare-sort U 0)
(declare-sort A 0)
(declare-fun x () A)
(declare-fun f (A) A)
(check-sat-assuming ( (let ((_let_0 (f x))) (let ((_let_1 (f _let_0))) (not (=> (and (= _let_1 x) (= (f _let_1) x)) (= _let_0 x))))) ))
