; COMMAND-LINE:  --uf-ho --ho-elim
; EXPECT: unsat

(set-logic ALL)
(declare-sort $$unsorted 0)
(declare-sort num 0)
(declare-fun agent_THFTYPE_i () $$unsorted)
(declare-fun attribute_THFTYPE_i () $$unsorted)
(declare-fun disjointRelation_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun disjoint_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun documentation_THFTYPE_i () $$unsorted)
(declare-fun domainSubclass_THFTYPE_IIiioIiioI ((-> $$unsorted $$unsorted Bool) $$unsorted $$unsorted) Bool)
(declare-fun domainSubclass_THFTYPE_IiiioI ($$unsorted $$unsorted $$unsorted) Bool)
(declare-fun domain_THFTYPE_IIIiioIIiioIoIiioI ((-> (-> $$unsorted $$unsorted Bool) (-> $$unsorted $$unsorted Bool) Bool) $$unsorted $$unsorted) Bool)
(declare-fun domain_THFTYPE_IIiiIiioI ((-> $$unsorted $$unsorted) $$unsorted $$unsorted) Bool)
(declare-fun domain_THFTYPE_IIiiioIiioI ((-> $$unsorted $$unsorted $$unsorted Bool) $$unsorted $$unsorted) Bool)
(declare-fun domain_THFTYPE_IIiioIiioI ((-> $$unsorted $$unsorted Bool) $$unsorted $$unsorted) Bool)
(declare-fun domain_THFTYPE_IiiioI ($$unsorted $$unsorted $$unsorted) Bool)
(declare-fun duration_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun equal_THFTYPE_i () $$unsorted)
(declare-fun greaterThan_THFTYPE_i () $$unsorted)
(declare-fun holdsDuring_THFTYPE_IiooI ($$unsorted Bool) Bool)
(declare-fun instance_THFTYPE_IIIiioIiioIioI ((-> (-> $$unsorted $$unsorted Bool) $$unsorted $$unsorted Bool) $$unsorted) Bool)
(declare-fun instance_THFTYPE_IIiiIioI ((-> $$unsorted $$unsorted) $$unsorted) Bool)
(declare-fun instance_THFTYPE_IIiiiIioI ((-> $$unsorted $$unsorted $$unsorted) $$unsorted) Bool)
(declare-fun instance_THFTYPE_IIiiioIioI ((-> $$unsorted $$unsorted $$unsorted Bool) $$unsorted) Bool)
(declare-fun instance_THFTYPE_IIiioIioI ((-> $$unsorted $$unsorted Bool) $$unsorted) Bool)
(declare-fun instance_THFTYPE_IIiooIioI ((-> $$unsorted Bool Bool) $$unsorted) Bool)
(declare-fun instance_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun instrument_THFTYPE_i () $$unsorted)
(declare-fun lAdditionFn_THFTYPE_i () $$unsorted)
(declare-fun lAsymmetricRelation_THFTYPE_i () $$unsorted)
(declare-fun lBeginFn_THFTYPE_IiiI ($$unsorted) $$unsorted)
(declare-fun lBill_THFTYPE_i () $$unsorted)
(declare-fun lBinaryFunction_THFTYPE_i () $$unsorted)
(declare-fun lBinaryPredicate_THFTYPE_i () $$unsorted)
(declare-fun lCardinalityFn_THFTYPE_IiiI ($$unsorted) $$unsorted)
(declare-fun lDayDuration_THFTYPE_i () $$unsorted)
(declare-fun lDay_THFTYPE_i () $$unsorted)
(declare-fun lEndFn_THFTYPE_IiiI ($$unsorted) $$unsorted)
(declare-fun lEntity_THFTYPE_i () $$unsorted)
(declare-fun lInheritableRelation_THFTYPE_i () $$unsorted)
(declare-fun lInteger_THFTYPE_i () $$unsorted)
(declare-fun lIrreflexiveRelation_THFTYPE_i () $$unsorted)
(declare-fun lMary_THFTYPE_i () $$unsorted)
(declare-fun lMeasureFn_THFTYPE_IiiiI ($$unsorted $$unsorted) $$unsorted)
(declare-fun lMonthFn_THFTYPE_i () $$unsorted)
(declare-fun lMonth_THFTYPE_i () $$unsorted)
(declare-fun lMultiplicationFn_THFTYPE_i () $$unsorted)
(declare-fun lObject_THFTYPE_i () $$unsorted)
(declare-fun lProcess_THFTYPE_i () $$unsorted)
(declare-fun lQuantity_THFTYPE_i () $$unsorted)
(declare-fun lRelationExtendedToQuantities_THFTYPE_i () $$unsorted)
(declare-fun lRelation_THFTYPE_i () $$unsorted)
(declare-fun lSetOrClass_THFTYPE_i () $$unsorted)
(declare-fun lSubtractionFn_THFTYPE_i () $$unsorted)
(declare-fun lSue_THFTYPE_i () $$unsorted)
(declare-fun lTemporalCompositionFn_THFTYPE_IiiiI ($$unsorted $$unsorted) $$unsorted)
(declare-fun lTemporalCompositionFn_THFTYPE_i () $$unsorted)
(declare-fun lTemporalRelation_THFTYPE_i () $$unsorted)
(declare-fun lTernaryPredicate_THFTYPE_i () $$unsorted)
(declare-fun lTimeInterval_THFTYPE_i () $$unsorted)
(declare-fun lTotalValuedRelation_THFTYPE_i () $$unsorted)
(declare-fun lTransitiveRelation_THFTYPE_i () $$unsorted)
(declare-fun lUnaryFunction_THFTYPE_i () $$unsorted)
(declare-fun lWhenFn_THFTYPE_IiiI ($$unsorted) $$unsorted)
(declare-fun lWhenFn_THFTYPE_i () $$unsorted)
(declare-fun lYearFn_THFTYPE_IiiI ($$unsorted) $$unsorted)
(declare-fun lYearFn_THFTYPE_i () $$unsorted)
(declare-fun lYear_THFTYPE_i () $$unsorted)
(declare-fun lessThan_THFTYPE_i () $$unsorted)
(declare-fun likes_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun located_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun meetsTemporally_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun minus_THFTYPE_IiiiI ($$unsorted $$unsorted) $$unsorted)
(declare-fun n12_THFTYPE_i () $$unsorted)
(declare-fun n1_THFTYPE_i () $$unsorted)
(declare-fun n2009_THFTYPE_i () $$unsorted)
(declare-fun n2_THFTYPE_i () $$unsorted)
(declare-fun n3_THFTYPE_i () $$unsorted)
(declare-fun orientation_THFTYPE_i () $$unsorted)
(declare-fun part_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun patient_THFTYPE_i () $$unsorted)
(declare-fun rangeSubclass_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun range_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun relatedInternalConcept_THFTYPE_IIiioIIiioIoI ((-> $$unsorted $$unsorted Bool) (-> $$unsorted $$unsorted Bool)) Bool)
(declare-fun relatedInternalConcept_THFTYPE_IiIiiIoI ($$unsorted (-> $$unsorted $$unsorted)) Bool)
(declare-fun relatedInternalConcept_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun result_THFTYPE_i () $$unsorted)
(declare-fun subProcess_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun subclass_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun subrelation_THFTYPE_IIioIIioIoI ((-> $$unsorted Bool) (-> $$unsorted Bool)) Bool)
(declare-fun subrelation_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(declare-fun temporalPart_THFTYPE_IiioI ($$unsorted $$unsorted) Bool)
(assert (forall ((REL2 $$unsorted) (CLASS1 $$unsorted) (CLASS2 $$unsorted) (REL1 $$unsorted)) (=> (and (rangeSubclass_THFTYPE_IiioI REL1 CLASS1) (rangeSubclass_THFTYPE_IiioI REL2 CLASS2) (disjoint_THFTYPE_IiioI CLASS1 CLASS2)) (disjointRelation_THFTYPE_IiioI REL1 REL2)) ))
(assert (subclass_THFTYPE_IiioI lInheritableRelation_THFTYPE_i lRelation_THFTYPE_i))
(assert (forall ((X $$unsorted) (Y $$unsorted) (Z $$unsorted)) (=> (and (subclass_THFTYPE_IiioI X Y) (instance_THFTYPE_IiioI Z X)) (instance_THFTYPE_IiioI Z Y)) ))
(assert (forall ((X $$unsorted) (Y $$unsorted)) (=> (subclass_THFTYPE_IiioI X Y) (and (instance_THFTYPE_IiioI X lSetOrClass_THFTYPE_i) (instance_THFTYPE_IiioI Y lSetOrClass_THFTYPE_i))) ))
(assert (holdsDuring_THFTYPE_IiooI (lYearFn_THFTYPE_IiiI n2009_THFTYPE_i) (forall ((X $$unsorted)) (=> (likes_THFTYPE_IiioI lMary_THFTYPE_i X) (likes_THFTYPE_IiioI lSue_THFTYPE_i X)) )))
(assert (holdsDuring_THFTYPE_IiooI (lYearFn_THFTYPE_IiiI n2009_THFTYPE_i) (forall ((X $$unsorted)) (=> (likes_THFTYPE_IiioI lMary_THFTYPE_i X) (likes_THFTYPE_IiioI lSue_THFTYPE_i X)) )))
(assert (forall ((THING $$unsorted)) (instance_THFTYPE_IiioI THING lEntity_THFTYPE_i) ))
(assert (forall ((P Bool) (Y $$unsorted)) (=> P (holdsDuring_THFTYPE_IiooI Y P)) ))
(assert (forall ((P Bool) (Y $$unsorted)) (=> P (holdsDuring_THFTYPE_IiooI Y P)) ))
(assert (forall ((NUMBER $$unsorted) (MONTH $$unsorted)) (=> (and (instance_THFTYPE_IiioI MONTH lMonth_THFTYPE_i) (duration_THFTYPE_IiioI MONTH (lMeasureFn_THFTYPE_IiiiI NUMBER lDayDuration_THFTYPE_i))) (= (lCardinalityFn_THFTYPE_IiiI (lTemporalCompositionFn_THFTYPE_IiiiI MONTH lDay_THFTYPE_i)) NUMBER)) ))
(assert (forall ((OBJ1 $$unsorted) (OBJ2 $$unsorted)) (=> (located_THFTYPE_IiioI OBJ1 OBJ2) (forall ((SUB $$unsorted)) (=> (part_THFTYPE_IiioI SUB OBJ1) (located_THFTYPE_IiioI SUB OBJ2)) )) ))
(assert (subclass_THFTYPE_IiioI lAsymmetricRelation_THFTYPE_i lIrreflexiveRelation_THFTYPE_i))
(assert (subclass_THFTYPE_IiioI lTotalValuedRelation_THFTYPE_i lInheritableRelation_THFTYPE_i))
(assert (forall ((NUMBER $$unsorted) (CLASS1 $$unsorted) (REL $$unsorted) (CLASS2 $$unsorted)) (=> (and (domainSubclass_THFTYPE_IiiioI REL NUMBER CLASS1) (domainSubclass_THFTYPE_IiiioI REL NUMBER CLASS2)) (or (subclass_THFTYPE_IiioI CLASS1 CLASS2) (subclass_THFTYPE_IiioI CLASS2 CLASS1))) ))
(assert (forall ((DAY $$unsorted)) (=> (instance_THFTYPE_IiioI DAY lDay_THFTYPE_i) (duration_THFTYPE_IiioI DAY (lMeasureFn_THFTYPE_IiiiI n1_THFTYPE_i lDayDuration_THFTYPE_i))) ))
(assert (forall ((REL2 $$unsorted) (NUMBER $$unsorted) (CLASS1 $$unsorted) (CLASS2 $$unsorted) (REL1 $$unsorted)) (=> (and (domainSubclass_THFTYPE_IiiioI REL1 NUMBER CLASS1) (domainSubclass_THFTYPE_IiiioI REL2 NUMBER CLASS2) (disjoint_THFTYPE_IiioI CLASS1 CLASS2)) (disjointRelation_THFTYPE_IiioI REL1 REL2)) ))
(assert (subclass_THFTYPE_IiioI lTemporalRelation_THFTYPE_i lRelation_THFTYPE_i))
(assert (subclass_THFTYPE_IiioI lYear_THFTYPE_i lTimeInterval_THFTYPE_i))
(assert (forall ((NUMBER $$unsorted) (CLASS1 $$unsorted) (REL $$unsorted) (CLASS2 $$unsorted)) (=> (and (domain_THFTYPE_IiiioI REL NUMBER CLASS1) (domain_THFTYPE_IiiioI REL NUMBER CLASS2)) (or (subclass_THFTYPE_IiioI CLASS1 CLASS2) (subclass_THFTYPE_IiioI CLASS2 CLASS1))) ))
(assert (forall ((REL (-> $$unsorted $$unsorted Bool))) (= (instance_THFTYPE_IIiioIioI REL lTransitiveRelation_THFTYPE_i) (forall ((INST1 $$unsorted) (INST2 $$unsorted) (INST3 $$unsorted)) (=> (and (REL INST1 INST2) (REL INST2 INST3)) (REL INST1 INST3)) )) ))
(assert (rangeSubclass_THFTYPE_IiioI lTemporalCompositionFn_THFTYPE_i lTimeInterval_THFTYPE_i))
(assert (forall ((REL2 $$unsorted) (CLASS1 $$unsorted) (CLASS2 $$unsorted) (REL1 $$unsorted)) (=> (and (range_THFTYPE_IiioI REL1 CLASS1) (range_THFTYPE_IiioI REL2 CLASS2) (disjoint_THFTYPE_IiioI CLASS1 CLASS2)) (disjointRelation_THFTYPE_IiioI REL1 REL2)) ))
(assert (subclass_THFTYPE_IiioI lRelationExtendedToQuantities_THFTYPE_i lRelation_THFTYPE_i))
(assert (subclass_THFTYPE_IiioI lMonth_THFTYPE_i lTimeInterval_THFTYPE_i))
(assert (forall ((TIME $$unsorted) (SITUATION Bool)) (=> (holdsDuring_THFTYPE_IiooI TIME (not SITUATION)) (not (holdsDuring_THFTYPE_IiooI TIME SITUATION))) ))
(assert (likes_THFTYPE_IiioI lMary_THFTYPE_i lBill_THFTYPE_i))
(assert (range_THFTYPE_IiioI lWhenFn_THFTYPE_i lTimeInterval_THFTYPE_i))
(assert (forall ((INTERVAL1 $$unsorted) (INTERVAL2 $$unsorted)) (= (meetsTemporally_THFTYPE_IiioI INTERVAL1 INTERVAL2) (= (lEndFn_THFTYPE_IiiI INTERVAL1) (lBeginFn_THFTYPE_IiiI INTERVAL2))) ))
(assert (forall ((SITUATION Bool) (TIME2 $$unsorted) (TIME1 $$unsorted)) (=> (and (holdsDuring_THFTYPE_IiooI TIME1 SITUATION) (temporalPart_THFTYPE_IiioI TIME2 TIME1)) (holdsDuring_THFTYPE_IiooI TIME2 SITUATION)) ))
(assert (range_THFTYPE_IiioI lMultiplicationFn_THFTYPE_i lQuantity_THFTYPE_i))
(assert (exists ((THING $$unsorted)) (instance_THFTYPE_IiioI THING lEntity_THFTYPE_i) ))
(assert (forall ((REL (-> $$unsorted $$unsorted Bool))) (= (instance_THFTYPE_IIiioIioI REL lIrreflexiveRelation_THFTYPE_i) (forall ((INST $$unsorted)) (not (REL INST INST)) )) ))
(assert (subclass_THFTYPE_IiioI lBinaryFunction_THFTYPE_i lInheritableRelation_THFTYPE_i))
(assert (forall ((NUMBER $$unsorted) (PRED1 $$unsorted) (CLASS1 $$unsorted) (PRED2 $$unsorted)) (=> (and (subrelation_THFTYPE_IiioI PRED1 PRED2) (domain_THFTYPE_IiiioI PRED2 NUMBER CLASS1)) (domain_THFTYPE_IiiioI PRED1 NUMBER CLASS1)) ))
(assert (subclass_THFTYPE_IiioI lTotalValuedRelation_THFTYPE_i lRelation_THFTYPE_i))
(assert (subclass_THFTYPE_IiioI lTernaryPredicate_THFTYPE_i lInheritableRelation_THFTYPE_i))
(assert (subclass_THFTYPE_IiioI lRelationExtendedToQuantities_THFTYPE_i lInheritableRelation_THFTYPE_i))
(assert (forall ((YEAR $$unsorted)) (=> (instance_THFTYPE_IiioI YEAR lYear_THFTYPE_i) (= (lCardinalityFn_THFTYPE_IiiI (lTemporalCompositionFn_THFTYPE_IiiiI YEAR lMonth_THFTYPE_i)) n12_THFTYPE_i)) ))
(assert (forall ((CLASS1 $$unsorted) (CLASS2 $$unsorted)) (=> (= CLASS1 CLASS2) (forall ((THING $$unsorted)) (= (instance_THFTYPE_IiioI THING CLASS1) (instance_THFTYPE_IiioI THING CLASS2)) )) ))
(assert (forall ((REL2 $$unsorted) (CLASS1 $$unsorted) (REL1 $$unsorted)) (=> (and (subrelation_THFTYPE_IiioI REL1 REL2) (rangeSubclass_THFTYPE_IiioI REL2 CLASS1)) (rangeSubclass_THFTYPE_IiioI REL1 CLASS1)) ))
(assert (forall ((YEAR2 $$unsorted) (YEAR1 $$unsorted)) (=> (and (instance_THFTYPE_IiioI YEAR1 lYear_THFTYPE_i) (instance_THFTYPE_IiioI YEAR2 lYear_THFTYPE_i) (= (minus_THFTYPE_IiiiI YEAR2 YEAR1) n1_THFTYPE_i)) (meetsTemporally_THFTYPE_IiioI YEAR1 YEAR2)) ))
(assert (forall ((REL2 (-> $$unsorted Bool)) (ROW $$unsorted) (REL1 (-> $$unsorted Bool))) (=> (and (subrelation_THFTYPE_IIioIIioIoI REL1 REL2) (REL1 ROW)) (REL2 ROW)) ))
(assert (forall ((THING2 $$unsorted) (THING1 $$unsorted)) (=> (= THING1 THING2) (forall ((CLASS $$unsorted)) (= (instance_THFTYPE_IiioI THING1 CLASS) (instance_THFTYPE_IiioI THING2 CLASS)) )) ))
(assert (forall ((CLASS1 $$unsorted) (REL $$unsorted) (CLASS2 $$unsorted)) (=> (and (range_THFTYPE_IiioI REL CLASS1) (range_THFTYPE_IiioI REL CLASS2)) (or (subclass_THFTYPE_IiioI CLASS1 CLASS2) (subclass_THFTYPE_IiioI CLASS2 CLASS1))) ))
(assert (forall ((CLASS1 $$unsorted) (CLASS2 $$unsorted)) (= (disjoint_THFTYPE_IiioI CLASS1 CLASS2) (forall ((INST $$unsorted)) (not (and (instance_THFTYPE_IiioI INST CLASS1) (instance_THFTYPE_IiioI INST CLASS2))) )) ))
(assert (forall ((SUBPROC $$unsorted) (PROC $$unsorted)) (=> (subProcess_THFTYPE_IiioI SUBPROC PROC) (temporalPart_THFTYPE_IiioI (lWhenFn_THFTYPE_IiiI SUBPROC) (lWhenFn_THFTYPE_IiiI PROC))) ))
(assert (rangeSubclass_THFTYPE_IiioI lMonthFn_THFTYPE_i lMonth_THFTYPE_i))
(assert (forall ((INTERVAL1 $$unsorted) (INTERVAL2 $$unsorted)) (=> (and (= (lBeginFn_THFTYPE_IiiI INTERVAL1) (lBeginFn_THFTYPE_IiiI INTERVAL2)) (= (lEndFn_THFTYPE_IiiI INTERVAL1) (lEndFn_THFTYPE_IiiI INTERVAL2))) (= INTERVAL1 INTERVAL2)) ))
(assert (forall ((REL2 $$unsorted) (CLASS1 $$unsorted) (REL1 $$unsorted)) (=> (and (subrelation_THFTYPE_IiioI REL1 REL2) (range_THFTYPE_IiioI REL2 CLASS1)) (range_THFTYPE_IiioI REL1 CLASS1)) ))
(assert (subclass_THFTYPE_IiioI lTemporalRelation_THFTYPE_i lInheritableRelation_THFTYPE_i))
(assert (forall ((REL2 $$unsorted) (NUMBER $$unsorted) (CLASS1 $$unsorted) (REL1 $$unsorted)) (=> (and (subrelation_THFTYPE_IiioI REL1 REL2) (domainSubclass_THFTYPE_IiiioI REL2 NUMBER CLASS1)) (domainSubclass_THFTYPE_IiiioI REL1 NUMBER CLASS1)) ))
(assert (forall ((SUBPROC $$unsorted) (PROC $$unsorted)) (=> (subProcess_THFTYPE_IiioI SUBPROC PROC) (forall ((REGION $$unsorted)) (=> (located_THFTYPE_IiioI PROC REGION) (located_THFTYPE_IiioI SUBPROC REGION)) )) ))
(assert (range_THFTYPE_IiioI lSubtractionFn_THFTYPE_i lQuantity_THFTYPE_i))
(assert (range_THFTYPE_IiioI lAdditionFn_THFTYPE_i lQuantity_THFTYPE_i))
(assert (forall ((OBJ $$unsorted) (PROCESS $$unsorted)) (=> (located_THFTYPE_IiioI PROCESS OBJ) (forall ((SUB $$unsorted)) (=> (subProcess_THFTYPE_IiioI SUB PROCESS) (located_THFTYPE_IiioI SUB OBJ)) )) ))
(assert (subclass_THFTYPE_IiioI lUnaryFunction_THFTYPE_i lInheritableRelation_THFTYPE_i))
(assert (subclass_THFTYPE_IiioI lDay_THFTYPE_i lTimeInterval_THFTYPE_i))
(assert (forall ((REL2 $$unsorted) (NUMBER $$unsorted) (CLASS1 $$unsorted) (CLASS2 $$unsorted) (REL1 $$unsorted)) (=> (and (domain_THFTYPE_IiiioI REL1 NUMBER CLASS1) (domain_THFTYPE_IiiioI REL2 NUMBER CLASS2) (disjoint_THFTYPE_IiioI CLASS1 CLASS2)) (disjointRelation_THFTYPE_IiioI REL1 REL2)) ))
(assert (rangeSubclass_THFTYPE_IiioI lYearFn_THFTYPE_i lYear_THFTYPE_i))
(assert (forall ((CLASS $$unsorted) (PRED1 $$unsorted) (PRED2 $$unsorted)) (=> (and (subrelation_THFTYPE_IiioI PRED1 PRED2) (instance_THFTYPE_IiioI PRED2 CLASS) (subclass_THFTYPE_IiioI CLASS lInheritableRelation_THFTYPE_i)) (instance_THFTYPE_IiioI PRED1 CLASS)) ))
(assert (forall ((CLASS1 $$unsorted) (REL $$unsorted) (CLASS2 $$unsorted)) (=> (and (rangeSubclass_THFTYPE_IiioI REL CLASS1) (rangeSubclass_THFTYPE_IiioI REL CLASS2)) (or (subclass_THFTYPE_IiioI CLASS1 CLASS2) (subclass_THFTYPE_IiioI CLASS2 CLASS1))) ))
(assert (subclass_THFTYPE_IiioI lBinaryPredicate_THFTYPE_i lInheritableRelation_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI meetsTemporally_THFTYPE_IiioI lTemporalRelation_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI duration_THFTYPE_IiioI lTotalValuedRelation_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lAdditionFn_THFTYPE_i lBinaryFunction_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI range_THFTYPE_IiioI lAsymmetricRelation_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lMonthFn_THFTYPE_i lBinaryFunction_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI disjointRelation_THFTYPE_IiioI n2_THFTYPE_i lRelation_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI meetsTemporally_THFTYPE_IiioI lAsymmetricRelation_THFTYPE_i))
(assert (domainSubclass_THFTYPE_IiiioI lMonthFn_THFTYPE_i n2_THFTYPE_i lYear_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI subProcess_THFTYPE_IiioI n1_THFTYPE_i lProcess_THFTYPE_i))
(assert (relatedInternalConcept_THFTYPE_IiioI lMonth_THFTYPE_i lMonthFn_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lessThan_THFTYPE_i lBinaryPredicate_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI meetsTemporally_THFTYPE_IiioI n1_THFTYPE_i lTimeInterval_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI equal_THFTYPE_i n2_THFTYPE_i lEntity_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI disjoint_THFTYPE_IiioI n2_THFTYPE_i lSetOrClass_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lSubtractionFn_THFTYPE_i lTotalValuedRelation_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI subProcess_THFTYPE_IiioI n2_THFTYPE_i lProcess_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI agent_THFTYPE_i n1_THFTYPE_i lProcess_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI relatedInternalConcept_THFTYPE_IiioI lBinaryPredicate_THFTYPE_i))
(assert (instance_THFTYPE_IiioI equal_THFTYPE_i lBinaryPredicate_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lAdditionFn_THFTYPE_i lRelationExtendedToQuantities_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lessThan_THFTYPE_i lIrreflexiveRelation_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI disjointRelation_THFTYPE_IiioI n1_THFTYPE_i lRelation_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI greaterThan_THFTYPE_i n2_THFTYPE_i lQuantity_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI range_THFTYPE_IiioI n2_THFTYPE_i lSetOrClass_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI subrelation_THFTYPE_IiioI n2_THFTYPE_i lRelation_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lYearFn_THFTYPE_IiiI lTemporalRelation_THFTYPE_i))
(assert (instance_THFTYPE_IiioI attribute_THFTYPE_i lAsymmetricRelation_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lWhenFn_THFTYPE_IiiI lUnaryFunction_THFTYPE_i))
(assert (subrelation_THFTYPE_IiioI result_THFTYPE_i patient_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI lMultiplicationFn_THFTYPE_i n1_THFTYPE_i lQuantity_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI patient_THFTYPE_i n1_THFTYPE_i lProcess_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lCardinalityFn_THFTYPE_IiiI lUnaryFunction_THFTYPE_i))
(assert (instance_THFTYPE_IiioI greaterThan_THFTYPE_i lRelationExtendedToQuantities_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lMultiplicationFn_THFTYPE_i lTotalValuedRelation_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI patient_THFTYPE_i n2_THFTYPE_i lEntity_THFTYPE_i))
(assert (instance_THFTYPE_IiioI documentation_THFTYPE_i lTernaryPredicate_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI duration_THFTYPE_IiioI lBinaryPredicate_THFTYPE_i))
(assert (instance_THFTYPE_IiioI orientation_THFTYPE_i lTernaryPredicate_THFTYPE_i))
(assert (domain_THFTYPE_IIiiioIiioI domain_THFTYPE_IiiioI n1_THFTYPE_i lRelation_THFTYPE_i))
(assert (instance_THFTYPE_IiioI greaterThan_THFTYPE_i lIrreflexiveRelation_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI result_THFTYPE_i n2_THFTYPE_i lEntity_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lMultiplicationFn_THFTYPE_i lRelationExtendedToQuantities_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lTemporalCompositionFn_THFTYPE_i lTemporalRelation_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI relatedInternalConcept_THFTYPE_IiioI n2_THFTYPE_i lEntity_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI subrelation_THFTYPE_IiioI lBinaryPredicate_THFTYPE_i))
(assert (relatedInternalConcept_THFTYPE_IIiioIIiioIoI disjointRelation_THFTYPE_IiioI disjoint_THFTYPE_IiioI))
(assert (domain_THFTYPE_IiiioI greaterThan_THFTYPE_i n1_THFTYPE_i lQuantity_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI part_THFTYPE_IiioI n1_THFTYPE_i lObject_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lBeginFn_THFTYPE_IiiI lTemporalRelation_THFTYPE_i))
(assert (domain_THFTYPE_IIiiIiioI lBeginFn_THFTYPE_IiiI n1_THFTYPE_i lTimeInterval_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lSubtractionFn_THFTYPE_i lRelationExtendedToQuantities_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI instance_THFTYPE_IiioI lBinaryPredicate_THFTYPE_i))
(assert (instance_THFTYPE_IiioI attribute_THFTYPE_i lIrreflexiveRelation_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI lessThan_THFTYPE_i n1_THFTYPE_i lQuantity_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI instance_THFTYPE_IiioI n1_THFTYPE_i lEntity_THFTYPE_i))
(assert (instance_THFTYPE_IIiooIioI holdsDuring_THFTYPE_IiooI lBinaryPredicate_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI part_THFTYPE_IiioI n2_THFTYPE_i lObject_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI temporalPart_THFTYPE_IiioI lTemporalRelation_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI instrument_THFTYPE_i n2_THFTYPE_i lObject_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI lAdditionFn_THFTYPE_i n1_THFTYPE_i lQuantity_THFTYPE_i))
(assert (domain_THFTYPE_IIiiIiioI lYearFn_THFTYPE_IiiI n1_THFTYPE_i lInteger_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI range_THFTYPE_IiioI lBinaryPredicate_THFTYPE_i))
(assert (instance_THFTYPE_IIIiioIiioIioI domain_THFTYPE_IIiioIiioI lTernaryPredicate_THFTYPE_i))
(assert (domain_THFTYPE_IIiiioIiioI domain_THFTYPE_IiiioI n3_THFTYPE_i lSetOrClass_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lCardinalityFn_THFTYPE_IiiI lTotalValuedRelation_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI documentation_THFTYPE_i n1_THFTYPE_i lEntity_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI rangeSubclass_THFTYPE_IiioI lAsymmetricRelation_THFTYPE_i))
(assert (relatedInternalConcept_THFTYPE_IiIiiIoI lYear_THFTYPE_i lYearFn_THFTYPE_IiiI))
(assert (domain_THFTYPE_IiiioI equal_THFTYPE_i n1_THFTYPE_i lEntity_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lCardinalityFn_THFTYPE_IiiI lAsymmetricRelation_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI disjoint_THFTYPE_IiioI lBinaryPredicate_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI temporalPart_THFTYPE_IiioI lBinaryPredicate_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lMultiplicationFn_THFTYPE_i lBinaryFunction_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI subProcess_THFTYPE_IiioI lBinaryPredicate_THFTYPE_i))
(assert (instance_THFTYPE_IiioI greaterThan_THFTYPE_i lTransitiveRelation_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI disjointRelation_THFTYPE_IiioI lIrreflexiveRelation_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lMonthFn_THFTYPE_i lTemporalRelation_THFTYPE_i))
(assert (instance_THFTYPE_IiioI greaterThan_THFTYPE_i lBinaryPredicate_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lBeginFn_THFTYPE_IiiI lUnaryFunction_THFTYPE_i))
(assert (instance_THFTYPE_IIiiiIioI lMeasureFn_THFTYPE_IiiiI lBinaryFunction_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI lMultiplicationFn_THFTYPE_i n2_THFTYPE_i lQuantity_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lBeginFn_THFTYPE_IiiI lTotalValuedRelation_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI meetsTemporally_THFTYPE_IiioI lBinaryPredicate_THFTYPE_i))
(assert (domainSubclass_THFTYPE_IiiioI lMonthFn_THFTYPE_i n1_THFTYPE_i lMonth_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI subclass_THFTYPE_IiioI lBinaryPredicate_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI result_THFTYPE_i n1_THFTYPE_i lProcess_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lEndFn_THFTYPE_IiiI lUnaryFunction_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI subclass_THFTYPE_IiioI n2_THFTYPE_i lSetOrClass_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lEndFn_THFTYPE_IiiI lTemporalRelation_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI lTemporalCompositionFn_THFTYPE_i n1_THFTYPE_i lTimeInterval_THFTYPE_i))
(assert (domain_THFTYPE_IIIiioIIiioIoIiioI relatedInternalConcept_THFTYPE_IIiioIIiioIoI n1_THFTYPE_i lEntity_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lTemporalCompositionFn_THFTYPE_i lBinaryFunction_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI subclass_THFTYPE_IiioI n1_THFTYPE_i lSetOrClass_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI lSubtractionFn_THFTYPE_i n2_THFTYPE_i lQuantity_THFTYPE_i))
(assert (instance_THFTYPE_IiioI equal_THFTYPE_i lRelationExtendedToQuantities_THFTYPE_i))
(assert (domainSubclass_THFTYPE_IiiioI lTemporalCompositionFn_THFTYPE_i n2_THFTYPE_i lTimeInterval_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lYearFn_THFTYPE_IiiI lUnaryFunction_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI orientation_THFTYPE_i n2_THFTYPE_i lObject_THFTYPE_i))
(assert (domain_THFTYPE_IIiiioIiioI domainSubclass_THFTYPE_IiiioI n1_THFTYPE_i lRelation_THFTYPE_i))
(assert (relatedInternalConcept_THFTYPE_IiioI lDay_THFTYPE_i lDayDuration_THFTYPE_i))
(assert (disjointRelation_THFTYPE_IiioI result_THFTYPE_i instrument_THFTYPE_i))
(assert (instance_THFTYPE_IIiiiIioI lMeasureFn_THFTYPE_IiiiI lTotalValuedRelation_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI instrument_THFTYPE_i n1_THFTYPE_i lProcess_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI duration_THFTYPE_IiioI lAsymmetricRelation_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lAdditionFn_THFTYPE_i lTotalValuedRelation_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lSubtractionFn_THFTYPE_i lBinaryFunction_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI instance_THFTYPE_IiioI n2_THFTYPE_i lSetOrClass_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI lSubtractionFn_THFTYPE_i n1_THFTYPE_i lQuantity_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI duration_THFTYPE_IiioI n1_THFTYPE_i lTimeInterval_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI subrelation_THFTYPE_IiioI n1_THFTYPE_i lRelation_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lEndFn_THFTYPE_IiiI lTotalValuedRelation_THFTYPE_i))
(assert (instance_THFTYPE_IIiiioIioI domainSubclass_THFTYPE_IiiioI lTernaryPredicate_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI meetsTemporally_THFTYPE_IiioI n2_THFTYPE_i lTimeInterval_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI lessThan_THFTYPE_i n2_THFTYPE_i lQuantity_THFTYPE_i))
(assert (instance_THFTYPE_IIiooIioI holdsDuring_THFTYPE_IiooI lAsymmetricRelation_THFTYPE_i))
(assert (domainSubclass_THFTYPE_IIiioIiioI rangeSubclass_THFTYPE_IiioI n2_THFTYPE_i lSetOrClass_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lessThan_THFTYPE_i lTransitiveRelation_THFTYPE_i))
(assert (domain_THFTYPE_IIiioIiioI disjoint_THFTYPE_IiioI n1_THFTYPE_i lSetOrClass_THFTYPE_i))
(assert (subrelation_THFTYPE_IiioI instrument_THFTYPE_i patient_THFTYPE_i))
(assert (domain_THFTYPE_IIiiIiioI lEndFn_THFTYPE_IiiI n1_THFTYPE_i lTimeInterval_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI disjointRelation_THFTYPE_IiioI lBinaryPredicate_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI orientation_THFTYPE_i n1_THFTYPE_i lObject_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lWhenFn_THFTYPE_IiiI lTotalValuedRelation_THFTYPE_i))
(assert (instance_THFTYPE_IiioI lessThan_THFTYPE_i lRelationExtendedToQuantities_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI attribute_THFTYPE_i n1_THFTYPE_i lObject_THFTYPE_i))
(assert (domain_THFTYPE_IiiioI lAdditionFn_THFTYPE_i n2_THFTYPE_i lQuantity_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI located_THFTYPE_IiioI lTransitiveRelation_THFTYPE_i))
(assert (domain_THFTYPE_IIiiioIiioI domainSubclass_THFTYPE_IiiioI n3_THFTYPE_i lSetOrClass_THFTYPE_i))
(assert (instance_THFTYPE_IIiiIioI lWhenFn_THFTYPE_IiiI lTemporalRelation_THFTYPE_i))
(assert (instance_THFTYPE_IIiioIioI rangeSubclass_THFTYPE_IiioI lBinaryPredicate_THFTYPE_i))
(assert (not (exists ((X $$unsorted) (Y $$unsorted)) (holdsDuring_THFTYPE_IiooI (lYearFn_THFTYPE_IiiI Y) (likes_THFTYPE_IiioI lSue_THFTYPE_i X)) )))
(meta-info :filename "CSR148^2")
(check-sat-assuming ( (not false) ))
