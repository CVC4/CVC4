(set-logic QF_SLIA)
(declare-fun a () String)
(assert (str.suffixof "a" a))
(assert (str.in_re a (re.* (re.union (str.to_re (str.replace_re a (re.++ (re.* (str.to_re "a")) (str.to_re "ba")) "")) (str.to_re "b")))))
(assert (str.in_re a (re.++ (re.* (str.to_re (ite (str.in_re a (re.* (str.to_re "b"))) "" "u"))) (re.* (str.to_re a)))))
(set-info :status sat)
(check-sat)
