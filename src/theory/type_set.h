/*********************                                                        */
/*! \file type_set.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Type set class
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__TYPE_SET_H
#define CVC4__THEORY__TYPE_SET_H

#include <unordered_map>
#include <unordered_set>

#include "theory/type_enumerator.h"

namespace CVC4 {
namespace theory {

/* Type set
 *
 * This class encapsulates a map from types to sets of nodes.
 */
class TypeSet
{
 public:
  typedef std::unordered_map<TypeNode, std::set<Node>*, TypeNodeHashFunction>
      TypeSetMap;
  typedef std::unordered_map<TypeNode, TypeEnumerator*, TypeNodeHashFunction>
      TypeToTypeEnumMap;
  typedef TypeSetMap::iterator iterator;
  typedef TypeSetMap::const_iterator const_iterator;

 public:
  TypeSet() : d_tep(NULL) {}
  ~TypeSet();
  /** set the properties of the type set
   *
   * These indicate information such as finite bounds
   * on the number of unique uninterpreted constants that can be
   * enumerated by this class.
   */
  void setTypeEnumeratorProperties(TypeEnumeratorProperties* tep);
  /** add node n to the set of values of t */
  void add(TypeNode t, TNode n);
  /** get the set of values of type t */
  std::set<Node>* getSet(TypeNode t) const;
  /** get the next enumerated term of type t
   *
   * useBaseType is whether
   */
  Node nextTypeEnum(TypeNode t, bool useBaseType = false);

  bool empty() { return d_typeSet.empty(); }
  iterator begin() { return d_typeSet.begin(); }
  iterator end() { return d_typeSet.end(); }
  static TypeNode getType(iterator it) { return (*it).first; }
  static std::set<Node>& getSet(iterator it) { return *(*it).second; }
 private:
  /** sets of values for each type */
  TypeSetMap d_typeSet;
  /** type enumerators for each type */
  TypeToTypeEnumMap d_teMap;
  /** pointer the type enumerator properties for this class. */
  TypeEnumeratorProperties* d_tep;

  /* add all sub-terms of n to the sets of this class
   *
   * If topLevel is true, then we add strict subterms only.
   *
   * Note that recursive traversal here is over enumerated expressions
   * (very low expression depth).
   */
  void addSubTerms(TNode n,
                   std::unordered_set<TNode, TNodeHashFunction>& visited,
                   bool topLevel = true);
}; /* class TypeSet */

} /* CVC4::theory namespace */
} /* CVC4 namespace */

#endif /* CVC4__THEORY__TYPE_SET_H */
