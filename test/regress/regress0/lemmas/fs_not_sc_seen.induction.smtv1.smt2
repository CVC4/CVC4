(set-option :incremental false)
(meta-info :source "The Formal Verification of a Reintegration Protocol. Author: Lee Pike. Website: http://www.cs.indiana.edu/~lepike/pub_pages/emsoft.html.

This benchmark was automatically translated into SMT-LIB format from
CVC format using CVC Lite")
(meta-info :status unsat)
(meta-info :category "industrial")
(meta-info :difficulty "0")
(set-logic QF_LRA)
(declare-fun x_0 () Real)
(declare-fun x_1 () Bool)
(declare-fun x_2 () Bool)
(declare-fun x_3 () Bool)
(declare-fun x_4 () Real)
(declare-fun x_5 () Bool)
(declare-fun x_6 () Bool)
(declare-fun x_7 () Bool)
(declare-fun x_8 () Real)
(declare-fun x_9 () Real)
(declare-fun x_10 () Real)
(declare-fun x_11 () Real)
(declare-fun x_12 () Bool)
(declare-fun x_13 () Bool)
(declare-fun x_14 () Real)
(declare-fun x_15 () Bool)
(declare-fun x_16 () Bool)
(declare-fun x_17 () Bool)
(declare-fun x_18 () Bool)
(declare-fun x_19 () Bool)
(declare-fun x_20 () Bool)
(declare-fun x_21 () Real)
(declare-fun x_22 () Real)
(declare-fun x_23 () Real)
(declare-fun x_24 () Real)
(declare-fun x_25 () Real)
(declare-fun x_26 () Real)
(declare-fun x_27 () Real)
(declare-fun x_28 () Bool)
(declare-fun x_29 () Bool)
(declare-fun x_30 () Bool)
(declare-fun x_31 () Bool)
(declare-fun x_32 () Bool)
(declare-fun x_33 () Bool)
(declare-fun x_34 () Bool)
(declare-fun x_35 () Bool)
(declare-fun x_36 () Bool)
(declare-fun x_37 () Bool)
(declare-fun x_38 () Real)
(declare-fun x_39 () Real)
(declare-fun x_40 () Real)
(declare-fun x_41 () Real)
(declare-fun x_42 () Real)
(declare-fun x_43 () Real)
(declare-fun x_44 () Real)
(declare-fun x_45 () Real)
(declare-fun x_46 () Real)
(declare-fun x_47 () Real)
(declare-fun x_48 () Real)
(declare-fun x_49 () Real)
(declare-fun x_50 () Real)
(declare-fun x_51 () Real)
(declare-fun x_52 () Real)
(declare-fun x_53 () Bool)
(declare-fun x_54 () Real)
(declare-fun x_55 () Real)
(declare-fun x_56 () Real)
(check-sat-assuming ( (let ((_let_0 (+ x_8 x_9))) (let ((_let_1 (<= x_10 x_11))) (let ((_let_2 (= x_12 x_13))) (let ((_let_3 (= x_4 0.0))) (let ((_let_4 (< x_10 x_14))) (let ((_let_5 (= x_11 x_10))) (let ((_let_6 (= x_4 2.0))) (let ((_let_7 (= x_15 x_16))) (let ((_let_8 (and (= x_17 x_18) (= x_19 x_20)))) (let ((_let_9 (= x_7 x_3))) (let ((_let_10 (and (= x_5 x_1) (= x_6 x_2)))) (let ((_let_11 (= x_21 x_22))) (let ((_let_12 (and (= x_23 x_24) (= x_25 x_26)))) (let ((_let_13 (= x_27 x_14))) (let ((_let_14 (= x_28 x_29))) (let ((_let_15 (= x_30 x_31))) (let ((_let_16 (and (= x_32 x_33) (= x_34 x_35)))) (let ((_let_17 (= x_36 x_37))) (let ((_let_18 (- x_38 x_8))) (let ((_let_19 (= x_4 1.0))) (let ((_let_20 (+ x_9 x_8))) (let ((_let_21 (<= x_39 x_11))) (let ((_let_22 (= x_15 (or x_16 (and _let_21 x_31))))) (let ((_let_23 (<= x_42 _let_0))) (let ((_let_24 (<= x_43 _let_0))) (let ((_let_25 (<= x_42 x_9))) (let ((_let_26 (<= x_43 x_9))) (let ((_let_27 (< x_42 x_10))) (let ((_let_28 (= x_11 x_42))) (let ((_let_29 (< x_43 x_10))) (let ((_let_30 (= x_11 x_43))) (let ((_let_31 (not _let_1))) (let ((_let_32 (and (= x_17 (or x_18 (and (and (not _let_25) (<= x_42 x_11)) x_33))) (= x_19 (or x_20 (and (and (not _let_26) (<= x_43 x_11)) x_35)))))) (let ((_let_33 (<= x_39 _let_0))) (let ((_let_34 (< x_39 x_10))) (let ((_let_35 (= x_11 x_39))) (let ((_let_36 (<= (ite x_3 (ite x_2 (ite x_1 3 2) x_40) (ite x_2 x_40 (ite x_1 1 0))) (* (* (ite x_16 (ite x_20 (ite x_18 0 1) x_41) (ite x_20 x_41 (ite x_18 2 3))) 1.0) (/ 1 2))))) (let ((_let_37 (not _let_23))) (let ((_let_38 (not _let_24))) (let ((_let_39 (and (not (<= x_39 x_9)) _let_21))) (let ((_let_40 (and (not (<= x_44 x_9)) (<= x_44 x_11)))) (let ((_let_41 (not _let_33))) (let ((_let_42 (= x_21 0.0))) (let ((_let_43 (= x_23 0.0))) (let ((_let_44 (= x_25 0.0))) (let ((_let_45 (= x_0 1.0))) (let ((_let_46 (not _let_45))) (let ((_let_47 (and (and (not x_1) (not x_2)) (not x_3)))) (let ((_let_48 (= x_0 0.0))) (let ((_let_49 (not _let_48))) (let ((_let_50 (and (not (<= x_47 x_9)) (<= x_47 x_11)))) (let ((_let_51 (= x_23 (ite (not x_18) (ite (and (and (not _let_25) (<= x_42 x_11)) (< x_24 3.0)) (+ x_24 1.0) x_24) x_24)))) (let ((_let_52 (= x_25 (ite (not x_20) (ite (and (and (not _let_26) (<= x_43 x_11)) (< x_26 3.0)) (+ x_26 1.0) x_26) x_26)))) (let ((_let_53 (or x_18 (= x_23 3.0)))) (let ((_let_54 (or x_20 (= x_25 3.0)))) (let ((_let_55 (or x_16 (= x_21 3.0)))) (let ((_let_56 (ite _let_39 2 1))) (let ((_let_57 (ite _let_39 3 2))) (let ((_let_58 (ite _let_39 1 0))) (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (and (<= x_4 2.0) (>= x_4 0.0)) (<= x_0 2.0)) (>= x_0 0.0)) (> x_8 0.0)) (>= x_8 0.0)) (>= x_9 0.0)) (>= x_10 0.0)) (>= x_11 0.0)) (>= x_14 0.0)) (or (or (or _let_42 (= x_21 1.0)) (= x_21 2.0)) (= x_21 3.0))) (not (< x_21 0.0))) (<= x_21 3.0)) (or (or (or (= x_22 0.0) (= x_22 1.0)) (= x_22 2.0)) (= x_22 3.0))) (not (< x_22 0.0))) (<= x_22 3.0)) (or (or (or _let_43 (= x_23 1.0)) (= x_23 2.0)) (= x_23 3.0))) (not (< x_23 0.0))) (<= x_23 3.0)) (or (or (or (= x_24 0.0) (= x_24 1.0)) (= x_24 2.0)) (= x_24 3.0))) (not (< x_24 0.0))) (<= x_24 3.0)) (or (or (or _let_44 (= x_25 1.0)) (= x_25 2.0)) (= x_25 3.0))) (not (< x_25 0.0))) (<= x_25 3.0)) (or (or (or (= x_26 0.0) (= x_26 1.0)) (= x_26 2.0)) (= x_26 3.0))) (not (< x_26 0.0))) (<= x_26 3.0)) (>= x_27 0.0)) (>= x_38 0.0)) (>= x_39 0.0)) (>= x_42 0.0)) (>= x_43 0.0)) (>= x_44 0.0)) (>= x_47 0.0)) (>= x_50 0.0)) (>= x_51 0.0)) (not (<= x_52 (* x_8 3.0)))) (>= x_52 0.0)) (>= x_54 0.0)) (>= x_55 0.0)) (>= x_56 0.0)) (or _let_46 _let_47)) (or (not _let_3) (and (and (not x_5) (not x_6)) (not x_7)))) (or _let_49 _let_47)) (= x_40 (/ (ite x_1 2 1) 1))) (= x_41 (/ (ite x_18 1 2) 1))) (= x_45 (/ _let_56 1))) (= x_46 (/ _let_56 1))) (= x_48 (+ (ite _let_50 (ite _let_40 _let_57 x_45) (ite _let_40 x_45 _let_58)) x_22))) (= x_49 (+ (ite _let_50 (ite _let_40 _let_57 x_46) (ite _let_40 x_46 _let_58)) x_22))) (or (or (and (and (and (and (and (and (or (and (and (and (and (and (and (and (and (and (and _let_3 _let_4) _let_5) _let_51) _let_52) (= x_21 (ite (not x_16) (ite (not (< x_48 3.0)) 3 x_48) x_22))) (= x_17 _let_53)) (= x_19 _let_54)) (= x_15 _let_55)) _let_14) _let_13) (and (and (and (and (and (and (and (and (and (and _let_3 (not _let_4)) x_28) (= x_11 x_14)) _let_51) _let_52) (= x_21 (ite (not x_16) (ite (not (< x_49 3.0)) 3 x_49) x_22))) (= x_17 (or _let_53 _let_43))) (= x_19 (or _let_54 _let_44))) (= x_15 (or _let_55 _let_42))) _let_13)) _let_15) _let_16) _let_17) _let_9) _let_10) _let_2) (and (and (and (and (and (and (and (or (and (and (and (and (and (and (and (and (and (and (and _let_19 (or (or (and (and (and (not _let_25) (not x_18)) (not x_33)) _let_23) (and (and (and (not _let_26) (not x_20)) (not x_35)) _let_24)) (and (and (not x_16) (not x_31)) _let_33))) (not x_36)) (or (or (or (or _let_25 _let_37) x_33) x_18) (not (< x_11 x_42)))) (or (or (or (or _let_26 _let_38) x_35) x_20) (not (< x_11 x_43)))) (or (or (or _let_41 x_31) x_16) (not (< x_11 x_39)))) (or (or (or (and (and (and (and (not x_33) (not x_18)) _let_23) _let_27) _let_28) (and (and (and (and (not x_35) (not x_20)) _let_24) _let_29) _let_30)) (and (and (and (and (not x_31) (not x_16)) _let_33) _let_34) _let_35)) (and (< x_10 _let_20) _let_5))) (= x_32 (or x_33 (and (not _let_25) (<= x_42 x_11))))) (= x_34 (or x_35 (and (not _let_26) (<= x_43 x_11))))) (= x_30 (or x_31 _let_21))) _let_32) _let_22) (and (and (and (and (and (and (and (and (and _let_19 (or (or (or _let_25 x_33) x_18) _let_37)) (or (or (or _let_26 x_35) x_20) _let_38)) (or (or x_31 x_16) _let_41)) x_36) (= x_11 _let_20)) _let_32) _let_22) _let_15) _let_16)) _let_11) _let_12) _let_13) _let_14) _let_9) _let_10) _let_2)) (and (and (and (and (and (and (and (or (and (and (and (and (and (and (and (and (and (and (and _let_6 _let_36) (not x_12)) (or (or (or _let_25 x_1) x_18) (<= x_11 x_42))) (or (or (or _let_26 x_2) x_20) (<= x_11 x_43))) (or (or x_3 x_16) (<= x_11 x_39))) (or (or (or (and (and (and (and (not x_1) (not x_18)) (< x_9 x_42)) _let_27) _let_28) (and (and (and (and (not x_2) (not x_20)) (< x_9 x_43)) _let_29) _let_30)) (and (and (and (not x_3) (not x_16)) _let_34) _let_35)) _let_5)) (= x_5 (or x_1 (= x_42 x_11)))) (= x_6 (or x_2 (= x_43 x_11)))) (= x_7 (or x_3 (= x_39 x_11)))) _let_7) _let_8) (and (and (and (and (and (and (and _let_6 (not _let_36)) x_12) _let_7) _let_8) (= x_11 x_9)) _let_9) _let_10)) _let_11) _let_12) _let_13) _let_14) _let_15) _let_16) _let_17))) (or (or (and _let_48 (= x_4 (ite (not x_29) x_0 1))) (and _let_45 (= x_4 (ite (not x_37) x_0 2)))) (and (and _let_49 _let_46) (= x_4 x_0)))) (or (and (and _let_1 (not (<= x_38 x_50))) (not (<= x_50 _let_18))) (and _let_31 (= x_50 x_42)))) (or (and (and _let_1 (not (<= x_38 x_51))) (not (<= x_51 _let_18))) (and _let_31 (= x_51 x_43)))) (or (and (and _let_1 (= x_38 (+ x_10 x_52))) x_53) (and (and _let_31 (not x_53)) (= x_38 x_10)))) (or (and (and (and (and _let_21 (not (<= x_54 x_11))) (not (<= x_55 x_11))) (< x_54 x_55)) (< x_55 x_56)) (and (and (and (not _let_21) (= x_54 x_39)) (= x_55 x_44)) (= x_56 x_47)))) _let_19) (or (or x_5 x_6) x_7))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) ))
