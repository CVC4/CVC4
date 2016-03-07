/*********************                                                        */
/*! \file clause_id.h
 ** \verbatim
 ** Original author: Tim King
 ** Major contributors: none
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2016  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Definition of ClauseId
 **
 ** A ClauseId is a shared identifier between the proofs module and the sat
 ** solver for a clause.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__PROOF__CLAUSE_ID_H
#define __CVC4__PROOF__CLAUSE_ID_H

namespace CVC4 {

/**
 * A ClauseId is a shared identifier between the proofs module and the sat
 * solver for a clause.
 */
typedef unsigned ClauseId;

}/* CVC4 namespace */

#endif /* __CVC4__PROOF__CLAUSE_ID_H */
