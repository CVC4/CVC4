(benchmark pred.smt
:status unsat
:logic QF_UF
:category { crafted }

:extrafuns ((x U))
:extrafuns ((y U))
:extrapreds ((f U))



:formula
(and
 (f x)
 (iff (f x) (f y))
 (not (f y))
)
)
