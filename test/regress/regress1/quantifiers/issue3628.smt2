; COMMAND-LINE: --no-check-models
; EXPECT: sat
(set-logic NRA)
(declare-fun a () Real)
(declare-fun b () Real)
(declare-fun c () Real)
(declare-fun d () Real)
(declare-fun e () Real)
(assert (let ((f (exists ((g Real))
             (= (< 0.0 (* b d)) (distinct (* (to_real 1) a c) 1.0)))))
  (not f)))
(assert (= a (* c e)))
(check-sat)
