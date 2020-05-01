; COMMAND-LINE: --cegqi-bv
; EXPECT: sat
(set-logic BV)
(set-info :status sat)
(declare-fun xI () (_ BitVec 32))
(declare-fun A () (_ BitVec 32))
(declare-fun B () (_ BitVec 32))
(declare-fun vuscore2dollarskuscore80 () (_ BitVec 32))
(declare-fun I1 () (_ BitVec 32))
(declare-fun xuscore2dollarskuscore74 () (_ BitVec 32))
(declare-fun v () (_ BitVec 32))
(declare-fun ts49uscore0 () (_ BitVec 32))
(declare-fun V () (_ BitVec 32))
(declare-fun t87uscore0dollarskuscore0 () (_ BitVec 32))
(declare-fun ep () (_ BitVec 32))
(declare-fun I1uscore2dollarskuscore74 () (_ BitVec 32))
(declare-fun x () (_ BitVec 32))
(assert (not (exists ((ts49uscore0 (_ BitVec 32))) (let ((?v_0 (bvsge vuscore2dollarskuscore80 (_ bv0 32))) (?v_1 (bvsle vuscore2dollarskuscore80 V)) (?v_3 (bvsdiv (bvmul vuscore2dollarskuscore80 vuscore2dollarskuscore80) (bvmul (_ bv2 32) B))) (?v_2 (bvmul (bvsdiv (_ bv1 32) (_ bv2 32)) (bvadd (bvmul (bvmul (_ bv2 32) t87uscore0dollarskuscore0) vuscore2dollarskuscore80) (bvmul (_ bv2 32) xuscore2dollarskuscore74))))) (=> (and (and (and (and (and (and (and (and (and (and (and (and (and (and (= I1uscore2dollarskuscore74 (_ bv2 32)) (=> (and (bvsle (_ bv0 32) ts49uscore0) (bvsle ts49uscore0 t87uscore0dollarskuscore0)) (and (and ?v_0 ?v_1) (bvsle ts49uscore0 ep)))) (bvsge t87uscore0dollarskuscore0 (_ bv0 32))) (= vuscore2dollarskuscore80 (_ bv0 32))) ?v_0) ?v_1) (bvsgt xI (bvadd xuscore2dollarskuscore74 ?v_3))) (= I1 (_ bv2 32))) (bvslt xI x)) (bvsgt B (_ bv0 32))) (bvsge v (_ bv0 32))) (bvsle v V)) (bvsge A (_ bv0 32))) (bvsgt V (_ bv0 32))) (bvsgt ep (_ bv0 32))) (or (or (= xI xuscore2dollarskuscore74) (bvslt xI ?v_2)) (bvsgt xI (bvadd ?v_2 ?v_3))))))))
(check-sat)
(exit)
