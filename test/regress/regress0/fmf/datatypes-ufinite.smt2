; COMMAND-LINE: --finite-model-find --lang=smt2.5
; EXPECT: sat
(set-logic ALL_SUPPORTED)
(declare-sort U 0)
(declare-fun a () U)
(declare-fun b () U)
(declare-fun c () U)
(declare-fun d () U)
(assert (distinct a b c))
(declare-sort V 0)
(declare-datatypes () ((ufin1 (cons1 (s11 U) (s12 U))) (ufin2 (cons2 (s21 V) (s22 U)) (cons3))))
(declare-fun P (ufin1 ufin2) Bool)
(declare-fun Q (ufin1 ufin1) Bool)
(assert (forall ((x ufin1) (y ufin2) (z ufin1)) (or (P x y) (Q x z))))
(assert (not (P (cons1 a a) cons3)))
(assert (not (Q (cons1 a d) (cons1 a b))))
(check-sat)
