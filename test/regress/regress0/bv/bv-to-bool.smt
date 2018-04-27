; COMMAND-LINE: --bv-to-bool
; EXPECT: sat
(benchmark fuzzsmt
:logic QF_BV
:status sat
:extrafuns ((v0 BitVec[16]))
:extrafuns ((v1 BitVec[2]))
:extrafuns ((v2 BitVec[11]))
:extrafuns ((v3 BitVec[5]))
:extrafuns ((v4 BitVec[15]))
:formula
(let (?e5 bv0[1])
(let (?e6 (ite (bvult v4 (sign_extend[13] v1)) bv1[1] bv0[1]))
(let (?e7 (bvadd (sign_extend[9] v1) v2))
(let (?e8 (bvcomp v4 v4))
(let (?e9 (bvadd ?e7 (zero_extend[10] ?e6)))
(let (?e10 (bvand v0 (sign_extend[11] v3)))
(let (?e11 (ite (bvsge (zero_extend[11] v3) v0) bv1[1] bv0[1]))
(let (?e12 (ite (bvsge (zero_extend[9] v1) ?e9) bv1[1] bv0[1]))
(let (?e13 (repeat[1] v0))
(let (?e14 (bvshl ?e6 ?e12))
(let (?e15 (ite (= bv1[1] (extract[0:0] v0)) ?e9 (zero_extend[10] ?e6)))
(let (?e16 (ite (bvsle (sign_extend[9] v1) v2) bv1[1] bv0[1]))
(let (?e17 (ite (bvsge v4 (zero_extend[14] ?e6)) bv1[1] bv0[1]))
(let (?e18 (bvcomp (sign_extend[10] ?e6) ?e9))
(let (?e19 (ite (bvsle ?e15 ?e15) bv1[1] bv0[1]))
(let (?e20 (ite (bvule ?e10 (zero_extend[15] ?e5)) bv1[1] bv0[1]))
(flet ($e21 (= (zero_extend[10] ?e18) ?e9))
(flet ($e22 (= ?e7 ?e7))
(flet ($e23 (= ?e17 ?e6))
(flet ($e24 (= (zero_extend[15] ?e17) ?e10))
(flet ($e25 (= (zero_extend[10] ?e16) ?e7))
(flet ($e26 (= (sign_extend[13] v1) v4))
(flet ($e27 (= (sign_extend[15] ?e16) v0))
(flet ($e28 (= (sign_extend[15] ?e18) ?e10))
(flet ($e29 (= ?e7 (sign_extend[10] ?e18)))
(flet ($e30 (= ?e9 (sign_extend[9] v1)))
(flet ($e31 (= ?e11 ?e18))
(flet ($e32 (= (sign_extend[15] ?e20) ?e10))
(flet ($e33 (= ?e18 ?e8))
(flet ($e34 (= ?e14 ?e6))
(flet ($e35 (= (zero_extend[15] ?e20) v0))
(flet ($e36 (= v4 (sign_extend[14] ?e11)))
(flet ($e37 (= (sign_extend[1] v4) ?e13))
(flet ($e38 (= ?e20 ?e16))
(flet ($e39 (= v1 (sign_extend[1] ?e14)))
(flet ($e40 (= ?e5 ?e19))
(flet ($e41 (= ?e7 (sign_extend[10] ?e14)))
(flet ($e42 (= ?e15 (sign_extend[6] v3)))
(flet ($e43 (= ?e18 ?e18))
(flet ($e44 (= ?e16 ?e8))
(flet ($e45 (= (sign_extend[15] ?e8) v0))
(flet ($e46 (= (zero_extend[4] ?e15) v4))
(flet ($e47 (= (sign_extend[14] ?e20) v4))
(flet ($e48 (= v3 (sign_extend[4] ?e17)))
(flet ($e49 (= ?e17 ?e6))
(flet ($e50 (= ?e10 (sign_extend[15] ?e16)))
(flet ($e51 (= ?e16 ?e18))
(flet ($e52 (= (sign_extend[10] ?e12) ?e9))
(flet ($e53 (= ?e8 ?e19))
(flet ($e54 (= (zero_extend[1] ?e14) v1))
(flet ($e55 (= v1 (sign_extend[1] ?e6)))
(flet ($e56 (= v4 (zero_extend[14] ?e14)))
(flet ($e57 (= ?e17 ?e20))
(flet ($e58 (= ?e20 ?e11))
(flet ($e59 (= (zero_extend[4] ?e6) v3))
(flet ($e60 (= v0 (zero_extend[5] ?e9)))
(flet ($e61 (= v0 (sign_extend[15] ?e17)))
(flet ($e62 (= ?e15 ?e9))
(flet ($e63 (= (sign_extend[4] ?e15) v4))
(flet ($e64 (= (zero_extend[10] ?e16) ?e15))
(flet ($e65 (= v4 (zero_extend[14] ?e18)))
(flet ($e66 (= (sign_extend[10] ?e14) ?e9))
(flet ($e67 (= ?e20 ?e17))
(flet ($e68 (= ?e14 ?e18))
(flet ($e69 (= ?e10 (sign_extend[5] ?e9)))
(flet ($e70 (= ?e5 ?e16))
(flet ($e71 (= (zero_extend[10] ?e19) ?e15))
(flet ($e72 (= ?e15 ?e9))
(flet ($e73 (= ?e12 ?e11))
(flet ($e74 (= (sign_extend[10] ?e14) ?e7))
(flet ($e75 (= ?e20 ?e20))
(flet ($e76 (= ?e12 ?e18))
(flet ($e77 (= ?e20 ?e16))
(flet ($e78 (= ?e17 ?e16))
(flet ($e79 (= (zero_extend[14] ?e17) v4))
(flet ($e80 (= ?e7 (sign_extend[10] ?e8)))
(flet ($e81 (= ?e11 ?e20))
(flet ($e82 (= ?e9 (sign_extend[10] ?e8)))
(flet ($e83 (= v0 (zero_extend[15] ?e18)))
(flet ($e84 (= ?e17 ?e12))
(flet ($e85 (= (zero_extend[4] ?e18) v3))
(flet ($e86 (= v1 (sign_extend[1] ?e5)))
(flet ($e87 (= ?e14 ?e5))
(flet ($e88 (= ?e13 (zero_extend[15] ?e14)))
(flet ($e89 (= ?e19 ?e16))
(flet ($e90 (= ?e20 ?e17))
(flet ($e91 (= ?e15 v2))
(flet ($e92 (or $e72 $e38))
(flet ($e93 (if_then_else $e58 $e65 $e60))
(flet ($e94 (not $e71))
(flet ($e95 (and $e75 $e63))
(flet ($e96 (and $e82 $e53))
(flet ($e97 (iff $e22 $e59))
(flet ($e98 (if_then_else $e96 $e41 $e29))
(flet ($e99 (not $e46))
(flet ($e100 (not $e39))
(flet ($e101 (not $e62))
(flet ($e102 (iff $e91 $e83))
(flet ($e103 (implies $e51 $e61))
(flet ($e104 (not $e33))
(flet ($e105 (xor $e84 $e45))
(flet ($e106 (implies $e54 $e50))
(flet ($e107 (iff $e40 $e57))
(flet ($e108 (xor $e30 $e89))
(flet ($e109 (implies $e68 $e103))
(flet ($e110 (if_then_else $e101 $e52 $e99))
(flet ($e111 (or $e80 $e110))
(flet ($e112 (iff $e108 $e88))
(flet ($e113 (xor $e86 $e78))
(flet ($e114 (not $e48))
(flet ($e115 (if_then_else $e67 $e92 $e49))
(flet ($e116 (implies $e77 $e93))
(flet ($e117 (and $e26 $e25))
(flet ($e118 (or $e47 $e117))
(flet ($e119 (or $e87 $e21))
(flet ($e120 (not $e64))
(flet ($e121 (not $e119))
(flet ($e122 (and $e106 $e118))
(flet ($e123 (or $e114 $e43))
(flet ($e124 (implies $e100 $e74))
(flet ($e125 (iff $e123 $e109))
(flet ($e126 (iff $e23 $e37))
(flet ($e127 (not $e121))
(flet ($e128 (and $e70 $e98))
(flet ($e129 (if_then_else $e76 $e90 $e122))
(flet ($e130 (iff $e81 $e111))
(flet ($e131 (implies $e24 $e24))
(flet ($e132 (iff $e130 $e42))
(flet ($e133 (if_then_else $e79 $e34 $e94))
(flet ($e134 (implies $e102 $e56))
(flet ($e135 (or $e66 $e27))
(flet ($e136 (and $e131 $e55))
(flet ($e137 (iff $e105 $e120))
(flet ($e138 (if_then_else $e129 $e85 $e32))
(flet ($e139 (xor $e44 $e132))
(flet ($e140 (xor $e133 $e139))
(flet ($e141 (and $e134 $e128))
(flet ($e142 (or $e127 $e113))
(flet ($e143 (implies $e136 $e136))
(flet ($e144 (iff $e143 $e36))
(flet ($e145 (not $e144))
(flet ($e146 (if_then_else $e35 $e137 $e142))
(flet ($e147 (if_then_else $e116 $e126 $e112))
(flet ($e148 (and $e141 $e97))
(flet ($e149 (implies $e146 $e115))
(flet ($e150 (not $e140))
(flet ($e151 (and $e150 $e95))
(flet ($e152 (if_then_else $e147 $e138 $e147))
(flet ($e153 (or $e135 $e31))
(flet ($e154 (iff $e148 $e73))
(flet ($e155 (or $e152 $e69))
(flet ($e156 (not $e107))
(flet ($e157 (if_then_else $e149 $e28 $e104))
(flet ($e158 (iff $e157 $e124))
(flet ($e159 (iff $e125 $e151))
(flet ($e160 (if_then_else $e154 $e159 $e145))
(flet ($e161 (iff $e155 $e155))
(flet ($e162 (iff $e160 $e160))
(flet ($e163 (iff $e158 $e156))
(flet ($e164 (iff $e162 $e162))
(flet ($e165 (and $e163 $e161))
(flet ($e166 (xor $e164 $e165))
(flet ($e167 (or $e166 $e166))
(flet ($e168 (or $e167 $e167))
(flet ($e169 (iff $e153 $e153))
(flet ($e170 (or $e168 $e168))
(flet ($e171 (or $e169 $e169))
(flet ($e172 (not $e171))
(flet ($e173 (implies $e170 $e170))
(flet ($e174 (not $e172))
(flet ($e175 (iff $e173 $e174))
$e175
))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))

