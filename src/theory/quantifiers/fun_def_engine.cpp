/*********************                                                        */
/*! \file fun_def_process.cpp
 ** \verbatim
 ** Original author: Andrew Reynolds
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2014  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** This class implements specialized techniques for (recursively) defined functions
 **/

#include <vector>

#include "theory/quantifiers/fun_def_engine.h"
#include "theory/rewriter.h"
#include "theory/quantifiers/term_database.h"

using namespace CVC4;
using namespace std;
using namespace CVC4::theory;
using namespace CVC4::theory::quantifiers;
using namespace CVC4::kind;

FunDefEngine::FunDefEngine( QuantifiersEngine * qe, context::Context* c ) : QuantifiersModule( qe ) {
  
}

/* whether this module needs to check this round */
bool FunDefEngine::needsCheck( Theory::Effort e ) { 
  return e>=Theory::EFFORT_LAST_CALL; 
}

/* reset at a round */
void FunDefEngine::reset_round( Theory::Effort e ){
  //TODO
}

/* Call during quantifier engine's check */
void FunDefEngine::check( Theory::Effort e, unsigned quant_e ) {
  //TODO
}

/* Called for new quantifiers */
void FunDefEngine::registerQuantifier( Node q ) {
  //TODO
}

/** called for everything that gets asserted */
void FunDefEngine::assertNode( Node n ) {
  //TODO
}
