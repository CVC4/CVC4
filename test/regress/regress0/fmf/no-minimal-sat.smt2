; COMMAND-LINE: --finite-model-find --uf-ss=no-minimal
; EXPECT: sat
(set-logic UF)
(set-info :status sat)
; generated by Nunchaku
(declare-sort i_ 0)
(declare-fun i2_ () i_)
(declare-fun i1_ () i_)
(declare-fun i3_ () i_)
(assert (and (not (= i2_ i3_)) (not (= i1_ i2_)) (not (= i1_ i3_))))
(declare-fun i4_ () i_)
(declare-fun i5_ () i_)
(declare-fun i6_ () i_)
(assert (distinct i4_ i5_ i6_))
(declare-fun P (i_) Bool)
(declare-fun f (i_) i_)
(assert (forall ((x i_)) (P (f x))))
(check-sat)

