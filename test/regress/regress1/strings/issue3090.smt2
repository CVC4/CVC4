(set-logic ALL)
(set-option :strings-exp true)
(set-info :status unsat)
(declare-const id String)
(assert (and (str.in_re id (re.+ (re.range "0" "9"))) (str.contains id "value")))
(check-sat)
