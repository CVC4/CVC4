(set-option :incremental false)
(set-info :source "MathSat group")
(set-info :status unsat)
(set-info :category "random")
(set-info :difficulty "3")
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
(check-sat-assuming ( (let ((_let_0 (f0_1 x8))) (let ((_let_1 (+ (- (* (/ (- 0 2) 1) x8) (* 20.0 x4)) (* 15.0 x3)))) (let ((_let_2 (f1_2 x7 x1))) (let ((_let_3 (f0_1 (f1_2 x9 x4)))) (let ((_let_4 (f0_1 (- (+ (* 28.0 x2) (* 12.0 x1)) (* 1.0 x5))))) (let ((_let_5 (f0_1 (f1_1 x2)))) (let ((_let_6 (f1_1 (f1_2 x0 x3)))) (let ((_let_7 (< _let_3 (/ (- 0 16) 1)))) (let ((_let_8 (< x4 (/ (- 0 28) 1)))) (let ((_let_9 (< (f1_1 x6) (/ (- 0 11) 1)))) (let ((_let_10 (< _let_3 22.0))) (let ((_let_11 (< (f1_1 x2) (/ (- 0 4) 1)))) (let ((_let_12 (= (- (+ (* 28.0 x2) (* 12.0 x1)) (* 1.0 x5)) (- (+ (* 28.0 x2) (* 12.0 x1)) (* 1.0 x5))))) (let ((_let_13 (< _let_1 (/ (- 0 2) 1)))) (let ((_let_14 (= (- (- (* 9.0 _let_6) (* 15.0 (+ (- (* 19.0 x0) (* 26.0 x5)) (* 13.0 x4)))) (* 10.0 (f1_2 x0 x3))) (f0_2 x9 x1)))) (let ((_let_15 (< (f0_2 x1 x0) 22.0))) (let ((_let_16 (= (f0_2 x0 x2) (f1_2 (f1_1 (f1_1 x7)) (f1_2 x0 x3))))) (let ((_let_17 (< x0 21.0))) (let ((_let_18 (< (f0_2 x1 x0) (/ (- 0 14) 1)))) (let ((_let_19 (< (+ (+ (* 2.0 x2) (* 25.0 x4)) (* 5.0 x0)) 22.0))) (let ((_let_20 (< _let_1 1.0))) (let ((_let_21 (= (f0_2 x1 x0) (f1_2 x8 x9)))) (let ((_let_22 (= (f0_2 x0 x2) (f1_1 x2)))) (let ((_let_23 (< (+ (+ (* 18.0 (f0_1 x3)) (* 25.0 x4)) (* 13.0 _let_1)) 20.0))) (let ((_let_24 (< _let_5 (/ (- 0 20) 1)))) (let ((_let_25 (= x0 x4))) (let ((_let_26 (< _let_6 (/ (- 0 8) 1)))) (let ((_let_27 (< (f0_2 x0 x2) (/ (- 0 8) 1)))) (let ((_let_28 (< (f1_1 x2) 26.0))) (let ((_let_29 (= (f1_2 x8 x9) (f1_1 x2)))) (let ((_let_30 (= (+ (+ (* 18.0 (f0_1 x3)) (* 25.0 x4)) (* 13.0 _let_1)) (f1_1 x2)))) (let ((_let_31 (< (- (+ (* 28.0 x2) (* 12.0 x1)) (* 1.0 x5)) 24.0))) (let ((_let_32 (< (f1_2 x9 x4) 9.0))) (let ((_let_33 (< _let_4 13.0))) (let ((_let_34 (< _let_2 16.0))) (let ((_let_35 (< (f1_1 x7) 9.0))) (let ((_let_36 (= (+ (+ (* 15.0 x2) (* 20.0 (f1_2 x8 x9))) (* 3.0 _let_1)) (f0_2 x9 x1)))) (let ((_let_37 (< (f1_2 x0 x3) 7.0))) (let ((_let_38 (< x6 9.0))) (let ((_let_39 (< (f1_1 x2) 18.0))) (let ((_let_40 (not _let_7))) (let ((_let_41 (not _let_20))) (let ((_let_42 (not _let_10))) (let ((_let_43 (not (< _let_0 8.0)))) (let ((_let_44 (not (< _let_3 (/ (- 0 3) 1))))) (let ((_let_45 (or _let_44 (< _let_3 (/ (- 0 3) 1))))) (let ((_let_46 (not _let_13))) (let ((_let_47 (not _let_28))) (let ((_let_48 (not (< _let_0 28.0)))) (let ((_let_49 (or (< _let_0 (/ (- 0 13) 1)) (not _let_16)))) (let ((_let_50 (not _let_29))) (let ((_let_51 (not _let_21))) (let ((_let_52 (not _let_12))) (let ((_let_53 (not (< x1 24.0)))) (let ((_let_54 (not (< (f1_1 x8) 16.0)))) (let ((_let_55 (not _let_39))) (let ((_let_56 (not _let_9))) (let ((_let_57 (not _let_23))) (let ((_let_58 (not _let_17))) (let ((_let_59 (not _let_32))) (let ((_let_60 (not (< (f1_1 (f1_1 x2)) (/ (- 0 1) 1))))) (let ((_let_61 (not (< _let_6 (/ (- 0 23) 1))))) (let ((_let_62 (not _let_22))) (let ((_let_63 (not _let_8))) (let ((_let_64 (not (< _let_5 9.0)))) (let ((_let_65 (not _let_35))) (let ((_let_66 (not _let_26))) (let ((_let_67 (not (< (f1_1 x7) (/ (- 0 6) 1))))) (let ((_let_68 (not _let_11))) (let ((_let_69 (not _let_24))) (let ((_let_70 (not _let_30))) (let ((_let_71 (not _let_14))) (let ((_let_72 (not _let_36))) (let ((_let_73 (or _let_72 _let_23))) (let ((_let_74 (not _let_25))) (let ((_let_75 (not _let_33))) (let ((_let_76 (not (= (f1_1 x9) (- (- (* (/ (- 0 8) 1) x1) (* 17.0 x2)) (* 21.0 x5)))))) (let ((_let_77 (not _let_18))) (let ((_let_78 (not (< _let_2 (/ (- 0 13) 1))))) (let ((_let_79 (not _let_34))) (let ((_let_80 (not _let_38))) (let ((_let_81 (not _let_37))) (let ((_let_82 (not (< x5 (/ (- 0 12) 1))))) (let ((_let_83 (not _let_27))) (let ((_let_84 (not (< (f1_1 x8) 0.0)))) (let ((_let_85 (not (< _let_0 (/ (- 0 13) 1))))) (let ((_let_86 (not _let_15))) (let ((_let_87 (not (= _let_2 (f1_2 _let_4 x2))))) (let ((_let_88 (not _let_31))) (let ((_let_89 (not (= (f0_1 x5) (f0_2 x1 x9))))) (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (or (or _let_40 _let_41) (not P1)) (or (or _let_42 _let_43) _let_38)) (or (or _let_27 (not P3)) _let_16)) (or _let_45 _let_46)) (or (or _let_47 _let_35) _let_48)) (or _let_49 _let_50)) (or (or _let_51 P2) (< x5 (/ (- 0 12) 1)))) (or (or _let_52 _let_50) _let_53)) (or (or (= _let_2 (f1_2 _let_4 x2)) _let_10) (not _let_19))) (or (or _let_54 P9) _let_55)) (or (or (not _let_16) _let_56) _let_57)) (or (or _let_29 _let_15) _let_32)) (or (or P5 (< _let_0 28.0)) (< _let_6 (/ (- 0 23) 1)))) (or (or _let_58 _let_19) _let_59)) (or (or _let_60 _let_22) _let_41)) (or (or _let_44 _let_34) (not P4))) (or (or _let_61 _let_62) _let_63)) (or (or _let_18 P7) _let_64)) (or (or _let_37 _let_20) _let_47)) (or (or _let_51 _let_65) _let_66)) (or (or _let_50 _let_16) (< (f1_1 x7) (/ (- 0 6) 1)))) (or (or (not P7) _let_30) _let_59)) (or (or (not P0) (not _let_16)) _let_67)) (or (or (= (f0_1 x5) (f0_2 x1 x9)) P9) _let_12)) (or (or _let_68 _let_59) _let_36)) (or (or _let_59 _let_8) _let_27)) (or (or _let_61 _let_69) _let_51)) (or (or (< x1 24.0) (< _let_0 28.0)) (< x5 (/ (- 0 12) 1)))) (or (or (= (f0_1 x5) (f0_2 x1 x9)) (not P2)) _let_41)) (or (or (= _let_2 (f1_2 _let_4 x2)) _let_52) (< (f1_1 (f1_1 x2)) (/ (- 0 1) 1)))) (or (or _let_41 _let_36) _let_60)) (or (or P2 _let_68) _let_70)) (or (or _let_11 _let_10) _let_12)) (or (or P3 _let_18) _let_69)) (or (or _let_52 _let_21) _let_13)) (or (or _let_71 _let_48) (< (f1_1 (f1_1 x2)) (/ (- 0 1) 1)))) (or _let_73 _let_15)) (or (or _let_43 _let_12) _let_74)) (or (or _let_41 (< _let_0 (/ (- 0 13) 1))) (< _let_0 8.0))) (or (or (< (f1_1 x8) 0.0) _let_75) _let_20)) (or (or _let_76 P5) _let_28)) (or (or _let_77 _let_65) _let_22)) (or (or _let_39 _let_30) _let_56)) (or (or _let_78 _let_19) _let_62)) (or (or _let_22 _let_57) (not P5))) (or (or P6 _let_76) (not P2))) (or (or _let_53 _let_53) _let_10)) (or (or _let_79 (not P4)) _let_46)) (or (or (< _let_3 (/ (- 0 3) 1)) _let_48) _let_80)) (or (or _let_75 (not P8)) _let_81)) (or (or (< x5 (/ (- 0 12) 1)) (not P6)) P1)) (or (or _let_62 _let_43) (not P8))) (or (or _let_42 (not P8)) _let_12)) (or (or _let_19 (not P1)) _let_11)) (or (or (not P7) _let_82) _let_48)) (or (or (not P8) (= (f0_1 x5) (f0_2 x1 x9))) _let_12)) (or (or _let_15 _let_58) _let_28)) (or (or _let_61 _let_64) _let_57)) (or (or (< _let_0 28.0) (< x5 (/ (- 0 12) 1))) _let_79)) (or (or _let_32 P3) _let_41)) (or (or _let_30 _let_10) (< _let_0 28.0))) (or (or _let_21 _let_27) _let_79)) (or (or _let_76 _let_29) _let_41)) (or (or _let_46 _let_43) (not P8))) (or (or (< (f1_1 x8) 0.0) (not P6)) P2)) (or (or _let_14 _let_17) _let_50)) (or (or _let_51 _let_32) _let_79)) (or (or _let_16 (< _let_0 8.0)) _let_47)) (or (or _let_38 _let_81) (< x1 24.0))) (or (or (< (f1_1 x8) 0.0) (< _let_0 8.0)) _let_67)) (or (or _let_9 _let_17) _let_39)) (or (or _let_36 _let_12) (not _let_16))) (or (or (= (f1_1 x9) (- (- (* (/ (- 0 8) 1) x1) (* 17.0 x2)) (* 21.0 x5))) _let_33) _let_62)) (or (or _let_38 _let_29) (not P8))) (or (or (< _let_3 (/ (- 0 3) 1)) (not P5)) _let_82)) (or (or _let_83 (not P6)) _let_52)) (or (or _let_48 _let_84) (< _let_6 (/ (- 0 23) 1)))) (or (or (not P4) _let_43) _let_41)) (or (or P7 P9) _let_40)) (or (or (< _let_0 8.0) _let_10) P0)) (or (or P5 (not P3)) _let_58)) (or (or _let_28 (< _let_3 (/ (- 0 3) 1))) P5)) (or (or _let_42 _let_58) P2)) (or _let_45 _let_19)) (or (or (< x1 24.0) (< _let_5 9.0)) _let_85)) (or (or _let_84 _let_69) P4)) (or (or P1 (< _let_0 8.0)) _let_76)) (or (or _let_17 _let_69) _let_78)) (or (or (< _let_0 (/ (- 0 13) 1)) _let_10) _let_32)) (or (or _let_76 _let_9) P7)) (or (or P9 (not P9)) _let_62)) (or (or (not P0) _let_52) _let_86)) (or (or _let_15 _let_64) (not P0))) (or (or _let_29 (< _let_3 (/ (- 0 3) 1))) _let_21)) (or (or (< (f1_1 (f1_1 x2)) (/ (- 0 1) 1)) _let_57) _let_24)) (or (or (< x1 24.0) P8) _let_87)) (or (or (not P4) _let_80) (< _let_0 28.0))) (or (or _let_55 (not _let_16)) _let_88)) (or (or (= (f1_1 x9) (- (- (* (/ (- 0 8) 1) x1) (* 17.0 x2)) (* 21.0 x5))) P1) _let_74)) (or (or _let_86 (< (f1_1 x8) 0.0)) (< _let_0 28.0))) (or (or _let_58 P9) (not P1))) (or (or P8 P1) _let_10)) (or (or _let_42 _let_27) (not P1))) (or _let_49 _let_77)) (or (or _let_19 (< _let_5 9.0)) _let_50)) (or (or _let_64 _let_81) P2)) (or (or _let_78 _let_57) _let_87)) (or (or (not P4) (< (f1_1 x8) 0.0)) _let_16)) (or (or P5 _let_68) _let_27)) (or (or _let_89 _let_14) _let_43)) (or (or _let_20 _let_38) _let_78)) (or (or _let_8 _let_74) _let_48)) (or (or _let_56 _let_77) P4)) (or (or (not P2) _let_83) (< _let_0 (/ (- 0 13) 1)))) (or (or _let_19 _let_48) (< (f1_1 x7) (/ (- 0 6) 1)))) (or (or (not P7) _let_43) _let_53)) (or (or _let_88 P4) _let_8)) (or (or _let_17 (< _let_3 (/ (- 0 3) 1))) _let_86)) (or (or (not P3) _let_48) _let_15)) (or (or _let_37 P9) _let_50)) (or (or _let_63 (= _let_2 (f1_2 _let_4 x2))) (= (f1_1 x9) (- (- (* (/ (- 0 8) 1) x1) (* 17.0 x2)) (* 21.0 x5))))) (or (or _let_39 _let_42) _let_54)) (or (or _let_42 (= (f0_1 x5) (f0_2 x1 x9))) (not P3))) (or (or _let_89 _let_39) _let_77)) (or (or _let_59 _let_53) _let_20)) (or (or _let_44 P8) _let_67)) (or _let_73 _let_52)) (or (or (< _let_0 (/ (- 0 13) 1)) _let_37) _let_43)) (or (or _let_61 _let_66) _let_70)) (or (or (not P7) _let_89) _let_68)) (or (or (not P2) _let_88) _let_47)) (or (or _let_36 _let_75) (not P4))) (or (or _let_23 (< _let_6 (/ (- 0 23) 1))) (< (f1_1 x7) (/ (- 0 6) 1)))) (or (or (< _let_0 (/ (- 0 13) 1)) _let_67) _let_33)) (or (or P4 (not P5)) _let_86)) (or (or (not P3) _let_9) _let_32)) (or (or _let_11 _let_23) _let_12)) (or (or _let_51 _let_48) P0)) (or (or _let_38 _let_26) P8)) (or (or _let_19 P7) (< (f1_1 (f1_1 x2)) (/ (- 0 1) 1)))) (or (or (not P4) _let_36) _let_62)) (or (or _let_51 _let_40) _let_65)) (or (or _let_7 _let_46) _let_61)) (or (or P7 _let_12) _let_77)) (or (or _let_83 _let_25) _let_79)) (or (or _let_17 _let_46) _let_7)) (or (or _let_36 (< (f1_1 (f1_1 x2)) (/ (- 0 1) 1))) _let_13)) (or (or _let_71 _let_50) _let_74)) (or (or (< (f1_1 x7) (/ (- 0 6) 1)) (< _let_0 (/ (- 0 13) 1))) P4)) (or (or _let_54 _let_30) _let_20)) (or (or _let_26 _let_15) _let_81)) (or (or (< x1 24.0) _let_71) _let_87)) (or (or (< (f1_1 x7) (/ (- 0 6) 1)) _let_83) _let_25)) (or _let_49 _let_54)) (or (or _let_65 _let_61) (= _let_2 (f1_2 _let_4 x2)))) (or (or _let_80 _let_75) _let_69)) (or (or _let_34 P5) (< _let_0 (/ (- 0 13) 1)))) (or (or _let_14 _let_7) _let_15)) (or (or _let_25 _let_25) _let_32)) (or (or _let_31 _let_67) (< _let_0 28.0))) (or (or (< _let_0 8.0) _let_87) _let_80)) (or (or _let_31 _let_54) _let_21)) (or (or (not P2) (< x1 24.0)) _let_19)) (or (or _let_41 _let_30) _let_19)) (or (or _let_33 _let_29) _let_11)) (or (or _let_53 _let_52) _let_75)) (or (or _let_70 _let_9) (not P5))) (or (or _let_17 _let_19) _let_76)) (or (or _let_67 (not P2)) _let_70)) (or (or (< _let_5 9.0) (< (f1_1 x8) 0.0)) _let_86)) (or (or (not P0) _let_55) _let_68)) (or (or _let_47 _let_63) (not P4))) (or (or P3 _let_41) _let_87)) (or (or _let_64 (not P7)) _let_80)) (or (or _let_51 _let_67) P8)) (or (or _let_82 _let_64) _let_87)) (or (or _let_48 (not P9)) _let_38)) (or (or _let_46 _let_22) _let_14)) (or (or _let_59 _let_22) _let_20)) (or (or _let_60 _let_70) _let_84)) (or (or _let_10 _let_28) _let_44)) (or (or (= (f0_1 x5) (f0_2 x1 x9)) _let_60) _let_82)) (or (or _let_48 P3) _let_89)) (or (or _let_18 _let_76) _let_89)) (or (or _let_37 _let_33) _let_43)) (or (or _let_24 _let_50) _let_18)) (or (or _let_85 _let_44) _let_8)) (or (or P9 _let_12) _let_80)) (or (or _let_72 _let_30) (not P4))) (or (or _let_34 _let_24) P3)) (or (or P6 _let_67) _let_53)) (or (or _let_36 _let_26) _let_27)) (or (or _let_58 _let_84) _let_43)) (or (or _let_11 _let_87) _let_61)) (or (or _let_42 _let_65) _let_84)) (or (or _let_27 P5) _let_85)) (or (or P3 _let_75) _let_25)) (or (or _let_64 _let_44) P5)) (or (or _let_20 _let_12) _let_39)) (or (or _let_35 (not P3)) _let_55)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) ))
