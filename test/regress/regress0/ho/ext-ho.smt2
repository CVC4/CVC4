; COMMAND-LINE: --hol
; EXPECT: sat
(set-logic ALL)
(set-info :status sat)
(declare-sort U 0)
(declare-fun f ((-> U U)) U)
(declare-fun g ((-> U U)) U)
(declare-fun h ((-> U U)) U)
(declare-fun a () U)
(declare-fun b () U)
(declare-fun c () U)
(assert (distinct f g h))
(check-sat)
