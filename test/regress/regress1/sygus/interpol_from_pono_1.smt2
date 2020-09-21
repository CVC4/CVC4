; COMMAND-LINE: --produce-interpols=default --sygus-active-gen=enum --check-interpols
; SCRUBBER: grep -v -E '(\(define-fun)'
; EXIT: 0
(set-logic ALL)
(declare-fun a_S_a2@1 () (_ BitVec 1))
(declare-fun a_R_a2@1 () (_ BitVec 1))
(declare-fun a_Q_a2@1 () (_ BitVec 1))
(declare-fun dve_invalid@1 () (_ BitVec 1))
(declare-fun v_x1@1 () (_ BitVec 16))
(declare-fun a_S_a1@1 () (_ BitVec 1))
(declare-fun v_c@1 () (_ BitVec 16))
(declare-fun v_x2@1 () (_ BitVec 16))
(declare-fun a_Q_a1@1 () (_ BitVec 1))
(declare-fun a_R_a1@1 () (_ BitVec 1))
(declare-fun f3@1 () (_ BitVec 1))
(declare-fun f0@1 () (_ BitVec 1))
(declare-fun f5@1 () (_ BitVec 1))
(declare-fun f1@1 () (_ BitVec 1))
(declare-fun f2@1 () (_ BitVec 1))
(declare-fun f4@1 () (_ BitVec 1))
(declare-fun dve_invalid@0 () (_ BitVec 1))
(declare-fun a_S_a2@0 () (_ BitVec 1))
(declare-fun a_R_a2@0 () (_ BitVec 1))
(declare-fun a_Q_a2@0 () (_ BitVec 1))
(declare-fun a_S_a1@0 () (_ BitVec 1))
(declare-fun a_R_a1@0 () (_ BitVec 1))
(declare-fun a_Q_a1@0 () (_ BitVec 1))
(declare-fun v_x2@0 () (_ BitVec 16))
(declare-fun v_x1@0 () (_ BitVec 16))
(declare-fun v_c@0 () (_ BitVec 16))
(declare-fun f5@0 () (_ BitVec 1))
(declare-fun f3@0 () (_ BitVec 1))
(declare-fun f4@0 () (_ BitVec 1))
(declare-fun f2@0 () (_ BitVec 1))
(declare-fun f0@0 () (_ BitVec 1))
(declare-fun f1@0 () (_ BitVec 1))
(assert (and (and (and (and (and (and (and (and (and (and (and true (= v_c@0 (_ bv0 16))) (= v_x1@0 (_ bv0 16))) (= v_x2@0 (_ bv0 16))) (= a_Q_a1@0 (_ bv0 1))) (= a_R_a1@0 (_ bv0 1))) (= a_S_a1@0 (_ bv0 1))) (= a_Q_a2@0 (_ bv0 1))) (= a_R_a2@0 (_ bv0 1))) (= a_S_a2@0 (_ bv0 1))) (= dve_invalid@0 (_ bv0 1))) (and (and (and (and (and (and (and (and (and (and true (= v_c@1 (bvxor (_ bv1 16) (ite (= f5@0 (_ bv1 1)) v_x2@0 (ite (= f2@0 (_ bv1 1)) v_x1@0 (bvxor (_ bv1 16) v_c@0)))))) (= v_x1@1 (ite (= f1@0 (_ bv1 1)) ((_ extract 15 0) (bvadd (bvashr (concat (bvxor (_ bv1 16) v_c@0) (_ bv0 16)) (_ bv16 32)) (bvashr (concat v_x1@0 (_ bv0 16)) (_ bv16 32)))) (ite (= f0@0 (_ bv1 1)) (bvxor (_ bv1 16) v_c@0) v_x1@0)))) (= v_x2@1 (ite (= f4@0 (_ bv1 1)) ((_ extract 15 0) (bvadd (bvashr (concat (bvxor (_ bv1 16) v_c@0) (_ bv0 16)) (_ bv16 32)) (bvashr (concat v_x2@0 (_ bv0 16)) (_ bv16 32)))) (ite (= f3@0 (_ bv1 1)) (bvxor (_ bv1 16) v_c@0) v_x2@0)))) (= a_Q_a1@1 (bvnot (bvor (bvand (bvnot a_Q_a1@0) (bvnot f0@0)) f2@0)))) (= a_R_a1@1 (bvand (bvor a_R_a1@0 f0@0) (bvnot f1@0)))) (= a_S_a1@1 (bvand (bvor a_S_a1@0 f1@0) (bvnot f2@0)))) (= a_Q_a2@1 (bvnot (bvor (bvand (bvnot a_Q_a2@0) (bvnot f3@0)) f5@0)))) (= a_R_a2@1 (bvand (bvor a_R_a2@0 f3@0) (bvnot f4@0)))) (= a_S_a2@1 (bvand (bvor a_S_a2@0 f4@0) (bvnot f5@0)))) (= dve_invalid@1 (bvnot (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvor (bvnot f0@0) (bvand (bvnot a_Q_a1@0) (ite (not (bvule (_ bv200 32) (bvashr (concat (bvxor (_ bv1 16) v_c@0) (_ bv0 16)) (_ bv16 32)))) (_ bv1 1) (_ bv0 1)))) (bvor a_R_a1@0 (bvnot f1@0))) (bvor a_S_a1@0 (bvnot f2@0))) (bvor (bvnot f3@0) (bvand (bvnot a_Q_a2@0) (ite (not (bvule (_ bv200 32) (bvashr (concat (bvxor (_ bv1 16) v_c@0) (_ bv0 16)) (_ bv16 32)))) (_ bv1 1) (_ bv0 1))))) (bvor a_R_a2@0 (bvnot f4@0))) (bvor a_S_a2@0 (bvnot f5@0))) (bvor f5@0 (bvor f4@0 (bvor f3@0 (bvor f2@0 (bvor f0@0 f1@0)))))) (bvnot (bvor (bvor (bvor (bvor (bvand f0@0 f1@0) (bvand f2@0 (bvor f0@0 f1@0))) (bvand f3@0 (bvor f2@0 (bvor f0@0 f1@0)))) (bvand f4@0 (bvor f3@0 (bvor f2@0 (bvor f0@0 f1@0))))) (bvand f5@0 (bvor f4@0 (bvor f3@0 (bvor f2@0 (bvor f0@0 f1@0)))))))) (bvand (bvand (bvand (bvnot (bvor (bvand (bvnot a_Q_a1@0) a_R_a1@0) (bvand a_S_a1@0 (bvor (bvnot a_Q_a1@0) a_R_a1@0)))) (bvor a_S_a1@0 (bvor (bvnot a_Q_a1@0) a_R_a1@0))) (bvnot (bvor (bvand (bvnot a_Q_a2@0) a_R_a2@0) (bvand a_S_a2@0 (bvor (bvnot a_Q_a2@0) a_R_a2@0))))) (bvor a_S_a2@0 (bvor (bvnot a_Q_a2@0) a_R_a2@0)))) (bvand (bvand (bvand (bvnot (bvor (bvand (bvand (bvor a_R_a1@0 f0@0) (bvnot f1@0)) (bvor (bvand (bvnot a_Q_a1@0) (bvnot f0@0)) f2@0)) (bvand (bvand (bvor a_S_a1@0 f1@0) (bvnot f2@0)) (bvor (bvand (bvor a_R_a1@0 f0@0) (bvnot f1@0)) (bvor (bvand (bvnot a_Q_a1@0) (bvnot f0@0)) f2@0))))) (bvor (bvand (bvor a_S_a1@0 f1@0) (bvnot f2@0)) (bvor (bvand (bvor a_R_a1@0 f0@0) (bvnot f1@0)) (bvor (bvand (bvnot a_Q_a1@0) (bvnot f0@0)) f2@0)))) (bvnot (bvor (bvand (bvand (bvor a_R_a2@0 f3@0) (bvnot f4@0)) (bvor (bvand (bvnot a_Q_a2@0) (bvnot f3@0)) f5@0)) (bvand (bvand (bvor a_S_a2@0 f4@0) (bvnot f5@0)) (bvor (bvand (bvor a_R_a2@0 f3@0) (bvnot f4@0)) (bvor (bvand (bvnot a_Q_a2@0) (bvnot f3@0)) f5@0)))))) (bvor (bvand (bvor a_S_a2@0 f4@0) (bvnot f5@0)) (bvor (bvand (bvor a_R_a2@0 f3@0) (bvnot f4@0)) (bvor (bvand (bvnot a_Q_a2@0) (bvnot f3@0)) f5@0))))) (bvnot dve_invalid@0)))))))

(get-interpol I (not (and true (not (not (= (bvand (bvnot dve_invalid@1) (bvcomp (_ bv500 32) (bvashr (concat (bvxor (_ bv1 16) v_c@1) (_ bv0 16)) (_ bv16 32)))) (_ bv1 1)))))))
