(set-logic ALL_SUPPORTED)
(set-option :strings-exp true)
(set-info :status unsat)
(declare-fun a () String)
(declare-fun b () String)
(declare-const c String)
(declare-const d String)
(declare-const g String)
(declare-const e String)
(declare-const f String)
(assert (or  
            (and (= d (str.++ e g)) (str.in.re e (re.* (str.to.re "HG4"))) (> 0 (str.to.int g)) (= 1 (str.len e)) (= 2 (str.len (str.substr b 0 (str.len d)))))  
            (and 
                (str.in.re (str.replace (str.replace a c "") "T" "") (re.* (re.union (str.to.re "a") (str.to.re "")))) 
                (= 0 (str.to.int (str.replace (str.replace a c "") "T" "")))))
)
(assert (= b (str.++ d f)))
(check-sat
