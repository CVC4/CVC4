/*********************                                                        */
/*! \file word.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of utility functions for words.
 **/

#include "theory/strings/word.h"

#include "util/regexp.h"

using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace strings {
namespace word {

Node mkEmptyWord(Kind k)
{
  NodeManager* nm = NodeManager::currentNM();
  if (k == CONST_STRING)
  {
    std::vector<unsigned> vec;
    return nm->mkConst(String(vec));
  }
  Unimplemented();
  return Node::null();
}

Node mkWord(const std::vector<Node>& xs)
{
  Assert(!xs.empty());
  NodeManager* nm = NodeManager::currentNM();
  Kind k = xs[0].getKind();
  if (k == CONST_STRING)
  {
    std::vector<unsigned> vec;
    for (TNode x : xs)
    {
      Assert(x.getKind() == CONST_STRING);
      String sx = x.getConst<String>();
      const std::vector<unsigned>& vecc = sx.getVec();
      vec.insert(vec.end(), vecc.begin(), vecc.end());
    }
    return nm->mkConst(String(vec));
  }
  Unimplemented();
  return Node::null();
}

size_t getLength(TNode x)
{
  Kind k = x.getKind();
  if (k == CONST_STRING)
  {
    return x.getConst<String>().size();
  }
  Unimplemented();
  return 0;
}

bool isEmpty(TNode x) { return getLength(x) == 0; }

std::size_t find(TNode x, TNode y, std::size_t start)
{
  Kind k = x.getKind();
  if (k == CONST_STRING)
  {
    Assert(y.getKind() == CONST_STRING);
    String sx = x.getConst<String>();
    String sy = y.getConst<String>();
    return sx.find(sy, start);
  }
  Unimplemented();
  return 0;
}

std::size_t rfind(TNode x, TNode y, std::size_t start)
{
  Kind k = x.getKind();
  if (k == CONST_STRING)
  {
    Assert(y.getKind() == CONST_STRING);
    String sx = x.getConst<String>();
    String sy = y.getConst<String>();
    return sx.rfind(sy, start);
  }
  Unimplemented();
  return 0;
}

bool hasPrefix(TNode x, TNode y)
{
  Kind k = x.getKind();
  if (k == CONST_STRING)
  {
    Assert(y.getKind() == CONST_STRING);
    String sx = x.getConst<String>();
    String sy = y.getConst<String>();
    return sx.hasPrefix(sy);
  }
  Unimplemented();
  return false;
}

bool hasSuffix(TNode x, TNode y)
{
  Kind k = x.getKind();
  if (k == CONST_STRING)
  {
    Assert(y.getKind() == CONST_STRING);
    String sx = x.getConst<String>();
    String sy = y.getConst<String>();
    return sx.hasSuffix(sy);
  }
  Unimplemented();
  return false;
}

Node replace(TNode x, TNode y, TNode t)
{
  NodeManager* nm = NodeManager::currentNM();
  Kind k = x.getKind();
  if (k == CONST_STRING)
  {
    Assert(y.getKind() == CONST_STRING);
    Assert(t.getKind() == CONST_STRING);
    String sx = x.getConst<String>();
    String sy = y.getConst<String>();
    String st = t.getConst<String>();
    return nm->mkConst(String(sx.replace(sy, st)));
  }
  Unimplemented();
  return Node::null();
}
Node substr(TNode x, std::size_t i)
{
  NodeManager* nm = NodeManager::currentNM();
  Kind k = x.getKind();
  if (k == CONST_STRING)
  {
    String sx = x.getConst<String>();
    return nm->mkConst(String(sx.substr(i)));
  }
  Unimplemented();
  return Node::null();
}
Node substr(TNode x, std::size_t i, std::size_t j)
{
  NodeManager* nm = NodeManager::currentNM();
  Kind k = x.getKind();
  if (k == CONST_STRING)
  {
    String sx = x.getConst<String>();
    return nm->mkConst(String(sx.substr(i, j)));
  }
  Unimplemented();
  return Node::null();
}

Node prefix(TNode x, std::size_t i) { return substr(x, 0, i); }

Node suffix(TNode x, std::size_t i)
{
  NodeManager* nm = NodeManager::currentNM();
  Kind k = x.getKind();
  if (k == CONST_STRING)
  {
    String sx = x.getConst<String>();
    return nm->mkConst(String(sx.suffix(i)));
  }
  Unimplemented();
  return Node::null();
}

bool noOverlapWith(TNode x, TNode y)
{
  Kind k = x.getKind();
  if (k == CONST_STRING)
  {
    Assert(y.getKind() == CONST_STRING);
    String sx = x.getConst<String>();
    String sy = y.getConst<String>();
    return sx.noOverlapWith(sy);
  }
  Unimplemented();
  return false;
}

std::size_t overlap(TNode x, TNode y)
{
  Kind k = x.getKind();
  if (k == CONST_STRING)
  {
    Assert(y.getKind() == CONST_STRING);
    String sx = x.getConst<String>();
    String sy = y.getConst<String>();
    return sx.overlap(sy);
  }
  Unimplemented();
  return 0;
}

std::size_t roverlap(TNode x, TNode y)
{
  Kind k = x.getKind();
  if (k == CONST_STRING)
  {
    Assert(y.getKind() == CONST_STRING);
    String sx = x.getConst<String>();
    String sy = y.getConst<String>();
    return sx.roverlap(sy);
  }
  Unimplemented();
  return 0;
}

}  // namespace word
}  // namespace strings
}  // namespace theory
}  // namespace CVC4
