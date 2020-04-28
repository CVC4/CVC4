(set-info :smt-lib-version 2.6)
(set-logic ALL)
(set-info :status sat)
(set-option :strings-exp true)
(set-option :strings-fmf true)
(set-option :produce-models true)
(declare-fun x () String)
(assert (= (str.len (str.replace_all x "B" "CC")) (+ (str.len x) 2)))
(assert (> (str.len x) 2))
(check-sat)
