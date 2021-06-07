; COMMAND-LINE: --hol --sort-inference
; EXPECT: sat
(set-logic ALL)
(set-option :sort-inference true)
(set-info :status sat)
(declare-fun a ( Int ) Int)
(declare-fun b ( Int ) Int)
(assert (and (distinct 0 (b 5)) (distinct a b )))
(check-sat)
