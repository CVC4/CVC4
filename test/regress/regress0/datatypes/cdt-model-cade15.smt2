(set-logic ALL_SUPPORTED)
(set-info :status sat)
(declare-codatatypes () ((Stream (C (c Stream)) (D (d Stream)) (E (e Stream)))))

(declare-const z Stream)
(declare-const x Stream)
(declare-const y Stream)
(declare-const u Stream)
(declare-const v Stream)
(declare-const w Stream)

(assert (= u (C z)))
(assert (= v (D z)))
(assert (= w (E y)))
(assert (= x (C v)))
(assert (distinct x y z u v w))
(check-sat)