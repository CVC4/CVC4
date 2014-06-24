(set-logic QF_ALL_SUPPORTED)
(set-info :status unsat)

; What was the bug?
; 
; When asserting equality to equality engine, correct reason
; was not being sent (the fact itself was being sent as reason)

(define-sort Elt () Int)
(define-sort mySet () (Set Elt ))
(define-fun smt_set_emp () mySet (as emptyset mySet))
(define-fun smt_set_mem ((x Elt) (s mySet)) Bool (member x s))
(define-fun smt_set_add ((s mySet) (x Elt)) mySet (union s (singleton x)))
(define-fun smt_set_cup ((s1 mySet) (s2 mySet)) mySet (union s1 s2))
(define-fun smt_set_cap ((s1 mySet) (s2 mySet)) mySet (intersection s1 s2))
;(define-fun smt_set_com ((s mySet)) mySet ((_ map not) s))
(define-fun smt_set_dif ((s1 mySet) (s2 mySet)) mySet (setminus s1 s2))
;(define-fun smt_set_sub ((s1 mySet) (s2 mySet)) Bool (= smt_set_emp (smt_set_dif s1 s2)))
(define-fun smt_set_sub ((s1 mySet) (s2 mySet)) Bool (subseteq s1 s2))
(declare-fun z3v56 () Int)
(declare-fun z3v57 () Int)
(assert (distinct z3v56 z3v57))
(declare-fun z3v58 () Int)
(declare-fun z3f59 (Int) Int)
(declare-fun z3v60 () Int)
(declare-fun z3f61 (Int) Bool)
(declare-fun z3v62 () Int)
(declare-fun z3v63 () Int)
(declare-fun z3v64 () Int)
(declare-fun z3v65 () Int)
(declare-fun z3f66 (Int) mySet)
(declare-fun z3f67 (Int) Bool)
(declare-fun z3f68 (Int Int) Int)
(declare-fun z3v69 () Int)
(declare-fun z3v70 () Int)
(declare-fun z3f71 (Int) Int)
(declare-fun z3v72 () Int)
(declare-fun z3v73 () Int)
(declare-fun z3v74 () Int)
(declare-fun z3v75 () Int)
(declare-fun z3v76 () Int)
(declare-fun z3v77 () Int)
(declare-fun z3v78 () Int)
(declare-fun z3v80 () Int)
(declare-fun z3v81 () Int)
(declare-fun z3f82 () Int)
(declare-fun z3v83 () Int)
(declare-fun z3v84 () Int)
(declare-fun z3v85 () Int)
(declare-fun z3v86 () Int)
(declare-fun z3v87 () Int)
(declare-fun z3v88 () Int)
(declare-fun z3v89 () Int)
(declare-fun z3v90 () Int)
(assert (= z3v90 z3v56))
(assert (z3f61 z3v90))
(assert (and (>= (z3f59 z3v58) 0) (z3f61 z3v60) (z3f61 z3v60) (= z3v60 z3v62) (= (z3f61 z3v60) (= z3v64 z3v63)) (= (z3f61 z3v62) (= z3v64 z3v63)) (>= (z3f59 z3v65) 0) (= (z3f66 z3v65) (smt_set_cup (smt_set_add smt_set_emp z3v64) (z3f66 z3v58))) (= (z3f59 z3v65) (+ 1 (z3f59 z3v58))) (= (z3f67 z3v65) false) (= z3v65 (z3f68 z3v64 z3v58)) (>= (z3f59 z3v65) 0) (= z3v65 z3v69) (>= (z3f59 z3v65) 0) (>= (z3f59 z3v69) 0) (z3f61 z3v56) (= (z3f71 z3v70) z3v70) (= (z3f71 z3v72) z3v72) (not (z3f61 z3v57)) (= (z3f71 z3v73) z3v73)))
(assert (not (= (z3f61 z3v90) (smt_set_mem z3v63 (z3f66 z3v69)))))
(check-sat)
