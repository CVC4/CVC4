(set-logic QF_ABV)
(declare-const bv_25-0 (_ BitVec 25))
(declare-const arr0 (Array (_ BitVec 25) Bool))
(assert (select arr0 (bvnot bv_25-0)))
(check-sat)
