; COMMAND-LINE: --finite-model-find -q
; EXPECT: sat
(set-logic ALL_SUPPORTED)
(set-info :status sat)
(declare-datatypes ((array!896 0)) (((array!896!897 (size!898 (_ BitVec 32)) (content!899 (Array (_ BitVec 32) (_ BitVec 32)))))))
(declare-datatypes ((tuple2!900 0)) (((tuple2!900!901 (_1!902 array!896) (_2!903 (_ BitVec 32))))))
(declare-fun error_value!904 () (_ BitVec 32))
(declare-fun error_value!905 () (_ BitVec 32))
(declare-fun error_value!906 () array!896)
(declare-fun error_value!907 () (_ BitVec 32))
(declare-fun error_value!908 () array!896)
(declare-fun error_value!909 () (_ BitVec 32))
(declare-fun while0!216 (array!896 (_ BitVec 32) array!896) tuple2!900)
(declare-fun isPositive!206 (array!896 (_ BitVec 32)) Bool)
(declare-fun rec!210 ((_ BitVec 32) array!896 (_ BitVec 32)) Bool)
(declare-fun arrayconst!910 () (Array (_ BitVec 32) (_ BitVec 32)))
(declare-sort I_while0!216 0)
(declare-fun while0!216_arg_0_1 (I_while0!216) array!896)
(declare-fun while0!216_arg_1_2 (I_while0!216) (_ BitVec 32))
(declare-fun while0!216_arg_2_3 (I_while0!216) array!896)
(declare-sort I_isPositive!206 0)
(declare-fun isPositive!206_arg_0_4 (I_isPositive!206) array!896)
(declare-fun isPositive!206_arg_1_5 (I_isPositive!206) (_ BitVec 32))
(declare-sort I_rec!210 0)
(declare-fun rec!210_arg_0_6 (I_rec!210) (_ BitVec 32))
(declare-fun rec!210_arg_1_7 (I_rec!210) array!896)
(declare-fun rec!210_arg_2_8 (I_rec!210) (_ BitVec 32))
(assert (forall ((?i I_while0!216)) (and (= (while0!216 (while0!216_arg_0_1 ?i) (while0!216_arg_1_2 ?i) (while0!216_arg_2_3 ?i)) (ite (bvslt (while0!216_arg_1_2 ?i) (size!898 (while0!216_arg_2_3 ?i))) (while0!216 (ite (bvslt (ite (and (bvslt (while0!216_arg_1_2 ?i) (size!898 (while0!216_arg_2_3 ?i))) (not (bvslt (while0!216_arg_1_2 ?i) (_ bv0 32)))) (select (content!899 (while0!216_arg_2_3 ?i)) (while0!216_arg_1_2 ?i)) error_value!904) (_ bv0 32)) (ite (and (bvslt (while0!216_arg_1_2 ?i) (size!898 (while0!216_arg_0_1 ?i))) (not (bvslt (while0!216_arg_1_2 ?i) (_ bv0 32)))) (array!896!897 (size!898 (while0!216_arg_0_1 ?i)) (store (content!899 (while0!216_arg_0_1 ?i)) (while0!216_arg_1_2 ?i) (bvneg (ite (and (bvslt (while0!216_arg_1_2 ?i) (size!898 (while0!216_arg_2_3 ?i))) (not (bvslt (while0!216_arg_1_2 ?i) (_ bv0 32)))) (select (content!899 (while0!216_arg_2_3 ?i)) (while0!216_arg_1_2 ?i)) error_value!905)))) error_value!906) (ite (and (bvslt (while0!216_arg_1_2 ?i) (size!898 (while0!216_arg_0_1 ?i))) (not (bvslt (while0!216_arg_1_2 ?i) (_ bv0 32)))) (array!896!897 (size!898 (while0!216_arg_0_1 ?i)) (store (content!899 (while0!216_arg_0_1 ?i)) (while0!216_arg_1_2 ?i) (ite (and (bvslt (while0!216_arg_1_2 ?i) (size!898 (while0!216_arg_2_3 ?i))) (not (bvslt (while0!216_arg_1_2 ?i) (_ bv0 32)))) (select (content!899 (while0!216_arg_2_3 ?i)) (while0!216_arg_1_2 ?i)) error_value!907))) error_value!908)) (bvadd (while0!216_arg_1_2 ?i) (_ bv1 32)) (while0!216_arg_2_3 ?i)) (tuple2!900!901 (while0!216_arg_0_1 ?i) (while0!216_arg_1_2 ?i)))) (ite (bvslt (while0!216_arg_1_2 ?i) (size!898 (while0!216_arg_2_3 ?i))) (not (forall ((?z I_while0!216)) (not (and (= (while0!216_arg_0_1 ?z) (ite (bvslt (ite (and (bvslt (while0!216_arg_1_2 ?i) (size!898 (while0!216_arg_2_3 ?i))) (not (bvslt (while0!216_arg_1_2 ?i) (_ bv0 32)))) (select (content!899 (while0!216_arg_2_3 ?i)) (while0!216_arg_1_2 ?i)) error_value!904) (_ bv0 32)) (ite (and (bvslt (while0!216_arg_1_2 ?i) (size!898 (while0!216_arg_0_1 ?i))) (not (bvslt (while0!216_arg_1_2 ?i) (_ bv0 32)))) (array!896!897 (size!898 (while0!216_arg_0_1 ?i)) (store (content!899 (while0!216_arg_0_1 ?i)) (while0!216_arg_1_2 ?i) (bvneg (ite (and (bvslt (while0!216_arg_1_2 ?i) (size!898 (while0!216_arg_2_3 ?i))) (not (bvslt (while0!216_arg_1_2 ?i) (_ bv0 32)))) (select (content!899 (while0!216_arg_2_3 ?i)) (while0!216_arg_1_2 ?i)) error_value!905)))) error_value!906) (ite (and (bvslt (while0!216_arg_1_2 ?i) (size!898 (while0!216_arg_0_1 ?i))) (not (bvslt (while0!216_arg_1_2 ?i) (_ bv0 32)))) (array!896!897 (size!898 (while0!216_arg_0_1 ?i)) (store (content!899 (while0!216_arg_0_1 ?i)) (while0!216_arg_1_2 ?i) (ite (and (bvslt (while0!216_arg_1_2 ?i) (size!898 (while0!216_arg_2_3 ?i))) (not (bvslt (while0!216_arg_1_2 ?i) (_ bv0 32)))) (select (content!899 (while0!216_arg_2_3 ?i)) (while0!216_arg_1_2 ?i)) error_value!907))) error_value!908))) (= (while0!216_arg_1_2 ?z) (bvadd (while0!216_arg_1_2 ?i) (_ bv1 32))) (= (while0!216_arg_2_3 ?z) (while0!216_arg_2_3 ?i)))) )) true)) ))
(assert (forall ((?i I_isPositive!206)) (and (= (isPositive!206 (isPositive!206_arg_0_4 ?i) (isPositive!206_arg_1_5 ?i)) (rec!210 (_ bv0 32) (isPositive!206_arg_0_4 ?i) (isPositive!206_arg_1_5 ?i))) (not (forall ((?z I_rec!210)) (not (and (= (rec!210_arg_0_6 ?z) (_ bv0 32)) (= (rec!210_arg_1_7 ?z) (isPositive!206_arg_0_4 ?i)) (= (rec!210_arg_2_8 ?z) (isPositive!206_arg_1_5 ?i)))) ))) ))
(assert (forall ((?i I_rec!210)) (and (= (rec!210 (rec!210_arg_0_6 ?i) (rec!210_arg_1_7 ?i) (rec!210_arg_2_8 ?i)) (ite (not (bvslt (rec!210_arg_0_6 ?i) (rec!210_arg_2_8 ?i))) true (ite (bvslt (ite (and (bvslt (rec!210_arg_0_6 ?i) (size!898 (rec!210_arg_1_7 ?i))) (not (bvslt (rec!210_arg_0_6 ?i) (_ bv0 32)))) (select (content!899 (rec!210_arg_1_7 ?i)) (rec!210_arg_0_6 ?i)) error_value!909) (_ bv0 32)) false (rec!210 (bvadd (rec!210_arg_0_6 ?i) (_ bv1 32)) (rec!210_arg_1_7 ?i) (rec!210_arg_2_8 ?i))))) (ite (not (bvslt (rec!210_arg_0_6 ?i) (rec!210_arg_2_8 ?i))) true (ite (bvslt (ite (and (bvslt (rec!210_arg_0_6 ?i) (size!898 (rec!210_arg_1_7 ?i))) (not (bvslt (rec!210_arg_0_6 ?i) (_ bv0 32)))) (select (content!899 (rec!210_arg_1_7 ?i)) (rec!210_arg_0_6 ?i)) error_value!909) (_ bv0 32)) true (not (forall ((?z I_rec!210)) (not (and (= (rec!210_arg_0_6 ?z) (bvadd (rec!210_arg_0_6 ?i) (_ bv1 32))) (= (rec!210_arg_1_7 ?z) (rec!210_arg_1_7 ?i)) (= (rec!210_arg_2_8 ?z) (rec!210_arg_2_8 ?i)))) ))))) ))
(assert (not (forall ((tab!211 array!896)) (or (or (bvslt (size!898 (_1!902 (while0!216 (array!896!897 (size!898 tab!211) arrayconst!910) (_ bv0 32) tab!211))) (_ bv0 32)) (forall ((?z I_while0!216)) (not (and (= (while0!216_arg_0_1 ?z) (array!896!897 (size!898 tab!211) arrayconst!910)) (= (while0!216_arg_1_2 ?z) (_ bv0 32)) (= (while0!216_arg_2_3 ?z) tab!211))) )) (or (isPositive!206 (_1!902 (while0!216 (array!896!897 (size!898 tab!211) arrayconst!910) (_ bv0 32) tab!211)) (size!898 tab!211)) (forall ((?z I_isPositive!206)) (not (and (= (isPositive!206_arg_0_4 ?z) (_1!902 (while0!216 (array!896!897 (size!898 tab!211) arrayconst!910) (_ bv0 32) tab!211))) (= (isPositive!206_arg_1_5 ?z) (size!898 tab!211)))) ) (forall ((?z I_while0!216)) (not (and (= (while0!216_arg_0_1 ?z) (array!896!897 (size!898 tab!211) arrayconst!910)) (= (while0!216_arg_1_2 ?z) (_ bv0 32)) (= (while0!216_arg_2_3 ?z) tab!211))) ))) )))
(check-sat)
