(set-option :incremental false)
(set-info :status unsat)
(set-info :category "crafted")
(set-logic QF_UF)
(declare-sort U 0)
(declare-sort A 0)
(declare-fun x () A)
(declare-fun f (A) A)
(check-sat-assuming ( (let ((_let_0 (f (f (f (f x)))))) (let ((_let_1 (f _let_0))) (let ((_let_2 (f _let_1))) (not (=> (and (= _let_2 (f (f x))) (= _let_1 (f (f (f x))))) (and (= _let_0 (f (f x))) (= _let_2 _let_0))))))) ))
