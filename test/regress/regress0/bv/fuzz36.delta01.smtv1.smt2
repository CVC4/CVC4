(set-option :incremental false)
(meta-info :status sat)
(set-logic QF_BV)
(declare-fun v0 () (_ BitVec 4))
(declare-fun v2 () (_ BitVec 4))
(declare-fun v3 () (_ BitVec 4))
(check-sat-assuming ( (let ((_let_0 (bvashr v2 v0))) (let ((_let_1 (bvmul v0 (bvnor (_ bv1 4) (_ bv0 4))))) (let ((_let_2 ((_ sign_extend 3) (ite (bvslt v0 (_ bv1 4)) (_ bv1 1) (_ bv0 1))))) (and (or false (distinct _let_0 v3) (bvugt (_ bv1 4) ((_ sign_extend 3) (ite (= v2 (_ bv0 4)) (_ bv1 1) (_ bv0 1))))) (bvslt v2 _let_0) (bvuge (bvnor v0 ((_ sign_extend 3) (ite (bvuge v3 _let_1) (_ bv1 1) (_ bv0 1)))) _let_0) (bvslt _let_0 (bvadd v0 (_ bv13 4))) (bvsle v0 (_ bv1 4)) (or false (distinct (_ bv0 4) _let_1) (= (_ bv0 4) _let_2)) (or false (bvsgt (_ bv0 1) (ite (= v2 v0) (_ bv1 1) (_ bv0 1))) (bvsle (_ bv0 4) (bvshl (_ bv1 4) _let_2))))))) ))
