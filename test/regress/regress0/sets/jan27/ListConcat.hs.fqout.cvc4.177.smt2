(set-logic QF_ALL_SUPPORTED)
(set-info :status unsat)
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
(declare-fun z3v54 () Int)
(declare-fun z3f55 (Int) Int)
(declare-fun z3v56 () Int)
(declare-fun z3v57 () Int)
(declare-fun z3v58 () Int)
(declare-fun z3v59 () Int)
(declare-fun z3f60 (Int) mySet)
(declare-fun z3f61 (Int) Bool)
(declare-fun z3f62 (Int Int) Int)
(declare-fun z3v63 () Int)
(declare-fun z3v64 () Int)
(declare-fun z3f65 (Int) mySet)
(declare-fun z3v66 () Int)
(declare-fun z3v67 () Int)
(declare-fun z3v68 () Int)
(declare-fun z3v69 () Int)
(declare-fun z3f70 (Int) Int)
(declare-fun z3v71 () Int)
(declare-fun z3v72 () Int)
(declare-fun z3v74 () Int)
(declare-fun z3v76 () Int)
(declare-fun z3v77 () Int)
(declare-fun z3v78 () Int)
(declare-fun z3v82 () Int)
(declare-fun z3v85 () Int)
(declare-fun z3v86 () Int)
(declare-fun z3v87 () Int)
(declare-fun z3f90 () Int)
(declare-fun z3v91 () Int)
(declare-fun z3v92 () Int)
(declare-fun z3v93 () Int)
(declare-fun z3v94 () Int)
(assert (= (z3f60 z3v94) (z3f65 z3v56)))
(assert (and (>= (z3f55 z3v56) 0) (>= (z3f55 z3v58) 0) (= (z3f60 z3v58) smt_set_emp) (= (z3f55 z3v58) 0) (= (z3f61 z3v58) true) (= z3v58 z3f90) (>= (z3f55 z3v58) 0) (= z3v58 z3v63) (>= (z3f55 z3v58) 0) (>= (z3f55 z3v64) 0) (= (z3f65 z3v64) (smt_set_cup (z3f60 z3v63) (z3f65 z3v56))) (= (z3f60 z3v64) (smt_set_cup (smt_set_add smt_set_emp z3v63) (z3f60 z3v56))) (= (z3f55 z3v64) (+ 1 (z3f55 z3v56))) (= (z3f61 z3v64) false) (= z3v64 (z3f62 z3v63 z3v56)) (>= (z3f55 z3v64) 0) (= z3v64 z3v66) (>= (z3f55 z3v64) 0) (>= (z3f55 z3v63) 0) (>= (z3f55 z3v66) 0) (= (z3f70 z3v69) z3v69) (= (z3f70 z3v71) z3v71) (= (z3f70 z3v72) z3v72)))
(assert (not (= (z3f60 z3v94) (z3f65 z3v66))))
(check-sat)
