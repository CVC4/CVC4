; EXPECT: sat
; COMMAND-LINE: --sygus-inference
(set-info :smt-lib-version 2.6)
(set-logic QF_ABV)
(set-info :status sat)
(declare-fun mem_35_57_4 () (Array (_ BitVec 32) (_ BitVec 8)))
(declare-fun R_EBX_6_67_3 () (_ BitVec 32))
(declare-fun R_EBP_0_60_2 () (_ BitVec 32))
(declare-fun R_ESP_1_58_1 () (_ BitVec 32))
(assert (let ((?v_0 (bvsub (bvsub R_ESP_1_58_1 (_ bv4 32)) (_ bv4 32)))) (let ((?v_6 (bvadd (bvadd ?v_0 (_ bv4 32)) (_ bv4 32))) (?v_2 (store (store (store (store mem_35_57_4 (bvadd ?v_0 (_ bv3 32)) ((_ extract 7 0) (bvlshr R_EBP_0_60_2 (_ bv24 32)))) (bvadd ?v_0 (_ bv2 32)) ((_ extract 7 0) (bvlshr R_EBP_0_60_2 (_ bv16 32)))) (bvadd ?v_0 (_ bv1 32)) ((_ extract 7 0) (bvlshr R_EBP_0_60_2 (_ bv8 32)))) (bvadd ?v_0 (_ bv0 32)) ((_ extract 7 0) R_EBP_0_60_2)))) (let ((?v_4 (bvor (bvor (bvor (concat (_ bv0 24) (select ?v_2 (_ bv134526656 32))) (bvshl (concat (_ bv0 24) (select ?v_2 (_ bv134526657 32))) (_ bv8 32))) (bvshl (concat (_ bv0 24) (select ?v_2 (_ bv134526658 32))) (_ bv16 32))) (bvshl (concat (_ bv0 24) (select ?v_2 (_ bv134526659 32))) (_ bv24 32)))) (?v_1 (bvsub (bvsub ?v_0 (_ bv4 32)) (_ bv4 32)))) (let ((?v_3 (store (store (store (store ?v_2 (bvadd ?v_1 (_ bv3 32)) ((_ extract 7 0) (bvlshr R_EBX_6_67_3 (_ bv24 32)))) (bvadd ?v_1 (_ bv2 32)) ((_ extract 7 0) (bvlshr R_EBX_6_67_3 (_ bv16 32)))) (bvadd ?v_1 (_ bv1 32)) ((_ extract 7 0) (bvlshr R_EBX_6_67_3 (_ bv8 32)))) (bvadd ?v_1 (_ bv0 32)) ((_ extract 7 0) R_EBX_6_67_3)))) (let ((?v_7 (concat (_ bv0 24) (select ?v_3 ?v_4)))) (let ((?v_5 (store ?v_3 ?v_4 ((_ extract 7 0) (bvadd ?v_7 (_ bv1 32))))) (?v_8 (concat (_ bv0 24) ((_ extract 7 0) ?v_7)))) (let ((?v_9 (bvand (bvsub ?v_8 (_ bv56 32)) (_ bv255 32)))) (let ((?v_10 (ite (= (_ bv1 32) (bvand (_ bv1 32) (bvlshr (bvand (bvxor ?v_8 (_ bv56 32)) (bvxor ?v_8 ?v_9)) (_ bv7 32)))) (_ bv1 1) (_ bv0 1)))) (= (_ bv1 1) (bvand (ite (not (= (bvor (bvor (bvor (concat (_ bv0 24) (select mem_35_57_4 (bvadd R_ESP_1_58_1 (_ bv0 32)))) (bvshl (concat (_ bv0 24) (select mem_35_57_4 (bvadd R_ESP_1_58_1 (_ bv1 32)))) (_ bv8 32))) (bvshl (concat (_ bv0 24) (select mem_35_57_4 (bvadd R_ESP_1_58_1 (_ bv2 32)))) (_ bv16 32))) (bvshl (concat (_ bv0 24) (select mem_35_57_4 (bvadd R_ESP_1_58_1 (_ bv3 32)))) (_ bv24 32))) (bvor (bvor (bvor (concat (_ bv0 24) (select ?v_5 (bvadd ?v_6 (_ bv0 32)))) (bvshl (concat (_ bv0 24) (select ?v_5 (bvadd ?v_6 (_ bv1 32)))) (_ bv8 32))) (bvshl (concat (_ bv0 24) (select ?v_5 (bvadd ?v_6 (_ bv2 32)))) (_ bv16 32))) (bvshl (concat (_ bv0 24) (select ?v_5 (bvadd ?v_6 (_ bv3 32)))) (_ bv24 32))))) (_ bv1 1) (_ bv0 1)) (bvand ((_ extract 0 0) (concat (_ bv0 31) (bvor (bvxor (ite (= (_ bv1 32) (bvand (_ bv1 32) (bvlshr ?v_9 (_ bv7 32)))) (_ bv1 1) (_ bv0 1)) ?v_10) (ite (= ?v_9 (_ bv0 32)) (_ bv1 1) (_ bv0 1))))) (bvand (bvnot ?v_10) (_ bv1 1))))))))))))))
(check-sat)
(exit)
