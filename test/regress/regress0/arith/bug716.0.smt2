; COMMAND-LINE: --lang=smt2.5
; SCRUBBER: sed -e 's/The fact in question: .*$/The fact in question: TERM/' -e 's/in a linear logic: .*$/in a linear logic: TERM/'
; EXPECT: (error "A non-linear fact was asserted to arithmetic in a linear logic.
; EXPECT: The fact in question: TERM
; EXPECT: ")
; EXIT: 1
(set-logic AUFBVDTLIRA)
;; produced by cvc4_15.drv ;;
(set-info :source |VC generated by SPARK 2014|)
(set-info :smt-lib-version 2.0)
(set-info :category industrial)
(set-info :status unknown)
;;; generated by SMT-LIB2 driver
;;; SMT-LIB2 driver: bit-vectors, common part
(set-option :produce-models true)
;;; SMT-LIB2: integer arithmetic
;;; SMT-LIB2: real arithmetic
(declare-datatypes () ((tuple0 (Tuple0))))
(declare-sort us_private 0)

(declare-fun us_null_ext__ () us_private)

(declare-sort us_type_of_heap 0)

(declare-datatypes ()
((us_type_of_heap__ref
 (mk___type_of_heap__ref (us_type_of_heap__content us_type_of_heap)))))
(declare-sort us_image 0)

(declare-datatypes () ((int__ref (mk_int__ref (int__content Int)))))
(declare-datatypes () ((bool__ref (mk_bool__ref (bool__content Bool)))))
(declare-datatypes () ((real__ref (mk_real__ref (real__content Real)))))
(declare-datatypes ()
((us_private__ref (mk___private__ref (us_private__content us_private)))))
(define-fun int__ref___projection ((a int__ref)) Int (int__content a))

(define-fun bool__ref___projection ((a bool__ref)) Bool (bool__content a))

(define-fun real__ref___projection ((a real__ref)) Real (real__content a))

(define-fun us_private__ref___projection ((a us_private__ref)) us_private
  (us_private__content a))

(declare-fun us_compatible_tags (Int Int) Bool)

;; __compatible_tags_refl
  (assert (forall ((tag Int)) (us_compatible_tags tag tag)))

(define-fun to_int1 ((b Bool)) Int (ite (= b true) 1 0))

(define-fun of_int ((i Int)) Bool (ite (= i 0) false true))

(define-fun in_range ((x Int)) Bool (or (= x 0) (= x 1)))

(declare-fun attr__ATTRIBUTE_IMAGE (Bool) us_image)

(declare-fun attr__ATTRIBUTE_VALUE__pre_check (us_image) Bool)

(declare-fun attr__ATTRIBUTE_VALUE (us_image) Bool)

(declare-fun power (Real Int) Real)

;; Power_0
  (assert (forall ((x Real)) (= (power x 0) 1.0)))

;; Power_s
  (assert
  (forall ((x Real) (n Int))
  (=> (<= 0 n) (= (power x (+ n 1)) (* x (power x n))))))

;; Power_s_alt
  (assert
  (forall ((x Real) (n Int))
  (=> (< 0 n) (= (power x n) (* x (power x (- n 1)))))))

;; Power_1
  (assert (forall ((x Real)) (= (power x 1) x)))

;; Power_sum
  (assert
  (forall ((x Real) (n Int) (m Int))
  (=> (<= 0 n)
  (=> (<= 0 m) (= (power x (+ n m)) (* (power x n) (power x m)))))))

;; Power_mult
  (assert
  (forall ((x Real) (n Int) (m Int))
  (=> (<= 0 n) (=> (<= 0 m) (= (power x (* n m)) (power (power x n) m))))))

;; Power_mult2
  (assert
  (forall ((x Real) (y Real) (n Int))
  (=> (<= 0 n) (= (power (* x y) n) (* (power x n) (power y n))))))

;; Pow_ge_one
  (assert
  (forall ((x Real) (n Int))
  (=> (and (<= 0 n) (<= 1.0 x)) (<= 1.0 (power x n)))))

(declare-datatypes ()
((mode (NearestTiesToEven) (ToZero) (Up) (Down) (NearestTiesToAway))))
(declare-sort single 0)

(declare-fun round (mode Real) Real)

(declare-fun value (single) Real)

(declare-fun exact (single) Real)

(declare-fun model (single) Real)

(define-fun round_error ((x single)) Real (ite (>= (- (value x) (exact x)) 0.0) (-
  (value x) (exact x)) (- (- (value x) (exact x)))))

(define-fun total_error ((x single)) Real (ite (>= (- (value x) (model x)) 0.0) (-
  (value x) (model x)) (- (- (value x) (model x)))))

(define-fun no_overflow ((m mode)
  (x Real)) Bool (<= (ite (>= (round m x) 0.0) (round m x) (- (round m x))) 340282346638528859811704183484516925440.0))

;; Bounded_real_no_overflow
  (assert
  (forall ((m mode) (x Real))
  (=> (<= (ite (>= x 0.0) x (- x)) 340282346638528859811704183484516925440.0)
  (no_overflow m x))))

;; Round_monotonic
  (assert
  (forall ((m mode) (x Real) (y Real))
  (=> (<= x y) (<= (round m x) (round m y)))))

;; Round_idempotent
  (assert
  (forall ((m1 mode) (m2 mode) (x Real))
  (= (round m1 (round m2 x)) (round m2 x))))

;; Round_value
  (assert (forall ((m mode) (x single)) (= (round m (value x)) (value x))))

;; Bounded_value
  (assert
  (forall ((x single))
  (<= (ite (>= (value x) 0.0) (value x) (- (value x))) 340282346638528859811704183484516925440.0)))

;; Exact_rounding_for_integers
  (assert
  (forall ((m mode) (i Int))
  (=> (and (<= (- 16777216) i) (<= i 16777216))
  (= (round m (to_real i)) (to_real i)))))

;; Round_down_le
  (assert (forall ((x Real)) (<= (round Down x) x)))

;; Round_up_ge
  (assert (forall ((x Real)) (<= x (round Up x))))

;; Round_down_neg
  (assert (forall ((x Real)) (= (round Down (- x)) (- (round Up x)))))

;; Round_up_neg
  (assert (forall ((x Real)) (= (round Up (- x)) (- (round Down x)))))

(declare-fun round_logic (mode Real) single)

;; Round_logic_def
  (assert
  (forall ((m mode) (x Real))
  (=> (no_overflow m x) (= (value (round_logic m x)) (round m x)))))

(define-fun of_real_post ((m mode) (x Real)
  (res single)) Bool (and (= (value res) (round m x))
                     (and (= (exact res) x) (= (model res) x))))

(define-fun add_post ((m mode) (x single) (y single)
  (res single)) Bool (and (= (value res) (round m (+ (value x) (value y))))
                     (and (= (exact res) (+ (exact x) (exact y)))
                     (= (model res) (+ (model x) (model y))))))

(define-fun sub_post ((m mode) (x single) (y single)
  (res single)) Bool (and (= (value res) (round m (- (value x) (value y))))
                     (and (= (exact res) (- (exact x) (exact y)))
                     (= (model res) (- (model x) (model y))))))

(define-fun mul_post ((m mode) (x single) (y single)
  (res single)) Bool (and (= (value res) (round m (* (value x) (value y))))
                     (and (= (exact res) (* (exact x) (exact y)))
                     (= (model res) (* (model x) (model y))))))

(define-fun div_post ((m mode) (x single) (y single)
  (res single)) Bool (and (= (value res) (round m (/ (value x) (value y))))
                     (and (= (exact res) (/ (exact x) (exact y)))
                     (= (model res) (/ (model x) (model y))))))

(define-fun neg_post ((x single)
  (res single)) Bool (and (= (value res) (- (value x)))
                     (and (= (exact res) (- (exact x)))
                     (= (model res) (- (model x))))))

(define-fun lt ((x single) (y single)) Bool (< (value x) (value y)))

(define-fun gt ((x single) (y single)) Bool (< (value y) (value x)))

(declare-sort double 0)

(declare-fun round1 (mode Real) Real)

(declare-fun value1 (double) Real)

(declare-fun exact1 (double) Real)

(declare-fun model1 (double) Real)

(define-fun round_error1 ((x double)) Real (ite (>= (- (value1 x) (exact1 x)) 0.0) (-
  (value1 x) (exact1 x)) (- (- (value1 x) (exact1 x)))))

(define-fun total_error1 ((x double)) Real (ite (>= (- (value1 x) (model1 x)) 0.0) (-
  (value1 x) (model1 x)) (- (- (value1 x) (model1 x)))))

(define-fun no_overflow1 ((m mode)
  (x Real)) Bool (<= (ite (>= (round1 m x) 0.0) (round1 m x) (- (round1 m x))) 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0))

;; Bounded_real_no_overflow
  (assert
  (forall ((m mode) (x Real))
  (=>
  (<= (ite (>= x 0.0) x (- x)) 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0)
  (no_overflow1 m x))))

;; Round_monotonic
  (assert
  (forall ((m mode) (x Real) (y Real))
  (=> (<= x y) (<= (round1 m x) (round1 m y)))))

;; Round_idempotent
  (assert
  (forall ((m1 mode) (m2 mode) (x Real))
  (= (round1 m1 (round1 m2 x)) (round1 m2 x))))

;; Round_value
  (assert
  (forall ((m mode) (x double)) (= (round1 m (value1 x)) (value1 x))))

;; Bounded_value
  (assert
  (forall ((x double))
  (<= (ite (>= (value1 x) 0.0) (value1 x) (- (value1 x))) 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0)))

;; Exact_rounding_for_integers
  (assert
  (forall ((m mode) (i Int))
  (=> (and (<= (- 9007199254740992) i) (<= i 9007199254740992))
  (= (round1 m (to_real i)) (to_real i)))))

;; Round_down_le
  (assert (forall ((x Real)) (<= (round1 Down x) x)))

;; Round_up_ge
  (assert (forall ((x Real)) (<= x (round1 Up x))))

;; Round_down_neg
  (assert (forall ((x Real)) (= (round1 Down (- x)) (- (round1 Up x)))))

;; Round_up_neg
  (assert (forall ((x Real)) (= (round1 Up (- x)) (- (round1 Down x)))))

(declare-fun round_logic1 (mode Real) double)

;; Round_logic_def
  (assert
  (forall ((m mode) (x Real))
  (=> (no_overflow1 m x) (= (value1 (round_logic1 m x)) (round1 m x)))))

(define-fun of_real_post1 ((m mode) (x Real)
  (res double)) Bool (and (= (value1 res) (round1 m x))
                     (and (= (exact1 res) x) (= (model1 res) x))))

(define-fun add_post1 ((m mode) (x double) (y double)
  (res double)) Bool (and
                     (= (value1 res) (round1 m (+ (value1 x) (value1 y))))
                     (and (= (exact1 res) (+ (exact1 x) (exact1 y)))
                     (= (model1 res) (+ (model1 x) (model1 y))))))

(define-fun sub_post1 ((m mode) (x double) (y double)
  (res double)) Bool (and
                     (= (value1 res) (round1 m (- (value1 x) (value1 y))))
                     (and (= (exact1 res) (- (exact1 x) (exact1 y)))
                     (= (model1 res) (- (model1 x) (model1 y))))))

(define-fun mul_post1 ((m mode) (x double) (y double)
  (res double)) Bool (and
                     (= (value1 res) (round1 m (* (value1 x) (value1 y))))
                     (and (= (exact1 res) (* (exact1 x) (exact1 y)))
                     (= (model1 res) (* (model1 x) (model1 y))))))

(define-fun div_post1 ((m mode) (x double) (y double)
  (res double)) Bool (and
                     (= (value1 res) (round1 m (/ (value1 x) (value1 y))))
                     (and (= (exact1 res) (/ (exact1 x) (exact1 y)))
                     (= (model1 res) (/ (model1 x) (model1 y))))))

(define-fun neg_post1 ((x double)
  (res double)) Bool (and (= (value1 res) (- (value1 x)))
                     (and (= (exact1 res) (- (exact1 x)))
                     (= (model1 res) (- (model1 x))))))

(define-fun lt1 ((x double) (y double)) Bool (< (value1 x) (value1 y)))

(define-fun gt1 ((x double) (y double)) Bool (< (value1 y) (value1 x)))

;; round_single_bound
  (assert
  (forall ((x Real))
  (! (and
     (<= (- (- x (* (/ 1.0 16777216.0) (ite (>= x 0.0) x (- x)))) (/ 1.0 1427247692705959881058285969449495136382746624.0))
     (round NearestTiesToEven x))
     (<= (round NearestTiesToEven x) (+ (+ x (* (/ 1.0 16777216.0) (ite (>= x 0.0) x (- x)))) (/ 1.0 1427247692705959881058285969449495136382746624.0)))) :pattern (
  (round NearestTiesToEven x)) )))

;; round_double_bound
  (assert
  (forall ((x Real))
  (! (and
     (<= (- (- x (* (/ 1.0 9007199254740992.0) (ite (>= x 0.0) x (- x)))) (/ 1.0 404804506614621236704990693437834614099113299528284236713802716054860679135990693783920767402874248990374155728633623822779617474771586953734026799881477019843034848553132722728933815484186432682479535356945490137124014966849385397236206711298319112681620113024717539104666829230461005064372655017292012526615415482186989568.0))
     (round1 NearestTiesToEven x))
     (<= (round1 NearestTiesToEven x) (+ (+ x (* (/ 1.0 9007199254740992.0) (ite (>= x 0.0) x (- x)))) (/ 1.0 404804506614621236704990693437834614099113299528284236713802716054860679135990693783920767402874248990374155728633623822779617474771586953734026799881477019843034848553132722728933815484186432682479535356945490137124014966849385397236206711298319112681620113024717539104666829230461005064372655017292012526615415482186989568.0)))) :pattern (
  (round1 NearestTiesToEven x)) )))

;; round_double_single
  (assert
  (forall ((x Real))
  (! (= (round1 NearestTiesToEven (round NearestTiesToEven x)) (round
                                                               NearestTiesToEven
                                                               x)) :pattern (
  (round NearestTiesToEven x)) )))

(declare-fun round2 (Real) Int)

;; Round_down
  (assert
  (forall ((x Real))
  (=> (< (- x (to_real (to_int x))) (/ 5.0 10.0)) (= (round2 x) (to_int x)))))

;; Round_up
  (assert
  (forall ((x Real))
  (=> (< (- (to_real (- 1 (to_int (- 1.0 x)))) x) (/ 5.0 10.0))
  (= (round2 x) (- 1 (to_int (- 1.0 x)))))))

;; Round_neg_tie
  (assert
  (forall ((x Real))
  (=> (and (= (- x (to_real (to_int x))) (/ 5.0 10.0)) (< x 0.0))
  (= (round2 x) (to_int x)))))

;; Round_pos_tie
  (assert
  (forall ((x Real))
  (=>
  (and (= (- (to_real (- 1 (to_int (- 1.0 x)))) x) (/ 5.0 10.0)) (< 0.0 x))
  (= (round2 x) (- 1 (to_int (- 1.0 x)))))))

;; Round_int
  (assert
  (forall ((i Int))
  (! (= (round2 (to_real i)) i) :pattern ((round2 (to_real i))) )))

;; Round_near_int
  (assert
  (forall ((i Int))
  (forall ((x Real))
  (=> (and (< (- (/ 5.0 10.0)) x) (< x (/ 5.0 10.0)))
  (= (round2 (+ (to_real i) x)) i)))))

;; Round_monotonic
  (assert
  (forall ((x Real) (y Real)) (=> (<= x y) (<= (round2 x) (round2 y)))))

;; Round_monotonic_int1
  (assert
  (forall ((x Real) (i Int)) (=> (<= x (to_real i)) (<= (round2 x) i))))

;; Round_monotonic_int2
  (assert
  (forall ((x Real) (i Int)) (=> (<= (to_real i) x) (<= i (round2 x)))))

;; Round_bound
  (assert
  (forall ((x Real))
  (and (<= (- x (/ 5.0 10.0)) (to_real (round2 x)))
  (<= (to_real (round2 x)) (+ x (/ 5.0 10.0))))))

(declare-fun remainder (Real Real) Real)

(declare-sort float 0)

(define-fun in_range1 ((x Real)) Bool (and
                                      (<= (- 340282346638528859811704183484516925440.0) x)
                                      (<= x 340282346638528859811704183484516925440.0)))

(declare-fun to_real1 (float) Real)

(declare-fun of_real (Real) float)

(declare-fun user_eq (float float) Bool)

(declare-fun next_representable (Real) Real)

(declare-fun prev_representable (Real) Real)

;; next_representable_def
  (assert
  (forall ((x Real))
  (! (< x (next_representable x)) :pattern ((next_representable x)) )))

;; prev_representable_def
  (assert
  (forall ((x Real))
  (! (< (prev_representable x) x) :pattern ((prev_representable x)) )))

(declare-fun attr__ATTRIBUTE_IMAGE1 (Real) us_image)

(declare-fun attr__ATTRIBUTE_VALUE__pre_check1 (us_image) Bool)

(declare-fun attr__ATTRIBUTE_VALUE1 (us_image) Real)

(declare-fun dummy () float)

;; inversion_axiom
  (assert
  (forall ((x float))
  (! (= (of_real (to_real1 x)) x) :pattern ((to_real1 x)) )))

;; representable_first
  (assert
  (= (round NearestTiesToEven (- 340282346638528859811704183484516925440.0)) (- 340282346638528859811704183484516925440.0)))

;; representable_last
  (assert
  (= (round NearestTiesToEven 340282346638528859811704183484516925440.0) 340282346638528859811704183484516925440.0))

;; range_axiom
  (assert (forall ((x float)) (in_range1 (to_real1 x))))

(declare-datatypes () ((float__ref (mk_float__ref (float__content float)))))
(define-fun float__ref___projection ((a float__ref)) float (float__content a))

(declare-sort weapon_kind 0)

(define-fun in_range2 ((x Int)) Bool (and (<= 0 x) (<= x 2)))

(define-fun bool_eq ((x Int) (y Int)) Bool (ite (= x y) true false))

(declare-fun attr__ATTRIBUTE_IMAGE2 (Int) us_image)

(declare-fun attr__ATTRIBUTE_VALUE__pre_check2 (us_image) Bool)

(declare-fun attr__ATTRIBUTE_VALUE2 (us_image) Int)

(declare-fun to_rep (weapon_kind) Int)

(declare-fun of_rep (Int) weapon_kind)

(declare-fun user_eq1 (weapon_kind weapon_kind) Bool)

(declare-fun dummy1 () weapon_kind)

;; inversion_axiom
  (assert
  (forall ((x weapon_kind))
  (! (= (of_rep (to_rep x)) x) :pattern ((to_rep x)) )))

;; range_axiom
  (assert
  (forall ((x weapon_kind)) (! (in_range2
  (to_rep x)) :pattern ((to_rep x)) )))

;; coerce_axiom
  (assert
  (forall ((x Int))
  (! (=> (in_range2 x) (= (to_rep (of_rep x)) x)) :pattern ((to_rep
                                                            (of_rep x))) )))

(declare-datatypes ()
((weapon_kind__ref (mk_weapon_kind__ref (weapon_kind__content weapon_kind)))))
(define-fun weapon_kind__ref___projection ((a weapon_kind__ref)) weapon_kind
  (weapon_kind__content a))

(declare-fun dps (Int) float)

(declare-fun damage (Int) Int)

(declare-fun bullet (Int) Int)

(declare-fun cooldown (Int) Int)

;; dps__post_axiom
  (assert true)

;; dps__def_axiom
  (assert
  (forall ((self Int))
  (! (=> (in_range2 self)
     (= (to_real1 (dps self)) (round NearestTiesToEven
                              (/ (round NearestTiesToEven
                                 (to_real (damage (bullet self)))) (round
                                                                   NearestTiesToEven
                                                                   (to_real
                                                                   (cooldown
                                                                   self))))))) :pattern (
  (dps self)) )))

(declare-sort integer 0)

(define-fun in_range3 ((x Int)) Bool (and (<= (- 2147483648) x)
                                     (<= x 2147483647)))

(define-fun bool_eq1 ((x Int) (y Int)) Bool (ite (= x y) true false))

(declare-fun attr__ATTRIBUTE_IMAGE3 (Int) us_image)

(declare-fun attr__ATTRIBUTE_VALUE__pre_check3 (us_image) Bool)

(declare-fun attr__ATTRIBUTE_VALUE3 (us_image) Int)

(declare-fun to_rep1 (integer) Int)

(declare-fun of_rep1 (Int) integer)

(declare-fun user_eq2 (integer integer) Bool)

(declare-fun dummy2 () integer)

;; inversion_axiom
  (assert
  (forall ((x integer))
  (! (= (of_rep1 (to_rep1 x)) x) :pattern ((to_rep1 x)) )))

;; range_axiom
  (assert
  (forall ((x integer)) (! (in_range3 (to_rep1 x)) :pattern ((to_rep1 x)) )))

;; coerce_axiom
  (assert
  (forall ((x Int))
  (! (=> (in_range3 x) (= (to_rep1 (of_rep1 x)) x)) :pattern ((to_rep1
                                                              (of_rep1 x))) )))

(declare-datatypes ()
((integer__ref (mk_integer__ref (integer__content integer)))))
(define-fun integer__ref___projection ((a integer__ref)) integer (integer__content
                                                                 a))

(define-fun dynamic_invariant ((temp___expr_15 Int) (temp___is_init_12 Bool)
  (temp___do_constant_13 Bool)
  (temp___do_toplevel_14 Bool)) Bool (=>
                                     (or (= temp___is_init_12 true)
                                     (<= (- 2147483648) 2147483647))
                                     (in_range3 temp___expr_15)))

(declare-sort bullet_kind 0)

(define-fun in_range4 ((x Int)) Bool (and (<= 0 x) (<= x 4)))

(define-fun bool_eq2 ((x Int) (y Int)) Bool (ite (= x y) true false))

(declare-fun attr__ATTRIBUTE_IMAGE4 (Int) us_image)

(declare-fun attr__ATTRIBUTE_VALUE__pre_check4 (us_image) Bool)

(declare-fun attr__ATTRIBUTE_VALUE4 (us_image) Int)

(declare-fun to_rep2 (bullet_kind) Int)

(declare-fun of_rep2 (Int) bullet_kind)

(declare-fun user_eq3 (bullet_kind bullet_kind) Bool)

(declare-fun dummy3 () bullet_kind)

;; inversion_axiom
  (assert
  (forall ((x bullet_kind))
  (! (= (of_rep2 (to_rep2 x)) x) :pattern ((to_rep2 x)) )))

;; range_axiom
  (assert
  (forall ((x bullet_kind)) (! (in_range4
  (to_rep2 x)) :pattern ((to_rep2 x)) )))

;; coerce_axiom
  (assert
  (forall ((x Int))
  (! (=> (in_range4 x) (= (to_rep2 (of_rep2 x)) x)) :pattern ((to_rep2
                                                              (of_rep2 x))) )))

(declare-datatypes ()
((bullet_kind__ref (mk_bullet_kind__ref (bullet_kind__content bullet_kind)))))
(define-fun bullet_kind__ref___projection ((a bullet_kind__ref)) bullet_kind
  (bullet_kind__content a))

(define-fun dynamic_invariant1 ((temp___expr_180 Int)
  (temp___is_init_177 Bool) (temp___do_constant_178 Bool)
  (temp___do_toplevel_179 Bool)) Bool (=>
                                      (or (= temp___is_init_177 true)
                                      (<= 0 4)) (in_range4 temp___expr_180)))

;; damage__post_axiom
  (assert
  (forall ((self Int))
  (! (=> (in_range4 self) (dynamic_invariant (damage self) true false true)) :pattern (
  (damage self)) )))

;; damage__def_axiom
  (assert
  (forall ((self Int))
  (! (=> (in_range4 self)
     (= (damage self) (ite (= self 0) 0
                      (ite (= self 1) 1
                      (ite (= self 2) 8 (ite (= self 3) 10 1)))))) :pattern (
  (damage self)) )))

(define-fun dynamic_invariant2 ((temp___expr_186 Int)
  (temp___is_init_183 Bool) (temp___do_constant_184 Bool)
  (temp___do_toplevel_185 Bool)) Bool (=>
                                      (or (= temp___is_init_183 true)
                                      (<= 0 2)) (in_range2 temp___expr_186)))

;; bullet__post_axiom
  (assert
  (forall ((self Int))
  (! (=> (in_range2 self) (dynamic_invariant1 (bullet self) true false true)) :pattern (
  (bullet self)) )))

;; bullet__def_axiom
  (assert
  (forall ((self Int))
  (! (=> (in_range2 self)
     (= (bullet self) (ite (= self 0) 1 (ite (= self 1) 1 2)))) :pattern (
  (bullet self)) )))

;; cooldown__post_axiom
  (assert
  (forall ((self Int))
  (! (=> (in_range2 self) (dynamic_invariant (cooldown self) true false
     true)) :pattern ((cooldown self)) )))

;; cooldown__def_axiom
  (assert
  (forall ((self Int))
  (! (=> (in_range2 self)
     (= (cooldown self) (ite (= self 0) 4 (ite (= self 1) 1 8)))) :pattern (
  (cooldown self)) )))

(assert
;; WP_parameter_def
 ;; File "weapons.ads", line 34, characters 0-0
  (not
  (forall ((w Int))
  (=> (and (<= 0 w) (<= w 2))
  (or (= w 2) (<= (to_real1 (dps w)) (to_real1 (dps (+ w 1)))))))))
(check-sat)
(get-info :reason-unknown)
