/*********************                                                        */
/*! \file justify_stack.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Justification stack
 **/

#include "decision/justify_stack.h"

namespace CVC4 {

JustifyStack::JustifyStack(context::Context* c)
    : d_context(c), d_current(c), d_stack(c), d_stackSizeValid(c, 0)
{
}
JustifyStack::~JustifyStack() {}

TNode JustifyStack::getCurrentAssertion() const { return d_current.get(); }
bool JustifyStack::hasCurrentAssertion() const
{
  return !d_current.get().isNull();
}
JustifyInfo* JustifyStack::getCurrent()
{
  Assert(d_stack.size() >= d_stackSizeValid.get());
  return d_stack[d_stackSizeValid.get() - 1].get();
}

void JustifyStack::pushToStack(TNode n, prop::SatValue desiredVal)
{
  if (Trace.isOn("jh-stack"))
  {
    for (size_t i = 0, ssize = d_stackSizeValid.get(); i < ssize; i++)
    {
      Trace("jh-stack") << " ";
    }
    Trace("jh-stack") << "- " << n << " " << desiredVal << std::endl;
  }
  // note that n is possibly negated here
  JustifyInfo* ji = getOrAllocJustifyInfo(d_stackSizeValid.get());
  ji->set(n, desiredVal);
  d_stackSizeValid = d_stackSizeValid + 1;
}

void JustifyStack::popStack()
{
  Assert(d_stackSizeValid.get() > 0);
  d_stackSizeValid = d_stackSizeValid - 1;
}

JustifyInfo* JustifyStack::getOrAllocJustifyInfo(size_t i)
{
  // don't request stack beyond the bound
  Assert(i <= d_stack.size());
  if (i == d_stack.size())
  {
    d_stack.push_back(std::make_shared<JustifyInfo>(d_context));
  }
  return d_stack[i].get();
}

}  // namespace CVC4
