(set-logic ALL_SUPPORTED)
(set-info :status unsat)
(set-option :strings-exp true)
(declare-const x Int)
(assert (str.contains (str.++ "some text" (int.to.str x) "tor") "vector"))
(check-sat)
