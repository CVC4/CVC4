(set-info :smt-lib-version 2.6)
(set-logic QF_SLIA)
(set-info :source |
Generated by: Quang Loc Le
Generated on: 2018-10-22
Application: Word equations in a decidable fragment
Target solver: Kepler_22
Publication: "A decision procedure for string logic with quadratic equations, regular expressions and length constraints" by Q.L. Le and M. He, APLAS 2018.
|)
(set-info :license "https://creativecommons.org/licenses/by/4.0/")
(set-info :category "crafted")
(set-info :status unsat)

(declare-fun  x1 () String )
(declare-fun  x2 () String )
(declare-fun  z () String )
(declare-fun  t () String )
(assert ( =( str.++( str.++( str.++ x1  "abc"  )  x2  )  z  ) ( str.++( str.++( str.++ x2  "bab"  )  x1  )  t  )  ) )
(check-sat)

(exit)
