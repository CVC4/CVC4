/*********************                                                        */
/*! \file logic_info_white.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Morgan Deters, Andres Noetzli, Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Unit testing for CVC4::LogicInfo class
 **
 ** Unit testing for CVC4::LogicInfo class.
 **/

#include <cxxtest/TestSuite.h>

#include "expr/kind.h"
#include "theory/logic_info.h"

using namespace CVC4;
using namespace CVC4::theory;

using namespace std;

class LogicInfoWhite : public CxxTest::TestSuite {

public:

#warning "This test is of questionable compatiblity with contrib/new-theory. Is the new theory id handled correctly by the Logic info."

  void testSmtlibLogics() {
    LogicInfo info("QF_SAT");
    TS_ASSERT( info.isLocked() );
    TS_ASSERT( !info.isSharingEnabled() );
    TS_ASSERT( info.isPure( THEORY_BOOL ) );
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( info.hasNothing() );

    info = LogicInfo("AUFLIA");
    TS_ASSERT( !info.isPure( THEORY_BOOL ) );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( info.isQuantified() );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( info.isLinear() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( !info.areRealsUsed() );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("AUFLIRA");
    TS_ASSERT( !info.isPure( THEORY_BOOL ) );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( info.isQuantified() );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( info.isLinear() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("AUFNIRA");
    TS_ASSERT( !info.isPure( THEORY_BOOL ) );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( info.isQuantified() );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.isLinear() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("LRA");
    TS_ASSERT( !info.isPure( THEORY_BOOL ) );
    TS_ASSERT( !info.isSharingEnabled() );
    TS_ASSERT( info.isQuantified() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( info.isLinear() );
    TS_ASSERT( !info.areIntegersUsed() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_ABV");
    TS_ASSERT( !info.isPure( THEORY_BOOL ) );
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isPure( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_AUFBV");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isPure( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_AUFLIA");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isPure( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( info.isLinear() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( !info.areRealsUsed() );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_AX");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( !info.isSharingEnabled() );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( info.isPure( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_BV");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( !info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isPure( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_IDL");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( !info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( info.isLinear() );
    TS_ASSERT( info.isDifferenceLogic() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.areRealsUsed() );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_LIA");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( !info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( info.isLinear() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.areRealsUsed() );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_LRA");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( !info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isLinear() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( !info.areIntegersUsed() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_NIA");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( !info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( !info.isLinear() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.areRealsUsed() );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_NRA");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( !info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.isLinear() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( !info.areIntegersUsed() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_RDL");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( !info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( info.isLinear() );
    TS_ASSERT( info.isDifferenceLogic() );
    TS_ASSERT( !info.areIntegersUsed() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_UF");
    TS_ASSERT( !info.isPure( THEORY_BOOL ) );
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( !info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isPure( THEORY_ARITH ) );
    TS_ASSERT( info.isPure( THEORY_UF ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_UFBV");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isPure( THEORY_BV ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_UFIDL");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( info.isLinear() );
    TS_ASSERT( info.isDifferenceLogic() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.areRealsUsed() );
    TS_ASSERT(!info.areTranscendentalsUsed());
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_UFLIA");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( info.isLinear() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.areRealsUsed() );
    TS_ASSERT(!info.areTranscendentalsUsed());
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_UFLRA");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( info.isLinear() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( !info.areIntegersUsed() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT(!info.areTranscendentalsUsed());
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_UFNRA");
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.isLinear() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( !info.areIntegersUsed() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT(!info.areTranscendentalsUsed());
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("UFLRA");
    TS_ASSERT( info.isQuantified() );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( info.isLinear() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( !info.areIntegersUsed() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT(!info.areTranscendentalsUsed());
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("UFNIA");
    TS_ASSERT( info.isQuantified() );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isPure( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.isLinear() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.areRealsUsed() );
    TS_ASSERT(!info.areTranscendentalsUsed());
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("QF_ALL_SUPPORTED");
    TS_ASSERT( info.isLocked() );
    TS_ASSERT( !info.isPure( THEORY_BOOL ) );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.isLinear() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT(info.areTranscendentalsUsed());
    TS_ASSERT( !info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );

    info = LogicInfo("ALL_SUPPORTED");
    TS_ASSERT( info.isLocked() );
    TS_ASSERT( !info.isPure( THEORY_BOOL ) );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( info.isQuantified() );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.isLinear() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT( info.hasEverything() );
    TS_ASSERT( !info.hasNothing() );
  }

  void testDefaultLogic() {
    LogicInfo info;
    TS_ASSERT( !info.isLocked() );

    TS_ASSERT_THROWS(info.getLogicString(), CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.isTheoryEnabled(THEORY_BUILTIN),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.isTheoryEnabled(THEORY_BOOL),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.isTheoryEnabled(THEORY_UF),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.isTheoryEnabled(THEORY_ARITH),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.isTheoryEnabled(THEORY_ARRAYS),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.isTheoryEnabled(THEORY_BV),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.isTheoryEnabled(THEORY_DATATYPES),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.isTheoryEnabled(THEORY_QUANTIFIERS),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(!info.isPure(THEORY_BUILTIN),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(!info.isPure(THEORY_BOOL),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(!info.isPure(THEORY_UF), CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(!info.isPure(THEORY_ARITH),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(!info.isPure(THEORY_ARRAYS),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(!info.isPure(THEORY_BV), CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(!info.isPure(THEORY_DATATYPES),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(!info.isPure(THEORY_QUANTIFIERS),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.isQuantified(), CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.areIntegersUsed(), CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.areRealsUsed(), CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(!info.isLinear(), CVC4::IllegalArgumentException&);

    info.lock();
    TS_ASSERT( info.isLocked() );
    TS_ASSERT_EQUALS( info.getLogicString(), "ALL" );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BUILTIN ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_QUANTIFIERS ) );
    TS_ASSERT( ! info.isPure( THEORY_BUILTIN ) );
    TS_ASSERT( ! info.isPure( THEORY_BOOL ) );
    TS_ASSERT( ! info.isPure( THEORY_UF ) );
    TS_ASSERT( ! info.isPure( THEORY_ARITH ) );
    TS_ASSERT( ! info.isPure( THEORY_ARRAYS ) );
    TS_ASSERT( ! info.isPure( THEORY_BV ) );
    TS_ASSERT( ! info.isPure( THEORY_DATATYPES ) );
    TS_ASSERT( ! info.isPure( THEORY_QUANTIFIERS ) );
    TS_ASSERT( info.isQuantified() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT(info.areTranscendentalsUsed());
    TS_ASSERT( !info.isLinear() );

    TS_ASSERT_THROWS(info.arithOnlyLinear(), CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.disableIntegers(), CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.disableQuantifiers(),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.disableTheory(THEORY_BV),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.disableTheory(THEORY_DATATYPES),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.enableIntegers(), CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.disableReals(), CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.disableTheory(THEORY_ARITH),
                     CVC4::IllegalArgumentException&);
    TS_ASSERT_THROWS(info.disableTheory(THEORY_UF),
                     CVC4::IllegalArgumentException&);

    info = info.getUnlockedCopy();
    TS_ASSERT( !info.isLocked() );
    info.disableTheory(THEORY_STRINGS);
    info.disableTheory(THEORY_SETS);
    info.arithOnlyLinear();
    info.disableIntegers();
    info.lock();
    TS_ASSERT_EQUALS(info.getLogicString(), "SEP_AUFBVFPDTLRA");

    info = info.getUnlockedCopy();
    TS_ASSERT( !info.isLocked() );
    info.disableQuantifiers();
    info.lock();
    TS_ASSERT_EQUALS(info.getLogicString(), "QF_SEP_AUFBVFPDTLRA");

    info = info.getUnlockedCopy();
    TS_ASSERT( !info.isLocked() );
    info.disableTheory(THEORY_BV);
    info.disableTheory(THEORY_DATATYPES);
    info.enableIntegers();
    info.disableReals();
    info.lock();
    TS_ASSERT_EQUALS(info.getLogicString(), "QF_SEP_AUFFPLIA");

    info = info.getUnlockedCopy();
    TS_ASSERT( !info.isLocked() );
    info.disableTheory(THEORY_ARITH);
    info.disableTheory(THEORY_UF);
    info.disableTheory(THEORY_FP);
    info.disableTheory(THEORY_SEP);
    info.lock();
    TS_ASSERT_EQUALS( info.getLogicString(), "QF_AX" );
    TS_ASSERT( info.isPure( THEORY_ARRAYS ) );
    TS_ASSERT( ! info.isQuantified() );

    // check all-excluded logic
    info = info.getUnlockedCopy();
    TS_ASSERT( !info.isLocked() );
    info.disableEverything();
    info.lock();
    TS_ASSERT( info.isLocked() );
    TS_ASSERT( !info.isSharingEnabled() );
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( info.isPure( THEORY_BOOL ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT_THROWS(info.isLinear(), IllegalArgumentException&);
    TS_ASSERT_THROWS(info.areIntegersUsed(), IllegalArgumentException&);
    TS_ASSERT_THROWS(info.isDifferenceLogic(), IllegalArgumentException&);
    TS_ASSERT_THROWS(info.areRealsUsed(), IllegalArgumentException&);
    TS_ASSERT_THROWS(info.areTranscendentalsUsed(), IllegalArgumentException&);

    // check copy is unchanged
    info = info.getUnlockedCopy();
    TS_ASSERT( !info.isLocked() );
    info.lock();
    TS_ASSERT( info.isLocked() );
    TS_ASSERT( !info.isSharingEnabled() );
    TS_ASSERT( !info.isQuantified() );
    TS_ASSERT( info.isPure( THEORY_BOOL ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( !info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT_THROWS(info.isLinear(), IllegalArgumentException&);
    TS_ASSERT_THROWS(info.areIntegersUsed(), IllegalArgumentException&);
    TS_ASSERT_THROWS(info.isDifferenceLogic(), IllegalArgumentException&);
    TS_ASSERT_THROWS(info.areRealsUsed(), IllegalArgumentException&);
    TS_ASSERT_THROWS(info.areTranscendentalsUsed(), IllegalArgumentException&);

    // check all-included logic
    info = info.getUnlockedCopy();
    TS_ASSERT( !info.isLocked() );
    info.enableEverything();
    info.lock();
    TS_ASSERT( info.isLocked() );
    TS_ASSERT( !info.isPure( THEORY_BOOL ) );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( info.isQuantified() );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.isLinear() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT(info.areTranscendentalsUsed());

    // check copy is unchanged
    info = info.getUnlockedCopy();
    TS_ASSERT( !info.isLocked() );
    info.lock();
    TS_ASSERT( info.isLocked() );
    TS_ASSERT( !info.isPure( THEORY_BOOL ) );
    TS_ASSERT( info.isSharingEnabled() );
    TS_ASSERT( info.isQuantified() );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARRAYS ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_UF ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_ARITH ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BV ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_DATATYPES ) );
    TS_ASSERT( info.isTheoryEnabled( THEORY_BOOL ) );
    TS_ASSERT( !info.isLinear() );
    TS_ASSERT( info.areIntegersUsed() );
    TS_ASSERT( !info.isDifferenceLogic() );
    TS_ASSERT( info.areRealsUsed() );
    TS_ASSERT(info.areTranscendentalsUsed());
  }

  void eq(const LogicInfo& logic1, const LogicInfo& logic2) const {
    cout << "asserting that " << logic1 << " == " << logic2 << endl;

    TS_ASSERT( logic1 == logic2 );
    TS_ASSERT( !(logic1 != logic2) );
    TS_ASSERT( !(logic1 < logic2) );
    TS_ASSERT( !(logic1 > logic2) );
    TS_ASSERT( logic1 <= logic2 );
    TS_ASSERT( logic1 >= logic2 );
    TS_ASSERT( logic1.isComparableTo(logic2) );

    TS_ASSERT( logic2 == logic1 );
    TS_ASSERT( !(logic2 != logic1) );
    TS_ASSERT( !(logic2 < logic1) );
    TS_ASSERT( !(logic2 > logic1) );
    TS_ASSERT( logic2 <= logic1 );
    TS_ASSERT( logic2 >= logic1 );
    TS_ASSERT( logic2.isComparableTo(logic1) );
  }

  void nc(const LogicInfo& logic1, const LogicInfo& logic2) const {
    cout << "asserting that " << logic1 << " is-not-comparable-to " << logic2 << endl;
    TS_ASSERT( !(logic1 == logic2) );
    TS_ASSERT( logic1 != logic2 );
    TS_ASSERT( !(logic1 < logic2) );
    TS_ASSERT( !(logic1 > logic2) );
    TS_ASSERT( !(logic1 <= logic2) );
    TS_ASSERT( !(logic1 >= logic2) );
    TS_ASSERT( !logic1.isComparableTo(logic2) );

    TS_ASSERT( !(logic2 == logic1) );
    TS_ASSERT( logic2 != logic1 );
    TS_ASSERT( !(logic2 < logic1) );
    TS_ASSERT( !(logic2 > logic1) );
    TS_ASSERT( !(logic2 <= logic1) );
    TS_ASSERT( !(logic2 >= logic1) );
    TS_ASSERT( !logic2.isComparableTo(logic1) );
  }

  void lt(const LogicInfo& logic1, const LogicInfo& logic2) const {
    cout << "asserting that " << logic1 << " < " << logic2 << endl;

    TS_ASSERT( !(logic1 == logic2) );
    TS_ASSERT( logic1 != logic2 );
    TS_ASSERT( logic1 < logic2 );
    TS_ASSERT( !(logic1 > logic2) );
    TS_ASSERT( logic1 <= logic2 );
    TS_ASSERT( !(logic1 >= logic2) );
    TS_ASSERT( logic1.isComparableTo(logic2) );

    TS_ASSERT( !(logic2 == logic1) );
    TS_ASSERT( logic2 != logic1 );
    TS_ASSERT( !(logic2 < logic1) );
    TS_ASSERT( logic2 > logic1 );
    TS_ASSERT( !(logic2 <= logic1) );
    TS_ASSERT( logic2 >= logic1 );
    TS_ASSERT( logic2.isComparableTo(logic1) );
  }

  void gt(const LogicInfo& logic1, const LogicInfo& logic2) const {
    cout << "asserting that " << logic1 << " > " << logic2 << endl;

    TS_ASSERT( !(logic1 == logic2) );
    TS_ASSERT( logic1 != logic2 );
    TS_ASSERT( !(logic1 < logic2) );
    TS_ASSERT( logic1 > logic2 );
    TS_ASSERT( !(logic1 <= logic2) );
    TS_ASSERT( logic1 >= logic2 );
    TS_ASSERT( logic1.isComparableTo(logic2) );

    TS_ASSERT( !(logic2 == logic1) );
    TS_ASSERT( logic2 != logic1 );
    TS_ASSERT( logic2 < logic1 );
    TS_ASSERT( !(logic2 > logic1) );
    TS_ASSERT( logic2 <= logic1 );
    TS_ASSERT( !(logic2 >= logic1) );
    TS_ASSERT( logic2.isComparableTo(logic1) );
  }

  void testComparison() {
    LogicInfo ufHo = LogicInfo("QF_UF").getUnlockedCopy();
    ufHo.enableHigherOrder();
    ufHo.lock();

    eq("QF_UF", "QF_UF");
    nc("QF_UF", "QF_LRA");
    nc("QF_UF", "QF_LIA");
    lt("QF_UF", "QF_UFLRA");
    lt("QF_UF", "QF_UFLIA");
    lt("QF_UF", "QF_AUFLIA");
    lt("QF_UF", "QF_AUFLIRA");
    nc("QF_UF", "QF_BV");
    nc("QF_UF", "QF_ABV");
    lt("QF_UF", "QF_AUFBV");
    nc("QF_UF", "QF_IDL");
    nc("QF_UF", "QF_RDL");
    lt("QF_UF", "QF_UFIDL");
    nc("QF_UF", "QF_NIA");
    nc("QF_UF", "QF_NRA");
    lt("QF_UF", "QF_AUFNIRA");
    nc("QF_UF", "LRA");
    nc("QF_UF", "NRA");
    nc("QF_UF", "NIA");
    lt("QF_UF", "UFLRA");
    lt("QF_UF", "UFNIA");
    lt("QF_UF", "AUFLIA");
    lt("QF_UF", "AUFLIRA");
    lt("QF_UF", "AUFNIRA");
    lt("QF_UF", "QF_UFC");
    lt("QF_UF", ufHo);
    nc("QF_UFC", ufHo);

    nc("QF_LRA", "QF_UF");
    eq("QF_LRA", "QF_LRA");
    nc("QF_LRA", "QF_LIA");
    lt("QF_LRA", "QF_UFLRA");
    nc("QF_LRA", "QF_UFLIA");
    nc("QF_LRA", "QF_AUFLIA");
    lt("QF_LRA", "QF_AUFLIRA");
    nc("QF_LRA", "QF_BV");
    nc("QF_LRA", "QF_ABV");
    nc("QF_LRA", "QF_AUFBV");
    nc("QF_LRA", "QF_IDL");
    gt("QF_LRA", "QF_RDL");
    nc("QF_LRA", "QF_UFIDL");
    nc("QF_LRA", "QF_NIA");
    lt("QF_LRA", "QF_NRA");
    lt("QF_LRA", "QF_AUFNIRA");
    lt("QF_LRA", "LRA");
    lt("QF_LRA", "NRA");
    nc("QF_LRA", "NIA");
    lt("QF_LRA", "UFLRA");
    nc("QF_LRA", "UFNIA");
    nc("QF_LRA", "AUFLIA");
    lt("QF_LRA", "AUFLIRA");
    lt("QF_LRA", "AUFNIRA");
    lt("QF_LRA", "QF_UFCLRA");

    nc("QF_LIA", "QF_UF");
    nc("QF_LIA", "QF_LRA");
    eq("QF_LIA", "QF_LIA");
    nc("QF_LIA", "QF_UFLRA");
    lt("QF_LIA", "QF_UFLIA");
    lt("QF_LIA", "QF_AUFLIA");
    lt("QF_LIA", "QF_AUFLIRA");
    nc("QF_LIA", "QF_BV");
    nc("QF_LIA", "QF_ABV");
    nc("QF_LIA", "QF_AUFBV");
    gt("QF_LIA", "QF_IDL");
    nc("QF_LIA", "QF_RDL");
    nc("QF_LIA", "QF_UFIDL");
    lt("QF_LIA", "QF_NIA");
    nc("QF_LIA", "QF_NRA");
    lt("QF_LIA", "QF_AUFNIRA");
    nc("QF_LIA", "LRA");
    nc("QF_LIA", "NRA");
    lt("QF_LIA", "NIA");
    nc("QF_LIA", "UFLRA");
    lt("QF_LIA", "UFNIA");
    lt("QF_LIA", "AUFLIA");
    lt("QF_LIA", "AUFLIRA");
    lt("QF_LIA", "AUFNIRA");

    gt("QF_UFLRA", "QF_UF");
    gt("QF_UFLRA", "QF_LRA");
    nc("QF_UFLRA", "QF_LIA");
    eq("QF_UFLRA", "QF_UFLRA");
    nc("QF_UFLRA", "QF_UFLIA");
    nc("QF_UFLRA", "QF_AUFLIA");
    lt("QF_UFLRA", "QF_AUFLIRA");
    nc("QF_UFLRA", "QF_BV");
    nc("QF_UFLRA", "QF_ABV");
    nc("QF_UFLRA", "QF_AUFBV");
    nc("QF_UFLRA", "QF_IDL");
    gt("QF_UFLRA", "QF_RDL");
    nc("QF_UFLRA", "QF_UFIDL");
    nc("QF_UFLRA", "QF_NIA");
    nc("QF_UFLRA", "QF_NRA");
    lt("QF_UFLRA", "QF_AUFNIRA");
    nc("QF_UFLRA", "LRA");
    nc("QF_UFLRA", "NRA");
    nc("QF_UFLRA", "NIA");
    lt("QF_UFLRA", "UFLRA");
    nc("QF_UFLRA", "UFNIA");
    nc("QF_UFLRA", "AUFLIA");
    lt("QF_UFLRA", "AUFLIRA");
    lt("QF_UFLRA", "AUFNIRA");

    gt("QF_UFLIA", "QF_UF");
    nc("QF_UFLIA", "QF_LRA");
    gt("QF_UFLIA", "QF_LIA");
    nc("QF_UFLIA", "QF_UFLRA");
    eq("QF_UFLIA", "QF_UFLIA");
    lt("QF_UFLIA", "QF_AUFLIA");
    lt("QF_UFLIA", "QF_AUFLIRA");
    nc("QF_UFLIA", "QF_BV");
    nc("QF_UFLIA", "QF_ABV");
    nc("QF_UFLIA", "QF_AUFBV");
    gt("QF_UFLIA", "QF_IDL");
    nc("QF_UFLIA", "QF_RDL");
    gt("QF_UFLIA", "QF_UFIDL");
    nc("QF_UFLIA", "QF_NIA");
    nc("QF_UFLIA", "QF_NRA");
    lt("QF_UFLIA", "QF_AUFNIRA");
    nc("QF_UFLIA", "LRA");
    nc("QF_UFLIA", "NRA");
    nc("QF_UFLIA", "NIA");
    nc("QF_UFLIA", "UFLRA");
    lt("QF_UFLIA", "UFNIA");
    lt("QF_UFLIA", "AUFLIA");
    lt("QF_UFLIA", "AUFLIRA");
    lt("QF_UFLIA", "AUFNIRA");

    gt("QF_AUFLIA", "QF_UF");
    nc("QF_AUFLIA", "QF_LRA");
    gt("QF_AUFLIA", "QF_LIA");
    nc("QF_AUFLIA", "QF_UFLRA");
    gt("QF_AUFLIA", "QF_UFLIA");
    eq("QF_AUFLIA", "QF_AUFLIA");
    lt("QF_AUFLIA", "QF_AUFLIRA");
    nc("QF_AUFLIA", "QF_BV");
    nc("QF_AUFLIA", "QF_ABV");
    nc("QF_AUFLIA", "QF_AUFBV");
    gt("QF_AUFLIA", "QF_IDL");
    nc("QF_AUFLIA", "QF_RDL");
    gt("QF_AUFLIA", "QF_UFIDL");
    nc("QF_AUFLIA", "QF_NIA");
    nc("QF_AUFLIA", "QF_NRA");
    lt("QF_AUFLIA", "QF_AUFNIRA");
    nc("QF_AUFLIA", "LRA");
    nc("QF_AUFLIA", "NRA");
    nc("QF_AUFLIA", "NIA");
    nc("QF_AUFLIA", "UFLRA");
    nc("QF_AUFLIA", "UFNIA");
    lt("QF_AUFLIA", "AUFLIA");
    lt("QF_AUFLIA", "AUFLIRA");
    lt("QF_AUFLIA", "AUFNIRA");

    gt("QF_AUFLIRA", "QF_UF");
    gt("QF_AUFLIRA", "QF_LRA");
    gt("QF_AUFLIRA", "QF_LIA");
    gt("QF_AUFLIRA", "QF_UFLRA");
    gt("QF_AUFLIRA", "QF_UFLIA");
    gt("QF_AUFLIRA", "QF_AUFLIA");
    eq("QF_AUFLIRA", "QF_AUFLIRA");
    nc("QF_AUFLIRA", "QF_BV");
    nc("QF_AUFLIRA", "QF_ABV");
    nc("QF_AUFLIRA", "QF_AUFBV");
    gt("QF_AUFLIRA", "QF_IDL");
    gt("QF_AUFLIRA", "QF_RDL");
    gt("QF_AUFLIRA", "QF_UFIDL");
    nc("QF_AUFLIRA", "QF_NIA");
    nc("QF_AUFLIRA", "QF_NRA");
    lt("QF_AUFLIRA", "QF_AUFNIRA");
    nc("QF_AUFLIRA", "LRA");
    nc("QF_AUFLIRA", "NRA");
    nc("QF_AUFLIRA", "NIA");
    nc("QF_AUFLIRA", "UFLRA");
    nc("QF_AUFLIRA", "UFNIA");
    nc("QF_AUFLIRA", "AUFLIA");
    lt("QF_AUFLIRA", "AUFLIRA");
    lt("QF_AUFLIRA", "AUFNIRA");

    nc("QF_BV", "QF_UF");
    nc("QF_BV", "QF_LRA");
    nc("QF_BV", "QF_LIA");
    nc("QF_BV", "QF_UFLRA");
    nc("QF_BV", "QF_UFLIA");
    nc("QF_BV", "QF_AUFLIA");
    nc("QF_BV", "QF_AUFLIRA");
    eq("QF_BV", "QF_BV");
    lt("QF_BV", "QF_ABV");
    lt("QF_BV", "QF_AUFBV");
    nc("QF_BV", "QF_IDL");
    nc("QF_BV", "QF_RDL");
    nc("QF_BV", "QF_UFIDL");
    nc("QF_BV", "QF_NIA");
    nc("QF_BV", "QF_NRA");
    nc("QF_BV", "QF_AUFNIRA");
    nc("QF_BV", "LRA");
    nc("QF_BV", "NRA");
    nc("QF_BV", "NIA");
    nc("QF_BV", "UFLRA");
    nc("QF_BV", "UFNIA");
    nc("QF_BV", "AUFLIA");
    nc("QF_BV", "AUFLIRA");
    nc("QF_BV", "AUFNIRA");

    nc("QF_ABV", "QF_UF");
    nc("QF_ABV", "QF_LRA");
    nc("QF_ABV", "QF_LIA");
    nc("QF_ABV", "QF_UFLRA");
    nc("QF_ABV", "QF_UFLIA");
    nc("QF_ABV", "QF_AUFLIA");
    nc("QF_ABV", "QF_AUFLIRA");
    gt("QF_ABV", "QF_BV");
    eq("QF_ABV", "QF_ABV");
    lt("QF_ABV", "QF_AUFBV");
    nc("QF_ABV", "QF_IDL");
    nc("QF_ABV", "QF_RDL");
    nc("QF_ABV", "QF_UFIDL");
    nc("QF_ABV", "QF_NIA");
    nc("QF_ABV", "QF_NRA");
    nc("QF_ABV", "QF_AUFNIRA");
    nc("QF_ABV", "LRA");
    nc("QF_ABV", "NRA");
    nc("QF_ABV", "NIA");
    nc("QF_ABV", "UFLRA");
    nc("QF_ABV", "UFNIA");
    nc("QF_ABV", "AUFLIA");
    nc("QF_ABV", "AUFLIRA");
    nc("QF_ABV", "AUFNIRA");

    gt("QF_AUFBV", "QF_UF");
    nc("QF_AUFBV", "QF_LRA");
    nc("QF_AUFBV", "QF_LIA");
    nc("QF_AUFBV", "QF_UFLRA");
    nc("QF_AUFBV", "QF_UFLIA");
    nc("QF_AUFBV", "QF_AUFLIA");
    nc("QF_AUFBV", "QF_AUFLIRA");
    gt("QF_AUFBV", "QF_BV");
    gt("QF_AUFBV", "QF_ABV");
    eq("QF_AUFBV", "QF_AUFBV");
    nc("QF_AUFBV", "QF_IDL");
    nc("QF_AUFBV", "QF_RDL");
    nc("QF_AUFBV", "QF_UFIDL");
    nc("QF_AUFBV", "QF_NIA");
    nc("QF_AUFBV", "QF_NRA");
    nc("QF_AUFBV", "QF_AUFNIRA");
    nc("QF_AUFBV", "LRA");
    nc("QF_AUFBV", "NRA");
    nc("QF_AUFBV", "NIA");
    nc("QF_AUFBV", "UFLRA");
    nc("QF_AUFBV", "UFNIA");
    nc("QF_AUFBV", "AUFLIA");
    nc("QF_AUFBV", "AUFLIRA");
    nc("QF_AUFBV", "AUFNIRA");

    nc("QF_IDL", "QF_UF");
    nc("QF_IDL", "QF_LRA");
    lt("QF_IDL", "QF_LIA");
    nc("QF_IDL", "QF_UFLRA");
    lt("QF_IDL", "QF_UFLIA");
    lt("QF_IDL", "QF_AUFLIA");
    lt("QF_IDL", "QF_AUFLIRA");
    nc("QF_IDL", "QF_BV");
    nc("QF_IDL", "QF_ABV");
    nc("QF_IDL", "QF_AUFBV");
    eq("QF_IDL", "QF_IDL");
    nc("QF_IDL", "QF_RDL");
    lt("QF_IDL", "QF_UFIDL");
    lt("QF_IDL", "QF_NIA");
    nc("QF_IDL", "QF_NRA");
    nc("QF_IDL", "QF_NRAT");
    lt("QF_IDL", "QF_AUFNIRA");
    nc("QF_IDL", "LRA");
    nc("QF_IDL", "NRA");
    lt("QF_IDL", "NIA");
    nc("QF_IDL", "UFLRA");
    lt("QF_IDL", "UFNIA");
    lt("QF_IDL", "AUFLIA");
    lt("QF_IDL", "AUFLIRA");
    lt("QF_IDL", "AUFNIRA");

    nc("QF_RDL", "QF_UF");
    lt("QF_RDL", "QF_LRA");
    nc("QF_RDL", "QF_LIA");
    lt("QF_RDL", "QF_UFLRA");
    nc("QF_RDL", "QF_UFLIA");
    nc("QF_RDL", "QF_AUFLIA");
    lt("QF_RDL", "QF_AUFLIRA");
    nc("QF_RDL", "QF_BV");
    nc("QF_RDL", "QF_ABV");
    nc("QF_RDL", "QF_AUFBV");
    nc("QF_RDL", "QF_IDL");
    eq("QF_RDL", "QF_RDL");
    nc("QF_RDL", "QF_UFIDL");
    nc("QF_RDL", "QF_NIA");
    lt("QF_RDL", "QF_NRA");
    lt("QF_RDL", "QF_AUFNIRA");
    lt("QF_RDL", "LRA");
    lt("QF_RDL", "NRA");
    nc("QF_RDL", "NIA");
    lt("QF_RDL", "UFLRA");
    nc("QF_RDL", "UFNIA");
    nc("QF_RDL", "AUFLIA");
    lt("QF_RDL", "AUFLIRA");
    lt("QF_RDL", "AUFNIRA");

    gt("QF_UFIDL", "QF_UF");
    nc("QF_UFIDL", "QF_LRA");
    nc("QF_UFIDL", "QF_LIA");
    nc("QF_UFIDL", "QF_UFLRA");
    lt("QF_UFIDL", "QF_UFLIA");
    lt("QF_UFIDL", "QF_AUFLIA");
    lt("QF_UFIDL", "QF_AUFLIRA");
    nc("QF_UFIDL", "QF_BV");
    nc("QF_UFIDL", "QF_ABV");
    nc("QF_UFIDL", "QF_AUFBV");
    gt("QF_UFIDL", "QF_IDL");
    nc("QF_UFIDL", "QF_RDL");
    eq("QF_UFIDL", "QF_UFIDL");
    nc("QF_UFIDL", "QF_NIA");
    nc("QF_UFIDL", "QF_NRA");
    lt("QF_UFIDL", "QF_AUFNIRA");
    nc("QF_UFIDL", "LRA");
    nc("QF_UFIDL", "NRA");
    nc("QF_UFIDL", "NIA");
    nc("QF_UFIDL", "UFLRA");
    lt("QF_UFIDL", "UFNIA");
    lt("QF_UFIDL", "AUFLIA");
    lt("QF_UFIDL", "AUFLIRA");
    lt("QF_UFIDL", "AUFNIRA");

    nc("QF_NIA", "QF_UF");
    nc("QF_NIA", "QF_LRA");
    gt("QF_NIA", "QF_LIA");
    nc("QF_NIA", "QF_UFLRA");
    nc("QF_NIA", "QF_UFLIA");
    nc("QF_NIA", "QF_AUFLIA");
    nc("QF_NIA", "QF_AUFLIRA");
    nc("QF_NIA", "QF_BV");
    nc("QF_NIA", "QF_ABV");
    nc("QF_NIA", "QF_AUFBV");
    gt("QF_NIA", "QF_IDL");
    nc("QF_NIA", "QF_RDL");
    nc("QF_NIA", "QF_UFIDL");
    eq("QF_NIA", "QF_NIA");
    nc("QF_NIA", "QF_NRA");
    lt("QF_NIA", "QF_AUFNIRA");
    nc("QF_NIA", "LRA");
    nc("QF_NIA", "NRA");
    lt("QF_NIA", "NIA");
    nc("QF_NIA", "UFLRA");
    lt("QF_NIA", "UFNIA");
    nc("QF_NIA", "AUFLIA");
    nc("QF_NIA", "AUFLIRA");
    lt("QF_NIA", "AUFNIRA");

    nc("QF_NRA", "QF_UF");
    gt("QF_NRA", "QF_LRA");
    nc("QF_NRA", "QF_LIA");
    nc("QF_NRA", "QF_UFLRA");
    nc("QF_NRA", "QF_UFLIA");
    nc("QF_NRA", "QF_AUFLIA");
    nc("QF_NRA", "QF_AUFLIRA");
    nc("QF_NRA", "QF_BV");
    nc("QF_NRA", "QF_ABV");
    nc("QF_NRA", "QF_AUFBV");
    nc("QF_NRA", "QF_IDL");
    gt("QF_NRA", "QF_RDL");
    nc("QF_NRA", "QF_UFIDL");
    nc("QF_NRA", "QF_NIA");
    eq("QF_NRA", "QF_NRA");
    lt("QF_NRA", "QF_AUFNIRA");
    nc("QF_NRA", "LRA");
    lt("QF_NRA", "NRA");
    nc("QF_NRA", "NIA");
    nc("QF_NRA", "UFLRA");
    nc("QF_NRA", "UFNIA");
    nc("QF_NRA", "AUFLIA");
    nc("QF_NRA", "AUFLIRA");
    lt("QF_NRA", "AUFNIRA");
    lt("QF_NRA", "QF_NRAT");

    gt("QF_AUFNIRA", "QF_UF");
    gt("QF_AUFNIRA", "QF_LRA");
    gt("QF_AUFNIRA", "QF_LIA");
    gt("QF_AUFNIRA", "QF_UFLRA");
    gt("QF_AUFNIRA", "QF_UFLIA");
    gt("QF_AUFNIRA", "QF_AUFLIA");
    gt("QF_AUFNIRA", "QF_AUFLIRA");
    nc("QF_AUFNIRA", "QF_BV");
    nc("QF_AUFNIRA", "QF_ABV");
    nc("QF_AUFNIRA", "QF_AUFBV");
    gt("QF_AUFNIRA", "QF_IDL");
    gt("QF_AUFNIRA", "QF_RDL");
    gt("QF_AUFNIRA", "QF_UFIDL");
    gt("QF_AUFNIRA", "QF_NIA");
    gt("QF_AUFNIRA", "QF_NRA");
    eq("QF_AUFNIRA", "QF_AUFNIRA");
    nc("QF_AUFNIRA", "LRA");
    nc("QF_AUFNIRA", "NRA");
    nc("QF_AUFNIRA", "NIA");
    nc("QF_AUFNIRA", "UFLRA");
    nc("QF_AUFNIRA", "UFNIA");
    nc("QF_AUFNIRA", "AUFLIA");
    nc("QF_AUFNIRA", "AUFLIRA");
    lt("QF_AUFNIRA", "AUFNIRA");
    lt("QF_AUFNIRA", "QF_AUFNIRAT");

    nc("LRA", "QF_UF");
    gt("LRA", "QF_LRA");
    nc("LRA", "QF_LIA");
    nc("LRA", "QF_UFLRA");
    nc("LRA", "QF_UFLIA");
    nc("LRA", "QF_AUFLIA");
    nc("LRA", "QF_AUFLIRA");
    nc("LRA", "QF_BV");
    nc("LRA", "QF_ABV");
    nc("LRA", "QF_AUFBV");
    nc("LRA", "QF_IDL");
    gt("LRA", "QF_RDL");
    nc("LRA", "QF_UFIDL");
    nc("LRA", "QF_NIA");
    nc("LRA", "QF_NRA");
    nc("LRA", "QF_AUFNIRA");
    eq("LRA", "LRA");
    lt("LRA", "NRA");
    nc("LRA", "NIA");
    lt("LRA", "UFLRA");
    nc("LRA", "UFNIA");
    nc("LRA", "AUFLIA");
    lt("LRA", "AUFLIRA");
    lt("LRA", "AUFNIRA");

    nc("NRA", "QF_UF");
    gt("NRA", "QF_LRA");
    nc("NRA", "QF_LIA");
    nc("NRA", "QF_UFLRA");
    nc("NRA", "QF_UFLIA");
    nc("NRA", "QF_AUFLIA");
    nc("NRA", "QF_AUFLIRA");
    nc("NRA", "QF_BV");
    nc("NRA", "QF_ABV");
    nc("NRA", "QF_AUFBV");
    nc("NRA", "QF_IDL");
    gt("NRA", "QF_RDL");
    nc("NRA", "QF_UFIDL");
    nc("NRA", "QF_NIA");
    gt("NRA", "QF_NRA");
    nc("NRA", "QF_AUFNIRA");
    gt("NRA", "LRA");
    eq("NRA", "NRA");
    nc("NRA", "NIA");
    nc("NRA", "UFLRA");
    nc("NRA", "UFNIA");
    nc("NRA", "AUFLIA");
    nc("NRA", "AUFLIRA");
    lt("NRA", "AUFNIRA");

    nc("NIA", "QF_UF");
    nc("NIA", "QF_LRA");
    gt("NIA", "QF_LIA");
    nc("NIA", "QF_UFLRA");
    nc("NIA", "QF_UFLIA");
    nc("NIA", "QF_AUFLIA");
    nc("NIA", "QF_AUFLIRA");
    nc("NIA", "QF_BV");
    nc("NIA", "QF_ABV");
    nc("NIA", "QF_AUFBV");
    gt("NIA", "QF_IDL");
    nc("NIA", "QF_RDL");
    nc("NIA", "QF_UFIDL");
    gt("NIA", "QF_NIA");
    nc("NIA", "QF_NRA");
    nc("NIA", "QF_AUFNIRA");
    nc("NIA", "LRA");
    nc("NIA", "NRA");
    eq("NIA", "NIA");
    nc("NIA", "UFLRA");
    lt("NIA", "UFNIA");
    nc("NIA", "AUFLIA");
    nc("NIA", "AUFLIRA");
    lt("NIA", "AUFNIRA");

    gt("UFLRA", "QF_UF");
    gt("UFLRA", "QF_LRA");
    nc("UFLRA", "QF_LIA");
    gt("UFLRA", "QF_UFLRA");
    nc("UFLRA", "QF_UFLIA");
    nc("UFLRA", "QF_AUFLIA");
    nc("UFLRA", "QF_AUFLIRA");
    nc("UFLRA", "QF_BV");
    nc("UFLRA", "QF_ABV");
    nc("UFLRA", "QF_AUFBV");
    nc("UFLRA", "QF_IDL");
    gt("UFLRA", "QF_RDL");
    nc("UFLRA", "QF_UFIDL");
    nc("UFLRA", "QF_NIA");
    nc("UFLRA", "QF_NRA");
    nc("UFLRA", "QF_AUFNIRA");
    gt("UFLRA", "LRA");
    nc("UFLRA", "NRA");
    nc("UFLRA", "NIA");
    eq("UFLRA", "UFLRA");
    nc("UFLRA", "UFNIA");
    nc("UFLRA", "AUFLIA");
    lt("UFLRA", "AUFLIRA");
    lt("UFLRA", "AUFNIRA");

    gt("UFNIA", "QF_UF");
    nc("UFNIA", "QF_LRA");
    gt("UFNIA", "QF_LIA");
    nc("UFNIA", "QF_UFLRA");
    gt("UFNIA", "QF_UFLIA");
    nc("UFNIA", "QF_AUFLIA");
    nc("UFNIA", "QF_AUFLIRA");
    nc("UFNIA", "QF_BV");
    nc("UFNIA", "QF_ABV");
    nc("UFNIA", "QF_AUFBV");
    gt("UFNIA", "QF_IDL");
    nc("UFNIA", "QF_RDL");
    gt("UFNIA", "QF_UFIDL");
    gt("UFNIA", "QF_NIA");
    nc("UFNIA", "QF_NRA");
    nc("UFNIA", "QF_AUFNIRA");
    nc("UFNIA", "LRA");
    nc("UFNIA", "NRA");
    gt("UFNIA", "NIA");
    nc("UFNIA", "UFLRA");
    eq("UFNIA", "UFNIA");
    nc("UFNIA", "AUFLIA");
    nc("UFNIA", "AUFLIRA");
    lt("UFNIA", "AUFNIRA");

    gt("AUFLIA", "QF_UF");
    nc("AUFLIA", "QF_LRA");
    gt("AUFLIA", "QF_LIA");
    nc("AUFLIA", "QF_UFLRA");
    gt("AUFLIA", "QF_UFLIA");
    gt("AUFLIA", "QF_AUFLIA");
    nc("AUFLIA", "QF_AUFLIRA");
    nc("AUFLIA", "QF_BV");
    nc("AUFLIA", "QF_ABV");
    nc("AUFLIA", "QF_AUFBV");
    gt("AUFLIA", "QF_IDL");
    nc("AUFLIA", "QF_RDL");
    gt("AUFLIA", "QF_UFIDL");
    nc("AUFLIA", "QF_NIA");
    nc("AUFLIA", "QF_NRA");
    nc("AUFLIA", "QF_AUFNIRA");
    nc("AUFLIA", "LRA");
    nc("AUFLIA", "NRA");
    nc("AUFLIA", "NIA");
    nc("AUFLIA", "UFLRA");
    nc("AUFLIA", "UFNIA");
    eq("AUFLIA", "AUFLIA");
    lt("AUFLIA", "AUFLIRA");
    lt("AUFLIA", "AUFNIRA");

    gt("AUFLIRA", "QF_UF");
    gt("AUFLIRA", "QF_LRA");
    gt("AUFLIRA", "QF_LIA");
    gt("AUFLIRA", "QF_UFLRA");
    gt("AUFLIRA", "QF_UFLIA");
    gt("AUFLIRA", "QF_AUFLIA");
    gt("AUFLIRA", "QF_AUFLIRA");
    nc("AUFLIRA", "QF_BV");
    nc("AUFLIRA", "QF_ABV");
    nc("AUFLIRA", "QF_AUFBV");
    gt("AUFLIRA", "QF_IDL");
    gt("AUFLIRA", "QF_RDL");
    gt("AUFLIRA", "QF_UFIDL");
    nc("AUFLIRA", "QF_NIA");
    nc("AUFLIRA", "QF_NRA");
    nc("AUFLIRA", "QF_AUFNIRA");
    gt("AUFLIRA", "LRA");
    nc("AUFLIRA", "NRA");
    nc("AUFLIRA", "NIA");
    gt("AUFLIRA", "UFLRA");
    nc("AUFLIRA", "UFNIA");
    gt("AUFLIRA", "AUFLIA");
    eq("AUFLIRA", "AUFLIRA");
    lt("AUFLIRA", "AUFNIRA");

    gt("AUFNIRA", "QF_UF");
    gt("AUFNIRA", "QF_LRA");
    gt("AUFNIRA", "QF_LIA");
    gt("AUFNIRA", "QF_UFLRA");
    gt("AUFNIRA", "QF_UFLIA");
    gt("AUFNIRA", "QF_AUFLIA");
    gt("AUFNIRA", "QF_AUFLIRA");
    nc("AUFNIRA", "QF_BV");
    nc("AUFNIRA", "QF_ABV");
    nc("AUFNIRA", "QF_AUFBV");
    gt("AUFNIRA", "QF_IDL");
    gt("AUFNIRA", "QF_RDL");
    gt("AUFNIRA", "QF_UFIDL");
    gt("AUFNIRA", "QF_NIA");
    gt("AUFNIRA", "QF_NRA");
    gt("AUFNIRA", "QF_AUFNIRA");
    gt("AUFNIRA", "LRA");
    gt("AUFNIRA", "NRA");
    gt("AUFNIRA", "NIA");
    gt("AUFNIRA", "UFLRA");
    gt("AUFNIRA", "UFNIA");
    gt("AUFNIRA", "AUFLIA");
    gt("AUFNIRA", "AUFLIRA");
    eq("AUFNIRA", "AUFNIRA");
    lt("AUFNIRA", "AUFNIRAT");

    gt("QF_UFC", "QF_UF");
    gt("QF_UFCLRA", "QF_UFLRA");

    gt(ufHo, "QF_UF");
  }

};/* class LogicInfoWhite */
