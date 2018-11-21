(set-info :smt-lib-version 2.5)
(set-logic ALL)
(set-info :status sat)
(set-option :strings-exp true)
(set-option :strings-fmf true)
(declare-fun x () String)
(declare-fun y () String)
(assert (= (str.len (str.replaceall x y "AB")) (+ (str.len (str.replaceall x y "A")) 3)))
(check-sat)
