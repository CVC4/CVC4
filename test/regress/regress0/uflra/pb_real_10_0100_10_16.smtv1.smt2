(set-option :incremental false)
(set-info :source "MathSat group")
(set-info :status sat)
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
(check-sat-assuming ( (let ((_let_0 (f0_2 x3 x2))) (let ((_let_1 (f1_2 x9 x8))) (let ((_let_2 (+ (- (* 13.0 x7) (* 13.0 x4)) (* 6.0 x2)))) (let ((_let_3 (- 0 22))) (let ((_let_4 (- (- (* (/ (- 0 24) 1) x3) (* 24.0 x8)) (* 4.0 x1)))) (let ((_let_5 (f1_2 (- (- (* (/ (- 0 18) 1) x4) (* 27.0 x1)) (* 24.0 x7)) x4))) (let ((_let_6 (* 22.0 x3))) (let ((_let_7 (- (- _let_6 (* 6.0 x9)) (* 13.0 x2)))) (let ((_let_8 (f0_2 x7 x0))) (let ((_let_9 (f1_2 _let_5 (+ (+ (* (/ (- 0 10) 1) x2) (* 5.0 (f0_1 x1))) (* 2.0 _let_1))))) (let ((_let_10 (+ (+ (* (/ (- 0 12) 1) x8) _let_6) (* 7.0 x4)))) (let ((_let_11 (- (+ (* (/ (- 0 28) 1) (f1_2 x2 x3)) (* 29.0 (f0_2 x4 x7))) (* 5.0 x7)))) (let ((_let_12 (< _let_9 24.0))) (let ((_let_13 (< (- (- (* (/ (- 0 18) 1) x4) (* 27.0 x1)) (* 24.0 x7)) (/ (- 0 20) 1)))) (let ((_let_14 (- 0 8))) (let ((_let_15 (< x9 11.0))) (let ((_let_16 (< _let_7 1.0))) (let ((_let_17 (< _let_8 22.0))) (let ((_let_18 (< _let_4 29.0))) (let ((_let_19 (< _let_1 8.0))) (let ((_let_20 (< (+ (+ (* (/ (- 0 10) 1) x2) (* 5.0 (f0_1 x1))) (* 2.0 _let_1)) 24.0))) (let ((_let_21 (< (f1_1 x2) 0.0))) (let ((_let_22 (< _let_1 11.0))) (let ((_let_23 (< x7 0.0))) (let ((_let_24 (< _let_5 (/ _let_14 1)))) (let ((_let_25 (< _let_8 (/ _let_3 1)))) (let ((_let_26 (= _let_7 (+ (- (* 10.0 x4) (* 6.0 x3)) (* 24.0 x1))))) (let ((_let_27 (= x3 _let_10))) (let ((_let_28 (= _let_4 _let_2))) (let ((_let_29 (= (- (- (* (/ (- 0 18) 1) x4) (* 27.0 x1)) (* 24.0 x7)) (f1_2 x9 x0)))) (let ((_let_30 (< (- (+ (* 19.0 _let_0) (* 5.0 (f1_2 x5 x8))) (* 17.0 x1)) 4.0))) (let ((_let_31 (< (f1_2 x7 x7) (/ (- 0 15) 1)))) (let ((_let_32 (< (f1_1 x6) (/ (- 0 11) 1)))) (let ((_let_33 (< (f1_1 x2) (/ (- 0 29) 1)))) (let ((_let_34 (< _let_1 (/ (- 0 9) 1)))) (let ((_let_35 (< (+ (+ (* 29.0 x2) (* 1.0 x3)) (* 3.0 (f1_1 x2))) 20.0))) (let ((_let_36 (< _let_5 7.0))) (let ((_let_37 (< (f1_1 x2) (/ _let_3 1)))) (let ((_let_38 (< _let_8 (/ (- 0 4) 1)))) (let ((_let_39 (< (+ (+ (* 29.0 x2) (* 1.0 x3)) (* 3.0 (f1_1 x2))) 13.0))) (let ((_let_40 (< _let_1 (/ (- 0 16) 1)))) (let ((_let_41 (< x9 (/ (- 0 19) 1)))) (let ((_let_42 (< (f0_2 x6 x0) 12.0))) (let ((_let_43 (< (f0_2 x8 x3) 19.0))) (let ((_let_44 (< x1 (/ (- 0 16) 1)))) (let ((_let_45 (= (f0_1 x6) x6))) (let ((_let_46 (< x4 (/ (- 0 2) 1)))) (let ((_let_47 (< (f0_1 x4) (/ (- 0 23) 1)))) (let ((_let_48 (< (f0_1 x6) 3.0))) (let ((_let_49 (< _let_8 1.0))) (let ((_let_50 (< (- (+ (* (/ (- 0 1) 1) x1) (* 9.0 _let_1)) (* 2.0 (f0_2 (f0_1 x1) x7))) 0.0))) (let ((_let_51 (< _let_2 (/ (- 0 21) 1)))) (let ((_let_52 (not _let_46))) (let ((_let_53 (not _let_49))) (let ((_let_54 (not _let_40))) (let ((_let_55 (not _let_42))) (let ((_let_56 (not (= (f0_1 x2) (f1_1 x6))))) (let ((_let_57 (not _let_32))) (let ((_let_58 (not _let_21))) (let ((_let_59 (not _let_22))) (let ((_let_60 (not (= x8 (+ (+ (* 29.0 x2) (* 1.0 x3)) (* 3.0 (f1_1 x2))))))) (let ((_let_61 (not _let_18))) (let ((_let_62 (not _let_38))) (let ((_let_63 (not _let_17))) (let ((_let_64 (not _let_45))) (let ((_let_65 (not _let_50))) (let ((_let_66 (not _let_23))) (let ((_let_67 (not _let_48))) (let ((_let_68 (not (< _let_11 (/ (- 0 19) 1))))) (let ((_let_69 (not _let_25))) (let ((_let_70 (not (< _let_10 2.0)))) (let ((_let_71 (not (< _let_10 9.0)))) (let ((_let_72 (not _let_39))) (let ((_let_73 (not _let_36))) (let ((_let_74 (not (< _let_5 9.0)))) (let ((_let_75 (not (< (- (- (* 10.0 x3) (* 22.0 x1)) (* 29.0 x8)) 22.0)))) (let ((_let_76 (not (< (- (+ (* (/ _let_3 1) x0) (* 19.0 x7)) (* 20.0 x1)) 4.0)))) (let ((_let_77 (not _let_47))) (let ((_let_78 (not _let_27))) (let ((_let_79 (or P8 _let_78))) (let ((_let_80 (not _let_33))) (let ((_let_81 (not _let_20))) (let ((_let_82 (not _let_29))) (let ((_let_83 (not _let_19))) (let ((_let_84 (not (< _let_11 15.0)))) (let ((_let_85 (not _let_26))) (let ((_let_86 (not (= _let_1 _let_9)))) (let ((_let_87 (not _let_13))) (let ((_let_88 (not _let_16))) (let ((_let_89 (not _let_15))) (let ((_let_90 (not _let_34))) (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (or (or _let_52 _let_53) _let_54) (or (or _let_55 (< _let_11 15.0)) (not P2))) (or (or _let_56 _let_55) _let_57)) (or (or P6 P9) _let_58)) (or (or (< _let_5 9.0) _let_59) (< (- (+ (* (/ _let_3 1) x0) (* 19.0 x7)) (* 20.0 x1)) 4.0))) (or (or _let_60 _let_59) _let_61)) (or (or (not P8) _let_62) _let_12)) (or (or _let_57 _let_23) _let_63)) (or (or _let_19 _let_64) P3)) (or (or (not P0) _let_65) _let_43)) (or (or (< _let_11 15.0) _let_66) _let_61)) (or (or _let_55 _let_67) (not P0))) (or (or (not _let_35) _let_13) _let_68)) (or (or (not P5) (not P7)) _let_31)) (or (or (not P0) _let_69) _let_41)) (or (or _let_70 (< _let_11 15.0)) _let_65)) (or (or _let_25 _let_71) _let_41)) (or (or _let_42 P4) _let_72)) (or (or _let_13 (< _let_5 9.0)) _let_47)) (or (or _let_50 (not _let_44)) _let_73)) (or (or (not _let_31) (< (- (- (* 10.0 x3) (* 22.0 x1)) (* 29.0 x8)) 22.0)) P1)) (or (or _let_26 _let_25) _let_36)) (or (or _let_62 P8) (not P3))) (or (or _let_73 (not P6)) _let_43)) (or (or (< _let_10 2.0) _let_74) _let_75)) (or (or _let_25 _let_41) (= _let_1 _let_9))) (or (or _let_76 _let_57) _let_69)) (or (or _let_68 _let_69) _let_77)) (or _let_79 _let_33)) (or (or _let_64 _let_30) _let_46)) (or (or P5 _let_30) _let_56)) (or (or _let_80 _let_40) _let_81)) (or (or _let_64 _let_25) _let_72)) (or (or _let_25 _let_28) _let_73)) (or (or _let_22 _let_18) _let_41)) (or (or _let_45 _let_32) P5)) (or (or _let_69 _let_19) _let_66)) (or (or _let_82 _let_61) (= _let_1 _let_9))) (or (or P7 _let_43) _let_70)) (or (or _let_82 _let_27) (not P8))) (or _let_79 P9)) (or (or _let_83 _let_61) _let_42)) (or (or _let_28 _let_46) (not P6))) (or (or _let_49 _let_17) P4)) (or (or _let_44 P2) _let_70)) (or (or P0 _let_16) _let_81)) (or (or _let_38 _let_84) _let_28)) (or (or _let_18 _let_17) _let_27)) (or (or _let_60 (not P3)) _let_42)) (or (or _let_85 _let_31) P8)) (or (or (not P8) _let_58) _let_67)) (or (or _let_35 _let_85) (not _let_24))) (or (or _let_86 _let_48) _let_87)) (or (or _let_87 (not _let_51)) _let_69)) (or (or (not P7) _let_25) _let_13)) (or (or _let_88 _let_26) (not P0))) (or (or _let_50 _let_82) _let_17)) (or (or _let_81 (not _let_12)) (not P9))) (or (or _let_33 _let_89) _let_26)) (or (or (not P8) _let_70) (not P1))) (or (or _let_90 _let_80) _let_68)) (or (or _let_46 (< (- (- (* 10.0 x3) (* 22.0 x1)) (* 29.0 x8)) 22.0)) (not P9))) (or (or _let_35 (not _let_43)) (not P8))) (or (or _let_29 _let_83) _let_39)) (or (or (not P7) (not _let_41)) _let_58)) (or (or _let_12 _let_18) _let_66)) (or (or _let_80 _let_90) _let_65)) (or (or _let_71 _let_15) _let_41)) (or (or _let_55 _let_33) P9)) (or (or _let_34 _let_89) _let_84)) (or (or _let_67 _let_13) _let_57)) (or (or _let_63 _let_20) _let_23)) (or (or _let_51 P2) P6)) (or (or _let_24 _let_23) _let_63)) (or (or _let_54 (not P7)) _let_88)) (or (or _let_28 _let_23) _let_73)) (or (or _let_26 _let_47) _let_82)) (or (or _let_37 (< _let_0 (/ _let_14 1))) _let_89)) (or (or _let_77 _let_49) _let_59)) (or (or _let_17 _let_76) P0)) (or (or _let_47 _let_72) _let_32)) (or (or _let_33 _let_57) _let_22)) (or (or P9 _let_19) _let_15)) (or (or _let_74 _let_60) (not P1))) (or (or _let_34 _let_36) _let_60)) (or (or _let_34 _let_42) (not P1))) (or (or _let_77 _let_87) _let_37)) (or (or _let_53 _let_89) (not P6))) (or (or _let_21 _let_15) _let_16)) (or (or _let_40 _let_36) _let_74)) (or (or _let_60 _let_40) P2)) (or (or (not P9) _let_31) (not _let_30))) (or (or _let_75 _let_85) P4)) (or (or _let_12 _let_89) _let_29)) (or (or _let_78 P1) _let_41)) (or (or _let_74 (not P3)) _let_25)) (or (or _let_19 _let_55) P3)) (or (or _let_88 _let_32) _let_86)) (or (or _let_18 _let_75) _let_80)) (or (or _let_33 _let_52) _let_22))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) ))
