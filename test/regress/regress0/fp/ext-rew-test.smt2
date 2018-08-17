; COMMAND-LINE: --ext-rew-prep
; EXPECT: unsat
(set-info :smt-lib-version 2.6)
(set-logic QF_FP)
(set-info :category "crafted")
(set-info :source |Alberto Griggio <griggio@fbk.eu>. These benchmarks were used for the evaluation in the following paper: L. Haller, A. Griggio, M. Brain, D. Kroening: Deciding floating-point logic with systematic abstraction. FMCAD 2012. Real-numbered literals have been automatically translated by MathSAT|)
(set-info :status unknown)
;; MathSAT API call trace
;; generated on 05/20/15 17:24:51

(declare-fun b13 () (_ FloatingPoint 11 53))
(declare-fun b25 () (_ FloatingPoint 11 53))
(declare-fun b22 () (_ FloatingPoint 11 53))
(declare-fun b82 () (_ FloatingPoint 11 53))
(declare-fun b14 () (_ FloatingPoint 11 53))
(declare-fun b34 () (_ FloatingPoint 11 53))
(declare-fun b43 () (_ FloatingPoint 11 53))
(declare-fun b85 () (_ FloatingPoint 11 53))
(declare-fun b16 () (_ FloatingPoint 11 53))
(declare-fun b131 () (_ FloatingPoint 11 53))
(declare-fun b126 () (_ FloatingPoint 11 53))
(define-fun _t_3 () RoundingMode RNE)
(define-fun _t_9 () (_ FloatingPoint 11 53) b131)
(define-fun _t_10 () (_ FloatingPoint 11 53) b43)
(define-fun _t_11 () Bool (= _t_9 _t_10))
(define-fun _t_12 () Bool (not _t_11))
(define-fun _t_13 () (_ FloatingPoint 11 53) b126)
(define-fun _t_14 () (_ FloatingPoint 11 53) b34)
(define-fun _t_15 () Bool (= _t_13 _t_14))
(define-fun _t_16 () Bool (not _t_15))
(define-fun _t_17 () (_ FloatingPoint 11 53) b13)
(define-fun _t_18 () (_ FloatingPoint 11 53) b25)
(define-fun _t_19 () (_ FloatingPoint 11 53) (fp.mul _t_3 _t_17 _t_18))
(define-fun _t_20 () (_ FloatingPoint 11 53) b16)
(define-fun _t_21 () (_ FloatingPoint 11 53) b22)
(define-fun _t_22 () (_ FloatingPoint 11 53) (fp.add _t_3 _t_20 _t_21))
(define-fun _t_23 () Bool (fp.lt _t_22 _t_19))
(define-fun _t_24 () Bool (not _t_23))
(define-fun _t_25 () (_ FloatingPoint 11 53) b14)
(define-fun _t_26 () (_ FloatingPoint 11 53) (fp.neg _t_25))
(define-fun _t_27 () (_ FloatingPoint 11 53) (fp.add _t_3 _t_17 _t_26))
(define-fun _t_28 () Bool (fp.lt _t_27 _t_21))
(define-fun _t_29 () Bool (not _t_28))
(define-fun _t_30 () (_ FloatingPoint 11 53) (fp.mul _t_3 _t_17 _t_25))
(define-fun _t_31 () Bool (fp.lt _t_30 _t_20))
(define-fun _t_32 () Bool (not _t_31))
(define-fun _t_33 () (_ FloatingPoint 11 53) (fp.add _t_3 _t_17 _t_25))
(define-fun _t_34 () Bool (fp.lt _t_33 _t_20))
(define-fun _t_35 () Bool (not _t_34))
(define-fun _t_36 () Bool (= _t_13 _t_20))
(define-fun _t_37 () Bool (and _t_35 _t_36))
(define-fun _t_38 () Bool (= _t_9 _t_21))
(define-fun _t_39 () Bool (and _t_37 _t_38))
(define-fun _t_40 () (_ FloatingPoint 11 53) b85)
(define-fun _t_41 () Bool (fp.leq _t_40 _t_25))
(define-fun _t_42 () Bool (and _t_39 _t_41))
(define-fun _t_43 () (_ FloatingPoint 11 53) b82)
(define-fun _t_44 () Bool (fp.leq _t_25 _t_43))
(define-fun _t_45 () Bool (and _t_42 _t_44))
(define-fun _t_46 () Bool (fp.leq _t_40 _t_17))
(define-fun _t_47 () Bool (and _t_45 _t_46))
(define-fun _t_48 () Bool (fp.leq _t_17 _t_43))
(define-fun _t_49 () Bool (and _t_47 _t_48))
(define-fun _t_50 () Bool (fp.leq _t_40 _t_20))
(define-fun _t_51 () Bool (and _t_49 _t_50))
(define-fun _t_52 () Bool (fp.leq _t_20 _t_43))
(define-fun _t_53 () Bool (and _t_51 _t_52))
(define-fun _t_54 () Bool (fp.leq _t_40 _t_21))
(define-fun _t_55 () Bool (and _t_53 _t_54))
(define-fun _t_56 () Bool (fp.leq _t_21 _t_43))
(define-fun _t_57 () Bool (and _t_55 _t_56))
(define-fun _t_58 () (_ FloatingPoint 11 53) (fp.add _t_3 _t_9 _t_13))
(define-fun _t_59 () Bool (fp.lt _t_58 _t_19))
(define-fun _t_60 () Bool (and _t_57 _t_59))
(define-fun _t_61 () Bool (fp.lt _t_27 _t_9))
(define-fun _t_62 () Bool (and _t_60 _t_61))
(define-fun _t_63 () Bool (fp.lt _t_33 _t_13))
(define-fun _t_64 () Bool (and _t_62 _t_63))
(define-fun _t_65 () Bool (and _t_32 _t_64))
(define-fun _t_66 () Bool (and _t_29 _t_65))
(define-fun _t_67 () Bool (and _t_24 _t_66))
(define-fun _t_68 () Bool (and _t_16 _t_67))
(define-fun _t_69 () Bool (and _t_12 _t_68))
(assert _t_69)
(check-sat)
(exit)
