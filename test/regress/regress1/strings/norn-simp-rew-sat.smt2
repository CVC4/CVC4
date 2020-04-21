(set-info :smt-lib-version 2.5)
(set-logic QF_SLIA)
(set-option :strings-exp true)
(set-info :status sat)

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

(assert (str.in_re (str.++ var_3 "z" var_4 ) (re.++ (re.* (re.union (re.union (str.to_re "z") (str.to_re "a")) (re.++ (str.to_re "b") (re.++ (re.* (str.to_re "b")) (re.union (str.to_re "z") (str.to_re "a")))))) (re.++ (str.to_re "b") (re.* (str.to_re "b"))))))
(assert (str.in_re var_4 (re.* (re.range "a" "u"))))
(assert (str.in_re var_4 (re.* (str.to_re "b"))))
(assert (str.in_re var_3 (re.* (re.range "a" "u"))))
(assert (str.in_re var_3 (re.* (str.to_re "a"))))
(assert (<= 0  (str.len var_4)))
(check-sat)
