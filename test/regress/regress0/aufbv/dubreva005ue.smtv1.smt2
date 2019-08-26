(set-option :incremental false)
(meta-info :source "We reverse an array of length 5 twice in memory at 5 positions.
We show via extensionality that memory has to be equal.

In one case swapping elements is done via XOR in the following way:
x ^= y;
y ^= x;
x ^= y;
In the other case the elements are just swapped.

Contributed by Robert Brummayer (robert.brummayer@gmail.com).")
(meta-info :status unsat)
(meta-info :category "crafted")
(meta-info :difficulty "2")
(set-logic QF_AUFBV)
(declare-fun a1 () (Array (_ BitVec 32) (_ BitVec 8)))
(declare-fun start1 () (_ BitVec 32))
(declare-fun start2 () (_ BitVec 32))
(declare-fun start3 () (_ BitVec 32))
(declare-fun start4 () (_ BitVec 32))
(declare-fun start5 () (_ BitVec 32))
(check-sat-assuming ( (let ((_let_0 (bvadd start1 (_ bv4 32)))) (let ((_let_1 (select a1 start1))) (let ((_let_2 (select a1 _let_0))) (let ((_let_3 (store (store a1 _let_0 _let_1) start1 _let_2))) (let ((_let_4 (bvadd (bvnot (_ bv1 32)) (_ bv1 32)))) (let ((_let_5 (store (store _let_3 (bvadd _let_0 _let_4) (select _let_3 (bvadd start1 (_ bv1 32)))) (bvadd start1 (_ bv1 32)) (select _let_3 (bvadd _let_0 _let_4))))) (let ((_let_6 (bvand (bvnot (bvand (bvnot _let_1) (bvnot _let_2))) (bvnot (bvand _let_1 _let_2))))) (let ((_let_7 (bvand (bvnot (bvand (bvnot _let_2) (bvnot _let_6))) (bvnot (bvand _let_2 _let_6))))) (let ((_let_8 (store (store (store a1 start1 _let_6) _let_0 _let_7) start1 (bvand (bvnot (bvand (bvnot _let_6) (bvnot _let_7))) (bvnot (bvand _let_6 _let_7)))))) (let ((_let_9 (select _let_8 (bvadd start1 (_ bv1 32))))) (let ((_let_10 (select _let_8 (bvadd _let_0 _let_4)))) (let ((_let_11 (bvand (bvnot (bvand (bvnot _let_9) (bvnot _let_10))) (bvnot (bvand _let_9 _let_10))))) (let ((_let_12 (bvand (bvnot (bvand (bvnot _let_10) (bvnot _let_11))) (bvnot (bvand _let_10 _let_11))))) (let ((_let_13 (store (store (store _let_8 (bvadd start1 (_ bv1 32)) _let_11) (bvadd _let_0 _let_4) _let_12) (bvadd start1 (_ bv1 32)) (bvand (bvnot (bvand (bvnot _let_11) (bvnot _let_12))) (bvnot (bvand _let_11 _let_12)))))) (let ((_let_14 (bvadd (_ bv4 32) start2))) (let ((_let_15 (store (store _let_5 _let_14 (select _let_5 start2)) start2 (select _let_5 _let_14)))) (let ((_let_16 (store (store _let_15 (bvadd _let_4 _let_14) (select _let_15 (bvadd (_ bv1 32) start2))) (bvadd (_ bv1 32) start2) (select _let_15 (bvadd _let_4 _let_14))))) (let ((_let_17 (select _let_13 start2))) (let ((_let_18 (select _let_13 _let_14))) (let ((_let_19 (bvand (bvnot (bvand (bvnot _let_17) (bvnot _let_18))) (bvnot (bvand _let_17 _let_18))))) (let ((_let_20 (bvand (bvnot (bvand (bvnot _let_18) (bvnot _let_19))) (bvnot (bvand _let_18 _let_19))))) (let ((_let_21 (store (store (store _let_13 start2 _let_19) _let_14 _let_20) start2 (bvand (bvnot (bvand (bvnot _let_19) (bvnot _let_20))) (bvnot (bvand _let_19 _let_20)))))) (let ((_let_22 (select _let_21 (bvadd (_ bv1 32) start2)))) (let ((_let_23 (select _let_21 (bvadd _let_4 _let_14)))) (let ((_let_24 (bvand (bvnot (bvand (bvnot _let_22) (bvnot _let_23))) (bvnot (bvand _let_22 _let_23))))) (let ((_let_25 (bvand (bvnot (bvand (bvnot _let_23) (bvnot _let_24))) (bvnot (bvand _let_23 _let_24))))) (let ((_let_26 (store (store (store _let_21 (bvadd (_ bv1 32) start2) _let_24) (bvadd _let_4 _let_14) _let_25) (bvadd (_ bv1 32) start2) (bvand (bvnot (bvand (bvnot _let_24) (bvnot _let_25))) (bvnot (bvand _let_24 _let_25)))))) (let ((_let_27 (bvadd (_ bv4 32) start3))) (let ((_let_28 (store (store _let_16 _let_27 (select _let_16 start3)) start3 (select _let_16 _let_27)))) (let ((_let_29 (store (store _let_28 (bvadd _let_4 _let_27) (select _let_28 (bvadd (_ bv1 32) start3))) (bvadd (_ bv1 32) start3) (select _let_28 (bvadd _let_4 _let_27))))) (let ((_let_30 (select _let_26 start3))) (let ((_let_31 (select _let_26 _let_27))) (let ((_let_32 (bvand (bvnot (bvand (bvnot _let_30) (bvnot _let_31))) (bvnot (bvand _let_30 _let_31))))) (let ((_let_33 (bvand (bvnot (bvand (bvnot _let_31) (bvnot _let_32))) (bvnot (bvand _let_31 _let_32))))) (let ((_let_34 (store (store (store _let_26 start3 _let_32) _let_27 _let_33) start3 (bvand (bvnot (bvand (bvnot _let_32) (bvnot _let_33))) (bvnot (bvand _let_32 _let_33)))))) (let ((_let_35 (select _let_34 (bvadd (_ bv1 32) start3)))) (let ((_let_36 (select _let_34 (bvadd _let_4 _let_27)))) (let ((_let_37 (bvand (bvnot (bvand (bvnot _let_35) (bvnot _let_36))) (bvnot (bvand _let_35 _let_36))))) (let ((_let_38 (bvand (bvnot (bvand (bvnot _let_36) (bvnot _let_37))) (bvnot (bvand _let_36 _let_37))))) (let ((_let_39 (store (store (store _let_34 (bvadd (_ bv1 32) start3) _let_37) (bvadd _let_4 _let_27) _let_38) (bvadd (_ bv1 32) start3) (bvand (bvnot (bvand (bvnot _let_37) (bvnot _let_38))) (bvnot (bvand _let_37 _let_38)))))) (let ((_let_40 (bvadd (_ bv4 32) start4))) (let ((_let_41 (store (store _let_29 _let_40 (select _let_29 start4)) start4 (select _let_29 _let_40)))) (let ((_let_42 (store (store _let_41 (bvadd _let_4 _let_40) (select _let_41 (bvadd (_ bv1 32) start4))) (bvadd (_ bv1 32) start4) (select _let_41 (bvadd _let_4 _let_40))))) (let ((_let_43 (select _let_39 start4))) (let ((_let_44 (select _let_39 _let_40))) (let ((_let_45 (bvand (bvnot (bvand (bvnot _let_43) (bvnot _let_44))) (bvnot (bvand _let_43 _let_44))))) (let ((_let_46 (bvand (bvnot (bvand (bvnot _let_44) (bvnot _let_45))) (bvnot (bvand _let_44 _let_45))))) (let ((_let_47 (store (store (store _let_39 start4 _let_45) _let_40 _let_46) start4 (bvand (bvnot (bvand (bvnot _let_45) (bvnot _let_46))) (bvnot (bvand _let_45 _let_46)))))) (let ((_let_48 (select _let_47 (bvadd (_ bv1 32) start4)))) (let ((_let_49 (select _let_47 (bvadd _let_4 _let_40)))) (let ((_let_50 (bvand (bvnot (bvand (bvnot _let_48) (bvnot _let_49))) (bvnot (bvand _let_48 _let_49))))) (let ((_let_51 (bvand (bvnot (bvand (bvnot _let_49) (bvnot _let_50))) (bvnot (bvand _let_49 _let_50))))) (let ((_let_52 (store (store (store _let_47 (bvadd (_ bv1 32) start4) _let_50) (bvadd _let_4 _let_40) _let_51) (bvadd (_ bv1 32) start4) (bvand (bvnot (bvand (bvnot _let_50) (bvnot _let_51))) (bvnot (bvand _let_50 _let_51)))))) (let ((_let_53 (bvadd (_ bv4 32) start5))) (let ((_let_54 (store (store _let_42 _let_53 (select _let_42 start5)) start5 (select _let_42 _let_53)))) (let ((_let_55 (select _let_52 start5))) (let ((_let_56 (select _let_52 _let_53))) (let ((_let_57 (bvand (bvnot (bvand (bvnot _let_55) (bvnot _let_56))) (bvnot (bvand _let_55 _let_56))))) (let ((_let_58 (bvand (bvnot (bvand (bvnot _let_56) (bvnot _let_57))) (bvnot (bvand _let_56 _let_57))))) (let ((_let_59 (store (store (store _let_52 start5 _let_57) _let_53 _let_58) start5 (bvand (bvnot (bvand (bvnot _let_57) (bvnot _let_58))) (bvnot (bvand _let_57 _let_58)))))) (let ((_let_60 (select _let_59 (bvadd (_ bv1 32) start5)))) (let ((_let_61 (select _let_59 (bvadd _let_4 _let_53)))) (let ((_let_62 (bvand (bvnot (bvand (bvnot _let_60) (bvnot _let_61))) (bvnot (bvand _let_60 _let_61))))) (let ((_let_63 (bvand (bvnot (bvand (bvnot _let_61) (bvnot _let_62))) (bvnot (bvand _let_61 _let_62))))) (not (= (bvnot (ite (= (store (store _let_54 (bvadd _let_4 _let_53) (select _let_54 (bvadd (_ bv1 32) start5))) (bvadd (_ bv1 32) start5) (select _let_54 (bvadd _let_4 _let_53))) (store (store (store _let_59 (bvadd (_ bv1 32) start5) _let_62) (bvadd _let_4 _let_53) _let_63) (bvadd (_ bv1 32) start5) (bvand (bvnot (bvand (bvnot _let_62) (bvnot _let_63))) (bvnot (bvand _let_62 _let_63))))) (_ bv1 1) (_ bv0 1))) (_ bv0 1))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) ))
