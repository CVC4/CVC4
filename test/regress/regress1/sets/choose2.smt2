(set-logic ALL)
(set-info :status unsat)
(set-option :produce-models true)
(declare-fun A () (Set Int))
(assert (distinct (choose A) (choose A)))
(check-sat)