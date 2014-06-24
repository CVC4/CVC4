; On a production build (as of 2014-05-16), takes several minutes
; to finish (2967466 decisions).

(set-logic QF_BV_SETS)
(set-info :status unsat)

(define-sort myset () (Set (Set (_ BitVec 1))))
(declare-fun S () myset)

; 0 elements
(assert (not (= S (as emptyset myset))))

; 1 element is S
(assert (not (= S (singleton (as emptyset (Set (_ BitVec 1)))))))
(assert (not (= S (singleton (singleton (_ bv0 1)) ))))
(assert (not (= S (singleton (singleton (_ bv1 1)) ))))
(assert (not (= S (singleton (union (singleton (_ bv0 1))
                                  (singleton (_ bv1 1)))))))

; 2 elements in S
(assert (not (= S (union (singleton (as emptyset (Set (_ BitVec 1))))
                         (singleton (singleton (_ bv0 1)))) )))
(assert (not (= S (union (singleton (as emptyset (Set (_ BitVec 1))))
                         (singleton (singleton (_ bv1 1)))))))
(assert (not (= S (union (singleton (as emptyset (Set (_ BitVec 1))))
                         (singleton (union (singleton (_ bv0 1))
                                         (singleton (_ bv1 1))))))))
(assert (not (= S (union (singleton (union (singleton (_ bv0 1))
                                         (singleton (_ bv1 1))))
                         (singleton (singleton (_ bv0 1)))) )))
(assert (not (= S (union (singleton (singleton (_ bv0 1)))
                         (singleton (singleton (_ bv1 1))))   )))
(assert (not (= S (union (singleton (union (singleton (_ bv0 1))
                                         (singleton (_ bv1 1))))
                         (singleton (singleton (_ bv1 1)))))))

; 3 elements in S
(assert (not (= S (union (singleton (singleton (_ bv1 1)))
                         (union (singleton (as emptyset (Set (_ BitVec 1))))
                                (singleton (singleton (_ bv0 1)))))  )))
(assert (not (= S (union (singleton (union (singleton (_ bv0 1))
                                         (singleton (_ bv1 1))))
                         (union (singleton (as emptyset (Set (_ BitVec 1))))
                                (singleton (singleton (_ bv1 1)))))  )))
(assert (not (= S (union (singleton (union (singleton (_ bv0 1))
                                         (singleton (_ bv1 1))))
                         (union (singleton (singleton (_ bv0 1)))
                                (singleton (singleton (_ bv1 1)))))  )))
(assert (not (= S (union (singleton (union (singleton (_ bv0 1))
                                         (singleton (_ bv1 1))))
                         (union (singleton (as emptyset (Set (_ BitVec 1))))
                                (singleton (singleton (_ bv0 1)))))  )))

; 4 elements in S
(assert (not (= S (union (singleton (union (singleton (_ bv0 1))
                                         (singleton (_ bv1 1))))
                         (union (singleton (singleton (_ bv1 1)))
                                (union (singleton (as emptyset (Set (_ BitVec 1))))
                                       (singleton (singleton (_ bv0 1))))))  )))

(check-sat)

; if you delete any of the above assertions, you should get sat
; (get-model)
