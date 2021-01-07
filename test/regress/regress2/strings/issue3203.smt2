; Temporarily disable checking of unsat cores (see issue #3606)
; COMMAND-LINE: --no-check-unsat-cores  --no-check-unsat-cores-new
(set-logic ALL_SUPPORTED)
(set-option :strings-exp true)
(set-info :status unsat)
(declare-fun a () String)
(declare-fun b () String)
(declare-fun d () String)
(declare-fun e () String)
(declare-fun f () Int)
(declare-fun g () String)
(declare-fun h () String)
(assert (or
            (not (= ( str.replace "B" ( str.at "A" f) "") "B"))
            (not (= ( str.replace "B" ( str.replace "B" g "") "")
                    ( str.at ( str.replace ( str.replace a d "") "C" "") ( str.indexof "B" ( str.replace ( str.replace a d "") "C" "") 0))))))
(assert (= a (str.++ (str.++ d "C") g)))
(assert (= b (str.++ e g)))
(check-sat)
