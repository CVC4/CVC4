; COMMAND-LINE: --fmf-fun
; EXPECT: sat
(set-logic ALL_SUPPORTED)
(define-fun-rec f ((x Int)) Bool false)
(assert (not (f 0)))
(check-sat)
