(set-logic QF_ALL)
(set-info :status unsat)
(declare-fun A () (Set Int) )
(declare-fun B () (Set Int) )
(declare-fun C () (Set Int) )
(declare-fun D () (Set Int) )
(declare-fun E () (Set Int) )
(declare-fun F () (Set Int) )
(declare-fun G () (Set Int) )
(declare-fun H () (Set Int) )
(declare-fun I () (Set Int) )
(declare-fun x () Int)
(assert (member x (intersection (union A B) C)))
(assert (not (member x G)))
(assert (= (union A B) D))
(assert (= C (intersection E F)))
(assert (and (= F H) (= G H) (= H I)))
(check-sat)
