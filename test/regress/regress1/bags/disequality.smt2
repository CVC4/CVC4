(set-logic ALL)
(set-info :status sat)
(declare-fun A () (Bag Int))
(declare-fun B () (Bag Int))
(declare-fun C () (Bag Int))
(declare-fun x () Int)
(declare-fun y () Int)
(assert (distinct A B C))
(assert (> (bag.count x A) 0))
(assert (> (bag.count y B) 0))
(assert (= (bag.count x A) (bag.count x B)))
(assert (= (bag.count y A) (bag.count y B)))
(assert (distinct x y))
(check-sat)