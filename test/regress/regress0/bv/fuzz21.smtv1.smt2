(set-option :incremental false)
(meta-info :status unsat)
(set-logic QF_BV)
(declare-fun v0 () (_ BitVec 4))
(declare-fun v1 () (_ BitVec 4))
(declare-fun v2 () (_ BitVec 4))
(declare-fun v3 () (_ BitVec 4))
(declare-fun v4 () (_ BitVec 4))
(check-sat-assuming ( (let ((_let_0 (bvadd (_ bv4 4) (_ bv1 4)))) (let ((_let_1 (ite (bvuge (bvxnor v0 v1) (bvxnor v0 v1)) (_ bv1 1) (_ bv0 1)))) (let ((_let_2 (ite (bvslt (_ bv4 4) v4) (_ bv1 1) (_ bv0 1)))) (let ((_let_3 ((_ zero_extend 0) v1))) (let ((_let_4 (ite (bvslt v1 v4) (_ bv1 1) (_ bv0 1)))) (let ((_let_5 (bvxnor ((_ sign_extend 3) (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv0 4)))) (let ((_let_6 (bvcomp (bvxnor v0 v1) v4))) (let ((_let_7 (ite (distinct ((_ zero_extend 3) _let_2) (_ bv0 4)) (_ bv1 1) (_ bv0 1)))) (let ((_let_8 (bvshl (bvand ((_ sign_extend 3) _let_4) v3) ((_ sign_extend 3) (bvmul (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)) _let_1))))) (let ((_let_9 (bvashr _let_3 ((_ zero_extend 3) (ite (bvule ((_ zero_extend 0) (_ bv4 4)) v1) (_ bv1 1) (_ bv0 1)))))) (let ((_let_10 (ite (distinct (bvlshr _let_3 v0) (bvnor _let_0 v0)) (_ bv1 1) (_ bv0 1)))) (let ((_let_11 ((_ rotate_right 1) (bvlshr _let_3 v0)))) (let ((_let_12 (bvsub ((_ sign_extend 3) (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1))) ((_ rotate_left 1) v3)))) (let ((_let_13 (bvxor ((_ zero_extend 3) (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1))) (_ bv4 4)))) (let ((_let_14 (ite (bvugt v4 (bvxnor v0 v1)) (_ bv1 1) (_ bv0 1)))) (let ((_let_15 (bvand (_ bv0 4) (bvsub (bvmul _let_3 _let_3) (_ bv4 4))))) (let ((_let_16 (bvashr v1 _let_8))) (let ((_let_17 ((_ sign_extend 3) (ite (= ((_ sign_extend 3) (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) v0) (_ bv1 1) (_ bv0 1))))) (let ((_let_18 (ite (= (_ bv1 1) ((_ extract 3 3) ((_ zero_extend 0) (_ bv4 4)))) _let_0 ((_ sign_extend 3) (bvadd (ite (bvugt (_ bv0 4) v3) (_ bv1 1) (_ bv0 1)) (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1))))))) (let ((_let_19 (bvsub v4 (bvmul _let_3 _let_3)))) (let ((_let_20 ((_ zero_extend 3) _let_4))) (let ((_let_21 ((_ sign_extend 3) ((_ extract 0 0) _let_1)))) (let ((_let_22 (ite (distinct _let_12 _let_21) (_ bv1 1) (_ bv0 1)))) (let ((_let_23 (ite (bvuge _let_9 ((_ zero_extend 3) (ite (bvsle (_ bv0 4) ((_ zero_extend 3) ((_ extract 0 0) _let_1))) (_ bv1 1) (_ bv0 1)))) (_ bv1 1) (_ bv0 1)))) (let ((_let_24 (bvxor v4 (bvnor _let_0 v0)))) (let ((_let_25 ((_ extract 2 2) ((_ repeat 1) _let_0)))) (let ((_let_26 ((_ repeat 4) (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1))))) (let ((_let_27 (bvadd _let_13 _let_15))) (let ((_let_28 (bvneg ((_ repeat 3) (ite (= ((_ sign_extend 3) (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) v0) (_ bv1 1) (_ bv0 1)))))) (let ((_let_29 (bvmul (bvxnor v0 v1) (_ bv0 4)))) (let ((_let_30 (ite (bvsgt ((_ zero_extend 3) ((_ zero_extend 0) _let_22)) _let_8) (_ bv1 1) (_ bv0 1)))) (let ((_let_31 (bvshl _let_12 _let_20))) (let ((_let_32 (bvcomp (bvmul (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)) _let_1) (ite (bvsge v0 v0) (_ bv1 1) (_ bv0 1))))) (let ((_let_33 (bvxor (bvcomp (bvsub (bvmul _let_3 _let_3) (_ bv4 4)) (bvneg (bvlshr _let_3 v0))) (ite (bvsge v0 v0) (_ bv1 1) (_ bv0 1))))) (let ((_let_34 ((_ sign_extend 1) ((_ repeat 3) (ite (= ((_ sign_extend 3) (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) v0) (_ bv1 1) (_ bv0 1)))))) (let ((_let_35 (ite (= ((_ rotate_left 1) v3) _let_34) (_ bv1 1) (_ bv0 1)))) (let ((_let_36 (ite (bvugt _let_23 _let_14) (_ bv1 1) (_ bv0 1)))) (let ((_let_37 ((_ sign_extend 3) _let_33))) (let ((_let_38 (bvsub (bvadd _let_24 _let_26) _let_37))) (let ((_let_39 ((_ sign_extend 0) (bvsub (bvmul _let_3 _let_3) (_ bv4 4))))) (let ((_let_40 (bvsub ((_ zero_extend 0) (_ bv4 4)) (bvnor _let_0 v0)))) (let ((_let_41 (bvneg _let_40))) (let ((_let_42 (ite (bvsgt (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1)) _let_6) (_ bv1 1) (_ bv0 1)))) (let ((_let_43 ((_ rotate_right 0) (bvashr (bvadd (ite (bvugt (_ bv0 4) v3) (_ bv1 1) (_ bv0 1)) (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1))) (bvadd (ite (bvugt (_ bv0 4) v3) (_ bv1 1) (_ bv0 1)) (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1))))))) (let ((_let_44 ((_ rotate_left 0) ((_ repeat 3) _let_25)))) (let ((_let_45 (bvand _let_32 _let_30))) (let ((_let_46 (bvnot ((_ repeat 3) (bvmul (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)) _let_1))))) (let ((_let_47 (bvmul ((_ repeat 3) _let_25) ((_ sign_extend 2) (ite (= ((_ zero_extend 0) (_ bv4 4)) ((_ sign_extend 3) _let_2)) (_ bv1 1) (_ bv0 1)))))) (let ((_let_48 ((_ sign_extend 3) (ite (bvult (bvmul _let_3 _let_3) (_ bv4 4)) (_ bv1 1) (_ bv0 1))))) (let ((_let_49 ((_ rotate_left 2) _let_13))) (let ((_let_50 (bvneg _let_33))) (let ((_let_51 (bvashr ((_ sign_extend 2) (ite (= ((_ sign_extend 3) (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) v0) (_ bv1 1) (_ bv0 1))) ((_ repeat 3) (bvmul (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)) _let_1))))) (let ((_let_52 (ite (distinct ((_ zero_extend 3) _let_7) _let_26) (_ bv1 1) (_ bv0 1)))) (let ((_let_53 (bvnand _let_34 ((_ sign_extend 3) (ite (bvsge v3 ((_ repeat 1) _let_0)) (_ bv1 1) (_ bv0 1)))))) (let ((_let_54 (bvult ((_ extract 0 0) (ite (bvsle (_ bv0 4) ((_ zero_extend 3) ((_ extract 0 0) _let_1))) (_ bv1 1) (_ bv0 1))) _let_43))) (let ((_let_55 ((_ sign_extend 3) _let_36))) (let ((_let_56 (bvult _let_11 (bvxnor v0 v1)))) (let ((_let_57 ((_ zero_extend 1) _let_46))) (let ((_let_58 ((_ zero_extend 3) (ite (bvult (bvmul _let_3 _let_3) (_ bv4 4)) (_ bv1 1) (_ bv0 1))))) (let ((_let_59 (= ((_ sign_extend 3) _let_1) v2))) (let ((_let_60 (bvuge _let_7 _let_52))) (let ((_let_61 ((_ sign_extend 1) _let_28))) (let ((_let_62 (bvsge ((_ sign_extend 1) _let_44) v0))) (let ((_let_63 (bvslt _let_29 ((_ zero_extend 3) ((_ zero_extend 0) _let_22))))) (let ((_let_64 ((_ sign_extend 2) (ite (bvugt (_ bv0 4) v3) (_ bv1 1) (_ bv0 1))))) (let ((_let_65 (bvsle _let_0 _let_19))) (let ((_let_66 (bvsge (_ bv4 4) (bvadd _let_24 _let_26)))) (let ((_let_67 ((_ sign_extend 1) ((_ repeat 3) _let_25)))) (let ((_let_68 (bvsle (bvneg (bvlshr _let_3 v0)) (_ bv1 4)))) (let ((_let_69 (= ((_ sign_extend 3) (ite (bvsgt _let_17 _let_8) (_ bv1 1) (_ bv0 1))) (_ bv4 4)))) (let ((_let_70 ((_ sign_extend 3) (ite (bvslt ((_ sign_extend 2) (bvcomp (bvsub (bvmul _let_3 _let_3) (_ bv4 4)) (bvneg (bvlshr _let_3 v0)))) _let_28) (_ bv1 1) (_ bv0 1))))) (let ((_let_71 (= v0 (bvmul _let_37 _let_38)))) (let ((_let_72 (bvsle _let_18 _let_8))) (let ((_let_73 (bvsgt ((_ sign_extend 2) _let_50) _let_47))) (let ((_let_74 (= _let_70 (bvadd _let_24 _let_26)))) (let ((_let_75 (bvugt (ite (bvslt ((_ sign_extend 2) (bvcomp (bvsub (bvmul _let_3 _let_3) (_ bv4 4)) (bvneg (bvlshr _let_3 v0)))) _let_28) (_ bv1 1) (_ bv0 1)) _let_10))) (let ((_let_76 (distinct _let_4 _let_22))) (let ((_let_77 (bvslt ((_ zero_extend 0) (_ bv4 4)) ((_ sign_extend 3) _let_2)))) (let ((_let_78 (bvule ((_ zero_extend 2) ((_ extract 0 0) _let_15)) _let_28))) (let ((_let_79 (bvsgt (bvneg (bvlshr _let_3 v0)) ((_ zero_extend 3) _let_6)))) (let ((_let_80 (bvult _let_43 _let_42))) (let ((_let_81 (bvult _let_40 ((_ sign_extend 3) (bvmul (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)) _let_1))))) (let ((_let_82 (bvsle ((_ sign_extend 2) _let_4) _let_28))) (let ((_let_83 (bvult _let_49 _let_12))) (let ((_let_84 (bvule (bvlshr _let_3 v0) (bvneg (bvlshr _let_3 v0))))) (let ((_let_85 (= ((_ sign_extend 2) ((_ repeat 1) (bvmul (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)) _let_1))) _let_51))) (let ((_let_86 (bvugt _let_70 _let_49))) (let ((_let_87 (bvsge (bvxor ((_ repeat 1) v1) _let_18) _let_39))) (let ((_let_88 ((_ sign_extend 1) _let_51))) (let ((_let_89 (bvsle _let_3 _let_41))) (let ((_let_90 (bvslt (bvmul _let_3 _let_3) _let_29))) (let ((_let_91 (bvugt ((_ zero_extend 3) (ite (bvslt ((_ sign_extend 2) (bvcomp (bvsub (bvmul _let_3 _let_3) (_ bv4 4)) (bvneg (bvlshr _let_3 v0)))) _let_28) (_ bv1 1) (_ bv0 1))) _let_11))) (let ((_let_92 (bvslt _let_22 _let_43))) (let ((_let_93 (distinct (bvsub (bvmul _let_3 _let_3) (_ bv4 4)) _let_19))) (let ((_let_94 (bvule (_ bv4 4) ((_ sign_extend 3) _let_25)))) (let ((_let_95 (bvule (bvadd (ite (bvugt (_ bv0 4) v3) (_ bv1 1) (_ bv0 1)) (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1))) _let_14))) (let ((_let_96 (bvsge _let_14 _let_35))) (let ((_let_97 (bvuge _let_24 _let_53))) (let ((_let_98 (distinct _let_19 _let_24))) (let ((_let_99 (bvult _let_9 _let_5))) (let ((_let_100 (distinct ((_ sign_extend 2) (ite (bvult (bvmul _let_3 _let_3) (_ bv4 4)) (_ bv1 1) (_ bv0 1))) _let_46))) (let ((_let_101 (bvslt _let_4 _let_52))) (let ((_let_102 (bvuge ((_ extract 0 0) _let_1) _let_10))) (let ((_let_103 (bvugt (bvsub (bvmul _let_3 _let_3) (_ bv4 4)) v4))) (let ((_let_104 (bvslt ((_ zero_extend 3) _let_43) (bvmul _let_3 _let_3)))) (let ((_let_105 (bvuge (bvnor _let_0 v0) _let_38))) (let ((_let_106 (bvslt _let_3 _let_38))) (let ((_let_107 (bvsgt ((_ sign_extend 3) _let_25) _let_26))) (let ((_let_108 ((_ sign_extend 1) _let_47))) (let ((_let_109 (bvsgt ((_ zero_extend 2) ((_ extract 0 0) _let_15)) ((_ repeat 3) (bvmul (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)) _let_1))))) (let ((_let_110 (bvslt ((_ sign_extend 2) _let_36) _let_51))) (let ((_let_111 (bvsle (_ bv1 4) v3))) (let ((_let_112 (bvsge _let_20 (bvxor v4 (_ bv0 4))))) (let ((_let_113 (= _let_53 ((_ zero_extend 3) (ite (bvule ((_ sign_extend 3) ((_ extract 0 0) (ite (bvsle (_ bv0 4) ((_ zero_extend 3) ((_ extract 0 0) _let_1))) (_ bv1 1) (_ bv0 1)))) v2) (_ bv1 1) (_ bv0 1)))))) (let ((_let_114 (bvslt (bvxor ((_ repeat 1) v1) _let_18) (bvmul _let_3 _let_3)))) (let ((_let_115 (bvsgt ((_ zero_extend 3) _let_25) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))))) (let ((_let_116 (bvugt ((_ sign_extend 2) (ite (bvule ((_ sign_extend 3) ((_ extract 0 0) (ite (bvsle (_ bv0 4) ((_ zero_extend 3) ((_ extract 0 0) _let_1))) (_ bv1 1) (_ bv0 1)))) v2) (_ bv1 1) (_ bv0 1))) _let_47))) (let ((_let_117 (bvsle (ite (bvsge v3 ((_ repeat 1) _let_0)) (_ bv1 1) (_ bv0 1)) (ite (= ((_ zero_extend 0) (_ bv4 4)) ((_ sign_extend 3) _let_2)) (_ bv1 1) (_ bv0 1))))) (let ((_let_118 (distinct _let_27 ((_ zero_extend 3) _let_42)))) (let ((_let_119 (bvule (bvlshr _let_3 v0) (bvsub (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2)) (bvsub (bvmul _let_3 _let_3) (_ bv4 4)))))) (let ((_let_120 (bvugt ((_ sign_extend 2) (ite (bvult (bvmul _let_3 _let_3) (_ bv4 4)) (_ bv1 1) (_ bv0 1))) _let_51))) (let ((_let_121 (bvuge v4 _let_38))) (let ((_let_122 (distinct _let_13 ((_ sign_extend 3) (ite (bvsgt _let_17 _let_8) (_ bv1 1) (_ bv0 1)))))) (let ((_let_123 (bvslt (bvshl (ite (bvslt ((_ sign_extend 2) (bvcomp (bvsub (bvmul _let_3 _let_3) (_ bv4 4)) (bvneg (bvlshr _let_3 v0)))) _let_28) (_ bv1 1) (_ bv0 1)) (ite (bvule ((_ zero_extend 0) (_ bv4 4)) v1) (_ bv1 1) (_ bv0 1))) _let_23))) (let ((_let_124 (bvsgt _let_26 (bvadd _let_24 _let_26)))) (let ((_let_125 (= ((_ zero_extend 2) _let_23) ((_ repeat 3) _let_25)))) (let ((_let_126 (bvsle _let_36 (ite (bvsle (_ bv0 4) ((_ zero_extend 3) ((_ extract 0 0) _let_1))) (_ bv1 1) (_ bv0 1))))) (let ((_let_127 (distinct _let_0 ((_ zero_extend 1) _let_28)))) (let ((_let_128 (bvsge v2 ((_ zero_extend 0) (_ bv4 4))))) (let ((_let_129 (bvsle (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2)) _let_58))) (let ((_let_130 (bvsgt (ite (bvsge ((_ sign_extend 1) ((_ repeat 3) (bvmul (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)) _let_1))) ((_ repeat 1) v1)) (_ bv1 1) (_ bv0 1)) _let_33))) (let ((_let_131 (distinct _let_51 ((_ zero_extend 2) (ite (bvsle (_ bv0 4) ((_ zero_extend 3) ((_ extract 0 0) _let_1))) (_ bv1 1) (_ bv0 1)))))) (let ((_let_132 (distinct (ite (= ((_ sign_extend 3) (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) v0) (_ bv1 1) (_ bv0 1)) _let_43))) (let ((_let_133 (not _let_77))) (let ((_let_134 (not _let_74))) (let ((_let_135 (not _let_92))) (let ((_let_136 (not _let_62))) (let ((_let_137 (not _let_82))) (let ((_let_138 (not _let_125))) (let ((_let_139 (not (bvslt _let_45 (ite (bvule ((_ zero_extend 0) (_ bv4 4)) v1) (_ bv1 1) (_ bv0 1)))))) (let ((_let_140 (not (bvsle (bvnor _let_0 v0) _let_9)))) (let ((_let_141 (not _let_85))) (let ((_let_142 (not (= _let_34 (bvxnor v0 v1))))) (let ((_let_143 (not _let_104))) (let ((_let_144 (not (= _let_61 _let_49)))) (let ((_let_145 (not (bvuge ((_ repeat 1) _let_0) ((_ zero_extend 3) ((_ zero_extend 0) _let_22)))))) (let ((_let_146 (not (bvule _let_22 _let_36)))) (let ((_let_147 (not _let_126))) (let ((_let_148 (not (distinct _let_32 _let_10)))) (let ((_let_149 (not (bvuge (bvcomp (bvand ((_ sign_extend 3) _let_4) v3) ((_ sign_extend 3) _let_25)) (ite (bvult (bvmul _let_3 _let_3) (_ bv4 4)) (_ bv1 1) (_ bv0 1)))))) (let ((_let_150 (not (bvugt ((_ sign_extend 3) _let_32) (bvnor _let_0 v0))))) (let ((_let_151 (not _let_116))) (let ((_let_152 (not (bvule _let_3 _let_67)))) (let ((_let_153 (not (bvsgt ((_ rotate_left 1) v3) ((_ sign_extend 3) (ite (bvugt (_ bv0 4) v3) (_ bv1 1) (_ bv0 1))))))) (let ((_let_154 (not (distinct _let_11 ((_ sign_extend 3) _let_30))))) (let ((_let_155 (not (= (_ bv1 4) ((_ sign_extend 3) _let_43))))) (let ((_let_156 (not (bvule ((_ sign_extend 3) ((_ extract 0 0) (ite (bvsle (_ bv0 4) ((_ zero_extend 3) ((_ extract 0 0) _let_1))) (_ bv1 1) (_ bv0 1)))) _let_38)))) (let ((_let_157 (not (bvuge _let_5 _let_70)))) (let ((_let_158 (not (bvsge (bvadd _let_24 _let_26) _let_40)))) (let ((_let_159 (not (bvsle ((_ zero_extend 3) (ite (bvule (bvadd ((_ sign_extend 3) _let_14) _let_12) v1) (_ bv1 1) (_ bv0 1))) _let_18)))) (let ((_let_160 (not _let_114))) (let ((_let_161 (not (bvule _let_8 _let_58)))) (let ((_let_162 (not (distinct _let_10 (ite (bvsge v0 v0) (_ bv1 1) (_ bv0 1)))))) (let ((_let_163 (not (bvslt _let_48 (bvmul _let_13 _let_48))))) (let ((_let_164 (not (bvult _let_70 (_ bv0 4))))) (let ((_let_165 (not (bvsge _let_11 _let_40)))) (let ((_let_166 (not (bvugt _let_15 ((_ sign_extend 3) (ite (bvsge v0 v0) (_ bv1 1) (_ bv0 1))))))) (let ((_let_167 (not _let_130))) (and (or _let_133 (not (bvsgt (_ bv4 4) _let_20)) (bvuge v2 ((_ zero_extend 3) ((_ repeat 1) (bvmul (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)) _let_1))))) (or _let_134 _let_135 _let_101) (or (not _let_91) _let_136 (not (bvuge ((_ zero_extend 2) (ite (bvule ((_ sign_extend 3) ((_ extract 0 0) (ite (bvsle (_ bv0 4) ((_ zero_extend 3) ((_ extract 0 0) _let_1))) (_ bv1 1) (_ bv0 1)))) v2) (_ bv1 1) (_ bv0 1))) _let_44))) (or _let_76 _let_137 _let_90) (or (bvugt ((_ zero_extend 3) (ite (bvule ((_ sign_extend 3) ((_ extract 0 0) (ite (bvsle (_ bv0 4) ((_ zero_extend 3) ((_ extract 0 0) _let_1))) (_ bv1 1) (_ bv0 1)))) v2) (_ bv1 1) (_ bv0 1))) _let_12) _let_60 _let_87) (or _let_62 (not (bvsgt _let_3 _let_27)) (not _let_132)) (or (= _let_61 _let_49) (= (bvcomp (bvand ((_ sign_extend 3) _let_4) v3) ((_ sign_extend 3) _let_25)) (ite (bvsge v0 v0) (_ bv1 1) (_ bv0 1))) _let_138) (or (not _let_124) _let_139 (not (bvult _let_64 _let_51))) (or (distinct _let_10 (ite (bvsge v0 v0) (_ bv1 1) (_ bv0 1))) _let_79 _let_92) (or (bvuge v0 _let_12) (not (bvsgt ((_ repeat 3) _let_25) ((_ zero_extend 2) _let_23))) _let_140) (or _let_133 (bvsgt (bvmul _let_37 _let_38) ((_ zero_extend 1) _let_47)) _let_141) (or _let_68 (not (bvugt _let_8 ((_ zero_extend 3) _let_10))) _let_100) (or _let_62 (= _let_61 _let_49) _let_142) (or _let_109 (not _let_93) _let_143) (or (not _let_101) _let_142 (bvuge _let_5 _let_70)) (or (bvult (ite (= ((_ sign_extend 3) (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) v0) (_ bv1 1) (_ bv0 1)) _let_33) _let_144 _let_81) (or (bvugt (ite (bvslt ((_ sign_extend 2) (bvcomp (bvsub (bvmul _let_3 _let_3) (_ bv4 4)) (bvneg (bvlshr _let_3 v0)))) _let_28) (_ bv1 1) (_ bv0 1)) (bvadd (ite (bvugt (_ bv0 4) v3) (_ bv1 1) (_ bv0 1)) (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)))) _let_123 (not _let_113)) (or _let_140 _let_140 (not _let_86)) (or (bvsge (bvlshr _let_3 v0) ((_ sign_extend 3) (ite (bvsge v0 v0) (_ bv1 1) (_ bv0 1)))) _let_145 _let_146) (or _let_129 (bvsle (bvadd _let_24 _let_26) ((_ sign_extend 3) _let_14)) (not _let_96)) (or (not _let_81) _let_109 _let_97) (or _let_146 _let_66 (bvuge _let_61 _let_0)) (or _let_95 (not (bvsge _let_88 ((_ repeat 1) v1))) _let_128) (or (not _let_59) (= (_ bv1 4) ((_ sign_extend 3) _let_43)) _let_63) (or (not _let_56) (not (bvslt ((_ repeat 1) _let_0) (bvxor v4 (_ bv0 4)))) _let_93) (or _let_147 _let_75 (not _let_128)) (or (not _let_60) _let_140 _let_142) (or _let_80 _let_94 _let_91) (or _let_72 _let_77 _let_148) (or (not (distinct _let_12 ((_ rotate_left 1) _let_29))) (bvult _let_51 _let_47) _let_65) (or _let_144 _let_149 _let_101) (or _let_125 _let_95 (not _let_123)) (or (not _let_89) _let_150 (bvsge (ite (bvsge ((_ sign_extend 1) ((_ repeat 3) (bvmul (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)) _let_1))) ((_ repeat 1) v1)) (_ bv1 1) (_ bv0 1)) (ite (bvsge v3 ((_ repeat 1) _let_0)) (_ bv1 1) (_ bv0 1)))) (or _let_141 _let_151 _let_59) (or (not _let_111) (not (= ((_ extract 0 0) _let_1) (ite (bvsge ((_ sign_extend 1) ((_ repeat 3) (bvmul (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)) _let_1))) ((_ repeat 1) v1)) (_ bv1 1) (_ bv0 1)))) (bvult _let_70 (_ bv0 4))) (or _let_152 _let_118 _let_112) (or _let_65 _let_105 _let_73) (or _let_153 (not (bvsgt _let_51 ((_ sign_extend 2) ((_ extract 0 0) (ite (bvsle (_ bv0 4) ((_ zero_extend 3) ((_ extract 0 0) _let_1))) (_ bv1 1) (_ bv0 1)))))) _let_106) (or _let_98 _let_76 _let_100) (or (not (= ((_ sign_extend 2) _let_35) _let_44)) _let_154 _let_135) (or (bvslt _let_12 _let_39) (bvuge (bvmul _let_37 _let_38) ((_ sign_extend 3) (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1)))) (bvugt ((_ zero_extend 2) ((_ extract 0 0) (ite (bvsle (_ bv0 4) ((_ zero_extend 3) ((_ extract 0 0) _let_1))) (_ bv1 1) (_ bv0 1)))) _let_44)) (or _let_78 _let_68 _let_155) (or (= _let_31 ((_ rotate_left 1) v3)) _let_93 _let_85) (or (not _let_69) _let_117 _let_148) (or _let_69 _let_134 _let_143) (or (bvsge _let_11 _let_40) (bvule ((_ sign_extend 3) ((_ extract 0 0) (ite (bvsle (_ bv0 4) ((_ zero_extend 3) ((_ extract 0 0) _let_1))) (_ bv1 1) (_ bv0 1)))) _let_38) _let_113) (or (not (distinct (bvsub (bvmul _let_3 _let_3) (_ bv4 4)) _let_41)) (bvugt _let_61 _let_16) (not (bvule _let_3 _let_88))) (or _let_150 _let_104 _let_138) (or (not (bvule (ite (bvule (bvadd ((_ sign_extend 3) _let_14) _let_12) v1) (_ bv1 1) (_ bv0 1)) ((_ extract 0 0) (ite (bvsle (_ bv0 4) ((_ zero_extend 3) ((_ extract 0 0) _let_1))) (_ bv1 1) (_ bv0 1))))) (bvugt ((_ sign_extend 3) _let_32) (bvnor _let_0 v0)) (bvuge (bvmul _let_37 _let_38) _let_108)) (or _let_143 _let_151 _let_74) (or _let_96 (bvule _let_8 _let_58) _let_110) (or _let_156 _let_92 _let_131) (or _let_110 _let_128 (not (bvsgt (ite (= ((_ zero_extend 0) (_ bv4 4)) ((_ sign_extend 3) _let_2)) (_ bv1 1) (_ bv0 1)) _let_45))) (or _let_157 (bvuge v0 (bvxor ((_ repeat 1) v1) _let_18)) (not (bvuge v0 _let_26))) (or (bvuge (bvadd ((_ sign_extend 3) _let_14) _let_12) _let_27) _let_103 (bvule (bvmul _let_37 _let_38) ((_ sign_extend 3) _let_52))) (or _let_104 (not (bvule ((_ zero_extend 3) (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1))) _let_53)) _let_158) (or (not _let_131) (bvule ((_ zero_extend 1) _let_28) _let_9) _let_94) (or (not (distinct _let_9 ((_ zero_extend 3) ((_ extract 0 0) _let_1)))) (not (bvslt ((_ sign_extend 3) _let_6) _let_39)) _let_126) (or _let_79 (not (bvsgt ((_ zero_extend 0) (_ bv4 4)) (bvmul _let_13 _let_48))) (not _let_122)) (or _let_120 (bvult _let_70 (_ bv0 4)) _let_159) (or (not _let_84) (not _let_118) (not _let_120)) (or (not _let_63) (not _let_90) (not (bvule (_ bv1 4) _let_57))) (or _let_121 _let_89 (bvult _let_19 ((_ sign_extend 3) (ite (bvsge v3 ((_ repeat 1) _let_0)) (_ bv1 1) (_ bv0 1))))) (or _let_160 _let_141 _let_105) (or (not _let_117) _let_109 _let_120) (or (bvsge v0 _let_8) _let_99 _let_119) (or (bvult _let_70 (_ bv0 4)) _let_127 (bvule (ite (bvule ((_ zero_extend 0) (_ bv4 4)) v1) (_ bv1 1) (_ bv0 1)) (ite (bvule ((_ zero_extend 0) (_ bv4 4)) v1) (_ bv1 1) (_ bv0 1)))) (or (bvslt ((_ zero_extend 3) ((_ zero_extend 0) _let_22)) _let_53) (not _let_78) _let_115) (or _let_150 _let_75 _let_102) (or (bvult _let_31 _let_57) _let_157 (not _let_97)) (or _let_124 _let_137 _let_132) (or _let_54 _let_153 (not _let_72)) (or (distinct _let_16 (_ bv1 4)) (not (bvsgt _let_41 ((_ sign_extend 1) _let_46))) (distinct ((_ rotate_left 1) v3) _let_41)) (or (not _let_87) _let_56 _let_136) (or (bvsge _let_11 ((_ zero_extend 3) _let_36)) _let_147 _let_66) (or _let_149 (not (distinct (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)) (ite (bvslt ((_ sign_extend 2) (bvcomp (bvsub (bvmul _let_3 _let_3) (_ bv4 4)) (bvneg (bvlshr _let_3 v0)))) _let_28) (_ bv1 1) (_ bv0 1)))) _let_114) (or (not (bvsgt _let_49 ((_ sign_extend 3) (bvcomp (bvsub (bvmul _let_3 _let_3) (_ bv4 4)) (bvneg (bvlshr _let_3 v0)))))) (not (distinct _let_29 _let_55)) (not _let_80)) (or _let_54 _let_161 _let_145) (or _let_150 _let_83 _let_162) (or (not (bvugt _let_29 ((_ zero_extend 3) _let_30))) _let_102 (bvult _let_70 (_ bv0 4))) (or (bvugt (ite (bvsge ((_ sign_extend 1) ((_ repeat 3) (bvmul (ite (bvslt _let_1 (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))) (_ bv1 1) (_ bv0 1)) _let_1))) ((_ repeat 1) v1)) (_ bv1 1) (_ bv0 1)) _let_30) (bvslt _let_0 _let_29) _let_163) (or (not _let_79) _let_84 _let_160) (or _let_54 _let_84 _let_60) (or (not (bvsge _let_53 _let_40)) (not _let_110) (not (bvuge _let_108 _let_31))) (or _let_66 _let_121 _let_103) (or (not (bvult ((_ repeat 1) _let_0) ((_ sign_extend 3) _let_35))) (not (bvugt _let_47 _let_64)) (not (distinct _let_55 _let_5))) (or _let_149 (not (bvult (bvsub (bvmul _let_3 _let_3) (_ bv4 4)) v0)) (not _let_129)) (or _let_77 _let_124 _let_82) (or (bvsgt _let_24 (bvand ((_ sign_extend 3) _let_4) v3)) _let_161 _let_164) (or _let_86 _let_165 _let_115) (or (not (bvuge v2 ((_ zero_extend 3) (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1))))) (not (bvslt (bvxnor v0 v1) _let_67)) _let_154) (or _let_104 (not _let_71) _let_139) (or _let_152 _let_166 (not (bvsgt _let_50 (ite (bvugt (_ bv0 4) v3) (_ bv1 1) (_ bv0 1))))) (or (not (bvule (bvadd (bvnor _let_0 v0) _let_20) (bvadd _let_24 _let_26))) _let_92 _let_85) (or _let_167 _let_156 (not (= _let_41 _let_8))) (or _let_125 _let_112 _let_158) (or (distinct _let_17 _let_11) _let_164 _let_122) (or _let_106 _let_56 _let_83) (or _let_165 _let_163 (bvuge _let_9 ((_ sign_extend 3) _let_23))) (or _let_162 _let_159 (not _let_107)) (or _let_130 _let_167 _let_132) (or (= (ite (distinct (_ bv0 4) (bvor (bvnor _let_0 v0) ((_ zero_extend 3) _let_2))) (_ bv1 1) (_ bv0 1)) _let_1) _let_155 _let_111) (or (not _let_119) (bvsle _let_53 _let_21) _let_98) (or _let_99 _let_78 _let_127) (or _let_166 _let_116 _let_71) (or _let_151 _let_73 _let_107)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) ))
