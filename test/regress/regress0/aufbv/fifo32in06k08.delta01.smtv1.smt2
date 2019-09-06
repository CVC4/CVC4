(set-option :incremental false)
(set-info :status unknown)
(set-logic QF_AUFBV)
(declare-fun full_fq_2 () (_ BitVec 1))
(declare-fun full_fs_2 () (_ BitVec 1))
(declare-fun reset_2 () (_ BitVec 1))
(declare-fun full_fq_1 () (_ BitVec 1))
(declare-fun reset_1 () (_ BitVec 1))
(declare-fun a504 () (Array (_ BitVec 6) (_ BitVec 32)))
(declare-fun enqeue_2 () (_ BitVec 1))
(declare-fun deqeue_2 () (_ BitVec 1))
(declare-fun a723 () (Array (_ BitVec 6) (_ BitVec 32)))
(declare-fun a942 () (Array (_ BitVec 6) (_ BitVec 32)))
(declare-fun a1161 () (Array (_ BitVec 6) (_ BitVec 32)))
(declare-fun a1380 () (Array (_ BitVec 6) (_ BitVec 32)))
(check-sat-assuming ( (not (= (_ bv0 1) (bvand (ite (= (_ bv0 32) (select a1380 (_ bv0 6))) (_ bv1 1) (_ bv0 1)) (bvand (ite (= a1161 a1380) (_ bv1 1) (_ bv0 1)) (bvand (ite (= a942 a1161) (_ bv1 1) (_ bv0 1)) (bvand (ite (= a723 a942) (_ bv1 1) (_ bv0 1)) (bvand (ite (= a723 (ite (= (_ bv1 1) deqeue_2) (ite (= (_ bv1 1) enqeue_2) (ite (= (_ bv1 1) full_fs_2) a504 (store a504 (_ bv0 6) (_ bv0 32))) a504) a504)) (_ bv1 1) (_ bv0 1)) (bvand (bvand reset_2 (bvand reset_1 (ite (= full_fq_2 (ite (= (_ bv1 1) reset_1) full_fq_1 (_ bv0 1))) (_ bv1 1) (_ bv0 1)))) (bvnot (bvand reset_2 (bvnot (ite (= full_fs_2 full_fq_2) (_ bv1 1) (_ bv0 1))))))))))))) ))
