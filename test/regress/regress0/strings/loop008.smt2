(set-logic QF_S)
(set-info :status sat)

(declare-fun x () String)

(assert (= (str.++ x "ab") (str.++ "ba" x)))
(assert (> (str.len x) 5))

(check-sat)
