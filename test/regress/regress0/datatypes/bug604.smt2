(set-logic ALL)
(set-info :status sat)
(declare-datatypes ((PairIntInt 0)) ( ( (pair (firstPairIntInt Int)
(secondPairIntInt Int)) ) ))
(declare-fun /ArrayIntOfPair () (Array Int PairIntInt))
(declare-datatypes ((SequenceABC 0)) ( ( (sequenceABC (a Int) (b Bool) (c Int)) )
))
(declare-fun /ArrayIntOfSequenceABC () (Array Int SequenceABC))
(check-sat)
