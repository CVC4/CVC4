; COMMAND-LINE: --nl-ext=full
; EXPECT: sat
(set-logic QF_NRAT)
(set-info :status sat)
(declare-fun time__AT0@0 () Real)
(declare-fun instance.y__AT0@0 () Real)
(declare-fun instance.x__AT0@0 () Real)
(declare-fun instance.x__AT0@1 () Real)
(declare-fun event_is_timed__AT0@1 () Bool)
(declare-fun event_is_timed__AT0@0 () Bool)
(declare-fun instance.EVENT.0__AT0@0 () Bool)
(declare-fun instance.EVENT.1__AT0@0 () Bool)
(declare-fun instance.y__AT0@1 () Real)
(declare-fun time__AT0@1 () Real)
(assert (let ((.def_0 (<= 0.0 instance.x__AT0@1))) (let ((.def_1 (not .def_0))) (let ((.def_2 (not instance.EVENT.1__AT0@0))) (let ((.def_3 (not instance.EVENT.0__AT0@0))) (let ((.def_4 (or .def_3 .def_2))) (let ((.def_5 (= event_is_timed__AT0@0 instance.EVENT.1__AT0@0))) (let ((.def_6 (<= time__AT0@0 time__AT0@1))) (let ((.def_7 (or .def_2 .def_6))) (let ((.def_8 (and .def_7 .def_5))) (let ((.def_9 (= time__AT0@0 time__AT0@1))) (let ((.def_10 (or instance.EVENT.1__AT0@0 .def_9))) (let ((.def_11 (and .def_10 .def_8))) (let ((.def_12 (* (- 1.0) instance.y__AT0@0))) (let ((.def_13 (+ instance.x__AT0@0 .def_12))) (let ((.def_14 (* (- 1.0) time__AT0@0))) (let ((.def_15 (+ .def_14 time__AT0@1))) (let ((.def_16 (exp .def_15))) (let ((.def_17 (* .def_16 .def_13))) (let ((.def_18 (* (- 1.0) .def_17))) (let ((.def_19 (* (- 1.0) instance.y__AT0@1))) (let ((.def_20 (+ .def_19 .def_18))) (let ((.def_21 (+ instance.x__AT0@1 .def_20))) (let ((.def_22 (= .def_21 0.0))) (let ((.def_23 (+ instance.y__AT0@0 instance.x__AT0@0))) (let ((.def_24 (* .def_23 .def_16))) (let ((.def_25 (* (- 1.0) .def_24))) (let ((.def_26 (+ instance.y__AT0@1 .def_25))) (let ((.def_27 (+ instance.x__AT0@1 .def_26))) (let ((.def_28 (= .def_27 0.0))) (let ((.def_29 (and .def_28 .def_22))) (let ((.def_30 (not .def_9))) (let ((.def_31 (= instance.x__AT0@0 instance.x__AT0@1))) (let ((.def_32 (= instance.y__AT0@0 instance.y__AT0@1))) (let ((.def_33 (and .def_32 .def_31))) (let ((.def_34 (or .def_33 .def_30))) (let ((.def_35 (and .def_34 .def_29))) (let ((.def_36 (and .def_35 .def_6))) (let ((.def_37 (or .def_2 .def_36))) (let ((.def_38 (and .def_37 .def_10))) (let ((.def_39 (and .def_3 .def_2))) (let ((.def_40 (or .def_39 .def_38))) (let ((.def_41 (and .def_40 .def_11))) (let ((.def_42 (not .def_39))) (let ((.def_43 (or .def_42 .def_33))) (let ((.def_44 (and .def_43 .def_41))) (let ((.def_45 (not event_is_timed__AT0@0))) (let ((.def_46 (= event_is_timed__AT0@1 .def_45))) (let ((.def_47 (and .def_46 .def_44))) (let ((.def_48 (and .def_47 .def_4))) (let ((.def_49 (= instance.x__AT0@0 1.0))) (let ((.def_50 (= instance.y__AT0@0 0.0))) (let ((.def_51 (and .def_50 .def_49))) (let ((.def_52 (= time__AT0@0 0.0))) (let ((.def_53 (and .def_52 .def_51))) (let ((.def_54 (and .def_53 .def_48 .def_1))) .def_54))))))))))))))))))))))))))))))))))))))))))))))))))))))))
(check-sat)
(exit)
