; COMMAND-LINE: --bitblast=lazy
; COMMAND-LINE: --bitblast=eager --no-check-models
; EXPECT: sat
; Automatically generated by SBV. Do not edit.
(set-logic QF_UFBV)
(set-info :status sat)
; --- uninterpreted sorts ---
; --- literal constants ---
(define-fun s_2 () Bool false)
(define-fun s_1 () Bool true)
(define-fun s77 () (_ BitVec 8) #x00)
(define-fun s78 () (_ BitVec 8) #x04)
(define-fun s81 () (_ BitVec 8) #x01)
(define-fun s83 () (_ BitVec 8) #xff)
; --- skolem constants ---
(declare-fun s0 () (_ BitVec 8))
(declare-fun s1 () (_ BitVec 8))
(declare-fun s2 () (_ BitVec 8))
(declare-fun s3 () Bool)
(declare-fun s4 () (_ BitVec 8))
(declare-fun s5 () Bool)
(declare-fun s6 () (_ BitVec 8))
(declare-fun s7 () Bool)
(declare-fun s8 () (_ BitVec 8))
(declare-fun s9 () Bool)
(declare-fun s10 () (_ BitVec 8))
(declare-fun s11 () Bool)
(declare-fun s12 () (_ BitVec 8))
(declare-fun s13 () Bool)
(declare-fun s14 () (_ BitVec 8))
(declare-fun s15 () Bool)
(declare-fun s16 () (_ BitVec 8))
(declare-fun s17 () Bool)
(declare-fun s18 () (_ BitVec 8))
(declare-fun s19 () Bool)
(declare-fun s20 () (_ BitVec 8))
(declare-fun s21 () Bool)
(declare-fun s22 () (_ BitVec 8))
(declare-fun s23 () Bool)
(declare-fun s24 () (_ BitVec 8))
(declare-fun s25 () Bool)
(declare-fun s26 () (_ BitVec 8))
(declare-fun s27 () Bool)
(declare-fun s28 () (_ BitVec 8))
(declare-fun s29 () Bool)
(declare-fun s30 () (_ BitVec 8))
(declare-fun s31 () Bool)
(declare-fun s32 () (_ BitVec 8))
(declare-fun s33 () Bool)
(declare-fun s34 () (_ BitVec 8))
(declare-fun s35 () Bool)
(declare-fun s36 () (_ BitVec 8))
(declare-fun s37 () Bool)
(declare-fun s38 () (_ BitVec 8))
(declare-fun s39 () Bool)
(declare-fun s40 () (_ BitVec 8))
(declare-fun s41 () Bool)
(declare-fun s42 () (_ BitVec 8))
; --- constant tables ---
; --- skolemized tables ---
(declare-fun table0 ((_ BitVec 8)) (_ BitVec 8))
(declare-fun table1 ((_ BitVec 8)) Bool)
(declare-fun table2 ((_ BitVec 8)) (_ BitVec 8))
(declare-fun table3 ((_ BitVec 8)) Bool)
; --- arrays ---
; --- uninterpreted constants ---
; --- user given axioms ---
; --- formula ---
(assert ; no quantifiers
   (let ((s43 (and s3 s5)))
   (let ((s44 (or s3 s5)))
   (let ((s45 (not s44)))
   (let ((s46 (= (bvcomp s4 s6) #b1)))
   (let ((s47 (and s45 s46)))
   (let ((s48 (or s43 s47)))
   (let ((s49 (and s11 s13)))
   (let ((s50 (or s11 s13)))
   (let ((s51 (not s50)))
   (let ((s52 (= (bvcomp s12 s14) #b1)))
   (let ((s53 (and s51 s52)))
   (let ((s54 (or s49 s53)))
   (let ((s55 (and s19 s21)))
   (let ((s56 (or s19 s21)))
   (let ((s57 (not s56)))
   (let ((s58 (= (bvcomp s20 s22) #b1)))
   (let ((s59 (and s57 s58)))
   (let ((s60 (or s55 s59)))
   (let ((s61 (and s27 s29)))
   (let ((s62 (or s27 s29)))
   (let ((s63 (not s62)))
   (let ((s64 (= (bvcomp s28 s30) #b1)))
   (let ((s65 (and s63 s64)))
   (let ((s66 (or s61 s65)))
   (let ((s67 (and s35 s37)))
   (let ((s68 (or s35 s37)))
   (let ((s69 (not s68)))
   (let ((s70 (= (bvcomp s36 s38) #b1)))
   (let ((s71 (and s69 s70)))
   (let ((s72 (or s67 s71)))
   (let ((s73 (and s66 s72)))
   (let ((s74 (and s60 s73)))
   (let ((s75 (and s54 s74)))
   (let ((s76 (and s48 s75)))
   (let ((s79 (bvurem s0 s78)))
   (let ((s80 (not (= (bvcomp s77 s79) #b1))))
   (let ((s82 (ite s80 s81 s77)))
   (let ((s84 (= (bvcomp s82 s83) #b1)))
   (let ((s85 (bvadd s78 s79)))
   (let ((s86 (ite s84 s85 s79)))
   (let ((s87 (ite (bvule #x04 s86) s77 (table0 s86))))
   (let ((s88 (= (bvcomp s8 s87) #b1)))
   (let ((s89 (ite (bvule #x04 s86) s_2 (table1 s86))))
   (let ((s90 (= s7 s89)))
   (let ((s91 (ite (bvule #x04 s86) s77 (table2 s86))))
   (let ((s92 (= (bvcomp s10 s91) #b1)))
   (let ((s93 (ite (bvule #x04 s86) s_2 (table3 s86))))
   (let ((s94 (= s9 s93)))
   (let ((s95 (and s92 s94)))
   (let ((s96 (and s90 s95)))
   (let ((s97 (and s88 s96)))
   (let ((s98 (and s7 s9)))
   (let ((s99 (or s7 s9)))
   (let ((s100 (not s99)))
   (let ((s101 (= (bvcomp s8 s10) #b1)))
   (let ((s102 (and s100 s101)))
   (let ((s103 (or s98 s102)))
   (let ((s104 (and s15 s17)))
   (let ((s105 (or s15 s17)))
   (let ((s106 (not s105)))
   (let ((s107 (= (bvcomp s16 s18) #b1)))
   (let ((s108 (and s106 s107)))
   (let ((s109 (or s104 s108)))
   (let ((s110 (and s23 s25)))
   (let ((s111 (or s23 s25)))
   (let ((s112 (not s111)))
   (let ((s113 (= (bvcomp s24 s26) #b1)))
   (let ((s114 (and s112 s113)))
   (let ((s115 (or s110 s114)))
   (let ((s116 (and s31 s33)))
   (let ((s117 (or s31 s33)))
   (let ((s118 (not s117)))
   (let ((s119 (= (bvcomp s32 s34) #b1)))
   (let ((s120 (and s118 s119)))
   (let ((s121 (or s116 s120)))
   (let ((s122 (and s39 s41)))
   (let ((s123 (or s39 s41)))
   (let ((s124 (not s123)))
   (let ((s125 (= (bvcomp s40 s42) #b1)))
   (let ((s126 (and s124 s125)))
   (let ((s127 (or s122 s126)))
   (let ((s128 (and s121 s127)))
   (let ((s129 (and s115 s128)))
   (let ((s130 (and s109 s129)))
   (let ((s131 (and s103 s130)))
   (let ((s132 (not s131)))
   (let ((s133 (and s97 s132)))
   (let ((s134 (and s76 s133)))
   (and (= (table0 #x00) s12)
        (= (table0 #x01) s20)
        (= (table0 #x02) s28)
        (= (table0 #x03) s36)
        (= (table1 #x00) s11)
        (= (table1 #x01) s19)
        (= (table1 #x02) s27)
        (= (table1 #x03) s35)
        (= (table2 #x00) s14)
        (= (table2 #x01) s22)
        (= (table2 #x02) s30)
        (= (table2 #x03) s38)
        (= (table3 #x00) s13)
        (= (table3 #x01) s21)
        (= (table3 #x02) s29)
        (= (table3 #x03) s37)
        s134))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))
(check-sat)
