; COMMAND-LINE: --finite-model-find
; EXPECT: sat
(set-logic ALL)
(set-info :smt-lib-version 2.6)
(set-info :category "unknown")
(set-info :status sat)
(declare-datatype UNIT ((Unit)))
(declare-datatype BOOL ((Truth)  (Falsity)))
(declare-sort resource$type 0)
(declare-sort process$type 0)
(declare-fun null () resource$type)
(declare-sort S$t$type 0)
(declare-fun S$empty () S$t$type)
(declare-fun S$mem (process$type S$t$type) BOOL)
(declare-fun S$add (process$type S$t$type) S$t$type)
(declare-fun S$remove (process$type S$t$type) S$t$type)
(declare-fun S$cardinality (S$t$type) Int)
(assert (forall ((e process$type)) (not (= (S$mem e S$empty) Truth))))
(assert (forall ((x process$type) (y process$type) (s S$t$type)) (= (S$mem x (S$add y s)) (ite (or (= x y) (= (S$mem x s) Truth)) Truth Falsity))))
(assert (forall ((x process$type) (y process$type) (s S$t$type)) (= (S$mem x (S$remove y s)) (ite (and (not (= x y)) (= (S$mem x s) Truth)) Truth Falsity))))
(assert (= (S$cardinality S$empty) 0))
(assert (forall ((s S$t$type)) (=> (= (S$cardinality s) 0) (= s S$empty))))
(assert (forall ((s S$t$type)) (>= (S$cardinality s) 0)))
(assert (forall ((x process$type) (s S$t$type)) (let ((?v_0 (S$cardinality s))) (= (S$cardinality (S$add x s)) (ite (= (S$mem x s) Truth) ?v_0 (+ ?v_0 1))))))
(assert (forall ((x process$type) (s S$t$type)) (let ((?v_0 (S$cardinality s))) (= (S$cardinality (S$remove x s)) (ite (= (S$mem x s) Truth) (- ?v_0 1) ?v_0)))))
(declare-fun count () (Array resource$type Int))
(declare-fun ref () (Array process$type resource$type))
(declare-fun valid () (Array resource$type BOOL))
(declare-fun destroy$r () resource$type)
(declare-fun valid$1 () (Array resource$type BOOL))
(assert (not (=> (forall ((p process$type)) (let ((?v_0 (select ref p))) (=> (not (= ?v_0 null)) (= (select valid ?v_0) Truth)))) (=> (not (= destroy$r null)) (=> (= (select valid destroy$r) Truth) (=> (= (select count destroy$r) 0) (=> (= valid$1 (store valid destroy$r Falsity)) (forall ((p process$type)) (let ((?v_1 (select ref p))) (=> (not (= ?v_1 null)) (= (select valid$1 ?v_1) Truth)))))))))))
(check-sat)
(exit)
