; COMMAND-LINE: --incremental
; EXPECT: unsat
; EXPECT: sat
; EXPECT: unsat
; EXPECT: sat
; EXPECT: unsat
; EXPECT: unsat

(set-logic QF_UFLIA)
(declare-fun _base () Int)
(declare-fun _n () Int)
(assert (let ((.def_5 (<= 0 _n)))
.def_5
))
(declare-fun ___z3z___ (Int) Bool)
(declare-fun ___z4z___ (Int) Bool)
(declare-fun ___z5z___ (Int) Bool)
(declare-fun ___z6z___ (Int) Bool)
(push 1)
(assert (let ((.def_117 (___z3z___ 0)))
(let ((.def_99 (___z4z___ 0)))
(let ((.def_108 (not .def_99)))
(let ((.def_98 (___z5z___ 0)))
(let ((.def_104 (not .def_98)))
(let ((.def_111 (or .def_104 .def_108)))
(let ((.def_101 (___z6z___ 0)))
(let ((.def_112 (or .def_101 .def_111)))
(let ((.def_109 (and .def_98 .def_108)))
(let ((.def_106 (not .def_101)))
(let ((.def_110 (and .def_106 .def_109)))
(let ((.def_113 (or .def_110 .def_112)))
(let ((.def_105 (and .def_99 .def_104)))
(let ((.def_107 (and .def_105 .def_106)))
(let ((.def_114 (or .def_107 .def_113)))
(let ((.def_100 (and .def_98 .def_99)))
(let ((.def_102 (and .def_100 .def_101)))
(let ((.def_115 (or .def_102 .def_114)))
(let ((.def_103 (not .def_102)))
(let ((.def_116 (and .def_103 .def_115)))
(let ((.def_118 (= .def_116 .def_117)))
.def_118
))))))))))))))))))))))
(assert (let ((.def_120 (= _base 0)))
(let ((.def_119 (___z6z___ (- 1))))
(let ((.def_121 (or .def_119 .def_120)))
(let ((.def_99 (___z4z___ 0)))
(let ((.def_122 (= .def_99 .def_121)))
.def_122
))))))
(assert (let ((.def_124 (___z4z___ (- 1))))
(let ((.def_120 (= _base 0)))
(let ((.def_125 (or .def_120 .def_124)))
(let ((.def_123 (not .def_120)))
(let ((.def_126 (and .def_123 .def_125)))
(let ((.def_98 (___z5z___ 0)))
(let ((.def_127 (= .def_98 .def_126)))
.def_127
))))))))
(assert (let ((.def_128 (___z5z___ (- 1))))
(let ((.def_120 (= _base 0)))
(let ((.def_129 (or .def_120 .def_128)))
(let ((.def_123 (not .def_120)))
(let ((.def_130 (and .def_123 .def_129)))
(let ((.def_101 (___z6z___ 0)))
(let ((.def_131 (= .def_101 .def_130)))
.def_131
))))))))
(assert (let ((.def_148 (___z3z___ (- 1))))
(let ((.def_124 (___z4z___ (- 1))))
(let ((.def_139 (not .def_124)))
(let ((.def_128 (___z5z___ (- 1))))
(let ((.def_135 (not .def_128)))
(let ((.def_142 (or .def_135 .def_139)))
(let ((.def_119 (___z6z___ (- 1))))
(let ((.def_143 (or .def_119 .def_142)))
(let ((.def_140 (and .def_128 .def_139)))
(let ((.def_137 (not .def_119)))
(let ((.def_141 (and .def_137 .def_140)))
(let ((.def_144 (or .def_141 .def_143)))
(let ((.def_136 (and .def_124 .def_135)))
(let ((.def_138 (and .def_136 .def_137)))
(let ((.def_145 (or .def_138 .def_144)))
(let ((.def_132 (and .def_124 .def_128)))
(let ((.def_133 (and .def_119 .def_132)))
(let ((.def_146 (or .def_133 .def_145)))
(let ((.def_134 (not .def_133)))
(let ((.def_147 (and .def_134 .def_146)))
(let ((.def_149 (= .def_147 .def_148)))
.def_149
))))))))))))))))))))))
(assert (let ((.def_152 (= _base (- 1))))
(let ((.def_151 (___z6z___ (- 2))))
(let ((.def_153 (or .def_151 .def_152)))
(let ((.def_124 (___z4z___ (- 1))))
(let ((.def_154 (= .def_124 .def_153)))
.def_154
))))))
(assert (let ((.def_156 (___z4z___ (- 2))))
(let ((.def_152 (= _base (- 1))))
(let ((.def_157 (or .def_152 .def_156)))
(let ((.def_155 (not .def_152)))
(let ((.def_158 (and .def_155 .def_157)))
(let ((.def_128 (___z5z___ (- 1))))
(let ((.def_159 (= .def_128 .def_158)))
.def_159
))))))))
(assert (let ((.def_160 (___z5z___ (- 2))))
(let ((.def_152 (= _base (- 1))))
(let ((.def_161 (or .def_152 .def_160)))
(let ((.def_155 (not .def_152)))
(let ((.def_162 (and .def_155 .def_161)))
(let ((.def_119 (___z6z___ (- 1))))
(let ((.def_163 (= .def_119 .def_162)))
.def_163
))))))))
(push 1)
(assert (let ((.def_148 (___z3z___ (- 1))))
(let ((.def_117 (___z3z___ 0)))
(let ((.def_164 (and .def_117 .def_148)))
(let ((.def_152 (= _base (- 1))))
(let ((.def_155 (not .def_152)))
(let ((.def_165 (or .def_155 .def_164)))
(let ((.def_166 (not .def_165)))
.def_166
))))))))
(assert true
)
(check-sat)
(pop 1)
(assert (let ((.def_148 (___z3z___ (- 1))))
.def_148
))
(assert (let ((.def_168 (___z3z___ (- 2))))
.def_168
))
(push 1)
(assert (let ((.def_188 (___z3z___ _n)))
(let ((.def_170 (___z4z___ _n)))
(let ((.def_179 (not .def_170)))
(let ((.def_169 (___z5z___ _n)))
(let ((.def_175 (not .def_169)))
(let ((.def_182 (or .def_175 .def_179)))
(let ((.def_172 (___z6z___ _n)))
(let ((.def_183 (or .def_172 .def_182)))
(let ((.def_180 (and .def_169 .def_179)))
(let ((.def_177 (not .def_172)))
(let ((.def_181 (and .def_177 .def_180)))
(let ((.def_184 (or .def_181 .def_183)))
(let ((.def_176 (and .def_170 .def_175)))
(let ((.def_178 (and .def_176 .def_177)))
(let ((.def_185 (or .def_178 .def_184)))
(let ((.def_171 (and .def_169 .def_170)))
(let ((.def_173 (and .def_171 .def_172)))
(let ((.def_186 (or .def_173 .def_185)))
(let ((.def_174 (not .def_173)))
(let ((.def_187 (and .def_174 .def_186)))
(let ((.def_189 (= .def_187 .def_188)))
.def_189
))))))))))))))))))))))
(assert (let ((.def_192 (= _n _base)))
(let ((.def_190 (+ _n (- 1))))
(let ((.def_191 (___z6z___ .def_190)))
(let ((.def_193 (or .def_191 .def_192)))
(let ((.def_170 (___z4z___ _n)))
(let ((.def_194 (= .def_170 .def_193)))
.def_194
)))))))
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_196 (___z4z___ .def_190)))
(let ((.def_192 (= _n _base)))
(let ((.def_197 (or .def_192 .def_196)))
(let ((.def_195 (not .def_192)))
(let ((.def_198 (and .def_195 .def_197)))
(let ((.def_169 (___z5z___ _n)))
(let ((.def_199 (= .def_169 .def_198)))
.def_199
)))))))))
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_200 (___z5z___ .def_190)))
(let ((.def_192 (= _n _base)))
(let ((.def_201 (or .def_192 .def_200)))
(let ((.def_195 (not .def_192)))
(let ((.def_202 (and .def_195 .def_201)))
(let ((.def_172 (___z6z___ _n)))
(let ((.def_203 (= .def_172 .def_202)))
.def_203
)))))))))
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_220 (___z3z___ .def_190)))
(let ((.def_196 (___z4z___ .def_190)))
(let ((.def_211 (not .def_196)))
(let ((.def_200 (___z5z___ .def_190)))
(let ((.def_207 (not .def_200)))
(let ((.def_214 (or .def_207 .def_211)))
(let ((.def_191 (___z6z___ .def_190)))
(let ((.def_215 (or .def_191 .def_214)))
(let ((.def_212 (and .def_200 .def_211)))
(let ((.def_209 (not .def_191)))
(let ((.def_213 (and .def_209 .def_212)))
(let ((.def_216 (or .def_213 .def_215)))
(let ((.def_208 (and .def_196 .def_207)))
(let ((.def_210 (and .def_208 .def_209)))
(let ((.def_217 (or .def_210 .def_216)))
(let ((.def_204 (and .def_196 .def_200)))
(let ((.def_205 (and .def_191 .def_204)))
(let ((.def_218 (or .def_205 .def_217)))
(let ((.def_206 (not .def_205)))
(let ((.def_219 (and .def_206 .def_218)))
(let ((.def_221 (= .def_219 .def_220)))
.def_221
)))))))))))))))))))))))
(assert (let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_226 (= .def_225 1)))
(let ((.def_190 (+ _n (- 1))))
(let ((.def_222 (+ (- 1) .def_190)))
(let ((.def_223 (___z6z___ .def_222)))
(let ((.def_227 (or .def_223 .def_226)))
(let ((.def_196 (___z4z___ .def_190)))
(let ((.def_228 (= .def_196 .def_227)))
.def_228
))))))))))
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_222 (+ (- 1) .def_190)))
(let ((.def_230 (___z4z___ .def_222)))
(let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_226 (= .def_225 1)))
(let ((.def_231 (or .def_226 .def_230)))
(let ((.def_229 (not .def_226)))
(let ((.def_232 (and .def_229 .def_231)))
(let ((.def_200 (___z5z___ .def_190)))
(let ((.def_233 (= .def_200 .def_232)))
.def_233
))))))))))))
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_222 (+ (- 1) .def_190)))
(let ((.def_234 (___z5z___ .def_222)))
(let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_226 (= .def_225 1)))
(let ((.def_235 (or .def_226 .def_234)))
(let ((.def_229 (not .def_226)))
(let ((.def_236 (and .def_229 .def_235)))
(let ((.def_191 (___z6z___ .def_190)))
(let ((.def_237 (= .def_191 .def_236)))
.def_237
))))))))))))
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_220 (___z3z___ .def_190)))
.def_220
)))
(assert (let ((.def_188 (___z3z___ _n)))
(let ((.def_152 (= _base (- 1))))
(let ((.def_155 (not .def_152)))
(let ((.def_238 (or .def_155 .def_188)))
(let ((.def_239 (not .def_238)))
.def_239
))))))
(assert true
)
(check-sat)
(pop 1)
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_220 (___z3z___ .def_190)))
.def_220
)))
(assert (let ((.def_240 (+ _n (- 2))))
(let ((.def_241 (___z3z___ .def_240)))
.def_241
)))
(assert (let ((.def_156 (___z4z___ (- 2))))
(let ((.def_249 (not .def_156)))
(let ((.def_160 (___z5z___ (- 2))))
(let ((.def_245 (not .def_160)))
(let ((.def_252 (or .def_245 .def_249)))
(let ((.def_151 (___z6z___ (- 2))))
(let ((.def_253 (or .def_151 .def_252)))
(let ((.def_250 (and .def_160 .def_249)))
(let ((.def_247 (not .def_151)))
(let ((.def_251 (and .def_247 .def_250)))
(let ((.def_254 (or .def_251 .def_253)))
(let ((.def_246 (and .def_156 .def_245)))
(let ((.def_248 (and .def_246 .def_247)))
(let ((.def_255 (or .def_248 .def_254)))
(let ((.def_242 (and .def_156 .def_160)))
(let ((.def_243 (and .def_151 .def_242)))
(let ((.def_256 (or .def_243 .def_255)))
(let ((.def_244 (not .def_243)))
(let ((.def_257 (and .def_244 .def_256)))
(let ((.def_168 (___z3z___ (- 2))))
(let ((.def_258 (= .def_168 .def_257)))
.def_258
))))))))))))))))))))))
(assert (let ((.def_261 (= _base (- 2))))
(let ((.def_260 (___z6z___ (- 3))))
(let ((.def_262 (or .def_260 .def_261)))
(let ((.def_156 (___z4z___ (- 2))))
(let ((.def_263 (= .def_156 .def_262)))
.def_263
))))))
(assert (let ((.def_265 (___z4z___ (- 3))))
(let ((.def_261 (= _base (- 2))))
(let ((.def_266 (or .def_261 .def_265)))
(let ((.def_264 (not .def_261)))
(let ((.def_267 (and .def_264 .def_266)))
(let ((.def_160 (___z5z___ (- 2))))
(let ((.def_268 (= .def_160 .def_267)))
.def_268
))))))))
(assert (let ((.def_269 (___z5z___ (- 3))))
(let ((.def_261 (= _base (- 2))))
(let ((.def_270 (or .def_261 .def_269)))
(let ((.def_264 (not .def_261)))
(let ((.def_271 (and .def_264 .def_270)))
(let ((.def_151 (___z6z___ (- 2))))
(let ((.def_272 (= .def_151 .def_271)))
.def_272
))))))))
(assert (let ((.def_188 (___z3z___ _n)))
(let ((.def_170 (___z4z___ _n)))
(let ((.def_179 (not .def_170)))
(let ((.def_169 (___z5z___ _n)))
(let ((.def_175 (not .def_169)))
(let ((.def_182 (or .def_175 .def_179)))
(let ((.def_172 (___z6z___ _n)))
(let ((.def_183 (or .def_172 .def_182)))
(let ((.def_180 (and .def_169 .def_179)))
(let ((.def_177 (not .def_172)))
(let ((.def_181 (and .def_177 .def_180)))
(let ((.def_184 (or .def_181 .def_183)))
(let ((.def_176 (and .def_170 .def_175)))
(let ((.def_178 (and .def_176 .def_177)))
(let ((.def_185 (or .def_178 .def_184)))
(let ((.def_171 (and .def_169 .def_170)))
(let ((.def_173 (and .def_171 .def_172)))
(let ((.def_186 (or .def_173 .def_185)))
(let ((.def_174 (not .def_173)))
(let ((.def_187 (and .def_174 .def_186)))
(let ((.def_189 (= .def_187 .def_188)))
.def_189
))))))))))))))))))))))
(assert (let ((.def_192 (= _n _base)))
(let ((.def_190 (+ _n (- 1))))
(let ((.def_191 (___z6z___ .def_190)))
(let ((.def_193 (or .def_191 .def_192)))
(let ((.def_170 (___z4z___ _n)))
(let ((.def_194 (= .def_170 .def_193)))
.def_194
)))))))
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_196 (___z4z___ .def_190)))
(let ((.def_192 (= _n _base)))
(let ((.def_197 (or .def_192 .def_196)))
(let ((.def_195 (not .def_192)))
(let ((.def_198 (and .def_195 .def_197)))
(let ((.def_169 (___z5z___ _n)))
(let ((.def_199 (= .def_169 .def_198)))
.def_199
)))))))))
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_200 (___z5z___ .def_190)))
(let ((.def_192 (= _n _base)))
(let ((.def_201 (or .def_192 .def_200)))
(let ((.def_195 (not .def_192)))
(let ((.def_202 (and .def_195 .def_201)))
(let ((.def_172 (___z6z___ _n)))
(let ((.def_203 (= .def_172 .def_202)))
.def_203
)))))))))
(push 1)
(assert (let ((.def_261 (= _base (- 2))))
(let ((.def_264 (not .def_261)))
(let ((.def_117 (___z3z___ 0)))
(let ((.def_273 (or .def_117 .def_264)))
(let ((.def_274 (not .def_273)))
.def_274
))))))
(assert true
)
(check-sat)
(pop 1)
(assert (let ((.def_276 (___z3z___ (- 3))))
.def_276
))
(push 1)
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_220 (___z3z___ .def_190)))
(let ((.def_196 (___z4z___ .def_190)))
(let ((.def_211 (not .def_196)))
(let ((.def_200 (___z5z___ .def_190)))
(let ((.def_207 (not .def_200)))
(let ((.def_214 (or .def_207 .def_211)))
(let ((.def_191 (___z6z___ .def_190)))
(let ((.def_215 (or .def_191 .def_214)))
(let ((.def_212 (and .def_200 .def_211)))
(let ((.def_209 (not .def_191)))
(let ((.def_213 (and .def_209 .def_212)))
(let ((.def_216 (or .def_213 .def_215)))
(let ((.def_208 (and .def_196 .def_207)))
(let ((.def_210 (and .def_208 .def_209)))
(let ((.def_217 (or .def_210 .def_216)))
(let ((.def_204 (and .def_196 .def_200)))
(let ((.def_205 (and .def_191 .def_204)))
(let ((.def_218 (or .def_205 .def_217)))
(let ((.def_206 (not .def_205)))
(let ((.def_219 (and .def_206 .def_218)))
(let ((.def_221 (= .def_219 .def_220)))
.def_221
)))))))))))))))))))))))
(assert (let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_226 (= .def_225 1)))
(let ((.def_190 (+ _n (- 1))))
(let ((.def_222 (+ (- 1) .def_190)))
(let ((.def_223 (___z6z___ .def_222)))
(let ((.def_227 (or .def_223 .def_226)))
(let ((.def_196 (___z4z___ .def_190)))
(let ((.def_228 (= .def_196 .def_227)))
.def_228
))))))))))
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_222 (+ (- 1) .def_190)))
(let ((.def_230 (___z4z___ .def_222)))
(let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_226 (= .def_225 1)))
(let ((.def_231 (or .def_226 .def_230)))
(let ((.def_229 (not .def_226)))
(let ((.def_232 (and .def_229 .def_231)))
(let ((.def_200 (___z5z___ .def_190)))
(let ((.def_233 (= .def_200 .def_232)))
.def_233
))))))))))))
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_222 (+ (- 1) .def_190)))
(let ((.def_234 (___z5z___ .def_222)))
(let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_226 (= .def_225 1)))
(let ((.def_235 (or .def_226 .def_234)))
(let ((.def_229 (not .def_226)))
(let ((.def_236 (and .def_229 .def_235)))
(let ((.def_191 (___z6z___ .def_190)))
(let ((.def_237 (= .def_191 .def_236)))
.def_237
))))))))))))
(assert (let ((.def_240 (+ _n (- 2))))
(let ((.def_278 (___z4z___ .def_240)))
(let ((.def_287 (not .def_278)))
(let ((.def_277 (___z5z___ .def_240)))
(let ((.def_283 (not .def_277)))
(let ((.def_290 (or .def_283 .def_287)))
(let ((.def_280 (___z6z___ .def_240)))
(let ((.def_291 (or .def_280 .def_290)))
(let ((.def_288 (and .def_277 .def_287)))
(let ((.def_285 (not .def_280)))
(let ((.def_289 (and .def_285 .def_288)))
(let ((.def_292 (or .def_289 .def_291)))
(let ((.def_284 (and .def_278 .def_283)))
(let ((.def_286 (and .def_284 .def_285)))
(let ((.def_293 (or .def_286 .def_292)))
(let ((.def_279 (and .def_277 .def_278)))
(let ((.def_281 (and .def_279 .def_280)))
(let ((.def_294 (or .def_281 .def_293)))
(let ((.def_282 (not .def_281)))
(let ((.def_295 (and .def_282 .def_294)))
(let ((.def_241 (___z3z___ .def_240)))
(let ((.def_296 (= .def_241 .def_295)))
.def_296
)))))))))))))))))))))))
(assert (let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_299 (= .def_225 2)))
(let ((.def_240 (+ _n (- 2))))
(let ((.def_297 (+ (- 1) .def_240)))
(let ((.def_298 (___z6z___ .def_297)))
(let ((.def_300 (or .def_298 .def_299)))
(let ((.def_278 (___z4z___ .def_240)))
(let ((.def_301 (= .def_278 .def_300)))
.def_301
))))))))))
(assert (let ((.def_240 (+ _n (- 2))))
(let ((.def_297 (+ (- 1) .def_240)))
(let ((.def_303 (___z4z___ .def_297)))
(let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_299 (= .def_225 2)))
(let ((.def_304 (or .def_299 .def_303)))
(let ((.def_302 (not .def_299)))
(let ((.def_305 (and .def_302 .def_304)))
(let ((.def_277 (___z5z___ .def_240)))
(let ((.def_306 (= .def_277 .def_305)))
.def_306
))))))))))))
(assert (let ((.def_240 (+ _n (- 2))))
(let ((.def_297 (+ (- 1) .def_240)))
(let ((.def_307 (___z5z___ .def_297)))
(let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_299 (= .def_225 2)))
(let ((.def_308 (or .def_299 .def_307)))
(let ((.def_302 (not .def_299)))
(let ((.def_309 (and .def_302 .def_308)))
(let ((.def_280 (___z6z___ .def_240)))
(let ((.def_310 (= .def_280 .def_309)))
.def_310
))))))))))))
(assert (let ((.def_261 (= _base (- 2))))
(let ((.def_264 (not .def_261)))
(let ((.def_188 (___z3z___ _n)))
(let ((.def_311 (or .def_188 .def_264)))
(let ((.def_312 (not .def_311)))
.def_312
))))))
(assert true
)
(check-sat)
(pop 1)
(assert (let ((.def_313 (+ _n (- 3))))
(let ((.def_314 (___z3z___ .def_313)))
.def_314
)))
(assert (let ((.def_265 (___z4z___ (- 3))))
(let ((.def_322 (not .def_265)))
(let ((.def_269 (___z5z___ (- 3))))
(let ((.def_318 (not .def_269)))
(let ((.def_325 (or .def_318 .def_322)))
(let ((.def_260 (___z6z___ (- 3))))
(let ((.def_326 (or .def_260 .def_325)))
(let ((.def_323 (and .def_269 .def_322)))
(let ((.def_320 (not .def_260)))
(let ((.def_324 (and .def_320 .def_323)))
(let ((.def_327 (or .def_324 .def_326)))
(let ((.def_319 (and .def_265 .def_318)))
(let ((.def_321 (and .def_319 .def_320)))
(let ((.def_328 (or .def_321 .def_327)))
(let ((.def_315 (and .def_265 .def_269)))
(let ((.def_316 (and .def_260 .def_315)))
(let ((.def_329 (or .def_316 .def_328)))
(let ((.def_317 (not .def_316)))
(let ((.def_330 (and .def_317 .def_329)))
(let ((.def_276 (___z3z___ (- 3))))
(let ((.def_331 (= .def_276 .def_330)))
.def_331
))))))))))))))))))))))
(assert (let ((.def_334 (= _base (- 3))))
(let ((.def_333 (___z6z___ (- 4))))
(let ((.def_335 (or .def_333 .def_334)))
(let ((.def_265 (___z4z___ (- 3))))
(let ((.def_336 (= .def_265 .def_335)))
.def_336
))))))
(assert (let ((.def_338 (___z4z___ (- 4))))
(let ((.def_334 (= _base (- 3))))
(let ((.def_339 (or .def_334 .def_338)))
(let ((.def_337 (not .def_334)))
(let ((.def_340 (and .def_337 .def_339)))
(let ((.def_269 (___z5z___ (- 3))))
(let ((.def_341 (= .def_269 .def_340)))
.def_341
))))))))
(assert (let ((.def_342 (___z5z___ (- 4))))
(let ((.def_334 (= _base (- 3))))
(let ((.def_343 (or .def_334 .def_342)))
(let ((.def_337 (not .def_334)))
(let ((.def_344 (and .def_337 .def_343)))
(let ((.def_260 (___z6z___ (- 3))))
(let ((.def_345 (= .def_260 .def_344)))
.def_345
))))))))
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_220 (___z3z___ .def_190)))
(let ((.def_196 (___z4z___ .def_190)))
(let ((.def_211 (not .def_196)))
(let ((.def_200 (___z5z___ .def_190)))
(let ((.def_207 (not .def_200)))
(let ((.def_214 (or .def_207 .def_211)))
(let ((.def_191 (___z6z___ .def_190)))
(let ((.def_215 (or .def_191 .def_214)))
(let ((.def_212 (and .def_200 .def_211)))
(let ((.def_209 (not .def_191)))
(let ((.def_213 (and .def_209 .def_212)))
(let ((.def_216 (or .def_213 .def_215)))
(let ((.def_208 (and .def_196 .def_207)))
(let ((.def_210 (and .def_208 .def_209)))
(let ((.def_217 (or .def_210 .def_216)))
(let ((.def_204 (and .def_196 .def_200)))
(let ((.def_205 (and .def_191 .def_204)))
(let ((.def_218 (or .def_205 .def_217)))
(let ((.def_206 (not .def_205)))
(let ((.def_219 (and .def_206 .def_218)))
(let ((.def_221 (= .def_219 .def_220)))
.def_221
)))))))))))))))))))))))
(assert (let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_226 (= .def_225 1)))
(let ((.def_190 (+ _n (- 1))))
(let ((.def_222 (+ (- 1) .def_190)))
(let ((.def_223 (___z6z___ .def_222)))
(let ((.def_227 (or .def_223 .def_226)))
(let ((.def_196 (___z4z___ .def_190)))
(let ((.def_228 (= .def_196 .def_227)))
.def_228
))))))))))
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_222 (+ (- 1) .def_190)))
(let ((.def_230 (___z4z___ .def_222)))
(let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_226 (= .def_225 1)))
(let ((.def_231 (or .def_226 .def_230)))
(let ((.def_229 (not .def_226)))
(let ((.def_232 (and .def_229 .def_231)))
(let ((.def_200 (___z5z___ .def_190)))
(let ((.def_233 (= .def_200 .def_232)))
.def_233
))))))))))))
(assert (let ((.def_190 (+ _n (- 1))))
(let ((.def_222 (+ (- 1) .def_190)))
(let ((.def_234 (___z5z___ .def_222)))
(let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_226 (= .def_225 1)))
(let ((.def_235 (or .def_226 .def_234)))
(let ((.def_229 (not .def_226)))
(let ((.def_236 (and .def_229 .def_235)))
(let ((.def_191 (___z6z___ .def_190)))
(let ((.def_237 (= .def_191 .def_236)))
.def_237
))))))))))))
(push 1)
(assert (let ((.def_334 (= _base (- 3))))
(let ((.def_337 (not .def_334)))
(let ((.def_117 (___z3z___ 0)))
(let ((.def_346 (or .def_117 .def_337)))
(let ((.def_347 (not .def_346)))
.def_347
))))))
(assert true
)
(check-sat)
(pop 1)
(assert (let ((.def_349 (___z3z___ (- 4))))
.def_349
))
(push 1)
(assert (let ((.def_240 (+ _n (- 2))))
(let ((.def_278 (___z4z___ .def_240)))
(let ((.def_287 (not .def_278)))
(let ((.def_277 (___z5z___ .def_240)))
(let ((.def_283 (not .def_277)))
(let ((.def_290 (or .def_283 .def_287)))
(let ((.def_280 (___z6z___ .def_240)))
(let ((.def_291 (or .def_280 .def_290)))
(let ((.def_288 (and .def_277 .def_287)))
(let ((.def_285 (not .def_280)))
(let ((.def_289 (and .def_285 .def_288)))
(let ((.def_292 (or .def_289 .def_291)))
(let ((.def_284 (and .def_278 .def_283)))
(let ((.def_286 (and .def_284 .def_285)))
(let ((.def_293 (or .def_286 .def_292)))
(let ((.def_279 (and .def_277 .def_278)))
(let ((.def_281 (and .def_279 .def_280)))
(let ((.def_294 (or .def_281 .def_293)))
(let ((.def_282 (not .def_281)))
(let ((.def_295 (and .def_282 .def_294)))
(let ((.def_241 (___z3z___ .def_240)))
(let ((.def_296 (= .def_241 .def_295)))
.def_296
)))))))))))))))))))))))
(assert (let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_299 (= .def_225 2)))
(let ((.def_240 (+ _n (- 2))))
(let ((.def_297 (+ (- 1) .def_240)))
(let ((.def_298 (___z6z___ .def_297)))
(let ((.def_300 (or .def_298 .def_299)))
(let ((.def_278 (___z4z___ .def_240)))
(let ((.def_301 (= .def_278 .def_300)))
.def_301
))))))))))
(assert (let ((.def_240 (+ _n (- 2))))
(let ((.def_297 (+ (- 1) .def_240)))
(let ((.def_303 (___z4z___ .def_297)))
(let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_299 (= .def_225 2)))
(let ((.def_304 (or .def_299 .def_303)))
(let ((.def_302 (not .def_299)))
(let ((.def_305 (and .def_302 .def_304)))
(let ((.def_277 (___z5z___ .def_240)))
(let ((.def_306 (= .def_277 .def_305)))
.def_306
))))))))))))
(assert (let ((.def_240 (+ _n (- 2))))
(let ((.def_297 (+ (- 1) .def_240)))
(let ((.def_307 (___z5z___ .def_297)))
(let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_299 (= .def_225 2)))
(let ((.def_308 (or .def_299 .def_307)))
(let ((.def_302 (not .def_299)))
(let ((.def_309 (and .def_302 .def_308)))
(let ((.def_280 (___z6z___ .def_240)))
(let ((.def_310 (= .def_280 .def_309)))
.def_310
))))))))))))
(assert (let ((.def_313 (+ _n (- 3))))
(let ((.def_351 (___z4z___ .def_313)))
(let ((.def_360 (not .def_351)))
(let ((.def_350 (___z5z___ .def_313)))
(let ((.def_356 (not .def_350)))
(let ((.def_363 (or .def_356 .def_360)))
(let ((.def_353 (___z6z___ .def_313)))
(let ((.def_364 (or .def_353 .def_363)))
(let ((.def_361 (and .def_350 .def_360)))
(let ((.def_358 (not .def_353)))
(let ((.def_362 (and .def_358 .def_361)))
(let ((.def_365 (or .def_362 .def_364)))
(let ((.def_357 (and .def_351 .def_356)))
(let ((.def_359 (and .def_357 .def_358)))
(let ((.def_366 (or .def_359 .def_365)))
(let ((.def_352 (and .def_350 .def_351)))
(let ((.def_354 (and .def_352 .def_353)))
(let ((.def_367 (or .def_354 .def_366)))
(let ((.def_355 (not .def_354)))
(let ((.def_368 (and .def_355 .def_367)))
(let ((.def_314 (___z3z___ .def_313)))
(let ((.def_369 (= .def_314 .def_368)))
.def_369
)))))))))))))))))))))))
(assert (let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_372 (= .def_225 3)))
(let ((.def_313 (+ _n (- 3))))
(let ((.def_370 (+ (- 1) .def_313)))
(let ((.def_371 (___z6z___ .def_370)))
(let ((.def_373 (or .def_371 .def_372)))
(let ((.def_351 (___z4z___ .def_313)))
(let ((.def_374 (= .def_351 .def_373)))
.def_374
))))))))))
(assert (let ((.def_313 (+ _n (- 3))))
(let ((.def_370 (+ (- 1) .def_313)))
(let ((.def_376 (___z4z___ .def_370)))
(let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_372 (= .def_225 3)))
(let ((.def_377 (or .def_372 .def_376)))
(let ((.def_375 (not .def_372)))
(let ((.def_378 (and .def_375 .def_377)))
(let ((.def_350 (___z5z___ .def_313)))
(let ((.def_379 (= .def_350 .def_378)))
.def_379
))))))))))))
(assert (let ((.def_313 (+ _n (- 3))))
(let ((.def_370 (+ (- 1) .def_313)))
(let ((.def_380 (___z5z___ .def_370)))
(let ((.def_224 (* (- 1) _base)))
(let ((.def_225 (+ _n .def_224)))
(let ((.def_372 (= .def_225 3)))
(let ((.def_381 (or .def_372 .def_380)))
(let ((.def_375 (not .def_372)))
(let ((.def_382 (and .def_375 .def_381)))
(let ((.def_353 (___z6z___ .def_313)))
(let ((.def_383 (= .def_353 .def_382)))
.def_383
))))))))))))
(assert (let ((.def_334 (= _base (- 3))))
(let ((.def_337 (not .def_334)))
(let ((.def_188 (___z3z___ _n)))
(let ((.def_384 (or .def_188 .def_337)))
(let ((.def_385 (not .def_384)))
.def_385
))))))
(assert true
)
(check-sat)
