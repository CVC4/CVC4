(set-info :smt-lib-version 2.5)
(set-logic ALL)
(set-info :status sat)
(set-option :strings-exp true)
(set-option :strings-fmf true)
(declare-fun x () String)
(assert (= (str.len (str.replaceall "ABBABAAB" x "C")) 5))
(check-sat)
