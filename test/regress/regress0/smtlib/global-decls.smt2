; COMMAND-LINE: --incremental
(set-option :global-declarations true)
(set-logic QF_UFDT)

(push 1)
(declare-datatype Struct1 (par (T0) ((mk-struct1 (struct1-proj0 T0)))))
(declare-datatypes ((Unit 0)) (((u))))
(declare-fun x () (Struct1 Bool))
(declare-sort U1 0)
(declare-sort U2 1)
(pop 1)

(assert (= x x))

(define-fun y () (Struct1 Bool) (mk-struct1 true))
(declare-const z Unit)
(assert (= u u))
(assert (is-mk-struct1 y))
(assert (is-u z))

(declare-const w1 U1)
(declare-const w2 (U2 Bool))

(set-info :status sat)
(check-sat)
