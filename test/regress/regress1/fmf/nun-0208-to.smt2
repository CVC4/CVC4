; COMMAND-LINE: --finite-model-find
; EXPECT: sat
(set-logic ALL)
(declare-sort b__ 0)
(declare-fun __nun_card_witness_0_ () b__)
(declare-sort a__ 0)
(declare-fun __nun_card_witness_1_ () a__)
(declare-datatypes ((prod__ 0))
   (((Pair__ (_select_Pair___0 a__) (_select_Pair___1 b__)))))
(declare-datatypes ((list2__ 0))
   (((Cons2__ (_select_Cons2___0 prod__) (_select_Cons2___1 list2__))
     (Nil2__ ))))
(declare-datatypes ((list__ 0))
   (((Cons__ (_select_Cons___0 a__) (_select_Cons___1 list__))
     (Nil__ ))))
(declare-datatypes ((list1__ 0))
   (((Cons1__ (_select_Cons1___0 b__) (_select_Cons1___1 list1__))
     (Nil1__ ))))
(declare-sort G_zip__ 0)
(declare-fun __nun_card_witness_2_ () G_zip__)
(declare-fun zip__ (list__ list1__) list2__)
(declare-fun proj_G_zip__0_ (G_zip__) list__)
(declare-fun proj_G_zip__1_ (G_zip__) list1__)
(assert
 (forall ((a/166 G_zip__))
    (and
     (= (zip__ (proj_G_zip__0_ a/166) (proj_G_zip__1_ a/166))
      (ite ((_ is Cons1__) (proj_G_zip__1_ a/166))
        (ite ((_ is Cons__) (proj_G_zip__0_ a/166))
          (Cons2__
           (Pair__ (_select_Cons___0 (proj_G_zip__0_ a/166))
            (_select_Cons1___0 (proj_G_zip__1_ a/166)))
           (zip__ (_select_Cons___1 (proj_G_zip__0_ a/166))
            (_select_Cons1___1 (proj_G_zip__1_ a/166))))
          Nil2__)
        Nil2__))
     (=> ((_ is Cons1__) (proj_G_zip__1_ a/166))
      (=> ((_ is Cons__) (proj_G_zip__0_ a/166))
       (exists ((a/168 G_zip__))
          (and
           (= (_select_Cons1___1 (proj_G_zip__1_ a/166))
            (proj_G_zip__1_ a/168))
           (= (_select_Cons___1 (proj_G_zip__0_ a/166))
            (proj_G_zip__0_ a/168)))))))))
(declare-datatypes ((nat__ 0)) (((Suc__ (_select_Suc___0 nat__)) (zero__))))
(declare-sort G_replicate__ 0)
(declare-fun __nun_card_witness_3_ () G_replicate__)
(declare-fun replicate__ (nat__ a__) list__)
(declare-fun proj_G_replicate__0_ (G_replicate__) nat__)
(declare-fun proj_G_replicate__1_ (G_replicate__) a__)
(assert
 (forall ((a/169 G_replicate__))
    (and
     (=
      (replicate__ (proj_G_replicate__0_ a/169)
       (proj_G_replicate__1_ a/169))
      (ite ((_ is Suc__) (proj_G_replicate__0_ a/169))
        (Cons__ (proj_G_replicate__1_ a/169)
         (replicate__ (_select_Suc___0 (proj_G_replicate__0_ a/169))
          (proj_G_replicate__1_ a/169)))
        Nil__))
     (=> ((_ is Suc__) (proj_G_replicate__0_ a/169))
      (exists ((a/171 G_replicate__))
         (and
          (= (proj_G_replicate__1_ a/169) (proj_G_replicate__1_ a/171))
          (= (_select_Suc___0 (proj_G_replicate__0_ a/169))
           (proj_G_replicate__0_ a/171))))))))
(declare-fun j__ () nat__)
(declare-fun x__ () a__)
(declare-sort G_replicate1__ 0)
(declare-fun __nun_card_witness_4_ () G_replicate1__)
(declare-fun replicate1__ (nat__ b__) list1__)
(declare-fun proj_G_replicate1__0_ (G_replicate1__) nat__)
(declare-fun proj_G_replicate1__1_ (G_replicate1__) b__)
(assert
 (forall ((a/172 G_replicate1__))
    (and
     (=
      (replicate1__ (proj_G_replicate1__0_ a/172)
       (proj_G_replicate1__1_ a/172))
      (ite ((_ is Suc__) (proj_G_replicate1__0_ a/172))
        (Cons1__ (proj_G_replicate1__1_ a/172)
         (replicate1__ (_select_Suc___0 (proj_G_replicate1__0_ a/172))
          (proj_G_replicate1__1_ a/172)))
        Nil1__))
     (=> ((_ is Suc__) (proj_G_replicate1__0_ a/172))
      (exists ((a/174 G_replicate1__))
         (and
          (= (proj_G_replicate1__1_ a/172) (proj_G_replicate1__1_ a/174))
          (= (_select_Suc___0 (proj_G_replicate1__0_ a/172))
           (proj_G_replicate1__0_ a/174))))))))
(declare-fun y__ () b__)
(declare-sort G_replicate2__ 0)
(declare-fun __nun_card_witness_5_ () G_replicate2__)
(declare-fun replicate2__ (nat__ prod__) list2__)
(declare-fun proj_G_replicate2__0_ (G_replicate2__) nat__)
(declare-fun proj_G_replicate2__1_ (G_replicate2__) prod__)
(assert
 (forall ((a/175 G_replicate2__))
    (and
     (=
      (replicate2__ (proj_G_replicate2__0_ a/175)
       (proj_G_replicate2__1_ a/175))
      (ite ((_ is Suc__) (proj_G_replicate2__0_ a/175))
        (Cons2__ (proj_G_replicate2__1_ a/175)
         (replicate2__ (_select_Suc___0 (proj_G_replicate2__0_ a/175))
          (proj_G_replicate2__1_ a/175)))
        Nil2__))
     (=> ((_ is Suc__) (proj_G_replicate2__0_ a/175))
      (exists ((a/177 G_replicate2__))
         (and
          (= (proj_G_replicate2__1_ a/175) (proj_G_replicate2__1_ a/177))
          (= (_select_Suc___0 (proj_G_replicate2__0_ a/175))
           (proj_G_replicate2__0_ a/177))))))))
(declare-sort G_less__eq__ 0)
(declare-fun __nun_card_witness_6_ () G_less__eq__)
(declare-fun less__eq__ (nat__ nat__) Bool)
(declare-fun proj_G_less__eq__0_ (G_less__eq__) nat__)
(declare-fun proj_G_less__eq__1_ (G_less__eq__) nat__)
(assert
 (forall ((a/178 G_less__eq__))
    (and
     (=
      (less__eq__ (proj_G_less__eq__0_ a/178) (proj_G_less__eq__1_ a/178))
      (=> ((_ is Suc__) (proj_G_less__eq__0_ a/178))
       (and ((_ is Suc__) (proj_G_less__eq__1_ a/178))
        (less__eq__ (_select_Suc___0 (proj_G_less__eq__0_ a/178))
         (_select_Suc___0 (proj_G_less__eq__1_ a/178))))))
     (exists ((a/182 G_less__eq__))
        (and
         (= (_select_Suc___0 (proj_G_less__eq__1_ a/178))
          (proj_G_less__eq__1_ a/182))
         (= (_select_Suc___0 (proj_G_less__eq__0_ a/178))
          (proj_G_less__eq__0_ a/182)))))))
(declare-sort G_min__ 0)
(declare-fun __nun_card_witness_7_ () G_min__)
(declare-fun min__ (nat__ nat__) nat__)
(declare-fun proj_G_min__0_ (G_min__) nat__)
(declare-fun proj_G_min__1_ (G_min__) nat__)
(assert
 (forall ((a/183 G_min__))
    (and
     (= (min__ (proj_G_min__0_ a/183) (proj_G_min__1_ a/183))
      (ite (less__eq__ (proj_G_min__0_ a/183) (proj_G_min__1_ a/183))
        (proj_G_min__0_ a/183) (proj_G_min__1_ a/183)))
     (exists ((a/184 G_less__eq__))
        (and (= (proj_G_min__1_ a/183) (proj_G_less__eq__1_ a/184))
         (= (proj_G_min__0_ a/183) (proj_G_less__eq__0_ a/184)))))))
(declare-fun i__ () nat__)
(assert
 (not
  (=>
   (and
    (exists ((a/212 G_min__))
       (and (= i__ (proj_G_min__1_ a/212)) (= i__ (proj_G_min__0_ a/212))))
    (exists ((a/208 G_replicate2__))
       (and (= (Pair__ x__ y__) (proj_G_replicate2__1_ a/208))
        (= (min__ i__ i__) (proj_G_replicate2__0_ a/208))
        (exists ((a/210 G_min__))
           (and (= i__ (proj_G_min__1_ a/210))
            (= i__ (proj_G_min__0_ a/210))))))
    (exists ((a/199 G_zip__))
       (and (= (replicate1__ j__ y__) (proj_G_zip__1_ a/199))
        (exists ((a/202 G_replicate1__))
           (and (= y__ (proj_G_replicate1__1_ a/202))
            (= j__ (proj_G_replicate1__0_ a/202))))
        (= (replicate__ j__ x__) (proj_G_zip__0_ a/199))
        (exists ((a/203 G_replicate__))
           (and (= x__ (proj_G_replicate__1_ a/203))
            (= j__ (proj_G_replicate__0_ a/203))))))
    (exists ((a/207 G_replicate1__))
       (and (= y__ (proj_G_replicate1__1_ a/207))
        (= j__ (proj_G_replicate1__0_ a/207))))
    (exists ((a/206 G_replicate__))
       (and (= x__ (proj_G_replicate__1_ a/206))
        (= j__ (proj_G_replicate__0_ a/206)))))
   (= (replicate2__ (min__ i__ i__) (Pair__ x__ y__))
    (zip__ (replicate__ j__ x__) (replicate1__ j__ y__))))))
(check-sat)
