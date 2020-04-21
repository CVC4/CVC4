(set-info :smt-lib-version 2.5)
(set-logic QF_SLIA)
(set-option :strings-exp true)
(set-info :status unsat)

(declare-fun var_0 () String)
(declare-fun var_1 () String)
(declare-fun var_2 () String)
(declare-fun var_3 () String)
(declare-fun var_4 () String)
(declare-fun var_5 () String)
(declare-fun var_6 () String)
(declare-fun var_7 () String)
(declare-fun var_8 () String)
(declare-fun var_9 () String)
(declare-fun var_10 () String)
(declare-fun var_11 () String)
(declare-fun var_12 () String)

(assert (str.in_re var_1 (re.* (re.range "a" "u"))))
(assert (str.in_re var_1 (re.++ (re.* (str.to_re "a")) (str.to_re "b"))))
(assert (str.in_re var_1 (re.++ (re.++ (re.++ (re.* (re.union (str.to_re "a") (str.to_re "b"))) (str.to_re "b")) (str.to_re "a")) (re.* (re.union (str.to_re "a") (str.to_re "b"))))))
(assert (not (str.in_re "" re.nostr)))
(check-sat)
