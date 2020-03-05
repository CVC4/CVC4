/*********************                                                        */
/*! \file sequence.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of the sequence data type.
 **/

#include "expr/sequence.h"

using namespace std;

namespace CVC4 {

Sequence::Sequence(TypeNode t, const std::vector<Node>& s) : d_type(t), d_seq(s)
{
}

int Sequence::cmp(const Sequence& y) const
{
  if (size() != y.size())
  {
    return size() < y.size() ? -1 : 1;
  }
  for (size_t i = 0, sz = size(); i < sz; ++i)
  {
    if (d_seq[i] != y.d_seq[i])
    {
      return d_seq[i] < y.d_seq[i] ? -1 : 1;
    }
  }
  return 0;
}

Sequence Sequence::concat(const Sequence& other) const
{
  Assert(d_type == other.d_type);
  std::vector<Node> ret_vec(d_seq);
  ret_vec.insert(ret_vec.end(), other.d_seq.begin(), other.d_seq.end());
  return Sequence(d_type, ret_vec);
}

bool Sequence::strncmp(const Sequence& y, std::size_t n) const
{
  std::size_t b = (size() >= y.size()) ? size() : y.size();
  std::size_t s = (size() <= y.size()) ? size() : y.size();
  if (n > s)
  {
    if (b != s)
    {
      return false;
    }
    n = s;
  }
  for (std::size_t i = 0; i < n; ++i)
  {
    if (d_seq[i] != y.d_seq[i])
    {
      return false;
    }
  }
  return true;
}

bool Sequence::rstrncmp(const Sequence& y, std::size_t n) const
{
  std::size_t b = (size() >= y.size()) ? size() : y.size();
  std::size_t s = (size() <= y.size()) ? size() : y.size();
  if (n > s)
  {
    if (b != s)
    {
      return false;
    }
    n = s;
  }
  for (std::size_t i = 0; i < n; ++i)
  {
    if (d_seq[size() - i - 1] != y.d_seq[y.size() - i - 1])
    {
      return false;
    }
  }
  return true;
}

Node Sequence::front() const
{
  Assert(!d_seq.empty());
  return d_seq.front();
}

Node Sequence::back() const
{
  Assert(!d_seq.empty());
  return d_seq.back();
}

std::size_t Sequence::overlap(const Sequence& y) const
{
  std::size_t i = size() < y.size() ? size() : y.size();
  for (; i > 0; i--)
  {
    Sequence s = suffix(i);
    Sequence p = y.prefix(i);
    if (s == p)
    {
      return i;
    }
  }
  return i;
}

std::size_t Sequence::roverlap(const Sequence& y) const
{
  std::size_t i = size() < y.size() ? size() : y.size();
  for (; i > 0; i--)
  {
    Sequence s = prefix(i);
    Sequence p = y.suffix(i);
    if (s == p)
    {
      return i;
    }
  }
  return i;
}

bool Sequence::isRepeated() const
{
  if (size() > 1)
  {
    Node f = d_seq[0];
    for (unsigned i = 1, sz = size(); i < sz; ++i)
    {
      if (f != d_seq[i])
      {
        return false;
      }
    }
  }
  return true;
}

bool Sequence::tailcmp(const Sequence& y, int& c) const
{
  int id_x = size() - 1;
  int id_y = y.size() - 1;
  while (id_x >= 0 && id_y >= 0)
  {
    if (d_seq[id_x] != y.d_seq[id_y])
    {
      c = id_x;
      return false;
    }
    --id_x;
    --id_y;
  }
  c = id_x == -1 ? (-(id_y + 1)) : (id_x + 1);
  return true;
}

std::size_t Sequence::find(const Sequence& y, const std::size_t start) const
{
  if (size() < y.size() + start)
  {
    return std::string::npos;
  }
  if (y.empty())
  {
    return start;
  }
  if (empty())
  {
    return std::string::npos;
  }
  std::vector<Node>::const_iterator itr = std::search(
      d_seq.begin() + start, d_seq.end(), y.d_seq.begin(), y.d_seq.end());
  if (itr != d_seq.end())
  {
    return itr - d_seq.begin();
  }
  return std::string::npos;
}

std::size_t Sequence::rfind(const Sequence& y, const std::size_t start) const
{
  if (size() < y.size() + start)
  {
    return std::string::npos;
  }
  if (y.empty())
  {
    return start;
  }
  if (empty())
  {
    return std::string::npos;
  }
  std::vector<Node>::const_reverse_iterator itr = std::search(
      d_seq.rbegin() + start, d_seq.rend(), y.d_seq.rbegin(), y.d_seq.rend());
  if (itr != d_seq.rend())
  {
    return itr - d_seq.rbegin();
  }
  return std::string::npos;
}

bool Sequence::hasPrefix(const Sequence& y) const
{
  size_t s = size();
  size_t ys = y.size();
  if (ys > s)
  {
    return false;
  }
  for (size_t i = 0; i < ys; i++)
  {
    if (d_seq[i] != y.d_seq[i])
    {
      return false;
    }
  }
  return true;
}

bool Sequence::hasSuffix(const Sequence& y) const
{
  size_t s = size();
  size_t ys = y.size();
  if (ys > s)
  {
    return false;
  }
  size_t idiff = s - ys;
  for (size_t i = 0; i < ys; i++)
  {
    if (d_seq[i + idiff] != y.d_seq[i])
    {
      return false;
    }
  }
  return true;
}

Sequence Sequence::replace(const Sequence& s, const Sequence& t) const
{
  Assert(d_type == s.d_type);
  Assert(d_type == t.d_type);
  std::size_t ret = find(s);
  if (ret != std::string::npos)
  {
    std::vector<Node> vec;
    vec.insert(vec.begin(), d_seq.begin(), d_seq.begin() + ret);
    vec.insert(vec.end(), t.d_seq.begin(), t.d_seq.end());
    vec.insert(vec.end(), d_seq.begin() + ret + s.size(), d_seq.end());
    return Sequence(d_type, vec);
  }
  return *this;
}

Sequence Sequence::substr(std::size_t i) const
{
  Assert(i <= size());
  std::vector<Node> ret_vec;
  std::vector<Node>::const_iterator itr = d_seq.begin() + i;
  ret_vec.insert(ret_vec.end(), itr, d_seq.end());
  return Sequence(d_type, ret_vec);
}

Sequence Sequence::substr(std::size_t i, std::size_t j) const
{
  Assert(i + j <= size());
  std::vector<Node> ret_vec;
  std::vector<Node>::const_iterator itr = d_seq.begin() + i;
  ret_vec.insert(ret_vec.end(), itr, itr + j);
  return Sequence(d_type, ret_vec);
}

bool Sequence::noOverlapWith(const Sequence& y) const
{
  Assert(d_type == y.d_type);
  return y.find(*this) == std::string::npos
         && this->find(y) == std::string::npos && this->overlap(y) == 0
         && y.overlap(*this) == 0;
}

size_t Sequence::maxSize() { return std::numeric_limits<uint32_t>::max(); }

std::ostream& operator<<(std::ostream& os, const Sequence& s)
{
  // FIXME
  return os << "\""
            << "\"";
}

}  // namespace CVC4
