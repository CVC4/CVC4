(set-logic UFNIA)
(set-info :source |
    VCC and HAVOC benchmarks.  Contributed by Nikolaj Bjorner, Leonardo de Moura,
    Michal Moskal, and Shaz Qadeer.
  |)
(set-info :smt-lib-version 2.6)
(set-info :category "industrial")
(set-info :status unsat)
(declare-fun x (Int Int) Int)
(declare-fun true_1 () Int)
(declare-fun false_1 () Int)
(declare-fun intGreater (Int Int) Int)
(declare-fun intAtLeast (Int Int) Int)
(declare-fun intAtMost (Int Int) Int)
(declare-fun intLess (Int Int) Int)
(declare-fun anyNeq (Int Int) Int)
(declare-fun anyEqual (Int Int) Int)
(declare-fun boolNot (Int) Int)
(declare-fun boolOr (Int Int) Int)
(declare-fun boolAnd (Int Int) Int)
(declare-fun boolImplies (Int Int) Int)
(declare-fun boolIff (Int Int) Int)
(declare-fun select2 (Int Int Int) Int)
(declare-fun store2 (Int Int Int Int) Int)
(declare-fun select1 (Int Int) Int)
(declare-fun store1 (Int Int Int) Int)
(declare-fun NULL_CHECK (Int) Int)
(declare-fun LIFT (Int) Int)
(declare-fun BIT_BAND (Int Int) Int)
(declare-fun POW2 (Int) Int)
(declare-fun choose (Int Int Int) Int)
(declare-fun DIV (Int Int) Int)
(declare-fun MULT (Int Int) Int)
(declare-fun PLUS (Int Int Int) Int)
(declare-fun MINUS_LEFT_PTR (Int Int Int) Int)
(declare-fun MINUS_BOTH_PTR_OR_BOTH_INT (Int Int Int) Int)
(declare-fun n_fooInv (Int) Int)
(declare-fun n_foo (Int) Int)
(declare-fun S_n_fooInv (Int) Int)
(declare-fun S_n_foo (Int) Int)
(declare-fun p_fooInv (Int) Int)
(declare-fun p_foo (Int) Int)
(declare-fun S_p_fooInv (Int) Int)
(declare-fun S_p_foo (Int) Int)
(declare-fun next_fooInv (Int) Int)
(declare-fun next_foo (Int) Int)
(declare-fun S_next_fooInv (Int) Int)
(declare-fun S_next_foo (Int) Int)
(declare-fun Match (Int Int) Int)
(declare-fun foo_name () Int)
(declare-fun n_foo_name () Int)
(declare-fun p_foo_name () Int)
(declare-fun next_foo_name () Int)
(declare-fun HasType (Int Int) Int)
(declare-fun Pfoo_name () Int)
(declare-fun Field (Int) Int)
(declare-fun PPfoo_name () Int)
(declare-fun PINT4_name () Int)
(declare-fun PPINT4_name () Int)
(declare-fun INT4_name () Int)
(declare-fun Values (Int) Int)
(declare-fun Unified (Int) Int)
(declare-fun Disjoint (Int Int) Int)
(declare-fun Subset (Int Int) Int)
(declare-fun Equal (Int Int) Int)
(declare-fun Inverse (Int Int) Int)
(declare-fun Difference (Int Int) Int)
(declare-fun Singleton (Int) Int)
(declare-fun Union (Int Int) Int)
(declare-fun Dereference (Int Int) Int)
(declare-fun Intersection (Int Int) Int)
(declare-fun Empty () Int)
(declare-fun HavocArray (Int Int Int) Int)
(declare-fun AtLeast (Int Int) Int)
(declare-fun Rep (Int Int) Int)
(declare-fun FourBytesToInt (Int Int Int Int) Int)
(declare-fun TwoBytesToInt (Int Int) Int)
(declare-fun OneByteToInt (Int) Int)
(declare-fun BYTE () Int)
(declare-fun FREED () Int)
(declare-fun ALLOCATED () Int)
(declare-fun UNALLOCATED () Int)
(declare-fun Mem_3 () Int)
(declare-fun result_bar_24_4_1__0 () Int)
(declare-fun Mem_0 () Int)
(declare-fun LOOP_12_Mem_0 () Int)
(declare-fun LOOP_12_Mem_1 () Int)
(declare-fun sum_0 () Int)
(declare-fun head_0 () Int)
(declare-fun head_2 () Int)
(declare-fun tempBoogie0_0 () Int)
(declare-fun tempBoogie0_3 () Int)
(declare-fun sum_2 () Int)
(declare-fun i_0 () Int)
(declare-fun i_3 () Int)
(declare-fun LOOP_12_alloc_0 () Int)
(declare-fun LOOP_12_alloc_1 () Int)
(declare-fun Mem () Int)
(declare-fun Base (Int) Int)
(declare-fun alloc () Int)
(declare-fun sum_1 () Int)
(declare-fun i_1 () Int)
(declare-fun head_1 () Int)
(declare-fun Mem_1 () Int)
(declare-fun tempBoogie0_2 () Int)
(declare-fun i_2 () Int)
(declare-fun Mem_2 () Int)
(declare-fun head1 () Int)
(declare-fun sum () Int)
(declare-fun result_bar_24_4_1_ () Int)
(declare-fun i () Int)
(declare-fun head () Int)
(assert (not (or (not (forall ((?A Int) (?i Int) (?v Int)) (= (select1 (store1 ?A ?i ?v) ?i) ?v))) (not (forall ((?A Int) (?i Int) (?j Int) (?v Int)) (=> (not (= ?i ?j)) (= (select1 (store1 ?A ?i ?v) ?j) (select1 ?A ?j))))) (not (forall ((?A Int) (?o Int) (?f Int) (?v Int)) (= (select2 (store2 ?A ?o ?f ?v) ?o ?f) ?v))) (not (forall ((?A Int) (?o Int) (?f Int) (?p Int) (?g Int) (?v Int)) (=> (not (= ?o ?p)) (= (select2 (store2 ?A ?o ?f ?v) ?p ?g) (select2 ?A ?p ?g))))) (not (forall ((?A Int) (?o Int) (?f Int) (?p Int) (?g Int) (?v Int)) (=> (not (= ?f ?g)) (= (select2 (store2 ?A ?o ?f ?v) ?p ?g) (select2 ?A ?p ?g))))) (not (forall ((?x_1 Int) (?y Int)) (= (= (boolIff ?x_1 ?y) true_1) (= (= ?x_1 true_1) (= ?y true_1))))) (not (forall ((?x_1 Int) (?y Int)) (= (= (boolImplies ?x_1 ?y) true_1) (=> (= ?x_1 true_1) (= ?y true_1))))) (not (forall ((?x_1 Int) (?y Int)) (= (= (boolAnd ?x_1 ?y) true_1) (not (or (not (= ?x_1 true_1)) (not (= ?y true_1))))))) (not (forall ((?x_1 Int) (?y Int)) (= (= (boolOr ?x_1 ?y) true_1) (or (= ?x_1 true_1) (= ?y true_1))))) (not (forall ((?x_1 Int)) (! (= (= (boolNot ?x_1) true_1) (not (= ?x_1 true_1))) :pattern ((boolNot ?x_1)) ))) (not (forall ((?x_1 Int) (?y Int)) (= (= (anyEqual ?x_1 ?y) true_1) (= ?x_1 ?y)))) (not (forall ((?x_1 Int) (?y Int)) (! (= (= (anyNeq ?x_1 ?y) true_1) (not (= ?x_1 ?y))) :pattern ((anyNeq ?x_1 ?y)) ))) (not (forall ((?x_1 Int) (?y Int)) (= (= (intLess ?x_1 ?y) true_1) (< ?x_1 ?y)))) (not (forall ((?x_1 Int) (?y Int)) (= (= (intAtMost ?x_1 ?y) true_1) (<= ?x_1 ?y)))) (not (forall ((?x_1 Int) (?y Int)) (= (= (intAtLeast ?x_1 ?y) true_1) (>= ?x_1 ?y)))) (not (forall ((?x_1 Int) (?y Int)) (= (= (intGreater ?x_1 ?y) true_1) (> ?x_1 ?y)))) (not (distinct false_1 true_1)) (not (forall ((?t Int)) (! (= (x ?t ?t) true_1) :pattern ((x ?t ?t)) ))) (not (forall ((?t Int) (?u Int) (?v Int)) (! (=> (not (or (not (= (x ?t ?u) true_1)) (not (= (x ?u ?v) true_1)))) (= (x ?t ?v) true_1)) :pattern ((x ?t ?u) (x ?u ?v)) ))) (not (forall ((?t Int) (?u Int)) (! (=> (not (or (not (= (x ?t ?u) true_1)) (not (= (x ?u ?t) true_1)))) (= ?t ?u)) :pattern ((x ?t ?u) (x ?u ?t)) ))))))
(assert (not (or (not (distinct UNALLOCATED ALLOCATED FREED BYTE next_foo_name p_foo_name n_foo_name INT4_name PINT4_name PPINT4_name PPfoo_name Pfoo_name foo_name)) (not (forall ((?b0 Int) (?c0 Int)) (! (=> (= (OneByteToInt ?b0) (OneByteToInt ?c0)) (= ?b0 ?c0)) :pattern ((OneByteToInt ?b0) (OneByteToInt ?c0)) ))) (not (forall ((?b0 Int) (?b1 Int) (?c0 Int) (?c1 Int)) (! (=> (= (TwoBytesToInt ?b0 ?b1) (TwoBytesToInt ?c0 ?c1)) (not (or (not (= ?b0 ?c0)) (not (= ?b1 ?c1))))) :pattern ((TwoBytesToInt ?b0 ?b1) (TwoBytesToInt ?c0 ?c1)) ))) (not (forall ((?b0 Int) (?b1 Int) (?b2 Int) (?b3 Int) (?c0 Int) (?c1 Int) (?c2 Int) (?c3 Int)) (! (=> (= (FourBytesToInt ?b0 ?b1 ?b2 ?b3) (FourBytesToInt ?c0 ?c1 ?c2 ?c3)) (not (or (not (= ?b0 ?c0)) (not (= ?b1 ?c1)) (not (= ?b2 ?c2)) (not (= ?b3 ?c3))))) :pattern ((FourBytesToInt ?b0 ?b1 ?b2 ?b3) (FourBytesToInt ?c0 ?c1 ?c2 ?c3)) ))) (not (forall ((?n Int) (?x_1 Int) (?y Int)) (! (=> (= (select1 (AtLeast ?n ?x_1) ?y) true_1) (not (or (not (<= ?x_1 ?y)) (not (= (Rep ?n ?x_1) (Rep ?n ?y)))))) :pattern ((select1 (AtLeast ?n ?x_1) ?y)) ))) (not (forall ((?n Int) (?x_1 Int) (?y Int)) (! (=> (not (or (not (<= ?x_1 ?y)) (not (= (Rep ?n ?x_1) (Rep ?n ?y))))) (= (select1 (AtLeast ?n ?x_1) ?y) true_1)) :pattern ((AtLeast ?n ?x_1) (Rep ?n ?x_1) (Rep ?n ?y)) ))) (not (forall ((?n Int) (?x_1 Int)) (! (= (select1 (AtLeast ?n ?x_1) ?x_1) true_1) :pattern ((AtLeast ?n ?x_1)) ))) (not (forall ((?n Int) (?x_1 Int) (?z Int)) (! (= (Rep ?n ?x_1) (Rep ?n (PLUS ?x_1 ?n ?z))) :pattern ((PLUS ?x_1 ?n ?z)) ))) (not (forall ((?n Int) (?x_1 Int)) (! (exists ((?k Int)) (= (- (Rep ?n ?x_1) ?x_1) (* ?n ?k))) :pattern ((Rep ?n ?x_1)) ))) (not (forall ((?x_1 Int) (?n Int) (?z Int)) (! (=> (<= ?z 0) (= (Equal (HavocArray ?x_1 ?n ?z) Empty) true_1)) :pattern ((HavocArray ?x_1 ?n ?z)) ))) (not (forall ((?x_1 Int) (?n Int) (?z Int)) (! (=> (> ?z 0) (= (Equal (HavocArray ?x_1 ?n ?z) (Difference (AtLeast ?n ?x_1) (AtLeast ?n (PLUS ?x_1 ?n ?z)))) true_1)) :pattern ((HavocArray ?x_1 ?n ?z)) ))) (not (forall ((?x_1 Int)) (not (= (select1 Empty ?x_1) true_1)))) (not (forall ((?x_1 Int) (?y Int)) (! (= (= (select1 (Singleton ?y) ?x_1) true_1) (= ?x_1 ?y)) :pattern ((select1 (Singleton ?y) ?x_1)) ))) (not (forall ((?y Int)) (! (= (select1 (Singleton ?y) ?y) true_1) :pattern ((Singleton ?y)) ))) (not (forall ((?x_1 Int) (?S Int) (?T Int)) (! (= (= (select1 (Union ?S ?T) ?x_1) true_1) (or (= (select1 ?S ?x_1) true_1) (= (select1 ?T ?x_1) true_1))) :pattern ((select1 (Union ?S ?T) ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int) (?T Int)) (! (=> (= (select1 ?S ?x_1) true_1) (= (select1 (Union ?S ?T) ?x_1) true_1)) :pattern ((Union ?S ?T) (select1 ?S ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int) (?T Int)) (! (=> (= (select1 ?T ?x_1) true_1) (= (select1 (Union ?S ?T) ?x_1) true_1)) :pattern ((Union ?S ?T) (select1 ?T ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int) (?T Int)) (! (= (= (select1 (Intersection ?S ?T) ?x_1) true_1) (not (or (not (= (select1 ?S ?x_1) true_1)) (not (= (select1 ?T ?x_1) true_1))))) :pattern ((select1 (Intersection ?S ?T) ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int) (?T Int)) (! (=> (not (or (not (= (select1 ?S ?x_1) true_1)) (not (= (select1 ?T ?x_1) true_1)))) (= (select1 (Intersection ?S ?T) ?x_1) true_1)) :pattern ((Intersection ?S ?T) (select1 ?S ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int) (?T Int)) (! (=> (not (or (not (= (select1 ?S ?x_1) true_1)) (not (= (select1 ?T ?x_1) true_1)))) (= (select1 (Intersection ?S ?T) ?x_1) true_1)) :pattern ((Intersection ?S ?T) (select1 ?T ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int) (?T Int)) (! (= (= (select1 (Difference ?S ?T) ?x_1) true_1) (not (or (not (= (select1 ?S ?x_1) true_1)) (not (not (= (select1 ?T ?x_1) true_1)))))) :pattern ((select1 (Difference ?S ?T) ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int) (?T Int)) (! (=> (= (select1 ?S ?x_1) true_1) (or (= (select1 (Difference ?S ?T) ?x_1) true_1) (= (select1 ?T ?x_1) true_1))) :pattern ((Difference ?S ?T) (select1 ?S ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int) (?M Int)) (! (=> (= (select1 (Dereference ?S ?M) ?x_1) true_1) (exists ((?y Int)) (not (or (not (= ?x_1 (select1 ?M ?y))) (not (= (select1 ?S ?y) true_1)))))) :pattern ((select1 (Dereference ?S ?M) ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int) (?M Int)) (! (=> (= (select1 ?S ?x_1) true_1) (= (select1 (Dereference ?S ?M) (select1 ?M ?x_1)) true_1)) :pattern ((select1 ?M ?x_1) (select1 ?S ?x_1) (Dereference ?S ?M)) ))) (not (forall ((?x_1 Int) (?y Int) (?S Int) (?M Int)) (! (=> (not (= (select1 ?S ?x_1) true_1)) (= (Equal (Dereference ?S (store1 ?M ?x_1 ?y)) (Dereference ?S ?M)) true_1)) :pattern ((Dereference ?S (store1 ?M ?x_1 ?y))) ))) (not (forall ((?x_1 Int) (?y Int) (?S Int) (?M Int)) (! (let ((?v_0 (select1 ?M ?x_1))) (=> (not (or (not (= (select1 ?S ?x_1) true_1)) (not (= (Equal (Intersection (Inverse ?M ?v_0) ?S) (Singleton ?x_1)) true_1)))) (= (Equal (Dereference ?S (store1 ?M ?x_1 ?y)) (Union (Difference (Dereference ?S ?M) (Singleton ?v_0)) (Singleton ?y))) true_1))) :pattern ((Dereference ?S (store1 ?M ?x_1 ?y))) ))) (not (forall ((?x_1 Int) (?y Int) (?S Int) (?M Int)) (! (=> (not (or (not (= (select1 ?S ?x_1) true_1)) (not (not (= (Equal (Intersection (Inverse ?M (select1 ?M ?x_1)) ?S) (Singleton ?x_1)) true_1))))) (= (Equal (Dereference ?S (store1 ?M ?x_1 ?y)) (Union (Dereference ?S ?M) (Singleton ?y))) true_1)) :pattern ((Dereference ?S (store1 ?M ?x_1 ?y))) ))) (not (forall ((?f Int) (?x_1 Int)) (! (= (select1 (Inverse ?f (select1 ?f ?x_1)) ?x_1) true_1) :pattern ((Inverse ?f (select1 ?f ?x_1))) ))) (not (forall ((?f Int) (?x_1 Int) (?y Int)) (! (= (Equal (Inverse (store1 ?f ?x_1 ?y) ?y) (Union (Inverse ?f ?y) (Singleton ?x_1))) true_1) :pattern ((Inverse (store1 ?f ?x_1 ?y) ?y)) ))) (not (forall ((?f Int) (?x_1 Int) (?y Int) (?z Int)) (! (or (= ?y ?z) (= (Equal (Inverse (store1 ?f ?x_1 ?y) ?z) (Difference (Inverse ?f ?z) (Singleton ?x_1))) true_1)) :pattern ((Inverse (store1 ?f ?x_1 ?y) ?z)) ))) (not (forall ((?S Int) (?T Int)) (! (= (= (Equal ?S ?T) true_1) (not (or (not (= (Subset ?S ?T) true_1)) (not (= (Subset ?T ?S) true_1))))) :pattern ((Equal ?S ?T)) ))) (not (forall ((?x_1 Int) (?S Int) (?T Int)) (! (=> (not (or (not (= (select1 ?S ?x_1) true_1)) (not (= (Subset ?S ?T) true_1)))) (= (select1 ?T ?x_1) true_1)) :pattern ((select1 ?S ?x_1) (Subset ?S ?T)) ))) (not (forall ((?S Int) (?T Int)) (! (or (= (Subset ?S ?T) true_1) (exists ((?x_1 Int)) (not (or (not (= (select1 ?S ?x_1) true_1)) (not (not (= (select1 ?T ?x_1) true_1))))))) :pattern ((Subset ?S ?T)) ))) (not (forall ((?x_1 Int) (?S Int) (?T Int)) (! (not (not (or (not (= (select1 ?S ?x_1) true_1)) (not (= (Disjoint ?S ?T) true_1)) (not (= (select1 ?T ?x_1) true_1))))) :pattern ((select1 ?S ?x_1) (Disjoint ?S ?T) (select1 ?T ?x_1)) ))) (not (forall ((?S Int) (?T Int)) (! (or (= (Disjoint ?S ?T) true_1) (exists ((?x_1 Int)) (not (or (not (= (select1 ?S ?x_1) true_1)) (not (= (select1 ?T ?x_1) true_1)))))) :pattern ((Disjoint ?S ?T)) ))) (not (forall ((?M Int) (?x_1 Int)) (! (= (select1 (Unified ?M) ?x_1) (select1 (select1 ?M (Field ?x_1)) ?x_1)) :pattern ((select1 (Unified ?M) ?x_1)) ))) (not (forall ((?M Int) (?x_1 Int) (?y Int)) (! (let ((?v_1 (Field ?x_1))) (= (Unified (store1 ?M ?v_1 (store1 (select1 ?M ?v_1) ?x_1 ?y))) (store1 (Unified ?M) ?x_1 ?y))) :pattern ((Unified (store1 ?M (Field ?x_1) (store1 (select1 ?M (Field ?x_1)) ?x_1 ?y)))) ))) (not (forall ((?v Int) (?t Int)) (! (=> (= (select1 (Values ?t) ?v) true_1) (= (HasType ?v ?t) true_1)) :pattern ((select1 (Values ?t) ?v)) ))) (not (forall ((?v Int) (?t Int)) (! (=> (= (HasType ?v ?t) true_1) (= (select1 (Values ?t) ?v) true_1)) :pattern ((HasType ?v ?t) (Values ?t)) ))) (not (forall ((?a Int)) (! (= (= (Match ?a next_foo_name) true_1) (= (Field ?a) next_foo_name)) :pattern ((Match ?a next_foo_name)) ))) (not (forall ((?v Int)) (! (= (= (HasType ?v next_foo_name) true_1) (or (= ?v 0) (not (or (not (> ?v 0)) (not (= (Match ?v foo_name) true_1)))))) :pattern ((HasType ?v next_foo_name))  :pattern ((Match ?v foo_name)) ))) (not (forall ((?a Int)) (! (= (= (Match ?a p_foo_name) true_1) (= (Field ?a) p_foo_name)) :pattern ((Match ?a p_foo_name)) ))) (not (forall ((?v Int)) (! (= (= (HasType ?v p_foo_name) true_1) (or (= ?v 0) (not (or (not (> ?v 0)) (not (= (Match ?v INT4_name) true_1)))))) :pattern ((HasType ?v p_foo_name))  :pattern ((Match ?v INT4_name)) ))) (not (forall ((?a Int)) (! (= (= (Match ?a n_foo_name) true_1) (= (Field ?a) n_foo_name)) :pattern ((Match ?a n_foo_name)) ))) (not (forall ((?v Int)) (= (HasType ?v n_foo_name) true_1))) (not (forall ((?a Int)) (! (= (= (Match ?a INT4_name) true_1) (= (Field ?a) INT4_name)) :pattern ((Match ?a INT4_name)) ))) (not (forall ((?v Int)) (= (HasType ?v INT4_name) true_1))) (not (forall ((?a Int)) (! (= (= (Match ?a PINT4_name) true_1) (= (Field ?a) PINT4_name)) :pattern ((Match ?a PINT4_name)) ))) (not (forall ((?v Int)) (! (= (= (HasType ?v PINT4_name) true_1) (or (= ?v 0) (not (or (not (> ?v 0)) (not (= (Match ?v INT4_name) true_1)))))) :pattern ((HasType ?v PINT4_name))  :pattern ((Match ?v INT4_name)) ))) (not (forall ((?a Int)) (! (= (= (Match ?a PPINT4_name) true_1) (= (Field ?a) PPINT4_name)) :pattern ((Match ?a PPINT4_name)) ))) (not (forall ((?v Int)) (! (= (= (HasType ?v PPINT4_name) true_1) (or (= ?v 0) (not (or (not (> ?v 0)) (not (= (Match ?v PINT4_name) true_1)))))) :pattern ((HasType ?v PPINT4_name))  :pattern ((Match ?v PINT4_name)) ))) (not (forall ((?a Int)) (! (= (= (Match ?a PPfoo_name) true_1) (= (Field ?a) PPfoo_name)) :pattern ((Match ?a PPfoo_name)) ))) (not (forall ((?v Int)) (! (= (= (HasType ?v PPfoo_name) true_1) (or (= ?v 0) (not (or (not (> ?v 0)) (not (= (Match ?v Pfoo_name) true_1)))))) :pattern ((HasType ?v PPfoo_name))  :pattern ((Match ?v Pfoo_name)) ))) (not (forall ((?a Int)) (! (= (= (Match ?a Pfoo_name) true_1) (= (Field ?a) Pfoo_name)) :pattern ((Match ?a Pfoo_name)) ))) (not (forall ((?v Int)) (! (= (= (HasType ?v Pfoo_name) true_1) (or (= ?v 0) (not (or (not (> ?v 0)) (not (= (Match ?v foo_name) true_1)))))) :pattern ((HasType ?v Pfoo_name))  :pattern ((Match ?v foo_name)) ))) (not (forall ((?a Int)) (! (= (= (Match ?a foo_name) true_1) (not (or (not true) (not (= (Match (next_foo ?a) next_foo_name) true_1)) (not (= (Match (p_foo ?a) p_foo_name) true_1)) (not (= (Match (n_foo ?a) n_foo_name) true_1))))) :pattern ((Match ?a foo_name)) ))) (not (forall ((?x_1 Int)) (! (= (next_fooInv (next_foo ?x_1)) ?x_1) :pattern ((next_foo ?x_1)) ))) (not (forall ((?x_1 Int)) (! (= (next_foo (next_fooInv ?x_1)) ?x_1) :pattern ((next_fooInv ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int)) (! (= (= (select1 (S_next_foo ?S) ?x_1) true_1) (= (select1 ?S (next_fooInv ?x_1)) true_1)) :pattern ((select1 (S_next_foo ?S) ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int)) (! (= (= (select1 (S_next_fooInv ?S) ?x_1) true_1) (= (select1 ?S (next_foo ?x_1)) true_1)) :pattern ((select1 (S_next_fooInv ?S) ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int)) (! (=> (= (select1 ?S ?x_1) true_1) (= (select1 (S_next_foo ?S) (next_foo ?x_1)) true_1)) :pattern ((select1 ?S ?x_1) (S_next_foo ?S)) ))) (not (forall ((?x_1 Int) (?S Int)) (! (=> (= (select1 ?S ?x_1) true_1) (= (select1 (S_next_fooInv ?S) (next_fooInv ?x_1)) true_1)) :pattern ((select1 ?S ?x_1) (S_next_fooInv ?S)) ))) (not (forall ((?x_1 Int)) (! (= (next_foo ?x_1) (+ ?x_1 0)) :pattern ((next_foo ?x_1)) ))) (not (forall ((?x_1 Int)) (! (= (next_fooInv ?x_1) (- ?x_1 0)) :pattern ((next_fooInv ?x_1)) ))) (not (forall ((?x_1 Int)) (! (= (MINUS_BOTH_PTR_OR_BOTH_INT ?x_1 0 1) (next_fooInv ?x_1)) :pattern ((MINUS_BOTH_PTR_OR_BOTH_INT ?x_1 0 1)) ))) (not (forall ((?x_1 Int)) (! (= (MINUS_LEFT_PTR ?x_1 1 0) (next_fooInv ?x_1)) :pattern ((MINUS_LEFT_PTR ?x_1 1 0)) ))) (not (forall ((?x_1 Int)) (! (= (p_fooInv (p_foo ?x_1)) ?x_1) :pattern ((p_foo ?x_1)) ))) (not (forall ((?x_1 Int)) (! (= (p_foo (p_fooInv ?x_1)) ?x_1) :pattern ((p_fooInv ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int)) (! (= (= (select1 (S_p_foo ?S) ?x_1) true_1) (= (select1 ?S (p_fooInv ?x_1)) true_1)) :pattern ((select1 (S_p_foo ?S) ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int)) (! (= (= (select1 (S_p_fooInv ?S) ?x_1) true_1) (= (select1 ?S (p_foo ?x_1)) true_1)) :pattern ((select1 (S_p_fooInv ?S) ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int)) (! (=> (= (select1 ?S ?x_1) true_1) (= (select1 (S_p_foo ?S) (p_foo ?x_1)) true_1)) :pattern ((select1 ?S ?x_1) (S_p_foo ?S)) ))) (not (forall ((?x_1 Int) (?S Int)) (! (=> (= (select1 ?S ?x_1) true_1) (= (select1 (S_p_fooInv ?S) (p_fooInv ?x_1)) true_1)) :pattern ((select1 ?S ?x_1) (S_p_fooInv ?S)) ))) (not (forall ((?x_1 Int)) (! (= (p_foo ?x_1) (+ ?x_1 4)) :pattern ((p_foo ?x_1)) ))) (not (forall ((?x_1 Int)) (! (= (p_fooInv ?x_1) (- ?x_1 4)) :pattern ((p_fooInv ?x_1)) ))) (not (forall ((?x_1 Int)) (! (= (MINUS_BOTH_PTR_OR_BOTH_INT ?x_1 4 1) (p_fooInv ?x_1)) :pattern ((MINUS_BOTH_PTR_OR_BOTH_INT ?x_1 4 1)) ))) (not (forall ((?x_1 Int)) (! (= (MINUS_LEFT_PTR ?x_1 1 4) (p_fooInv ?x_1)) :pattern ((MINUS_LEFT_PTR ?x_1 1 4)) ))) (not (forall ((?x_1 Int)) (! (= (n_fooInv (n_foo ?x_1)) ?x_1) :pattern ((n_foo ?x_1)) ))) (not (forall ((?x_1 Int)) (! (= (n_foo (n_fooInv ?x_1)) ?x_1) :pattern ((n_fooInv ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int)) (! (= (= (select1 (S_n_foo ?S) ?x_1) true_1) (= (select1 ?S (n_fooInv ?x_1)) true_1)) :pattern ((select1 (S_n_foo ?S) ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int)) (! (= (= (select1 (S_n_fooInv ?S) ?x_1) true_1) (= (select1 ?S (n_foo ?x_1)) true_1)) :pattern ((select1 (S_n_fooInv ?S) ?x_1)) ))) (not (forall ((?x_1 Int) (?S Int)) (! (=> (= (select1 ?S ?x_1) true_1) (= (select1 (S_n_foo ?S) (n_foo ?x_1)) true_1)) :pattern ((select1 ?S ?x_1) (S_n_foo ?S)) ))) (not (forall ((?x_1 Int) (?S Int)) (! (=> (= (select1 ?S ?x_1) true_1) (= (select1 (S_n_fooInv ?S) (n_fooInv ?x_1)) true_1)) :pattern ((select1 ?S ?x_1) (S_n_fooInv ?S)) ))) (not (forall ((?x_1 Int)) (! (= (n_foo ?x_1) (+ ?x_1 8)) :pattern ((n_foo ?x_1)) ))) (not (forall ((?x_1 Int)) (! (= (n_fooInv ?x_1) (- ?x_1 8)) :pattern ((n_fooInv ?x_1)) ))) (not (forall ((?x_1 Int)) (! (= (MINUS_BOTH_PTR_OR_BOTH_INT ?x_1 8 1) (n_fooInv ?x_1)) :pattern ((MINUS_BOTH_PTR_OR_BOTH_INT ?x_1 8 1)) ))) (not (forall ((?x_1 Int)) (! (= (MINUS_LEFT_PTR ?x_1 1 8) (n_fooInv ?x_1)) :pattern ((MINUS_LEFT_PTR ?x_1 1 8)) ))) (not (forall ((?a Int) (?b Int) (?size Int)) (! (let ((?v_3 (MINUS_BOTH_PTR_OR_BOTH_INT ?a ?b ?size)) (?v_2 (- ?a ?b))) (not (or (not (<= (* ?size ?v_3) ?v_2)) (not (< ?v_2 (* ?size (+ ?v_3 1))))))) :pattern ((MINUS_BOTH_PTR_OR_BOTH_INT ?a ?b ?size)) ))) (not (forall ((?a Int) (?a_size Int) (?b Int)) (! (= (MINUS_LEFT_PTR ?a ?a_size ?b) (- ?a (* ?a_size ?b))) :pattern ((MINUS_LEFT_PTR ?a ?a_size ?b)) ))) (not (forall ((?a Int) (?a_size Int) (?b Int)) (! (= (PLUS ?a ?a_size ?b) (+ ?a (* ?a_size ?b))) :pattern ((PLUS ?a ?a_size ?b)) ))) (not (forall ((?a Int) (?b Int)) (! (= (MULT ?a ?b) (* ?a ?b)) :pattern ((MULT ?a ?b)) ))) (not (forall ((?a Int) (?b Int)) (! (let ((?v_4 (DIV ?a ?b))) (=> (not (or (not (>= ?a 0)) (not (> ?b 0)))) (not (or (not (<= (* ?b ?v_4) ?a)) (not (< ?a (* ?b (+ ?v_4 1)))))))) :pattern ((DIV ?a ?b)) ))) (not (forall ((?a Int) (?b Int)) (! (let ((?v_5 (DIV ?a ?b))) (=> (not (or (not (>= ?a 0)) (not (< ?b 0)))) (not (or (not (<= (* ?b ?v_5) ?a)) (not (< ?a (* ?b (- ?v_5 1)))))))) :pattern ((DIV ?a ?b)) ))) (not (forall ((?a Int) (?b Int)) (! (let ((?v_6 (DIV ?a ?b))) (=> (not (or (not (< ?a 0)) (not (> ?b 0)))) (not (or (not (>= (* ?b ?v_6) ?a)) (not (> ?a (* ?b (- ?v_6 1)))))))) :pattern ((DIV ?a ?b)) ))) (not (forall ((?a Int) (?b Int)) (! (let ((?v_7 (DIV ?a ?b))) (=> (not (or (not (< ?a 0)) (not (< ?b 0)))) (not (or (not (>= (* ?b ?v_7) ?a)) (not (> ?a (* ?b (+ ?v_7 1)))))))) :pattern ((DIV ?a ?b)) ))) (not (= (POW2 1) true_1)) (not (= (POW2 2) true_1)) (not (= (POW2 4) true_1)) (not (= (POW2 8) true_1)) (not (= (POW2 16) true_1)) (not (= (POW2 32) true_1)) (not (= (POW2 64) true_1)) (not (= (POW2 128) true_1)) (not (= (POW2 256) true_1)) (not (= (POW2 512) true_1)) (not (= (POW2 1024) true_1)) (not (= (POW2 2048) true_1)) (not (= (POW2 4096) true_1)) (not (= (POW2 8192) true_1)) (not (= (POW2 16384) true_1)) (not (= (POW2 32768) true_1)) (not (= (POW2 65536) true_1)) (not (= (POW2 131072) true_1)) (not (= (POW2 262144) true_1)) (not (= (POW2 524288) true_1)) (not (= (POW2 1048576) true_1)) (not (= (POW2 2097152) true_1)) (not (= (POW2 4194304) true_1)) (not (= (POW2 8388608) true_1)) (not (= (POW2 16777216) true_1)) (not (= (POW2 33554432) true_1)) (not (forall ((?a Int) (?b Int) (?c Int)) (! (=> (= ?a true_1) (= (choose ?a ?b ?c) ?b)) :pattern ((choose ?a ?b ?c)) ))) (not (forall ((?a Int) (?b Int) (?c Int)) (! (=> (not (= ?a true_1)) (= (choose ?a ?b ?c) ?c)) :pattern ((choose ?a ?b ?c)) ))) (not (forall ((?a Int) (?b Int)) (! (=> (= ?a ?b) (= (BIT_BAND ?a ?b) ?a)) :pattern ((BIT_BAND ?a ?b)) ))) (not (forall ((?a Int) (?b Int)) (! (=> (not (or (not (= (POW2 ?a) true_1)) (not (= (POW2 ?b) true_1)) (not (not (= ?a ?b))))) (= (BIT_BAND ?a ?b) 0)) :pattern ((BIT_BAND ?a ?b)) ))) (not (forall ((?a Int) (?b Int)) (! (=> (or (= ?a 0) (= ?b 0)) (= (BIT_BAND ?a ?b) 0)) :pattern ((BIT_BAND ?a ?b)) ))) (not (forall ((?a Int)) (! (= (= ?a true_1) (not (= (LIFT ?a) 0))) :pattern ((LIFT ?a)) ))) (not (forall ((?a Int)) (! (=> (= ?a 0) (not (= (boolNot ?a) 0))) :pattern ((boolNot ?a)) ))) (not (forall ((?a Int)) (! (=> (not (= ?a 0)) (= (boolNot ?a) 0)) :pattern ((boolNot ?a)) ))) (not (forall ((?a Int)) (! (=> (= ?a 0) (not (= (NULL_CHECK ?a) 0))) :pattern ((NULL_CHECK ?a)) ))) (not (forall ((?a Int)) (! (=> (not (= ?a 0)) (= (NULL_CHECK ?a) 0)) :pattern ((NULL_CHECK ?a)) ))))))
(assert (let ((?v_52 (forall ((?a Int)) (! (let ((?v_53 (Field ?a))) (= (HasType (select1 (select1 Mem_3 ?v_53) ?a) ?v_53) true_1)) :pattern ((select1 (select1 Mem_3 (Field ?a)) ?a)) ))) (?v_51 (= (HasType result_bar_24_4_1__0 INT4_name) true_1)) (?v_50 (not (or (not (<= 0 head_0)) (not (<= head_0 0))))) (?v_49 (= head_0 0)) (?v_48 (=> true (=> false true))) (?v_44 (= (HasType sum_1 INT4_name) true_1))) (let ((?v_47 (not ?v_44)) (?v_29 (= (HasType i_1 INT4_name) true_1))) (let ((?v_35 (not ?v_29)) (?v_43 (= (HasType head_1 Pfoo_name) true_1))) (let ((?v_46 (not ?v_43)) (?v_28 (forall ((?a Int)) (! (let ((?v_54 (Field ?a))) (= (HasType (select1 (select1 Mem_1 ?v_54) ?a) ?v_54) true_1)) :pattern ((select1 (select1 Mem_1 (Field ?a)) ?a)) )))) (let ((?v_42 (not ?v_28)) (?v_12 (Field 0))) (let ((?v_10 (select1 (select1 Mem ?v_12) 0)) (?v_45 (select1 (select1 Mem_1 ?v_12) 0)) (?v_11 (forall ((?f Int)) (! (let ((?v_55 (select1 alloc (Base ?f)))) (or (= ?v_55 UNALLOCATED) (= ?v_55 ?v_55))) :pattern ((select1 alloc (Base ?f))) ))) (?v_41 (next_foo head_0)) (?v_22 (not ?v_49)) (?v_19 (n_foo head_0))) (let ((?v_23 (Field ?v_19))) (let ((?v_24 (select1 (select1 Mem_1 ?v_23) ?v_19))) (let ((?v_40 (< i_1 ?v_24)) (?v_30 (= (HasType tempBoogie0_2 INT4_name) true_1))) (let ((?v_36 (not ?v_30)) (?v_39 (= (HasType i_2 INT4_name) true_1)) (?v_14 (= (HasType head_0 Pfoo_name) true_1))) (let ((?v_17 (not ?v_14)) (?v_34 (forall ((?a Int)) (! (let ((?v_56 (Field ?a))) (= (HasType (select1 (select1 Mem_2 ?v_56) ?a) ?v_56) true_1)) :pattern ((select1 (select1 Mem_2 (Field ?a)) ?a)) )))) (let ((?v_38 (not ?v_34)) (?v_21 (select1 (select1 Mem_0 ?v_12) 0)) (?v_37 (not (or (not (<= 0 i_2)) (not (<= i_2 (select1 (select1 Mem_2 ?v_23) ?v_19)))))) (?v_25 (p_foo head_0))) (let ((?v_26 (select1 (select1 Mem_1 (Field ?v_25)) ?v_25))) (let ((?v_27 (PLUS ?v_26 4 i_1))) (let ((?v_32 (Field ?v_27))) (let ((?v_33 (select1 Mem_1 ?v_32)) (?v_31 (not (= ?v_26 0))) (?v_15 (= (HasType sum_0 INT4_name) true_1))) (let ((?v_18 (not ?v_15)) (?v_2 (= (HasType 0 INT4_name) true_1))) (let ((?v_8 (not ?v_2)) (?v_13 (forall ((?a Int)) (! (let ((?v_57 (Field ?a))) (= (HasType (select1 (select1 Mem_0 ?v_57) ?a) ?v_57) true_1)) :pattern ((select1 (select1 Mem_0 (Field ?a)) ?a)) )))) (let ((?v_16 (not ?v_13)) (?v_4 (not (<= 0 0)))) (let ((?v_20 (not (or ?v_4 (not (<= 0 (select1 (select1 Mem_0 ?v_23) ?v_19)))))) (?v_5 (not (or ?v_4 ?v_4))) (?v_0 (= (HasType head1 Pfoo_name) true_1))) (let ((?v_7 (not ?v_0)) (?v_1 (forall ((?a Int)) (! (let ((?v_58 (Field ?a))) (= (HasType (select1 (select1 Mem ?v_58) ?a) ?v_58) true_1)) :pattern ((select1 (select1 Mem (Field ?a)) ?a)) )))) (let ((?v_6 (not ?v_1)) (?v_9 (not (or ?v_4 ?v_4 ?v_4 ?v_4))) (?v_3 (= (HasType sum INT4_name) true_1))) (not (=> true (=> ?v_0 (=> ?v_1 (=> (forall ((?H_pfoo Int)) (let ((?v_59 (n_foo ?H_pfoo))) (let ((?v_61 (select1 (select1 Mem (Field ?v_59)) ?v_59)) (?v_60 (p_foo ?H_pfoo))) (=> (= (HasType ?H_pfoo Pfoo_name) true_1) (not (or (not (>= ?v_61 0)) (not (= (Subset Empty (HavocArray (select1 (select1 Mem (Field ?v_60)) ?v_60) 4 ?v_61)) true_1)) (not (forall ((?H_pint Int)) (! (=> (= (select1 (HavocArray (select1 (select1 Mem (Field ?v_60)) ?v_60) 4 ?v_61) ?H_pint) true_1) (= (HasType ?H_pint PINT4_name) true_1)) :pattern ((select1 (HavocArray (select1 (select1 Mem (Field ?v_60)) ?v_60) 4 ?v_61) ?H_pint)) ))))))))) (=> true (=> (not (= (select1 alloc head1) UNALLOCATED)) (=> (= (HasType head Pfoo_name) true_1) (=> ?v_0 (=> (= (HasType i INT4_name) true_1) (=> ?v_3 (=> (= (HasType result_bar_24_4_1_ INT4_name) true_1) (=> true (=> true (=> true (=> true (=> true (=> true (not (or ?v_6 (not (=> ?v_1 (not (or ?v_7 (not (=> ?v_0 (not (or ?v_8 (not (=> ?v_2 (not (or (not ?v_3) (not (=> ?v_3 (=> ?v_5 (=> true (=> ?v_5 (=> ?v_5 (=> true (=> ?v_5 (not (or ?v_6 (not (=> ?v_1 (not (or ?v_7 (not (=> ?v_0 (not (or ?v_8 (not (=> ?v_2 (not (or ?v_8 (not (=> ?v_2 (=> ?v_9 (=> true (=> ?v_9 (=> ?v_9 (=> true (=> ?v_11 (=> (forall ((?f Int) (?m Int)) (! (let ((?v_62 (select1 (select1 Mem ?f) ?m))) (= ?v_62 ?v_62)) :pattern ((select1 (select1 Mem ?f) ?m)) )) (=> (= ?v_10 ?v_10) (not (or ?v_6 (not (=> ?v_1 (not (or ?v_7 (not (=> ?v_0 (not (or ?v_8 (not (=> ?v_2 (not (or ?v_8 (not (=> ?v_2 (=> true (=> true (=> true (=> ?v_11 (=> (forall ((?f Int) (?m Int)) (! (= (select1 (select1 Mem_0 ?f) ?m) (select1 (select1 Mem ?f) ?m)) :pattern ((select1 (select1 Mem_0 ?f) ?m)) )) (=> (= ?v_21 ?v_10) (=> ?v_13 (=> ?v_14 (=> (= (HasType i_0 INT4_name) true_1) (=> ?v_15 (=> true (not (or (not (=> true (=> true (=> ?v_22 (=> true (=> true (=> true (not (or ?v_16 (not (=> ?v_13 (not (or ?v_17 (not (=> ?v_14 (not (or ?v_8 (not (=> ?v_2 (not (or ?v_18 (not (=> ?v_15 (=> ?v_5 (=> true (=> ?v_5 (=> ?v_5 (=> true (=> ?v_5 (not (or ?v_16 (not (=> ?v_13 (not (or ?v_17 (not (=> ?v_14 (not (or ?v_8 (not (=> ?v_2 (not (or ?v_18 (not (=> ?v_15 (=> ?v_5 (=> true (=> ?v_5 (=> ?v_5 (=> true (not (or (not ?v_20) (not (=> ?v_20 (=> ?v_11 (=> (forall ((?f Int) (?m Int)) (! (let ((?v_63 (select1 (select1 Mem_0 ?f) ?m))) (= ?v_63 ?v_63)) :pattern ((select1 (select1 Mem_0 ?f) ?m)) )) (=> (= ?v_21 ?v_21) (not (or ?v_16 (not (=> ?v_13 (not (or ?v_17 (not (=> ?v_14 (not (or ?v_8 (not (=> ?v_2 (not (or ?v_18 (not (=> ?v_15 (=> ?v_22 (=> true (=> true (=> true (=> (not (or (not (<= 0 i_1)) (not (<= i_1 ?v_24)))) (=> ?v_11 (=> (forall ((?f Int) (?m Int)) (! (= (select1 (select1 Mem_1 ?f) ?m) (select1 (select1 Mem_0 ?f) ?m)) :pattern ((select1 (select1 Mem_1 ?f) ?m)) )) (=> (= ?v_45 ?v_21) (=> ?v_28 (=> ?v_14 (=> ?v_29 (=> ?v_44 (=> ?v_22 (=> true (not (or (not (=> true (=> true (=> ?v_40 (=> true (=> true (=> true (=> ?v_22 (=> ?v_31 (=> (= tempBoogie0_2 (PLUS sum_1 1 (select1 ?v_33 ?v_27))) (not (or ?v_42 (not (=> ?v_28 (not (or ?v_17 (not (=> ?v_14 (not (or ?v_35 (not (=> ?v_29 (not (or ?v_36 (not (=> ?v_30 (=> true (=> true (=> true (=> ?v_22 (=> ?v_31 (=> (= Mem_2 (store1 Mem_1 ?v_32 (store1 ?v_33 ?v_27 42))) (not (or ?v_38 (not (=> ?v_34 (not (or ?v_17 (not (=> ?v_14 (not (or ?v_35 (not (=> ?v_29 (not (or ?v_36 (not (=> ?v_30 (=> true (=> true (=> true (=> (= i_2 (PLUS i_1 1 1)) (=> true (=> true (=> true (not (or (not ?v_37) (not (=> ?v_37 (=> ?v_11 (=> (forall ((?f Int) (?m Int)) (! (= (select1 (select1 Mem_2 ?f) ?m) (select1 (select1 Mem_0 ?f) ?m)) :pattern ((select1 (select1 Mem_2 ?f) ?m)) )) (=> (= (select1 (select1 Mem_2 ?v_12) 0) ?v_21) (not (or ?v_38 (not (=> ?v_34 (not (or ?v_17 (not (=> ?v_14 (not (or (not ?v_39) (not (=> ?v_39 (not (or ?v_36 (not (=> ?v_30 (=> ?v_22 ?v_48))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) (not (=> true (=> true (=> (not ?v_40) (=> true (=> true (=> true (=> ?v_22 (=> (= head_1 (select1 (select1 Mem_1 (Field ?v_41)) ?v_41)) (not (or ?v_42 (not (=> ?v_28 (not (or ?v_46 (not (=> ?v_43 (not (or ?v_35 (not (=> ?v_29 (not (or ?v_47 (not (=> ?v_44 (=> true (=> true (=> true (=> ?v_11 (=> (forall ((?f Int) (?m Int)) (! (= (select1 (select1 Mem_1 ?f) ?m) (select1 (select1 Mem ?f) ?m)) :pattern ((select1 (select1 Mem_1 ?f) ?m)) )) (=> (= ?v_45 ?v_10) (not (or ?v_42 (not (=> ?v_28 (not (or ?v_46 (not (=> ?v_43 (not (or ?v_35 (not (=> ?v_29 (not (or ?v_47 (not (=> ?v_44 ?v_48)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) (not (=> true (=> true (=> ?v_49 (=> ?v_50 (=> true (=> ?v_50 (=> ?v_50 (=> true (=> ?v_50 (=> (forall ((?m Int)) (! (let ((?v_65 (Field ?m)) (?v_64 (not (not (= (select1 alloc (Base ?m)) ALLOCATED))))) (=> (not (or ?v_64 ?v_64)) (= (select1 (select1 Mem_0 ?v_65) ?m) (select1 (select1 Mem ?v_65) ?m)))) :pattern ((select1 (select1 Mem_0 (Field ?m)) ?m)) )) (=> ?v_50 (=> true (=> (= LOOP_12_alloc_1 LOOP_12_alloc_0) (=> (= i_3 i_0) (=> (= sum_2 sum_0) (=> (= tempBoogie0_3 tempBoogie0_0) (=> (= head_2 head_0) (=> (= result_bar_24_4_1__0 sum_0) (=> (= LOOP_12_Mem_1 LOOP_12_Mem_0) (=> (= Mem_3 Mem_0) (=> true (=> true (not (or (not ?v_51) (not (=> ?v_51 (not (or (not ?v_52) (not (=> ?v_52 true)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))
(check-sat)
(exit)
