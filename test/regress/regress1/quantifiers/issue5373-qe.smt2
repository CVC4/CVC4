; COMMAND-LINE: -i
; SCRUBBER: sed 's/(.*)/()/g'
; EXPECT: ()
; EXPECT: ()
; EXIT: 0
(set-logic LRA)
(declare-fun v2 () Bool)
(declare-fun v3 () Bool)
(declare-fun v4 () Bool)
(declare-fun v5 () Bool)
(declare-fun v6 () Bool)
(declare-fun v7 () Bool)
(declare-fun v8 () Bool)
(declare-fun v9 () Bool)
(declare-fun r4 () Real)
(declare-fun r5 () Real)
(declare-fun v12 () Bool)
(declare-fun v13 () Bool)
(declare-fun v14 () Bool)
(declare-fun r7 () Real)
(declare-fun r8 () Real)
(declare-fun v19 () Bool)
(declare-fun r9 () Real)
(declare-fun v20 () Bool)
(declare-fun r10 () Real)
(declare-fun v21 () Bool)
(declare-fun v23 () Bool)
(declare-fun r46 () Real)
(declare-fun r49 () Real)
(declare-fun v126 () Bool)
(declare-fun r50 () Real)
(declare-fun r52 () Real)
(declare-fun v129 () Bool)
(declare-fun r53 () Real)
(declare-fun r54 () Real)
(declare-fun v135 () Bool)
(get-qe (exists ((q469 Real) (q470 Bool) (q471 Bool) (q472 Bool) (q473 Real) (q474 Real) (q475 Real)) (or (> r5 (- 0.0) 0.84840254 (+ 0.922 0.0 (- 0.0) 0.07865684) 6616.0 0.0) (exists ((q469 Real) (q470 Bool) (q471 Bool) (q472 Bool) (q473 Real) (q474 Real) (q475 Real)) (not (> q475 r53 q474 q469))) (< 0.71 0.81 7.6 r7 6616.0 0.07865684) v135 (or (< 0.07865684 r8 0.07865684 r7) (or v9 (distinct r7 0.07865684) true (distinct r7 0.07865684) v8 (< 0.07865684 r8 0.07865684 r7) (xor (> r5 (- 0.0) 0.84840254 (+ 0.922 0.0 (- 0.0) 0.07865684) 6616.0 0.0) (exists ((q0 Bool) (q1 Bool) (q2 Real) (q3 Real) (q4 Real) (q5 Bool)) (or true (exists ((q0 Bool) (q1 Bool) (q2 Real) (q3 Real) (q4 Real) (q5 Bool)) (=> (= q3 7.6 6616.0) (and q1 q1 q0 v6 q5 v5 true))) true v6 v2)) v13 true v9 (forall ((q6 Bool)) (xor (not (= q6 q6 v9 v12 v5 q6 (distinct true true true true true) q6)) (exists ((q6 Bool)) (not (or q6 q6 v5 q6))) v9 true (xor v3 v5))) (distinct v9 (> r5 (- 0.0) 0.84840254 (+ 0.922 0.0 (- 0.0) 0.07865684) 6616.0 0.0)) v2) (>= (+ 0.922 0.0 (- 0.0) 0.07865684) 0.84840254 (- 0.0) (- 0.0))) v23 v14))))
(get-qe (exists ((q478 Real) (q479 Bool) (q480 Bool) (q481 Bool) (q482 Real) (q483 Real) (q484 Real)) (or (=> (or q480 (or (< 0.07865684 r8 0.07865684 r7) (or v9 (distinct r7 0.07865684) true (distinct r7 0.07865684) v8 (< 0.07865684 r8 0.07865684 r7) (xor (> r5 (- 0.0) 0.84840254 (+ 0.922 0.0 (- 0.0) 0.07865684) 6616.0 0.0) (exists ((q0 Bool) (q1 Bool) (q2 Real) (q3 Real) (q4 Real) (q5 Bool)) (or true (exists ((q0 Bool) (q1 Bool) (q2 Real) (q3 Real) (q4 Real) (q5 Bool)) (=> (= q3 7.6 6616.0) (and q1 q1 q0 v6 q5 v5 true))) true v6 v2)) v13 true v9 (forall ((q6 Bool)) (xor (not (= q6 q6 v9 v12 v5 q6 (distinct true true true true true) q6)) (exists ((q6 Bool)) (not (or q6 q6 v5 q6))) v9 true (xor v3 v5))) (distinct v9 (> r5 (- 0.0) 0.84840254 (+ 0.922 0.0 (- 0.0) 0.07865684) 6616.0 0.0)) v2) (>= (+ 0.922 0.0 (- 0.0) 0.07865684) 0.84840254 (- 0.0) (- 0.0))) v23 v14) q481 v13 q479 q480 (< r9 (- (+ (+ 0.0 (- 0.0) 0.07865684) 0.84840254 (+ 0.0 (- 0.0) 0.07865684) (- 0.0) r7))) v7 (exists ((q469 Real) (q470 Bool) (q471 Bool) (q472 Bool) (q473 Real) (q474 Real) (q475 Real)) (or (> r5 (- 0.0) 0.84840254 (+ 0.922 0.0 (- 0.0) 0.07865684) 6616.0 0.0) (exists ((q469 Real) (q470 Bool) (q471 Bool) (q472 Bool) (q473 Real) (q474 Real) (q475 Real)) (not (> q475 r53 q474 q469))) (< 0.71 0.81 7.6 r7 6616.0 0.07865684) v135 (or (< 0.07865684 r8 0.07865684 r7) (or v9 (distinct r7 0.07865684) true (distinct r7 0.07865684) v8 (< 0.07865684 r8 0.07865684 r7) (xor (> r5 (- 0.0) 0.84840254 (+ 0.922 0.0 (- 0.0) 0.07865684) 6616.0 0.0) (exists ((q0 Bool) (q1 Bool) (q2 Real) (q3 Real) (q4 Real) (q5 Bool)) (or true (exists ((q0 Bool) (q1 Bool) (q2 Real) (q3 Real) (q4 Real) (q5 Bool)) (=> (= q3 7.6 6616.0) (and q1 q1 q0 v6 q5 v5 true))) true v6 v2)) v13 true v9 (forall ((q6 Bool)) (xor (not (= q6 q6 v9 v12 v5 q6 (distinct true true true true true) q6)) (exists ((q6 Bool)) (not (or q6 q6 v5 q6))) v9 true (xor v3 v5))) (distinct v9 (> r5 (- 0.0) 0.84840254 (+ 0.922 0.0 (- 0.0) 0.07865684) 6616.0 0.0)) v2) (>= (+ 0.922 0.0 (- 0.0) 0.07865684) 0.84840254 (- 0.0) (- 0.0))) v23 v14))) q481 (=> (exists ((q381 Bool) (q382 Real) (q383 Bool) (q384 Real) (q385 Bool) (q386 Real) (q387 Real) (q388 Bool)) (or v6 (exists ((q381 Bool) (q382 Real) (q383 Bool) (q384 Real) (q385 Bool) (q386 Real) (q387 Real) (q388 Bool)) (=> (=> q381 q381) (> (/ 0.0 123.0) (- 6616.0) (- r5 (+ (+ 0.0 (- 0.0) 0.07865684) r8 0.84840254) 4279.2708 4279.2708) 5985.0))) v2 (= 0.0 5985.0 (+ 0.0 (- 0.0) 0.07865684) 0.84840254) (distinct true true true true true))) (forall ((q19 Real)) (and (not (>= (+ (+ 0.0 (- 0.0) 0.07865684) 0.84840254 (+ 0.0 (- 0.0) 0.07865684) (- 0.0) r7) q19)) (forall ((q19 Real)) (not (> q19 r8 q19 q19))) v5 v19 (exists ((q14 Bool) (q15 Real) (q16 Bool) (q17 Bool)) (xor (=> (<= 0.922 q15 q15 q15) (distinct v5 q16 q17 (exists ((q0 Bool) (q1 Bool) (q2 Real) (q3 Real) (q4 Real) (q5 Bool)) (or true (exists ((q0 Bool) (q1 Bool) (q2 Real) (q3 Real) (q4 Real) (q5 Bool)) (=> (= q3 7.6 6616.0) (and q1 q1 q0 v6 q5 v5 true))) true v6 v2)) q16 v9)) (forall ((q14 Bool) (q15 Real) (q16 Bool) (q17 Bool)) (and v7 v9 v6 v4 v6 v7 true v8)) v13 v9 v4)))))) (> r54 q478 q478 7.6 q484)) (forall ((q478 Real) (q479 Bool) (q480 Bool) (q481 Bool) (q482 Real) (q483 Real) (q484 Real)) (=> (distinct q483 (- (/ (- 33.0) (- 33.0)) r7 r54 (+ r49 r50 5985.0 (+ 0.922 0.0 (- 0.0) 0.07865684)) (+ 6616.0 (+ r46 4279.2708))) q483 q482 q482) (xor v2 q479 (distinct (exists ((q378 Real) (q379 Real) (q380 Real)) (and (not (>= q380 q378 (+ 0.922 0.0 (- 0.0) 0.07865684) (- 33.0))) (forall ((q378 Real) (q379 Real) (q380 Real)) true) (distinct r10 r9 123.0 (- 0.0) r4 0.07865684) (exists ((q375 Real) (q376 Real) (q377 Real)) (and v12 (exists ((q375 Real) (q376 Real) (q377 Real)) (not (>= (- 6616.0) q375 q376 q375 q375))) (exists ((q373 Real) (q374 Real)) (xor (= (- 6616.0) (- 0.0) (+ 0.922 0.0 (- 0.0) 0.07865684)) (exists ((q373 Real) (q374 Real)) (not (> 0.07865684 q373))) v9 (xor v3 v5) (xor v12 (distinct r7 r5 (+ 0.0 (- 0.0) 0.07865684) (+ (+ 0.0 (- 0.0) 0.07865684) r8 0.84840254)) v14 v9))) v13 v7)) v20)) (forall ((q389 Real) (q390 Real) (q391 Real) (q392 Real) (q393 Real) (q394 Real)) (xor (< r5 4279.2708 r5 0.922 r10 (+ (+ 0.0 (- 0.0) 0.07865684) r8 0.84840254)) (forall ((q389 Real) (q390 Real) (q391 Real) (q392 Real) (q393 Real) (q394 Real)) (not (distinct q392 r10))) (exists ((q0 Bool) (q1 Bool) (q2 Real) (q3 Real) (q4 Real) (q5 Bool)) (or true (exists ((q0 Bool) (q1 Bool) (q2 Real) (q3 Real) (q4 Real) (q5 Bool)) (=> (= q3 7.6 6616.0) (and q1 q1 q0 v6 q5 v5 true))) true v6 v2)) (forall ((q7 Bool) (q8 Bool) (q9 Bool) (q10 Bool) (q11 Real) (q12 Real) (q13 Real)) (and v3 (exists ((q7 Bool) (q8 Bool) (q9 Bool) (q10 Bool) (q11 Real) (q12 Real) (q13 Real)) true) v2 (and v7 v9 v6 v4 v6 v7 true v8) v7)) v9)) v20 (= (- 6616.0) (- 0.0) (+ 0.922 0.0 (- 0.0) 0.07865684)) (> r52 r9 0.71)) (distinct 0.07865684 (+ 0.922 0.0 (- 0.0) 0.07865684)) q481 q479 v9 v129))) (=> (exists ((q373 Real) (q374 Real)) (xor (= (- 6616.0) (- 0.0) (+ 0.922 0.0 (- 0.0) 0.07865684)) (exists ((q373 Real) (q374 Real)) (not (> 0.07865684 q373))) v9 (xor v3 v5) (xor v12 (distinct r7 r5 (+ 0.0 (- 0.0) 0.07865684) (+ (+ 0.0 (- 0.0) 0.07865684) r8 0.84840254)) v14 v9))) v126) (distinct r7 r5 (+ 0.0 (- 0.0) 0.07865684) (+ (+ 0.0 (- 0.0) 0.07865684) r8 0.84840254)) v21)))
