; COMMAND_LINE: --dt-non-simple-rec
; EXPECT: unsat
(set-logic ALL)
(set-info :status unsat)
(declare-datatypes ((E 0)(T 0)) (
((Yes) (No))
((Map (m (Array E E)) ) )
))
(declare-fun a () T)
(declare-fun b () T)
(declare-fun c () T)
(assert (distinct a b c))
(assert (= (select (m a) Yes) (select (m b) Yes)))
(assert (= (select (m b) Yes) (select (m c) Yes)))
(check-sat)
