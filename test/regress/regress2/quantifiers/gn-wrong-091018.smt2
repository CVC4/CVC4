; COMMAND-LINE: --global-negate
; EXPECT: sat
(set-info :smt-lib-version 2.6)
(set-logic BV)
(set-info :status sat)
(set-info :source |
These benchmarks used in the paper:

  Dejan Jovanovic and Leonardo de Moura.  Solving Non-Linear Arithmetic.
  In IJCAR 2012, published as LNCS volume 7364, pp. 339--354.

The keymaera family contains VCs from Keymaera verification, see:

  A. Platzer, J.-D. Quesel, and P. Rummer.  Real world verification.
  In CADE 2009, pages 485-501. Springer, 2009.

Submitted by Dejan Jovanovic for SMT-LIB.

 KeYmaera example: bouncing-ball, node 5808 For more info see: No further information available.

Translated to BV by Mathias Preiner.
|)
(set-info :license "https://creativecommons.org/licenses/by/4.0/")
(set-info :category "industrial")
(set-info :status unknown)
(declare-fun v () (_ BitVec 32))
(declare-fun g () (_ BitVec 32))
(declare-fun t () (_ BitVec 32))
(declare-fun V () (_ BitVec 32))
(declare-fun vuscore2dollarskuscore0 () (_ BitVec 32))
(declare-fun huscore2dollarskuscore0 () (_ BitVec 32))
(declare-fun c () (_ BitVec 32))
(declare-fun t1uscore0dollarskuscore0 () (_ BitVec 32))
(declare-fun tuscore2dollarskuscore0 () (_ BitVec 32))
(declare-fun ts1uscore0 () (_ BitVec 32))
(declare-fun h () (_ BitVec 32))
(assert (not (exists ((ts1uscore0 (_ BitVec 32))) (let ((?v_2 (bvmul (_ bv2 32) huscore2dollarskuscore0)) (?v_0 (bvsdiv g (_ bv2 32))) (?v_1 (bvneg g)) (?v_4 (bvadd t1uscore0dollarskuscore0 tuscore2dollarskuscore0)) (?v_3 (bvmul (bvneg (_ bv1 32)) g))) (=> (and (and (and (and (and (and (and (and (and (and (and (and (=> (and (bvsle (_ bv0 32) ts1uscore0) (bvsle ts1uscore0 t1uscore0dollarskuscore0)) (bvsge (bvmul (bvsdiv (_ bv1 32) (_ bv2 32)) (bvadd (bvadd ?v_2 (bvmul ?v_3 (bvmul ts1uscore0 ts1uscore0))) (bvmul (bvmul (_ bv2 32) ts1uscore0) vuscore2dollarskuscore0))) (_ bv0 32))) (bvsge t1uscore0dollarskuscore0 (_ bv0 32))) (= huscore2dollarskuscore0 (bvadd (bvmul ?v_0 (bvmul tuscore2dollarskuscore0 tuscore2dollarskuscore0)) (bvmul vuscore2dollarskuscore0 tuscore2dollarskuscore0)))) (bvsge huscore2dollarskuscore0 (_ bv0 32))) (bvsge tuscore2dollarskuscore0 (_ bv0 32))) (bvsle vuscore2dollarskuscore0 (bvadd (bvmul ?v_1 tuscore2dollarskuscore0) V))) (bvsgt g (_ bv0 32))) (bvsle (_ bv0 32) c)) (bvslt c (_ bv1 32))) (= h (bvadd (bvmul ?v_0 (bvmul t t)) (bvmul v t)))) (bvsge h (_ bv0 32))) (bvsge t (_ bv0 32))) (bvsle v (bvadd (bvmul ?v_1 t) V))) (or (bvsgt ?v_4 (_ bv0 32)) (= (bvmul (bvsdiv (_ bv1 32) (_ bv2 32)) (bvadd (bvadd ?v_2 (bvmul ?v_3 (bvmul t1uscore0dollarskuscore0 t1uscore0dollarskuscore0))) (bvmul (bvmul (_ bv2 32) t1uscore0dollarskuscore0) vuscore2dollarskuscore0))) (bvadd (bvmul ?v_0 (bvmul ?v_4 ?v_4)) (bvmul (bvadd (bvmul ?v_3 t1uscore0dollarskuscore0) vuscore2dollarskuscore0) ?v_4)))))))))
(check-sat)
(exit)
