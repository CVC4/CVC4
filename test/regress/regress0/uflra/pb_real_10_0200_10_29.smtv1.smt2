(set-option :incremental false)
(meta-info :source "MathSat group")
(meta-info :status unsat)
(meta-info :category "random")
(meta-info :difficulty "3")
(set-logic QF_UFLRA)
(declare-fun f0_1 (Real) Real)
(declare-fun f0_2 (Real Real) Real)
(declare-fun f0_3 (Real Real Real) Real)
(declare-fun f0_4 (Real Real Real Real) Real)
(declare-fun f1_1 (Real) Real)
(declare-fun f1_2 (Real Real) Real)
(declare-fun f1_3 (Real Real Real) Real)
(declare-fun f1_4 (Real Real Real Real) Real)
(declare-fun x0 () Real)
(declare-fun x1 () Real)
(declare-fun x2 () Real)
(declare-fun x3 () Real)
(declare-fun x4 () Real)
(declare-fun x5 () Real)
(declare-fun x6 () Real)
(declare-fun x7 () Real)
(declare-fun x8 () Real)
(declare-fun x9 () Real)
(declare-fun P0 () Bool)
(declare-fun P1 () Bool)
(declare-fun P2 () Bool)
(declare-fun P3 () Bool)
(declare-fun P4 () Bool)
(declare-fun P5 () Bool)
(declare-fun P6 () Bool)
(declare-fun P7 () Bool)
(declare-fun P8 () Bool)
(declare-fun P9 () Bool)
(check-sat-assuming ( (let ((_let_0 (+ (- (* 13.0 x2) (* 27.0 x7)) (* 14.0 x6)))) (let ((_let_1 (f0_2 x2 x3))) (let ((_let_2 (f1_2 x3 x3))) (let ((_let_3 (f1_2 x4 _let_0))) (let ((_let_4 (f1_2 x3 x2))) (let ((_let_5 (+ (- (* (/ (- 0 14) 1) x6) (* 18.0 x9)) (* 23.0 x0)))) (let ((_let_6 (- (- (* 10.0 (- (+ (* (/ (- 0 4) 1) x1) (* 4.0 x6)) (* 12.0 _let_3))) (* 15.0 _let_4)) (* 2.0 (- (- (* 29.0 x6) (* 23.0 x6)) (* 9.0 x3)))))) (let ((_let_7 (f0_1 x3))) (let ((_let_8 (- 0 23))) (let ((_let_9 (+ (+ (* 26.0 x9) (* 15.0 x5)) (* 2.0 x4)))) (let ((_let_10 (- 0 13))) (let ((_let_11 (f1_2 x1 x4))) (let ((_let_12 (= (f1_2 x7 x2) x6))) (let ((_let_13 (< (f1_1 x2) 18.0))) (let ((_let_14 (< (f0_2 x2 x0) 24.0))) (let ((_let_15 (< x7 (/ (- 0 3) 1)))) (let ((_let_16 (< (f0_1 (- (- (* (/ (- 0 9) 1) x9) (* 28.0 x8)) (* 17.0 x6))) (/ (- 0 28) 1)))) (let ((_let_17 (= (f1_1 x5) _let_11))) (let ((_let_18 (= (f1_1 x8) _let_4))) (let ((_let_19 (< (f0_2 (f0_2 x2 x0) (- (+ (* (/ (- 0 29) 1) x0) (* 2.0 x7)) (* 9.0 x6))) (/ _let_10 1)))) (let ((_let_20 (< (f1_1 x0) (/ (- 0 29) 1)))) (let ((_let_21 (< _let_1 (/ (- 0 17) 1)))) (let ((_let_22 (< _let_7 27.0))) (let ((_let_23 (< (f0_1 x6) 25.0))) (let ((_let_24 (= x9 x2))) (let ((_let_25 (= (- (+ (* (/ (- 0 27) 1) (f1_1 x0)) (* 20.0 _let_7)) (* 16.0 _let_0)) (f0_2 (f0_2 x2 x0) (- (+ (* (/ (- 0 29) 1) x0) (* 2.0 x7)) (* 9.0 x6)))))) (let ((_let_26 (- 0 10))) (let ((_let_27 (< (f1_1 x0) (/ _let_26 1)))) (let ((_let_28 (< _let_5 2.0))) (let ((_let_29 (< (- (- (* (/ (- 0 12) 1) _let_9) (* 17.0 (- (+ (* (/ (- 0 29) 1) x0) (* 2.0 x7)) (* 9.0 x6)))) (* 15.0 _let_6)) 17.0))) (let ((_let_30 (< (f0_1 (+ (- (* (/ _let_8 1) x9) (* 27.0 x6)) (* 1.0 x8))) 25.0))) (let ((_let_31 (< _let_9 27.0))) (let ((_let_32 (= _let_11 (f1_2 x0 x9)))) (let ((_let_33 (< _let_7 12.0))) (let ((_let_34 (< _let_3 26.0))) (let ((_let_35 (< (f0_1 (- (- (* (/ (- 0 9) 1) x9) (* 28.0 x8)) (* 17.0 x6))) 11.0))) (let ((_let_36 (< x5 16.0))) (let ((_let_37 (< _let_2 11.0))) (let ((_let_38 (< _let_7 14.0))) (let ((_let_39 (< x9 (/ (- 0 8) 1)))) (let ((_let_40 (< x5 (/ _let_26 1)))) (let ((_let_41 (< x3 (/ (- 0 19) 1)))) (let ((_let_42 (< (- (+ (* 24.0 x0) (* 26.0 x6)) (* 1.0 x7)) (/ (- 0 15) 1)))) (let ((_let_43 (= x0 (f0_2 (f0_2 x2 x0) (- (+ (* (/ (- 0 29) 1) x0) (* 2.0 x7)) (* 9.0 x6)))))) (let ((_let_44 (< _let_2 26.0))) (let ((_let_45 (< _let_5 9.0))) (let ((_let_46 (= _let_1 (- (+ (* (/ _let_10 1) x9) (* 19.0 x6)) (* 23.0 x1))))) (let ((_let_47 (not (= _let_4 (f1_1 x8))))) (let ((_let_48 (not _let_17))) (let ((_let_49 (not _let_18))) (let ((_let_50 (not _let_27))) (let ((_let_51 (not (< (- (- (* (/ (- 0 9) 1) x9) (* 28.0 x8)) (* 17.0 x6)) (/ (- 0 24) 1))))) (let ((_let_52 (not (< _let_7 7.0)))) (let ((_let_53 (not (< _let_4 17.0)))) (let ((_let_54 (not _let_41))) (let ((_let_55 (not _let_40))) (let ((_let_56 (not (< (f1_1 x5) 28.0)))) (let ((_let_57 (not _let_13))) (let ((_let_58 (not _let_29))) (let ((_let_59 (not _let_15))) (let ((_let_60 (not _let_19))) (let ((_let_61 (not _let_37))) (let ((_let_62 (not _let_23))) (let ((_let_63 (not (= _let_2 (f1_1 x2))))) (let ((_let_64 (not (< _let_11 10.0)))) (let ((_let_65 (not _let_43))) (let ((_let_66 (not _let_20))) (let ((_let_67 (not (< _let_6 29.0)))) (let ((_let_68 (not _let_36))) (let ((_let_69 (not _let_34))) (let ((_let_70 (not _let_14))) (let ((_let_71 (not _let_25))) (let ((_let_72 (not _let_22))) (let ((_let_73 (not _let_33))) (let ((_let_74 (not _let_38))) (let ((_let_75 (not _let_45))) (let ((_let_76 (not _let_42))) (let ((_let_77 (not _let_31))) (let ((_let_78 (not _let_30))) (let ((_let_79 (not (< x5 19.0)))) (let ((_let_80 (not (< _let_3 13.0)))) (let ((_let_81 (not (< (f1_1 x8) (/ (- 0 15) 1))))) (let ((_let_82 (not _let_35))) (let ((_let_83 (or _let_64 _let_74))) (let ((_let_84 (not _let_21))) (let ((_let_85 (not (< (- (+ (* (/ (- 0 27) 1) (f1_1 x0)) (* 20.0 _let_7)) (* 16.0 _let_0)) (/ (- 0 18) 1))))) (let ((_let_86 (not (< x0 (/ _let_8 1))))) (let ((_let_87 (not _let_39))) (let ((_let_88 (not (< (f0_2 x2 x0) 27.0)))) (let ((_let_89 (not _let_24))) (let ((_let_90 (not _let_46))) (let ((_let_91 (not (< _let_3 9.0)))) (let ((_let_92 (not _let_44))) (let ((_let_93 (not _let_12))) (let ((_let_94 (not _let_32))) (let ((_let_95 (not _let_16))) (let ((_let_96 (not _let_28))) (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (or (or _let_47 _let_33) _let_48) (or (or _let_49 (< x0 (/ _let_8 1))) _let_50)) (or (or _let_30 (not P8)) _let_51)) (or (or _let_52 _let_38) P0)) (or (or (not P3) _let_25) (< _let_3 9.0))) (or (or _let_51 P2) (not P9))) (or (or _let_53 (< (- (+ (* (/ (- 0 27) 1) (f1_1 x0)) (* 20.0 _let_7)) (* 16.0 _let_0)) (/ (- 0 18) 1))) _let_25)) (or (or _let_54 _let_25) (not P3))) (or (or (not P4) _let_32) _let_55)) (or (or _let_45 _let_53) _let_53)) (or (or _let_19 _let_55) (not P9))) (or (or _let_23 _let_56) _let_20)) (or (or (= _let_2 (f1_1 x2)) (< (f1_1 x5) 28.0)) _let_57)) (or (or P8 _let_58) _let_28)) (or (or _let_59 (not P8)) P7)) (or (or _let_23 _let_20) (< _let_6 29.0))) (or (or _let_45 _let_60) P4)) (or (or _let_38 _let_17) (not P1))) (or (or _let_61 _let_62) _let_63)) (or (or _let_45 _let_28) _let_12)) (or (or _let_24 (< (f1_1 x5) 28.0)) _let_64)) (or (or (< (f0_2 x2 x0) 27.0) (not P5)) _let_57)) (or (or _let_16 (< _let_11 10.0)) _let_65)) (or (or _let_40 _let_66) _let_36)) (or (or _let_53 P1) _let_56)) (or (or _let_60 (= _let_2 (f1_1 x2))) P0)) (or (or _let_30 P7) _let_23)) (or (or P0 _let_38) _let_13)) (or (or _let_67 _let_67) _let_50)) (or (or _let_61 _let_68) _let_63)) (or (or (not P2) (not P9)) (< (f1_1 x8) (/ (- 0 15) 1)))) (or (or (not P9) _let_24) _let_69)) (or (or _let_70 (< x0 (/ _let_8 1))) P4)) (or (or _let_71 _let_16) _let_72)) (or (or _let_55 _let_37) (< _let_11 10.0))) (or (or _let_73 _let_74) _let_75)) (or (or _let_53 _let_76) _let_70)) (or (or _let_77 _let_38) _let_20)) (or (or _let_34 _let_27) _let_51)) (or (or _let_61 _let_16) _let_68)) (or (or _let_16 _let_21) (not P3))) (or (or (not P5) _let_16) (not P9))) (or (or _let_24 _let_58) (not P5))) (or (or _let_44 (not P5)) _let_78)) (or (or _let_12 _let_12) _let_68)) (or (or _let_65 (= _let_4 (f1_1 x8))) P8)) (or (or (< _let_3 9.0) _let_61) _let_22)) (or (or _let_43 _let_35) _let_39)) (or (or _let_63 _let_29) _let_28)) (or (or _let_13 _let_79) (not P9))) (or (or _let_19 P9) _let_80)) (or (or _let_16 _let_27) (< (f0_2 x2 x0) 27.0))) (or (or (not P2) _let_17) _let_81)) (or (or (< (f1_1 x8) (/ (- 0 15) 1)) _let_82) (not P9))) (or _let_83 _let_54)) (or (or (not P2) _let_62) _let_59)) (or (or (< _let_3 13.0) _let_74) (not P7))) (or (or (< (f1_1 x5) 28.0) (< (f0_2 x2 x0) 27.0)) _let_33)) (or (or (not P1) _let_60) _let_84)) (or (or _let_36 (not P3)) _let_25)) (or (or _let_20 _let_38) _let_24)) (or (or _let_57 P6) (< x0 (/ _let_8 1)))) (or (or _let_13 _let_45) (< _let_3 13.0))) (or (or _let_71 _let_85) (< (f1_1 x5) 28.0))) (or (or _let_60 _let_55) _let_50)) (or (or _let_86 _let_44) _let_55)) (or (or _let_53 _let_85) _let_61)) (or (or _let_15 _let_46) _let_34)) (or (or P9 (< _let_11 10.0)) _let_65)) (or (or (< _let_4 17.0) P2) (< (- (- (* (/ (- 0 9) 1) x9) (* 28.0 x8)) (* 17.0 x6)) (/ (- 0 24) 1)))) (or (or _let_28 _let_82) _let_56)) (or (or _let_27 (not P6)) _let_23)) (or (or _let_80 _let_52) _let_78)) (or (or P1 _let_42) P9)) (or (or _let_87 _let_57) P1)) (or (or _let_75 _let_37) (not P7))) (or (or _let_32 _let_50) _let_84)) (or (or _let_16 (not P3)) _let_19)) (or (or _let_40 _let_53) _let_52)) (or (or _let_88 _let_87) _let_16)) (or (or _let_25 _let_57) _let_53)) (or (or _let_36 _let_58) _let_89)) (or (or _let_71 _let_17) (< _let_3 9.0))) (or (or (not P6) _let_27) _let_79)) (or (or P3 (= _let_2 (f1_1 x2))) _let_47)) (or (or _let_28 _let_13) _let_87)) (or (or _let_85 _let_64) P7)) (or (or _let_15 _let_57) _let_82)) (or (or _let_90 _let_53) P8)) (or (or _let_77 P2) (< _let_4 17.0))) (or (or _let_57 _let_53) (not P7))) (or (or _let_64 _let_91) _let_67)) (or (or _let_68 P7) _let_41)) (or (or _let_70 _let_56) P5)) (or (or _let_90 _let_49) _let_17)) (or (or _let_76 _let_47) _let_49)) (or (or (not P7) _let_75) _let_74)) (or (or _let_14 P1) _let_18)) (or (or _let_65 _let_58) (< (- (- (* (/ (- 0 9) 1) x9) (* 28.0 x8)) (* 17.0 x6)) (/ (- 0 24) 1)))) (or (or _let_12 _let_90) (< (f1_1 x8) (/ (- 0 15) 1)))) (or (or _let_30 (< _let_4 17.0)) _let_55)) (or (or (< (- (+ (* (/ (- 0 27) 1) (f1_1 x0)) (* 20.0 _let_7)) (* 16.0 _let_0)) (/ (- 0 18) 1)) _let_25) (not P1))) (or (or _let_89 _let_35) P6)) (or (or _let_15 _let_60) _let_23)) (or (or (not P0) _let_92) _let_18)) (or (or _let_14 _let_23) _let_58)) (or (or _let_64 _let_52) _let_19)) (or (or (< x5 19.0) _let_28) _let_69)) (or (or _let_38 _let_45) (< (f1_1 x8) (/ (- 0 15) 1)))) (or (or (not P1) _let_38) _let_91)) (or (or _let_45 _let_86) _let_37)) (or (or _let_69 _let_41) _let_25)) (or (or _let_23 P3) _let_20)) (or (or _let_16 _let_13) P8)) (or (or _let_64 _let_73) (not P2))) (or (or _let_44 _let_87) _let_93)) (or (or _let_20 P2) _let_23)) (or (or _let_79 (< _let_4 17.0)) _let_49)) (or (or _let_22 (< (- (- (* (/ (- 0 9) 1) x9) (* 28.0 x8)) (* 17.0 x6)) (/ (- 0 24) 1))) (< _let_3 9.0))) (or (or P3 _let_53) (< _let_7 7.0))) (or (or _let_91 _let_21) P0)) (or (or (< _let_7 7.0) _let_41) _let_20)) (or (or _let_53 _let_94) P8)) (or (or _let_85 _let_44) P9)) (or (or (not P0) _let_49) _let_40)) (or (or (< _let_3 13.0) P3) (< _let_6 29.0))) (or _let_83 _let_94)) (or (or _let_93 _let_45) _let_17)) (or (or (not P1) (not P6)) _let_18)) (or (or (not P3) (= _let_2 (f1_1 x2))) _let_90)) (or (or _let_51 _let_67) (< _let_11 10.0))) (or (or _let_57 _let_87) _let_34)) (or (or (not P6) _let_56) _let_62)) (or (or _let_54 (not P7)) _let_53)) (or (or _let_49 _let_72) _let_27)) (or (or _let_72 _let_54) _let_37)) (or (or _let_18 _let_78) (< _let_3 13.0))) (or (or (not P3) (not P8)) (not P6))) (or (or _let_28 _let_59) _let_55)) (or (or (< x5 19.0) _let_14) (not P4))) (or (or (< (f1_1 x8) (/ (- 0 15) 1)) _let_66) _let_94)) (or (or _let_94 _let_63) _let_12)) (or (or (not P6) _let_85) P7)) (or (or _let_95 _let_72) _let_82)) (or (or _let_92 _let_44) _let_63)) (or (or _let_38 _let_62) _let_63)) (or (or _let_88 _let_17) _let_40)) (or (or _let_84 _let_33) _let_75)) (or (or _let_95 _let_45) _let_93)) (or (or _let_91 _let_91) _let_24)) (or (or P1 _let_34) _let_62)) (or (or _let_96 _let_39) _let_62)) (or (or _let_37 _let_45) _let_85)) (or (or _let_17 _let_57) (< _let_4 17.0))) (or (or _let_16 _let_46) _let_88)) (or (or (= _let_2 (f1_1 x2)) _let_58) _let_79)) (or (or (not P4) _let_46) _let_59)) (or (or _let_62 (< _let_3 9.0)) _let_33)) (or (or (< _let_4 17.0) (not P8)) _let_89)) (or (or P1 _let_40) _let_20)) (or (or (< _let_3 13.0) _let_29) _let_86)) (or (or _let_92 _let_79) (not P9))) (or (or _let_48 _let_42) _let_13)) (or (or _let_67 _let_18) _let_50)) (or (or (< _let_3 9.0) _let_22) _let_96)) (or (or _let_64 (not P7)) _let_51)) (or (or _let_51 (not P2)) _let_56)) (or (or _let_42 _let_30) _let_46)) (or (or P5 P9) _let_80)) (or (or P9 _let_27) (< _let_4 17.0))) (or (or _let_63 _let_91) _let_72)) (or (or _let_60 _let_52) P6)) (or (or _let_68 (< _let_6 29.0)) P8)) (or (or _let_55 _let_44) P1)) (or (or _let_41 (< _let_4 17.0)) _let_42)) (or (or _let_67 _let_52) _let_36)) (or (or _let_19 _let_36) _let_41)) (or (or _let_78 _let_87) P4)) (or (or _let_27 P5) _let_56)) (or (or _let_66 _let_53) _let_81)) (or (or (not P7) _let_15) _let_63)) (or (or _let_29 (not P1)) _let_71)) (or (or _let_81 _let_74) _let_93)) (or (or _let_67 _let_22) _let_12)) (or (or _let_86 _let_76) _let_27)) (or (or _let_58 P3) _let_32)) (or (or P6 _let_28) _let_20)) (or (or _let_64 _let_32) _let_43)) (or (or _let_93 (not P8)) P5)) (or (or _let_23 _let_21) _let_31)) (or (or _let_69 _let_63) _let_40)) (or (or P0 _let_80) P4)) (or (or _let_71 _let_70) _let_29)) (or (or _let_46 _let_25) _let_96)) (or (or _let_62 _let_30) _let_53)) (or (or _let_75 (not P2)) _let_34)) (or (or _let_52 _let_39) _let_91)) (or (or _let_87 (not P1)) _let_86)) (or (or _let_71 _let_67) _let_78)) (or (or _let_42 _let_44) _let_31))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) ))
