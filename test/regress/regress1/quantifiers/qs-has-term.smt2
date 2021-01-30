(set-info :smt-lib-version 2.6)
(set-logic AUFLIRA)
(set-info :source |
NASA benchmarks from "Using Automated Theorem Provers to Certify Auto-generated
Aerospace Software", IJCAR 2004.  Translated from TPTP format by Yeting Ge and
Clark Barrett
|)
(set-info :category "industrial")
(set-info :status unsat)
(declare-fun def () Real)
(declare-fun dv_ds1_filter_init () (Array Int (Array Int Real)))
(declare-fun h_ds1_filter_init () (Array Int (Array Int Real)))
(declare-fun id_ds1_filter_init () (Array Int (Array Int Real)))
(declare-fun init () Real)
(declare-fun phi_ds1_filter_init () (Array Int (Array Int Real)))
(declare-fun pminus_ds1_filter_init () (Array Int (Array Int Real)))
(declare-fun pv5 () Int)
(declare-fun q_ds1_filter_init () (Array Int (Array Int Real)))
(declare-fun r_ds1_filter_init () (Array Int (Array Int Real)))
(declare-fun use () Real)
(declare-fun xhatmin_ds1_filter_init () (Array Int (Array Int Real)))
(declare-fun zhat_ds1_filter_init () (Array Int (Array Int Real)))
(declare-fun zpred_ds1_filter_init () (Array Int (Array Int Real)))
(declare-fun uniform_int_rnd (Int Int) Int)
(declare-fun abs_ (Real) Real)
(declare-fun log (Real) Real)
(declare-fun exp (Real) Real)
(declare-fun cos (Real) Real)
(declare-fun sin (Real) Real)
(declare-fun sqrt (Real) Real)
(declare-fun divide (Real Real) Real)
(declare-fun cond (Int Real Real) Real)
(declare-fun tptp_term_equal (Real Real) Int)
(declare-fun tptp_term_equals (Real Real) Int)
(declare-fun tptp_term_and (Real Real) Int)
(declare-fun sum (Int Int Real) Real)
(declare-fun dim (Int Int) Int)
(declare-fun trans ((Array Int (Array Int Real))) (Array Int (Array Int Real)))
(declare-fun inv ((Array Int (Array Int Real))) (Array Int (Array Int Real)))
(declare-fun tptp_mmul ((Array Int (Array Int Real)) (Array Int (Array Int Real))) (Array Int (Array Int Real)))
(declare-fun tptp_madd ((Array Int (Array Int Real)) (Array Int (Array Int Real))) (Array Int (Array Int Real)))
(declare-fun tptp_msub ((Array Int (Array Int Real)) (Array Int (Array Int Real))) (Array Int (Array Int Real)))
(declare-fun tptp_const_array1 (Int Real) (Array Int Real))
(declare-fun tptp_const_array2 (Int Int Real) (Array Int (Array Int Real)))
(assert (forall ((?X_0 Int) (?C_1 Int)) (=> (>= ?X_0 0) (<= (uniform_int_rnd ?C_1 ?X_0) ?X_0))))
(assert (forall ((?X_2 Int) (?C_3 Int)) (=> (>= ?X_2 0) (>= (uniform_int_rnd ?C_3 ?X_2) 0))))
(assert (forall ((?I_4 Int) (?L_5 Int) (?U_6 Int) (?Val_7 Real)) (=> (and (<= ?L_5 ?I_4) (<= ?I_4 ?U_6)) (= (select (tptp_const_array1 (dim ?L_5 ?U_6) ?Val_7) ?I_4) ?Val_7))))
(assert (forall ((?I_8 Int) (?L1_9 Int) (?U1_10 Int) (?J_11 Int) (?L2_12 Int) (?U2_13 Int) (?Val_14 Real)) (=> (and (and (and (<= ?L1_9 ?I_8) (<= ?I_8 ?U1_10)) (<= ?L2_12 ?J_11)) (<= ?J_11 ?U2_13)) (= (select (select (tptp_const_array2 (dim ?L1_9 ?U1_10) (dim ?L2_12 ?U2_13) ?Val_14) ?I_8) ?J_11) ?Val_14))))
(assert (forall ((?I0_15 Int) (?J0_16 Int) (?A_17 (Array Int (Array Int Real))) (?B_18 (Array Int (Array Int Real))) (?N_19 Int)) (let ((?v_0 (tptp_mmul ?A_17 (tptp_mmul ?B_18 (trans ?A_17))))) (=> (and (and (and (and (>= ?I0_15 0) (<= ?I0_15 ?N_19)) (>= ?J0_16 0)) (<= ?J0_16 ?N_19)) (= (select (select ?B_18 ?I0_15) ?J0_16) (select (select ?B_18 ?J0_16) ?I0_15))) (= (select (select ?v_0 ?I0_15) ?J0_16) (select (select ?v_0 ?J0_16) ?I0_15))))))
(assert (forall ((?I0_20 Int) (?J0_21 Int) (?I_22 Int) (?J_23 Int) (?A_24 (Array Int (Array Int Real))) (?B_25 (Array Int (Array Int Real))) (?N_26 Int) (?M_27 Int)) (let ((?v_0 (tptp_mmul ?A_24 (tptp_mmul ?B_25 (trans ?A_24))))) (=> (and (and (and (and (and (and (and (and (>= ?I0_20 0) (<= ?I0_20 ?N_26)) (>= ?J0_21 0)) (<= ?J0_21 ?N_26)) (>= ?I_22 0)) (<= ?I_22 ?M_27)) (>= ?J_23 0)) (<= ?J_23 ?M_27)) (= (select (select ?B_25 ?I_22) ?J_23) (select (select ?B_25 ?J_23) ?I_22))) (= (select (select ?v_0 ?I0_20) ?J0_21) (select (select ?v_0 ?J0_21) ?I0_20))))))
(assert (forall ((?I_28 Int) (?J_29 Int) (?A_30 (Array Int (Array Int Real))) (?B_31 (Array Int (Array Int Real))) (?N_32 Int)) (let ((?v_0 (tptp_madd ?A_30 ?B_31))) (=> (and (and (and (and (and (>= ?I_28 0) (<= ?I_28 ?N_32)) (>= ?J_29 0)) (<= ?J_29 ?N_32)) (= (select (select ?A_30 ?I_28) ?J_29) (select (select ?A_30 ?J_29) ?I_28))) (= (select (select ?B_31 ?I_28) ?J_29) (select (select ?B_31 ?J_29) ?I_28))) (= (select (select ?v_0 ?I_28) ?J_29) (select (select ?v_0 ?J_29) ?I_28))))))
(assert (forall ((?I_33 Int) (?J_34 Int) (?A_35 (Array Int (Array Int Real))) (?B_36 (Array Int (Array Int Real))) (?N_37 Int)) (let ((?v_0 (tptp_msub ?A_35 ?B_36))) (=> (and (and (and (and (and (>= ?I_33 0) (<= ?I_33 ?N_37)) (>= ?J_34 0)) (<= ?J_34 ?N_37)) (= (select (select ?A_35 ?I_33) ?J_34) (select (select ?A_35 ?J_34) ?I_33))) (= (select (select ?B_36 ?I_33) ?J_34) (select (select ?B_36 ?J_34) ?I_33))) (= (select (select ?v_0 ?I_33) ?J_34) (select (select ?v_0 ?J_34) ?I_33))))))
(assert (forall ((?I_38 Int) (?J_39 Int) (?A_40 (Array Int (Array Int Real))) (?N_41 Int)) (let ((?v_0 (trans ?A_40))) (=> (and (and (and (and (>= ?I_38 0) (<= ?I_38 ?N_41)) (>= ?J_39 0)) (<= ?J_39 ?N_41)) (= (select (select ?A_40 ?I_38) ?J_39) (select (select ?A_40 ?J_39) ?I_38))) (= (select (select ?v_0 ?I_38) ?J_39) (select (select ?v_0 ?J_39) ?I_38))))))
(assert (forall ((?I_42 Int) (?J_43 Int) (?A_44 (Array Int (Array Int Real))) (?N_45 Int)) (let ((?v_0 (inv ?A_44))) (=> (and (and (and (and (>= ?I_42 0) (<= ?I_42 ?N_45)) (>= ?J_43 0)) (<= ?J_43 ?N_45)) (= (select (select ?A_44 ?I_42) ?J_43) (select (select ?A_44 ?J_43) ?I_42))) (= (select (select ?v_0 ?I_42) ?J_43) (select (select ?v_0 ?J_43) ?I_42))))))
(assert (forall ((?I0_46 Int) (?J0_47 Int) (?I_48 Int) (?J_49 Int) (?A_50 (Array Int (Array Int Real))) (?B_51 (Array Int (Array Int Real))) (?C_52 (Array Int (Array Int Real))) (?D_53 (Array Int (Array Int Real))) (?E_54 (Array Int (Array Int Real))) (?F_55 (Array Int (Array Int Real))) (?N_56 Int) (?M_57 Int)) (let ((?v_0 (tptp_madd ?A_50 (tptp_mmul ?B_51 (tptp_mmul (tptp_madd (tptp_mmul ?C_52 (tptp_mmul ?D_53 (trans ?C_52))) (tptp_mmul ?E_54 (tptp_mmul ?F_55 (trans ?E_54)))) (trans ?B_51)))))) (=> (and (and (and (and (and (and (and (and (and (and (>= ?I0_46 0) (<= ?I0_46 ?N_56)) (>= ?J0_47 0)) (<= ?J0_47 ?N_56)) (>= ?I_48 0)) (<= ?I_48 ?M_57)) (>= ?J_49 0)) (<= ?J_49 ?M_57)) (= (select (select ?D_53 ?I_48) ?J_49) (select (select ?D_53 ?J_49) ?I_48))) (= (select (select ?A_50 ?I0_46) ?J0_47) (select (select ?A_50 ?J0_47) ?I0_46))) (= (select (select ?F_55 ?I0_46) ?J0_47) (select (select ?F_55 ?J0_47) ?I0_46))) (= (select (select ?v_0 ?I0_46) ?J0_47) (select (select ?v_0 ?J0_47) ?I0_46))))))
(assert (forall ((?Body_58 Real)) (= (sum 0 (- 1) ?Body_58) 0.0)))
(assert (not (= def use)))
(assert (let ((?v_0 (= init init)) (?v_6 (>= pv5 0)) (?v_1 (- 999 1))) (let ((?v_7 (<= pv5 ?v_1))) (let ((?v_4 (and (and ?v_0 ?v_6) ?v_7)) (?v_9 (dim 0 ?v_1))) (let ((?v_2 (tptp_const_array2 (dim 0 (- 3 1)) ?v_9 init)) (?v_5 (> pv5 0))) (let ((?v_3 (not ?v_5)) (?v_8 (and (and (and ?v_0 (= (select (select xhatmin_ds1_filter_init 3) 0) init)) (= (select (select xhatmin_ds1_filter_init 4) 0) init)) (= (select (select xhatmin_ds1_filter_init 5) 0) init)))) (let ((?v_11 (and (and ?v_8 ?v_6) ?v_7)) (?v_10 (tptp_const_array2 (dim 0 2) ?v_9 init))) (not (=> (and (and (and (and (and (and (and (and ?v_4 (forall ((?A_59 Int) (?B_60 Int)) (=> (and (and (and (>= ?A_59 0) (>= ?B_60 0)) (<= ?A_59 2)) (<= ?B_60 5)) (= (select (select h_ds1_filter_init ?A_59) ?B_60) init)))) (forall ((?C_61 Int) (?D_62 Int)) (=> (and (and (and (>= ?C_61 0) (>= ?D_62 0)) (<= ?C_61 5)) (<= ?D_62 5)) (= (select (select phi_ds1_filter_init ?C_61) ?D_62) init)))) (forall ((?E_63 Int) (?F_64 Int)) (=> (and (and (and (>= ?E_63 0) (>= ?F_64 0)) (<= ?E_63 (- 6 1))) (<= ?F_64 0)) (= (select (select dv_ds1_filter_init ?E_63) ?F_64) init)))) (forall ((?G_65 Int) (?H_66 Int)) (=> (and (and (and (>= ?G_65 0) (>= ?H_66 0)) (<= ?G_65 5)) (<= ?H_66 5)) (= (select (select q_ds1_filter_init ?G_65) ?H_66) init)))) (forall ((?I_67 Int) (?J_68 Int)) (=> (and (and (and (>= ?I_67 0) (>= ?J_68 0)) (<= ?I_67 2)) (<= ?J_68 2)) (= (select (select r_ds1_filter_init ?I_67) ?J_68) init)))) (forall ((?K_69 Int) (?L_70 Int)) (=> (and (and (and (>= ?K_69 0) (>= ?L_70 0)) (<= ?K_69 5)) (<= ?L_70 0)) (= (select (select xhatmin_ds1_filter_init ?K_69) ?L_70) init)))) (forall ((?M_71 Int) (?N_72 Int)) (=> (and (and (and (>= ?M_71 0) (>= ?N_72 0)) (<= ?M_71 5)) (<= ?N_72 5)) (= (select (select pminus_ds1_filter_init ?M_71) ?N_72) init)))) (forall ((?O_73 Int)) (=> (and (>= ?O_73 0) (<= ?O_73 (- 6 1))) (forall ((?P_74 Int)) (=> (and (>= ?P_74 0) (<= ?P_74 (- 6 1))) (= (select (select id_ds1_filter_init ?O_73) ?P_74) init)))))) (and (and (and (and (and (and (and (and ?v_0 (= (select (select xhatmin_ds1_filter_init 0) 0) init)) (= (select (select xhatmin_ds1_filter_init 1) 0) init)) (= (select (select xhatmin_ds1_filter_init 2) 0) init)) (= (select (select ?v_2 0) pv5) init)) (= (select (select ?v_2 1) pv5) init)) (= (select (select ?v_2 2) pv5) init)) (=> ?v_3 (and (and ?v_0 (=> ?v_3 (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and ?v_4 (forall ((?Q_75 Int) (?R_76 Int)) (=> (and (and (and (>= ?Q_75 0) (>= ?R_76 0)) (<= ?Q_75 2)) (<= ?R_76 5)) (= (select (select h_ds1_filter_init ?Q_75) ?R_76) init)))) (forall ((?S_77 Int) (?T_78 Int)) (=> (and (and (and (>= ?S_77 0) (>= ?T_78 0)) (<= ?S_77 5)) (<= ?T_78 5)) (= (select (select phi_ds1_filter_init ?S_77) ?T_78) init)))) (forall ((?U_79 Int) (?V_80 Int)) (=> (and (and (and (>= ?U_79 0) (>= ?V_80 0)) (<= ?U_79 (- 6 1))) (<= ?V_80 0)) (= (select (select dv_ds1_filter_init ?U_79) ?V_80) init)))) (forall ((?W_81 Int) (?X_82 Int)) (=> (and (and (and (>= ?W_81 0) (>= ?X_82 0)) (<= ?W_81 5)) (<= ?X_82 5)) (= (select (select q_ds1_filter_init ?W_81) ?X_82) init)))) (forall ((?Y_83 Int) (?Z_84 Int)) (=> (and (and (and (>= ?Y_83 0) (>= ?Z_84 0)) (<= ?Y_83 2)) (<= ?Z_84 2)) (= (select (select r_ds1_filter_init ?Y_83) ?Z_84) init)))) (forall ((?A1_85 Int) (?B1_86 Int)) (=> (and (and (and (>= ?A1_85 0) (>= ?B1_86 0)) (<= ?A1_85 5)) (<= ?B1_86 0)) (= (select (select (tptp_const_array2 (dim 0 (- 6 1)) (dim 0 0) init) ?A1_85) ?B1_86) init)))) (forall ((?C1_87 Int) (?D1_88 Int)) (let ((?v_12 (dim 0 (- 6 1)))) (=> (and (and (and (>= ?C1_87 0) (>= ?D1_88 0)) (<= ?C1_87 5)) (<= ?D1_88 5)) (= (select (select (tptp_const_array2 ?v_12 ?v_12 init) ?C1_87) ?D1_88) init))))) (forall ((?E1_89 Int)) (=> (and (>= ?E1_89 0) (<= ?E1_89 (- 6 1))) (forall ((?F1_90 Int)) (=> (and (>= ?F1_90 0) (<= ?F1_90 (- 6 1))) (= (select (select id_ds1_filter_init ?E1_89) ?F1_90) init)))))) (forall ((?G1_91 Int) (?H1_92 Int)) (let ((?v_13 (- 6 1))) (=> (and (and (and (>= ?G1_91 0) (>= ?H1_92 0)) (<= ?G1_91 ?v_13)) (<= ?H1_92 ?v_13)) (= (select (select q_ds1_filter_init ?G1_91) ?H1_92) init))))) (forall ((?I1_93 Int) (?J1_94 Int)) (let ((?v_14 (- 6 1))) (=> (and (and (and (>= ?I1_93 0) (>= ?J1_94 0)) (<= ?I1_93 ?v_14)) (<= ?J1_94 ?v_14)) (= (select (select phi_ds1_filter_init ?I1_93) ?J1_94) init))))) (forall ((?K1_95 Int) (?L1_96 Int)) (let ((?v_15 (- 6 1))) (let ((?v_16 (dim 0 ?v_15))) (=> (and (and (and (>= ?K1_95 0) (>= ?L1_96 0)) (<= ?K1_95 ?v_15)) (<= ?L1_96 ?v_15)) (= (select (select (tptp_const_array2 ?v_16 ?v_16 init) ?K1_95) ?L1_96) init)))))) (forall ((?M1_97 Int) (?N1_98 Int)) (let ((?v_17 (- 6 1))) (=> (and (and (and (>= ?M1_97 0) (>= ?N1_98 0)) (<= ?M1_97 ?v_17)) (<= ?N1_98 ?v_17)) (= (select (select phi_ds1_filter_init ?M1_97) ?N1_98) init))))) (forall ((?O1_99 Int) (?P1_100 Int)) (=> (and (and (and (>= ?O1_99 0) (>= ?P1_100 0)) (<= ?O1_99 (- 6 1))) (<= ?P1_100 0)) (= (select (select dv_ds1_filter_init ?O1_99) ?P1_100) init)))) (forall ((?Q1_101 Int) (?R1_102 Int)) (let ((?v_18 (- 6 1))) (=> (and (and (and (>= ?Q1_101 0) (>= ?R1_102 0)) (<= ?Q1_101 ?v_18)) (<= ?R1_102 ?v_18)) (= (select (select phi_ds1_filter_init ?Q1_101) ?R1_102) init))))) (forall ((?S1_103 Int) (?T1_104 Int)) (let ((?v_19 (- 6 1))) (=> (and (and (and (>= ?S1_103 0) (>= ?T1_104 0)) (<= ?S1_103 ?v_19)) (<= ?T1_104 0)) (= (select (select (tptp_const_array2 (dim 0 ?v_19) (dim 0 0) init) ?S1_103) ?T1_104) init))))) (forall ((?U1_105 Int) (?V1_106 Int)) (let ((?v_20 (- 6 1))) (=> (and (and (and (>= ?U1_105 0) (>= ?V1_106 0)) (<= ?U1_105 ?v_20)) (<= ?V1_106 ?v_20)) (= (select (select id_ds1_filter_init ?U1_105) ?V1_106) init))))) (forall ((?W1_107 Int) (?X1_108 Int)) (let ((?v_22 (- 3 1)) (?v_21 (- 6 1))) (=> (and (and (and (>= ?W1_107 0) (>= ?X1_108 0)) (<= ?W1_107 ?v_21)) (<= ?X1_108 ?v_22)) (= (select (select (tptp_const_array2 (dim 0 ?v_21) (dim 0 ?v_22) init) ?W1_107) ?X1_108) init))))) (forall ((?Y1_109 Int) (?Z1_110 Int)) (=> (and (and (and (>= ?Y1_109 0) (>= ?Z1_110 0)) (<= ?Y1_109 (- 3 1))) (<= ?Z1_110 (- 6 1))) (= (select (select h_ds1_filter_init ?Y1_109) ?Z1_110) init)))) (forall ((?A2_111 Int) (?B2_112 Int)) (let ((?v_23 (- 6 1))) (=> (and (and (and (>= ?A2_111 0) (>= ?B2_112 0)) (<= ?A2_111 ?v_23)) (<= ?B2_112 ?v_23)) (= (select (select pminus_ds1_filter_init ?A2_111) ?B2_112) init))))) (forall ((?C2_113 Int) (?D2_114 Int)) (=> (and (and (and (>= ?C2_113 0) (>= ?D2_114 0)) (<= ?C2_113 (- 6 1))) (<= ?D2_114 0)) (= (select (select xhatmin_ds1_filter_init ?C2_113) ?D2_114) init)))) (forall ((?E2_115 Int) (?F2_116 Int)) (let ((?v_25 (- 3 1)) (?v_24 (- 6 1))) (=> (and (and (and (>= ?E2_115 0) (>= ?F2_116 0)) (<= ?E2_115 ?v_24)) (<= ?F2_116 ?v_25)) (= (select (select (tptp_const_array2 (dim 0 ?v_24) (dim 0 ?v_25) init) ?E2_115) ?F2_116) init))))) (forall ((?G2_117 Int) (?H2_118 Int)) (let ((?v_26 (store zhat_ds1_filter_init 0 (store (select zhat_ds1_filter_init 0) 0 init)))) (let ((?v_27 (store ?v_26 1 (store (select ?v_26 1) 0 init)))) (=> (and (and (and (>= ?G2_117 0) (>= ?H2_118 0)) (<= ?G2_117 (- 3 1))) (<= ?H2_118 0)) (= (select (select (store ?v_27 2 (store (select ?v_27 2) 0 init)) ?G2_117) ?H2_118) init)))))) (forall ((?I2_119 Int) (?J2_120 Int)) (let ((?v_28 (store zpred_ds1_filter_init 0 (store (select zpred_ds1_filter_init 0) 0 init)))) (let ((?v_29 (store ?v_28 1 (store (select ?v_28 1) 0 init)))) (=> (and (and (and (>= ?I2_119 0) (>= ?J2_120 0)) (<= ?I2_119 (- 3 1))) (<= ?J2_120 0)) (= (select (select (store ?v_29 2 (store (select ?v_29 2) 0 init)) ?I2_119) ?J2_120) init)))))) (forall ((?K2_121 Int) (?L2_122 Int)) (let ((?v_30 (- 6 1))) (=> (and (and (and (>= ?K2_121 0) (>= ?L2_122 0)) (<= ?K2_121 ?v_30)) (<= ?L2_122 ?v_30)) (= (select (select pminus_ds1_filter_init ?K2_121) ?L2_122) init))))) (forall ((?M2_123 Int) (?N2_124 Int)) (=> (and (and (and (>= ?M2_123 0) (>= ?N2_124 0)) (<= ?M2_123 (- 3 1))) (<= ?N2_124 (- 6 1))) (= (select (select h_ds1_filter_init ?M2_123) ?N2_124) init)))) (forall ((?O2_125 Int) (?P2_126 Int)) (let ((?v_31 (- 3 1))) (=> (and (and (and (>= ?O2_125 0) (>= ?P2_126 0)) (<= ?O2_125 ?v_31)) (<= ?P2_126 ?v_31)) (= (select (select r_ds1_filter_init ?O2_125) ?P2_126) init))))) (forall ((?Q2_127 Int) (?R2_128 Int)) (=> (and (and (and (>= ?Q2_127 0) (>= ?R2_128 0)) (<= ?Q2_127 (- 3 1))) (<= ?R2_128 (- 6 1))) (= (select (select h_ds1_filter_init ?Q2_127) ?R2_128) init)))) (forall ((?S2_129 Int) (?T2_130 Int)) (let ((?v_32 (- 6 1))) (=> (and (and (and (>= ?S2_129 0) (>= ?T2_130 0)) (<= ?S2_129 ?v_32)) (<= ?T2_130 ?v_32)) (= (select (select pminus_ds1_filter_init ?S2_129) ?T2_130) init))))) (forall ((?U2_131 Int) (?V2_132 Int)) (=> (and (and (and (>= ?U2_131 0) (>= ?V2_132 0)) (<= ?U2_131 (- 3 1))) (<= ?V2_132 (- 6 1))) (= (select (select h_ds1_filter_init ?U2_131) ?V2_132) init)))))) (=> ?v_5 (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and ?v_11 (forall ((?W2_133 Int) (?X2_134 Int)) (=> (and (and (and (>= ?W2_133 0) (>= ?X2_134 0)) (<= ?W2_133 2)) (<= ?X2_134 5)) (= (select (select h_ds1_filter_init ?W2_133) ?X2_134) init)))) (forall ((?Y2_135 Int) (?Z2_136 Int)) (=> (and (and (and (>= ?Y2_135 0) (>= ?Z2_136 0)) (<= ?Y2_135 5)) (<= ?Z2_136 5)) (= (select (select phi_ds1_filter_init ?Y2_135) ?Z2_136) init)))) (forall ((?A3_137 Int) (?B3_138 Int)) (let ((?v_33 (store dv_ds1_filter_init 2 (store (select dv_ds1_filter_init 2) 0 init)))) (let ((?v_34 (store ?v_33 1 (store (select ?v_33 1) 0 init)))) (=> (and (and (and (>= ?A3_137 0) (>= ?B3_138 0)) (<= ?A3_137 (- 6 1))) (<= ?B3_138 0)) (= (select (select (store ?v_34 0 (store (select ?v_34 0) 0 init)) ?A3_137) ?B3_138) init)))))) (forall ((?C3_139 Int) (?D3_140 Int)) (=> (and (and (and (>= ?C3_139 0) (>= ?D3_140 0)) (<= ?C3_139 5)) (<= ?D3_140 5)) (= (select (select q_ds1_filter_init ?C3_139) ?D3_140) init)))) (forall ((?E3_141 Int) (?F3_142 Int)) (=> (and (and (and (>= ?E3_141 0) (>= ?F3_142 0)) (<= ?E3_141 2)) (<= ?F3_142 2)) (= (select (select r_ds1_filter_init ?E3_141) ?F3_142) init)))) (forall ((?G3_143 Int) (?H3_144 Int)) (=> (and (and (and (>= ?G3_143 0) (>= ?H3_144 0)) (<= ?G3_143 5)) (<= ?H3_144 0)) (= (select (select (tptp_const_array2 (dim 0 (- 6 1)) (dim 0 0) init) ?G3_143) ?H3_144) init)))) (forall ((?I3_145 Int) (?J3_146 Int)) (let ((?v_35 (dim 0 (- 6 1)))) (=> (and (and (and (>= ?I3_145 0) (>= ?J3_146 0)) (<= ?I3_145 5)) (<= ?J3_146 5)) (= (select (select (tptp_const_array2 ?v_35 ?v_35 init) ?I3_145) ?J3_146) init))))) (forall ((?K3_147 Int)) (=> (and (>= ?K3_147 0) (<= ?K3_147 (- 6 1))) (forall ((?L3_148 Int)) (=> (and (>= ?L3_148 0) (<= ?L3_148 (- 6 1))) (= (select (select id_ds1_filter_init ?K3_147) ?L3_148) init)))))) (forall ((?M3_149 Int) (?N3_150 Int)) (let ((?v_36 (- 6 1))) (=> (and (and (and (>= ?M3_149 0) (>= ?N3_150 0)) (<= ?M3_149 ?v_36)) (<= ?N3_150 ?v_36)) (= (select (select q_ds1_filter_init ?M3_149) ?N3_150) init))))) (forall ((?O3_151 Int) (?P3_152 Int)) (let ((?v_37 (- 6 1))) (=> (and (and (and (>= ?O3_151 0) (>= ?P3_152 0)) (<= ?O3_151 ?v_37)) (<= ?P3_152 ?v_37)) (= (select (select phi_ds1_filter_init ?O3_151) ?P3_152) init))))) (forall ((?Q3_153 Int) (?R3_154 Int)) (let ((?v_38 (- 6 1))) (let ((?v_39 (dim 0 ?v_38))) (=> (and (and (and (>= ?Q3_153 0) (>= ?R3_154 0)) (<= ?Q3_153 ?v_38)) (<= ?R3_154 ?v_38)) (= (select (select (tptp_const_array2 ?v_39 ?v_39 init) ?Q3_153) ?R3_154) init)))))) (forall ((?S3_155 Int) (?T3_156 Int)) (let ((?v_40 (- 6 1))) (=> (and (and (and (>= ?S3_155 0) (>= ?T3_156 0)) (<= ?S3_155 ?v_40)) (<= ?T3_156 ?v_40)) (= (select (select phi_ds1_filter_init ?S3_155) ?T3_156) init))))) (forall ((?U3_157 Int) (?V3_158 Int)) (let ((?v_41 (store dv_ds1_filter_init 2 (store (select dv_ds1_filter_init 2) 0 init)))) (let ((?v_42 (store ?v_41 1 (store (select ?v_41 1) 0 init)))) (=> (and (and (and (>= ?U3_157 0) (>= ?V3_158 0)) (<= ?U3_157 (- 6 1))) (<= ?V3_158 0)) (= (select (select (store ?v_42 0 (store (select ?v_42 0) 0 init)) ?U3_157) ?V3_158) init)))))) (forall ((?W3_159 Int) (?X3_160 Int)) (let ((?v_43 (- 6 1))) (=> (and (and (and (>= ?W3_159 0) (>= ?X3_160 0)) (<= ?W3_159 ?v_43)) (<= ?X3_160 ?v_43)) (= (select (select phi_ds1_filter_init ?W3_159) ?X3_160) init))))) (forall ((?Y3_161 Int) (?Z3_162 Int)) (let ((?v_44 (- 6 1))) (=> (and (and (and (>= ?Y3_161 0) (>= ?Z3_162 0)) (<= ?Y3_161 ?v_44)) (<= ?Z3_162 0)) (= (select (select (tptp_const_array2 (dim 0 ?v_44) (dim 0 0) init) ?Y3_161) ?Z3_162) init))))) (forall ((?A4_163 Int) (?B4_164 Int)) (let ((?v_45 (- 6 1))) (=> (and (and (and (>= ?A4_163 0) (>= ?B4_164 0)) (<= ?A4_163 ?v_45)) (<= ?B4_164 ?v_45)) (= (select (select id_ds1_filter_init ?A4_163) ?B4_164) init))))) (forall ((?C4_165 Int) (?D4_166 Int)) (let ((?v_47 (- 3 1)) (?v_46 (- 6 1))) (=> (and (and (and (>= ?C4_165 0) (>= ?D4_166 0)) (<= ?C4_165 ?v_46)) (<= ?D4_166 ?v_47)) (= (select (select (tptp_const_array2 (dim 0 ?v_46) (dim 0 ?v_47) init) ?C4_165) ?D4_166) init))))) (forall ((?E4_167 Int) (?F4_168 Int)) (=> (and (and (and (>= ?E4_167 0) (>= ?F4_168 0)) (<= ?E4_167 (- 3 1))) (<= ?F4_168 (- 6 1))) (= (select (select h_ds1_filter_init ?E4_167) ?F4_168) init)))) (forall ((?G4_169 Int) (?H4_170 Int)) (let ((?v_48 (- 6 1))) (=> (and (and (and (>= ?G4_169 0) (>= ?H4_170 0)) (<= ?G4_169 ?v_48)) (<= ?H4_170 ?v_48)) (= (select (select pminus_ds1_filter_init ?G4_169) ?H4_170) init))))) (forall ((?I4_171 Int) (?J4_172 Int)) (=> (and (and (and (>= ?I4_171 0) (>= ?J4_172 0)) (<= ?I4_171 (- 6 1))) (<= ?J4_172 0)) (= (select (select xhatmin_ds1_filter_init ?I4_171) ?J4_172) init)))) (forall ((?K4_173 Int) (?L4_174 Int)) (let ((?v_50 (- 3 1)) (?v_49 (- 6 1))) (=> (and (and (and (>= ?K4_173 0) (>= ?L4_174 0)) (<= ?K4_173 ?v_49)) (<= ?L4_174 ?v_50)) (= (select (select (tptp_const_array2 (dim 0 ?v_49) (dim 0 ?v_50) init) ?K4_173) ?L4_174) init))))) (forall ((?M4_175 Int) (?N4_176 Int)) (let ((?v_51 (store zhat_ds1_filter_init 0 (store (select zhat_ds1_filter_init 0) 0 init)))) (let ((?v_52 (store ?v_51 1 (store (select ?v_51 1) 0 init)))) (=> (and (and (and (>= ?M4_175 0) (>= ?N4_176 0)) (<= ?M4_175 (- 3 1))) (<= ?N4_176 0)) (= (select (select (store ?v_52 2 (store (select ?v_52 2) 0 init)) ?M4_175) ?N4_176) init)))))) (forall ((?O4_177 Int) (?P4_178 Int)) (let ((?v_53 (store zpred_ds1_filter_init 0 (store (select zpred_ds1_filter_init 0) 0 init)))) (let ((?v_54 (store ?v_53 1 (store (select ?v_53 1) 0 init)))) (=> (and (and (and (>= ?O4_177 0) (>= ?P4_178 0)) (<= ?O4_177 (- 3 1))) (<= ?P4_178 0)) (= (select (select (store ?v_54 2 (store (select ?v_54 2) 0 init)) ?O4_177) ?P4_178) init)))))) (forall ((?Q4_179 Int) (?R4_180 Int)) (let ((?v_55 (- 6 1))) (=> (and (and (and (>= ?Q4_179 0) (>= ?R4_180 0)) (<= ?Q4_179 ?v_55)) (<= ?R4_180 ?v_55)) (= (select (select pminus_ds1_filter_init ?Q4_179) ?R4_180) init))))) (forall ((?S4_181 Int) (?T4_182 Int)) (=> (and (and (and (>= ?S4_181 0) (>= ?T4_182 0)) (<= ?S4_181 (- 3 1))) (<= ?T4_182 (- 6 1))) (= (select (select h_ds1_filter_init ?S4_181) ?T4_182) init)))) (forall ((?U4_183 Int) (?V4_184 Int)) (let ((?v_56 (- 3 1))) (=> (and (and (and (>= ?U4_183 0) (>= ?V4_184 0)) (<= ?U4_183 ?v_56)) (<= ?V4_184 ?v_56)) (= (select (select r_ds1_filter_init ?U4_183) ?V4_184) init))))) (forall ((?W4_185 Int) (?X4_186 Int)) (=> (and (and (and (>= ?W4_185 0) (>= ?X4_186 0)) (<= ?W4_185 (- 3 1))) (<= ?X4_186 (- 6 1))) (= (select (select h_ds1_filter_init ?W4_185) ?X4_186) init)))) (forall ((?Y4_187 Int) (?Z4_188 Int)) (let ((?v_57 (- 6 1))) (=> (and (and (and (>= ?Y4_187 0) (>= ?Z4_188 0)) (<= ?Y4_187 ?v_57)) (<= ?Z4_188 ?v_57)) (= (select (select pminus_ds1_filter_init ?Y4_187) ?Z4_188) init))))) (forall ((?A5_189 Int) (?B5_190 Int)) (=> (and (and (and (>= ?A5_189 0) (>= ?B5_190 0)) (<= ?A5_189 (- 3 1))) (<= ?B5_190 (- 6 1))) (= (select (select h_ds1_filter_init ?A5_189) ?B5_190) init)))))))) (=> ?v_5 (and (and (and (and (and ?v_8 (= (select (select ?v_10 0) pv5) init)) (= (select (select ?v_10 1) pv5) init)) (= (select (select ?v_10 2) pv5) init)) (=> ?v_3 (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and ?v_4 (forall ((?C5_191 Int) (?D5_192 Int)) (=> (and (and (and (>= ?C5_191 0) (>= ?D5_192 0)) (<= ?C5_191 2)) (<= ?D5_192 5)) (= (select (select h_ds1_filter_init ?C5_191) ?D5_192) init)))) (forall ((?E5_193 Int) (?F5_194 Int)) (let ((?v_58 (store phi_ds1_filter_init 2 (store (select phi_ds1_filter_init 2) 1 init)))) (let ((?v_59 (store ?v_58 2 (store (select ?v_58 2) 0 init)))) (let ((?v_60 (store ?v_59 1 (store (select ?v_59 1) 2 init)))) (let ((?v_61 (store ?v_60 1 (store (select ?v_60 1) 0 init)))) (let ((?v_62 (store ?v_61 0 (store (select ?v_61 0) 2 init)))) (=> (and (and (and (>= ?E5_193 0) (>= ?F5_194 0)) (<= ?E5_193 5)) (<= ?F5_194 5)) (= (select (select (store ?v_62 0 (store (select ?v_62 0) 1 init)) ?E5_193) ?F5_194) init))))))))) (forall ((?G5_195 Int) (?H5_196 Int)) (=> (and (and (and (>= ?G5_195 0) (>= ?H5_196 0)) (<= ?G5_195 (- 6 1))) (<= ?H5_196 0)) (= (select (select dv_ds1_filter_init ?G5_195) ?H5_196) init)))) (forall ((?I5_197 Int) (?J5_198 Int)) (=> (and (and (and (>= ?I5_197 0) (>= ?J5_198 0)) (<= ?I5_197 5)) (<= ?J5_198 5)) (= (select (select q_ds1_filter_init ?I5_197) ?J5_198) init)))) (forall ((?K5_199 Int) (?L5_200 Int)) (=> (and (and (and (>= ?K5_199 0) (>= ?L5_200 0)) (<= ?K5_199 2)) (<= ?L5_200 2)) (= (select (select r_ds1_filter_init ?K5_199) ?L5_200) init)))) (forall ((?M5_201 Int) (?N5_202 Int)) (=> (and (and (and (>= ?M5_201 0) (>= ?N5_202 0)) (<= ?M5_201 5)) (<= ?N5_202 0)) (= (select (select (tptp_const_array2 (dim 0 (- 6 1)) (dim 0 0) init) ?M5_201) ?N5_202) init)))) (forall ((?O5_203 Int) (?P5_204 Int)) (let ((?v_63 (dim 0 (- 6 1)))) (=> (and (and (and (>= ?O5_203 0) (>= ?P5_204 0)) (<= ?O5_203 5)) (<= ?P5_204 5)) (= (select (select (tptp_const_array2 ?v_63 ?v_63 init) ?O5_203) ?P5_204) init))))) (forall ((?Q5_205 Int)) (=> (and (>= ?Q5_205 0) (<= ?Q5_205 (- 6 1))) (forall ((?R5_206 Int)) (=> (and (>= ?R5_206 0) (<= ?R5_206 (- 6 1))) (= (select (select id_ds1_filter_init ?Q5_205) ?R5_206) init)))))) (forall ((?S5_207 Int) (?T5_208 Int)) (let ((?v_64 (- 6 1))) (=> (and (and (and (>= ?S5_207 0) (>= ?T5_208 0)) (<= ?S5_207 ?v_64)) (<= ?T5_208 ?v_64)) (= (select (select q_ds1_filter_init ?S5_207) ?T5_208) init))))) (forall ((?U5_209 Int) (?V5_210 Int)) (let ((?v_65 (- 6 1)) (?v_66 (store phi_ds1_filter_init 2 (store (select phi_ds1_filter_init 2) 1 init)))) (let ((?v_67 (store ?v_66 2 (store (select ?v_66 2) 0 init)))) (let ((?v_68 (store ?v_67 1 (store (select ?v_67 1) 2 init)))) (let ((?v_69 (store ?v_68 1 (store (select ?v_68 1) 0 init)))) (let ((?v_70 (store ?v_69 0 (store (select ?v_69 0) 2 init)))) (=> (and (and (and (>= ?U5_209 0) (>= ?V5_210 0)) (<= ?U5_209 ?v_65)) (<= ?V5_210 ?v_65)) (= (select (select (store ?v_70 0 (store (select ?v_70 0) 1 init)) ?U5_209) ?V5_210) init))))))))) (forall ((?W5_211 Int) (?X5_212 Int)) (let ((?v_71 (- 6 1))) (let ((?v_72 (dim 0 ?v_71))) (=> (and (and (and (>= ?W5_211 0) (>= ?X5_212 0)) (<= ?W5_211 ?v_71)) (<= ?X5_212 ?v_71)) (= (select (select (tptp_const_array2 ?v_72 ?v_72 init) ?W5_211) ?X5_212) init)))))) (forall ((?Y5_213 Int) (?Z5_214 Int)) (let ((?v_73 (- 6 1)) (?v_74 (store phi_ds1_filter_init 2 (store (select phi_ds1_filter_init 2) 1 init)))) (let ((?v_75 (store ?v_74 2 (store (select ?v_74 2) 0 init)))) (let ((?v_76 (store ?v_75 1 (store (select ?v_75 1) 2 init)))) (let ((?v_77 (store ?v_76 1 (store (select ?v_76 1) 0 init)))) (let ((?v_78 (store ?v_77 0 (store (select ?v_77 0) 2 init)))) (=> (and (and (and (>= ?Y5_213 0) (>= ?Z5_214 0)) (<= ?Y5_213 ?v_73)) (<= ?Z5_214 ?v_73)) (= (select (select (store ?v_78 0 (store (select ?v_78 0) 1 init)) ?Y5_213) ?Z5_214) init))))))))) (forall ((?A6_215 Int) (?B6_216 Int)) (=> (and (and (and (>= ?A6_215 0) (>= ?B6_216 0)) (<= ?A6_215 (- 6 1))) (<= ?B6_216 0)) (= (select (select dv_ds1_filter_init ?A6_215) ?B6_216) init)))) (forall ((?C6_217 Int) (?D6_218 Int)) (let ((?v_79 (- 6 1)) (?v_80 (store phi_ds1_filter_init 2 (store (select phi_ds1_filter_init 2) 1 init)))) (let ((?v_81 (store ?v_80 2 (store (select ?v_80 2) 0 init)))) (let ((?v_82 (store ?v_81 1 (store (select ?v_81 1) 2 init)))) (let ((?v_83 (store ?v_82 1 (store (select ?v_82 1) 0 init)))) (let ((?v_84 (store ?v_83 0 (store (select ?v_83 0) 2 init)))) (=> (and (and (and (>= ?C6_217 0) (>= ?D6_218 0)) (<= ?C6_217 ?v_79)) (<= ?D6_218 ?v_79)) (= (select (select (store ?v_84 0 (store (select ?v_84 0) 1 init)) ?C6_217) ?D6_218) init))))))))) (forall ((?E6_219 Int) (?F6_220 Int)) (let ((?v_85 (- 6 1))) (=> (and (and (and (>= ?E6_219 0) (>= ?F6_220 0)) (<= ?E6_219 ?v_85)) (<= ?F6_220 0)) (= (select (select (tptp_const_array2 (dim 0 ?v_85) (dim 0 0) init) ?E6_219) ?F6_220) init))))) (forall ((?G6_221 Int) (?H6_222 Int)) (let ((?v_86 (- 6 1))) (=> (and (and (and (>= ?G6_221 0) (>= ?H6_222 0)) (<= ?G6_221 ?v_86)) (<= ?H6_222 ?v_86)) (= (select (select id_ds1_filter_init ?G6_221) ?H6_222) init))))) (forall ((?I6_223 Int) (?J6_224 Int)) (let ((?v_88 (- 3 1)) (?v_87 (- 6 1))) (=> (and (and (and (>= ?I6_223 0) (>= ?J6_224 0)) (<= ?I6_223 ?v_87)) (<= ?J6_224 ?v_88)) (= (select (select (tptp_const_array2 (dim 0 ?v_87) (dim 0 ?v_88) init) ?I6_223) ?J6_224) init))))) (forall ((?K6_225 Int) (?L6_226 Int)) (=> (and (and (and (>= ?K6_225 0) (>= ?L6_226 0)) (<= ?K6_225 (- 3 1))) (<= ?L6_226 (- 6 1))) (= (select (select h_ds1_filter_init ?K6_225) ?L6_226) init)))) (forall ((?M6_227 Int) (?N6_228 Int)) (let ((?v_89 (- 6 1))) (=> (and (and (and (>= ?M6_227 0) (>= ?N6_228 0)) (<= ?M6_227 ?v_89)) (<= ?N6_228 ?v_89)) (= (select (select pminus_ds1_filter_init ?M6_227) ?N6_228) init))))) (forall ((?O6_229 Int) (?P6_230 Int)) (=> (and (and (and (>= ?O6_229 0) (>= ?P6_230 0)) (<= ?O6_229 (- 6 1))) (<= ?P6_230 0)) (= (select (select xhatmin_ds1_filter_init ?O6_229) ?P6_230) init)))) (forall ((?Q6_231 Int) (?R6_232 Int)) (let ((?v_91 (- 3 1)) (?v_90 (- 6 1))) (=> (and (and (and (>= ?Q6_231 0) (>= ?R6_232 0)) (<= ?Q6_231 ?v_90)) (<= ?R6_232 ?v_91)) (= (select (select (tptp_const_array2 (dim 0 ?v_90) (dim 0 ?v_91) init) ?Q6_231) ?R6_232) init))))) (forall ((?S6_233 Int) (?T6_234 Int)) (let ((?v_92 (store zhat_ds1_filter_init 0 (store (select zhat_ds1_filter_init 0) 0 init)))) (let ((?v_93 (store ?v_92 1 (store (select ?v_92 1) 0 init)))) (=> (and (and (and (>= ?S6_233 0) (>= ?T6_234 0)) (<= ?S6_233 (- 3 1))) (<= ?T6_234 0)) (= (select (select (store ?v_93 2 (store (select ?v_93 2) 0 init)) ?S6_233) ?T6_234) init)))))) (forall ((?U6_235 Int) (?V6_236 Int)) (let ((?v_94 (store zpred_ds1_filter_init 0 (store (select zpred_ds1_filter_init 0) 0 init)))) (let ((?v_95 (store ?v_94 1 (store (select ?v_94 1) 0 init)))) (=> (and (and (and (>= ?U6_235 0) (>= ?V6_236 0)) (<= ?U6_235 (- 3 1))) (<= ?V6_236 0)) (= (select (select (store ?v_95 2 (store (select ?v_95 2) 0 init)) ?U6_235) ?V6_236) init)))))) (forall ((?W6_237 Int) (?X6_238 Int)) (let ((?v_96 (- 6 1))) (=> (and (and (and (>= ?W6_237 0) (>= ?X6_238 0)) (<= ?W6_237 ?v_96)) (<= ?X6_238 ?v_96)) (= (select (select pminus_ds1_filter_init ?W6_237) ?X6_238) init))))) (forall ((?Y6_239 Int) (?Z6_240 Int)) (=> (and (and (and (>= ?Y6_239 0) (>= ?Z6_240 0)) (<= ?Y6_239 (- 3 1))) (<= ?Z6_240 (- 6 1))) (= (select (select h_ds1_filter_init ?Y6_239) ?Z6_240) init)))) (forall ((?A7_241 Int) (?B7_242 Int)) (let ((?v_97 (- 3 1))) (=> (and (and (and (>= ?A7_241 0) (>= ?B7_242 0)) (<= ?A7_241 ?v_97)) (<= ?B7_242 ?v_97)) (= (select (select r_ds1_filter_init ?A7_241) ?B7_242) init))))) (forall ((?C7_243 Int) (?D7_244 Int)) (=> (and (and (and (>= ?C7_243 0) (>= ?D7_244 0)) (<= ?C7_243 (- 3 1))) (<= ?D7_244 (- 6 1))) (= (select (select h_ds1_filter_init ?C7_243) ?D7_244) init)))) (forall ((?E7_245 Int) (?F7_246 Int)) (let ((?v_98 (- 6 1))) (=> (and (and (and (>= ?E7_245 0) (>= ?F7_246 0)) (<= ?E7_245 ?v_98)) (<= ?F7_246 ?v_98)) (= (select (select pminus_ds1_filter_init ?E7_245) ?F7_246) init))))) (forall ((?G7_247 Int) (?H7_248 Int)) (=> (and (and (and (>= ?G7_247 0) (>= ?H7_248 0)) (<= ?G7_247 (- 3 1))) (<= ?H7_248 (- 6 1))) (= (select (select h_ds1_filter_init ?G7_247) ?H7_248) init)))))) (=> ?v_5 (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and ?v_11 (forall ((?I7_249 Int) (?J7_250 Int)) (=> (and (and (and (>= ?I7_249 0) (>= ?J7_250 0)) (<= ?I7_249 2)) (<= ?J7_250 5)) (= (select (select h_ds1_filter_init ?I7_249) ?J7_250) init)))) (forall ((?K7_251 Int) (?L7_252 Int)) (let ((?v_99 (store phi_ds1_filter_init 2 (store (select phi_ds1_filter_init 2) 1 init)))) (let ((?v_100 (store ?v_99 2 (store (select ?v_99 2) 0 init)))) (let ((?v_101 (store ?v_100 1 (store (select ?v_100 1) 2 init)))) (let ((?v_102 (store ?v_101 1 (store (select ?v_101 1) 0 init)))) (let ((?v_103 (store ?v_102 0 (store (select ?v_102 0) 2 init)))) (=> (and (and (and (>= ?K7_251 0) (>= ?L7_252 0)) (<= ?K7_251 5)) (<= ?L7_252 5)) (= (select (select (store ?v_103 0 (store (select ?v_103 0) 1 init)) ?K7_251) ?L7_252) init))))))))) (forall ((?M7_253 Int) (?N7_254 Int)) (let ((?v_104 (store dv_ds1_filter_init 2 (store (select dv_ds1_filter_init 2) 0 init)))) (let ((?v_105 (store ?v_104 1 (store (select ?v_104 1) 0 init)))) (=> (and (and (and (>= ?M7_253 0) (>= ?N7_254 0)) (<= ?M7_253 (- 6 1))) (<= ?N7_254 0)) (= (select (select (store ?v_105 0 (store (select ?v_105 0) 0 init)) ?M7_253) ?N7_254) init)))))) (forall ((?O7_255 Int) (?P7_256 Int)) (=> (and (and (and (>= ?O7_255 0) (>= ?P7_256 0)) (<= ?O7_255 5)) (<= ?P7_256 5)) (= (select (select q_ds1_filter_init ?O7_255) ?P7_256) init)))) (forall ((?Q7_257 Int) (?R7_258 Int)) (=> (and (and (and (>= ?Q7_257 0) (>= ?R7_258 0)) (<= ?Q7_257 2)) (<= ?R7_258 2)) (= (select (select r_ds1_filter_init ?Q7_257) ?R7_258) init)))) (forall ((?S7_259 Int) (?T7_260 Int)) (=> (and (and (and (>= ?S7_259 0) (>= ?T7_260 0)) (<= ?S7_259 5)) (<= ?T7_260 0)) (= (select (select (tptp_const_array2 (dim 0 (- 6 1)) (dim 0 0) init) ?S7_259) ?T7_260) init)))) (forall ((?U7_261 Int) (?V7_262 Int)) (let ((?v_106 (dim 0 (- 6 1)))) (=> (and (and (and (>= ?U7_261 0) (>= ?V7_262 0)) (<= ?U7_261 5)) (<= ?V7_262 5)) (= (select (select (tptp_const_array2 ?v_106 ?v_106 init) ?U7_261) ?V7_262) init))))) (forall ((?W7_263 Int)) (=> (and (>= ?W7_263 0) (<= ?W7_263 (- 6 1))) (forall ((?X7_264 Int)) (=> (and (>= ?X7_264 0) (<= ?X7_264 (- 6 1))) (= (select (select id_ds1_filter_init ?W7_263) ?X7_264) init)))))) (forall ((?Y7_265 Int) (?Z7_266 Int)) (let ((?v_107 (- 6 1))) (=> (and (and (and (>= ?Y7_265 0) (>= ?Z7_266 0)) (<= ?Y7_265 ?v_107)) (<= ?Z7_266 ?v_107)) (= (select (select q_ds1_filter_init ?Y7_265) ?Z7_266) init))))) (forall ((?A8_267 Int) (?B8_268 Int)) (let ((?v_108 (- 6 1)) (?v_109 (store phi_ds1_filter_init 2 (store (select phi_ds1_filter_init 2) 1 init)))) (let ((?v_110 (store ?v_109 2 (store (select ?v_109 2) 0 init)))) (let ((?v_111 (store ?v_110 1 (store (select ?v_110 1) 2 init)))) (let ((?v_112 (store ?v_111 1 (store (select ?v_111 1) 0 init)))) (let ((?v_113 (store ?v_112 0 (store (select ?v_112 0) 2 init)))) (=> (and (and (and (>= ?A8_267 0) (>= ?B8_268 0)) (<= ?A8_267 ?v_108)) (<= ?B8_268 ?v_108)) (= (select (select (store ?v_113 0 (store (select ?v_113 0) 1 init)) ?A8_267) ?B8_268) init))))))))) (forall ((?C8_269 Int) (?D8_270 Int)) (let ((?v_114 (- 6 1))) (let ((?v_115 (dim 0 ?v_114))) (=> (and (and (and (>= ?C8_269 0) (>= ?D8_270 0)) (<= ?C8_269 ?v_114)) (<= ?D8_270 ?v_114)) (= (select (select (tptp_const_array2 ?v_115 ?v_115 init) ?C8_269) ?D8_270) init)))))) (forall ((?E8_271 Int) (?F8_272 Int)) (let ((?v_116 (- 6 1)) (?v_117 (store phi_ds1_filter_init 2 (store (select phi_ds1_filter_init 2) 1 init)))) (let ((?v_118 (store ?v_117 2 (store (select ?v_117 2) 0 init)))) (let ((?v_119 (store ?v_118 1 (store (select ?v_118 1) 2 init)))) (let ((?v_120 (store ?v_119 1 (store (select ?v_119 1) 0 init)))) (let ((?v_121 (store ?v_120 0 (store (select ?v_120 0) 2 init)))) (=> (and (and (and (>= ?E8_271 0) (>= ?F8_272 0)) (<= ?E8_271 ?v_116)) (<= ?F8_272 ?v_116)) (= (select (select (store ?v_121 0 (store (select ?v_121 0) 1 init)) ?E8_271) ?F8_272) init))))))))) (forall ((?G8_273 Int) (?H8_274 Int)) (let ((?v_122 (store dv_ds1_filter_init 2 (store (select dv_ds1_filter_init 2) 0 init)))) (let ((?v_123 (store ?v_122 1 (store (select ?v_122 1) 0 init)))) (=> (and (and (and (>= ?G8_273 0) (>= ?H8_274 0)) (<= ?G8_273 (- 6 1))) (<= ?H8_274 0)) (= (select (select (store ?v_123 0 (store (select ?v_123 0) 0 init)) ?G8_273) ?H8_274) init)))))) (forall ((?I8_275 Int) (?J8_276 Int)) (let ((?v_124 (- 6 1)) (?v_125 (store phi_ds1_filter_init 2 (store (select phi_ds1_filter_init 2) 1 init)))) (let ((?v_126 (store ?v_125 2 (store (select ?v_125 2) 0 init)))) (let ((?v_127 (store ?v_126 1 (store (select ?v_126 1) 2 init)))) (let ((?v_128 (store ?v_127 1 (store (select ?v_127 1) 0 init)))) (let ((?v_129 (store ?v_128 0 (store (select ?v_128 0) 2 init)))) (=> (and (and (and (>= ?I8_275 0) (>= ?J8_276 0)) (<= ?I8_275 ?v_124)) (<= ?J8_276 ?v_124)) (= (select (select (store ?v_129 0 (store (select ?v_129 0) 1 init)) ?I8_275) ?J8_276) init))))))))) (forall ((?K8_277 Int) (?L8_278 Int)) (let ((?v_130 (- 6 1))) (=> (and (and (and (>= ?K8_277 0) (>= ?L8_278 0)) (<= ?K8_277 ?v_130)) (<= ?L8_278 0)) (= (select (select (tptp_const_array2 (dim 0 ?v_130) (dim 0 0) init) ?K8_277) ?L8_278) init))))) (forall ((?M8_279 Int) (?N8_280 Int)) (let ((?v_131 (- 6 1))) (=> (and (and (and (>= ?M8_279 0) (>= ?N8_280 0)) (<= ?M8_279 ?v_131)) (<= ?N8_280 ?v_131)) (= (select (select id_ds1_filter_init ?M8_279) ?N8_280) init))))) (forall ((?O8_281 Int) (?P8_282 Int)) (let ((?v_133 (- 3 1)) (?v_132 (- 6 1))) (=> (and (and (and (>= ?O8_281 0) (>= ?P8_282 0)) (<= ?O8_281 ?v_132)) (<= ?P8_282 ?v_133)) (= (select (select (tptp_const_array2 (dim 0 ?v_132) (dim 0 ?v_133) init) ?O8_281) ?P8_282) init))))) (forall ((?Q8_283 Int) (?R8_284 Int)) (=> (and (and (and (>= ?Q8_283 0) (>= ?R8_284 0)) (<= ?Q8_283 (- 3 1))) (<= ?R8_284 (- 6 1))) (= (select (select h_ds1_filter_init ?Q8_283) ?R8_284) init)))) (forall ((?S8_285 Int) (?T8_286 Int)) (let ((?v_134 (- 6 1))) (=> (and (and (and (>= ?S8_285 0) (>= ?T8_286 0)) (<= ?S8_285 ?v_134)) (<= ?T8_286 ?v_134)) (= (select (select pminus_ds1_filter_init ?S8_285) ?T8_286) init))))) (forall ((?U8_287 Int) (?V8_288 Int)) (=> (and (and (and (>= ?U8_287 0) (>= ?V8_288 0)) (<= ?U8_287 (- 6 1))) (<= ?V8_288 0)) (= (select (select xhatmin_ds1_filter_init ?U8_287) ?V8_288) init)))) (forall ((?W8_289 Int) (?X8_290 Int)) (let ((?v_136 (- 3 1)) (?v_135 (- 6 1))) (=> (and (and (and (>= ?W8_289 0) (>= ?X8_290 0)) (<= ?W8_289 ?v_135)) (<= ?X8_290 ?v_136)) (= (select (select (tptp_const_array2 (dim 0 ?v_135) (dim 0 ?v_136) init) ?W8_289) ?X8_290) init))))) (forall ((?Y8_291 Int) (?Z8_292 Int)) (let ((?v_137 (store zhat_ds1_filter_init 0 (store (select zhat_ds1_filter_init 0) 0 init)))) (let ((?v_138 (store ?v_137 1 (store (select ?v_137 1) 0 init)))) (=> (and (and (and (>= ?Y8_291 0) (>= ?Z8_292 0)) (<= ?Y8_291 (- 3 1))) (<= ?Z8_292 0)) (= (select (select (store ?v_138 2 (store (select ?v_138 2) 0 init)) ?Y8_291) ?Z8_292) init)))))) (forall ((?A9_293 Int) (?B9_294 Int)) (let ((?v_139 (store zpred_ds1_filter_init 0 (store (select zpred_ds1_filter_init 0) 0 init)))) (let ((?v_140 (store ?v_139 1 (store (select ?v_139 1) 0 init)))) (=> (and (and (and (>= ?A9_293 0) (>= ?B9_294 0)) (<= ?A9_293 (- 3 1))) (<= ?B9_294 0)) (= (select (select (store ?v_140 2 (store (select ?v_140 2) 0 init)) ?A9_293) ?B9_294) init)))))) (forall ((?C9_295 Int) (?D9_296 Int)) (let ((?v_141 (- 6 1))) (=> (and (and (and (>= ?C9_295 0) (>= ?D9_296 0)) (<= ?C9_295 ?v_141)) (<= ?D9_296 ?v_141)) (= (select (select pminus_ds1_filter_init ?C9_295) ?D9_296) init))))) (forall ((?E9_297 Int) (?F9_298 Int)) (=> (and (and (and (>= ?E9_297 0) (>= ?F9_298 0)) (<= ?E9_297 (- 3 1))) (<= ?F9_298 (- 6 1))) (= (select (select h_ds1_filter_init ?E9_297) ?F9_298) init)))) (forall ((?G9_299 Int) (?H9_300 Int)) (let ((?v_142 (- 3 1))) (=> (and (and (and (>= ?G9_299 0) (>= ?H9_300 0)) (<= ?G9_299 ?v_142)) (<= ?H9_300 ?v_142)) (= (select (select r_ds1_filter_init ?G9_299) ?H9_300) init))))) (forall ((?I9_301 Int) (?J9_302 Int)) (=> (and (and (and (>= ?I9_301 0) (>= ?J9_302 0)) (<= ?I9_301 (- 3 1))) (<= ?J9_302 (- 6 1))) (= (select (select h_ds1_filter_init ?I9_301) ?J9_302) init)))) (forall ((?K9_303 Int) (?L9_304 Int)) (let ((?v_143 (- 6 1))) (=> (and (and (and (>= ?K9_303 0) (>= ?L9_304 0)) (<= ?K9_303 ?v_143)) (<= ?L9_304 ?v_143)) (= (select (select pminus_ds1_filter_init ?K9_303) ?L9_304) init))))) (forall ((?M9_305 Int) (?N9_306 Int)) (=> (and (and (and (>= ?M9_305 0) (>= ?N9_306 0)) (<= ?M9_305 (- 3 1))) (<= ?N9_306 (- 6 1))) (= (select (select h_ds1_filter_init ?M9_305) ?N9_306) init)))))))))))))))))
(check-sat)
(exit)
