(set-logic QF_S)
(set-option :strings-exp true)
(set-info :status sat)

(declare-fun Y () String)

(assert (= Y "0001"))
;(assert (= (str.to.int Y) (- 1)))
(assert (= (str.to.int Y) 1))

(check-sat)
