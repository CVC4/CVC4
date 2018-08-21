; COMMAND-LINE: --decision=justification
; EXPECT: unsat

(benchmark pp_regfile.smt
:logic QF_AUFLIA
:extrafuns ((REGFILE_INIT Array))
:extrafuns ((BDEST_S2M_INIT Int))
:extrapreds ((CLOCK_INIT))
:status unknown
:formula
(let (?n1 0)
(let (?n2 (select REGFILE_INIT ?n1))
(let (?n3 1)
(let (?n4 (ite CLOCK_INIT ?n3 ?n1))
(flet ($n5 (= ?n1 ?n4))
(let (?n6 (ite $n5 ?n1 ?n3))
(flet ($n7 (= ?n3 ?n6))
(flet ($n8 (= ?n1 BDEST_S2M_INIT))
(flet ($n9 false)
(flet ($n10 true)
(flet ($n11 (if_then_else $n8 $n9 $n10))
(let (?n12 (store REGFILE_INIT BDEST_S2M_INIT ?n1))
(let (?n13 (ite $n11 ?n12 REGFILE_INIT))
(let (?n14 (select ?n13 ?n4))
(let (?n15 (ite $n7 ?n1 ?n14))
(flet ($n16 (= ?n1 ?n15))
(flet ($n17 (if_then_else $n16 $n9 $n10))
(flet ($n18 (if_then_else CLOCK_INIT $n9 $n10))
(let (?n19 (store ?n13 ?n3 ?n1))
(let (?n20 (ite $n18 ?n19 ?n13))
(let (?n21 (store ?n20 ?n3 ?n1))
(let (?n22 (ite $n17 ?n20 ?n21))
(let (?n23 (select ?n22 ?n1))
(flet ($n24 (= ?n2 ?n23))
(flet ($n25 (not $n24))
$n25
))))))))))))))))))))))))))
