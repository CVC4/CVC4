(set-info :smt-lib-version 2.6)
(set-logic ALL)
(set-info :status sat)
(set-option :strings-exp true)
(set-option :re-elim false)
(declare-fun x () String)
(assert (let ((_let_0 (re.* re.allchar ))) (and (not (= (str.in.re x (re.++ _let_0 re.allchar  _let_0 (str.to.re (str.++ "A" (str.++ "B" (str.++ "C" "A")))) _let_0 re.allchar  _let_0)) (str.in.re x (re.++ _let_0 re.allchar  _let_0 re.allchar  _let_0 (str.to.re (str.++ "B" (str.++ "C" (str.++ "B" "B")))) _let_0)))) (not (= (str.in.re x (re.++ _let_0 re.allchar  _let_0 (str.to.re (str.++ "C" (str.++ "B" "C"))) _let_0 (str.to.re "B") _let_0)) (str.in.re x (re.++ _let_0 re.allchar  _let_0 (str.to.re "C") _let_0 (str.to.re (str.++ "B" (str.++ "C" "B"))) _let_0)))))))
(check-sat)
