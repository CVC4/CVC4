(set-logic ALL)
(set-info :status sat)
(declare-fun y () Int)
(assert (= 0 ((_ iand 5) y 1)))
(check-sat)
