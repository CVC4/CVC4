; COMMAND-LINE: --sort-inference --finite-model-find --no-produce-unsat-cores --no-check-unsat-cores
; EXPECT: unsat
(set-logic ALL)
(assert (not (exists ((X Int)) (not (= X 12)) )))
(check-sat)

