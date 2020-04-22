; COMMAND-LINE: --finite-model-find
; EXPECT: unsat
; Preamble  --------------
(set-option :produce-models true)
(set-logic ALL_SUPPORTED)
;(declare-datatypes ((x2 0)) (((x1))))
(declare-datatypes ((x5 0)) (((x3) (x4))))
(declare-sort x6 0)
(declare-fun x7 (x6) x5)
(declare-fun x8 () x6)
(assert (not (= x3 (x7 x8))))
(declare-fun x9 () x6)
(assert (not (= x3 (x7 x9))))
(declare-fun x11 () Int)
(declare-sort x12 0)
(declare-fun x13 () x12)
(declare-datatypes ((x17 0)) (((x14) (x15) (x16))))
(declare-datatypes ((x22 0)) (((x21 (x18 Int) (x19 Int) (x20 x5)))))
(declare-datatypes ((x29 0)) (((x28 (x23 x5) (x24 x5) (x25 Int) (x26 Int) (x27 Int)))))
(declare-sort x30 0)
(declare-sort x31 0)
(declare-fun x32 () x31)
(declare-datatypes ((x36 0)) (((x35 (x33 Int) (x34 Int)))))
(declare-fun x37 () x36)
(declare-datatypes ((x45 0)) (((x44 (x38 x5) (x39 x6) (x40 x6) (x41 x6) (x42 x36) (x43 x31)))))
(declare-fun x47 (x12) x31)
(declare-fun x46 (x31) x12)
(declare-datatypes ((x54 0)) (((x49 (x48 x22)) (x51 (x50 x29)) (d53 (s52 x12)))))
(declare-fun x57 (x22) x29)
(declare-fun x56 (x12) x22)
(declare-fun x55 (x29) x22)
(declare-fun x61 () (Array x6 x5))
(declare-fun x66 () (Array x6 x17))
(declare-fun x64 () (Array x6 x54))
(declare-fun x67 () (Array x6 x54))
(declare-fun x65 () (Array x6 x54))
(declare-fun x62 () (Array x30 x45))
(declare-fun x70 () (Array x30 x45))
(declare-fun x68 () (Array x30 x45))
(declare-fun x63 () x30)
(declare-fun x59 (x22) x12)
(declare-fun x60 (x29) x12)
(declare-fun x58 (x12) x29)
(declare-fun x71 () x6)
(declare-fun x69 () x6)
(assert
(not
  (=> (and
    (forall ((x73 x30)) (=> (= x3 (x38 (select x62 x73))) (and (= (select x66 (x40 (select x62 x73))) x15) (= x3 (x7 (x40 (select x62 x73)))) (= (select x61 (x40 (select x62 x73))) x3) (= (x23 (ite ((_ is x49) (select x67 (x40 (select x62 x73)))) (let ((x74 (x48 (select x67 (x40 (select x62 x73)))))) (x57 x74)) (ite ((_ is x51) (select x67 (x40 (select x62 x73)))) (let ((x75 (x50 (select x67 (x40 (select x62 x73)))))) x75) (let ((x76 (s52 (select x67 (x40 (select x62 x73)))))) (x58 x76))))) x3))))
    (forall ((x72 x6)) (=> (and (= x16 (select x66 x72)) (= (x7 x72) x3) (= (select x61 x72) x3)) (= (ite ((_ is d53) (select x67 x72)) x3 x4) x3))))

    (= (ite (= (x38 (select x62 x63)) x3) (ite (and (=> (= (x40 (select x62 x63)) x69) (=> (= (x41 (select x62 x63)) x71) (=> (= x65 (store x67 x71 (d53 (x46 (x43 (select x62 x63)))))) (=> (= x70 (store x62 x63 (let ((x77 (select x62 x63))) (x44 (x38 x77) (x39 x77) (x40 x77) (x41 x77) (x42 x77) x32)))) (=> (= x68 (store x70 x63 (let ((x78 (select x70 x63))) (x44 x4 (x39 x78) (x40 x78) (x41 x78) (x42 x78) (x43 x78)))))
    (=> (= (store x65 x69 (x51 (let ((x82 (ite ((_ is x49) (select x65 x69)) (let ((x79 (x48 (select x65 x69)))) (x57 x79)) (ite ((_ is x51) (select x65 x69)) (let ((x80 (x50 (select x65 x69)))) x80) (let ((x81 (s52 (select x65 x69)))) (x58 x81)))))) (x28 x4 x3 (x25 x82) (x26 x82) (+ (x27 (ite ((_ is x49) (select x65 x69)) (let ((x83 (x48 (select x65 x69)))) (x57 x83)) (ite ((_ is x51) (select x65 x69)) (let ((x84 (x50 (select x65 x69)))) x84) (let ((x85 (s52 (select x65 x69)))) (x58 x85))))) 1))))) x64) (forall ((x86 x6)) (=> (and (= x3 (x7 x86)) (= x3 (select x61 x86)) (= (select x66 x86) x16)) (= (ite ((_ is d53) (select x64 x86)) x3 x4) x3))))))))) (= x3 (x38 (select x62 x63)))) x3 x4) (ite (forall ((x87 x6)) (=> (and (= x3 (select x61 x87)) (= x3 (x7 x87)) (= x16 (select x66 x87))) (= x3 (ite ((_ is d53) (select x67 x87)) x3 x4)))) x3 x4)) x3))))
(check-sat)
