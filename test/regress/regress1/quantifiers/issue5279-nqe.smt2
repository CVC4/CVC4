(set-logic LIA)
(set-option :cegqi-nested-qe true)
(set-info :status unsat)
(assert (forall ((a Int) (b Bool)) (= a (ite b 0 1))))
(check-sat)
