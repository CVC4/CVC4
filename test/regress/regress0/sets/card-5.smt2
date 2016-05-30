(set-logic QF_UFLIAFS)
(declare-sort E 0)
(declare-fun s () (Set E))
(declare-fun t () (Set E))
(declare-fun u () (Set E))
(assert (>= (card (union s t)) 8))
(assert (>= (card (union s u)) 8))
;(assert (<= (card (union t u)) 5))
(assert (<= (card s) 5))
(assert (= (as emptyset (Set E)) (intersection t u)))
(declare-fun x1 () E)
(declare-fun x2 () E)
(declare-fun x3 () E)
(declare-fun x4 () E)
(declare-fun x5 () E)
(declare-fun x6 () E)
(assert (member x1 s))
(assert (member x2 s))
(assert (member x3 s))
(assert (member x4 s))
(assert (member x5 s))
(assert (member x6 s))
(assert (distinct x1 x2 x3 x4 x5 x6))
(check-sat)
