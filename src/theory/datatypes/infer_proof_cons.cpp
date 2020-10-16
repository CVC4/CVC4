/*********************                                                        */
/*! \file infer_proof_cons.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of inference to proof conversion  for datatypes
 **/

#include "theory/datatypes/infer_proof_cons.h"

#include "theory/rewriter.h"

using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace datatypes {

InferProofCons::InferProofCons(context::Context* c, ProofNodeManager* pnm)
    : d_pnm(pnm), d_lazyFactMap(c)
{
  Assert(d_pnm != nullptr);
}

void InferProofCons::notifyFact(const DatatypesInference& di)
{
  Node fact = di.d_conc;
  if (d_lazyFactMap.find(fact) != d_lazyFactMap.end())
  {
    return;
  }
  Node symFact = CDProof::getSymmFact(fact);
  if (!symFact.isNull() && d_lazyFactMap.find(symFact) != d_lazyFactMap.end())
  {
    return;
  }
  std::shared_ptr<DatatypesInference> dic =
      std::make_shared<DatatypesInference>(di);
  d_lazyFactMap.insert(di.d_conc, dic);
}

void InferProofCons::convert(InferId infer, Node conc, Node exp, CDProof* cdp)
{
}

std::shared_ptr<ProofNode> InferProofCons::getProofFor(Node fact)
{
  // temporary proof
  CDProof pf(d_pnm);
  // get the inference
  NodeDatatypesInferenceMap::iterator it = d_lazyFactMap.find(fact);
  if (it == d_lazyFactMap.end())
  {
    Node factSym = CDProof::getSymmFact(fact);
    if (!factSym.isNull())
    {
      // Use the symmetric fact. There is no need to explictly make a
      // SYMM proof, as this is handled by CDProof::getProofFor below.
      it = d_lazyFactMap.find(factSym);
    }
  }
  AlwaysAssert(it != d_lazyFactMap.end());
  // now go back and convert it to proof steps and add to proof
  std::shared_ptr<DatatypesInference> di = (*it).second;
  // run the conversion
  convert(di->getInferId(), di->d_conc, di->d_exp, &pf);
  return pf.getProofFor(fact);
}

std::string InferProofCons::identify() const
{
  return "datatypes::InferProofCons";
}

}  // namespace datatypes
}  // namespace theory
}  // namespace CVC4
