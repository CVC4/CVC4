; COMMAND-LINE: --nl-ext
; EXPECT: sat
(set-logic ALL)
(assert (or (< 60.3 (exp 4.1) 60.4) (< (exp 5.1) 164.1)))
(check-sat)
