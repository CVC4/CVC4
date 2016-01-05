; COMMAND-LINE: --finite-model-find
; EXPECT: sat
(set-logic ALL_SUPPORTED)
(set-info :status sat)
(declare-sort $$unsorted 0)
(declare-fun cowlNothing ($$unsorted) Bool)
(declare-fun cowlThing ($$unsorted) Bool)
(declare-fun xsd_integer ($$unsorted) Bool)
(declare-fun xsd_string ($$unsorted) Bool)
(declare-fun is () $$unsorted)
(assert (and (forall ((X $$unsorted)) (cowlThing X) ) (forall ((X $$unsorted)) (not (cowlNothing X)) )))
(assert (forall ((X $$unsorted)) (= (xsd_string X) (not (xsd_integer X))) ))
(assert (and (forall ((X $$unsorted)) (or (not (cowlThing X)) (= X is)) ) (cowlThing is)))
(assert (cowlThing is))
(assert (and (forall ((X $$unsorted)) (cowlThing X) ) (forall ((X $$unsorted)) (not (cowlNothing X)) ) (forall ((X $$unsorted)) (or (not (cowlThing X)) (= X is)) )))
(check-sat)
