/*********************                                                        */
/*! \file skolem_cache.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief A cache of skolems for theory of strings.
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__STRINGS__SKOLEM_CACHE_H
#define __CVC4__THEORY__STRINGS__SKOLEM_CACHE_H

#include <vector>
#include "context/cdhashmap.h"
#include "theory/rewriter.h"
#include "theory/theory.h"
#include "util/hash.h"

namespace CVC4 {
namespace theory {
namespace strings {

/**
 * A cache of all string skolems generated by the TheoryStrings class. This
 * cache is used to ensure that duplicate skolems are not generated when
 * possible, and helps identify what skolems were allocated in the current run.
 */
class SkolemCache
{
 public:
  SkolemCache();
  /** Identifiers for skolem types 
   *
   * The comments below document the properties of each skolem introduced by
   * inference in the strings solver, where by skolem we mean the fresh
   * string variable that witnesses each of "exists k".
   * 
   * The skolems with _REV suffixes are used for the reverse version of the
   * preconditions below, e.g. where we are considering a' ++ a = b' ++ b.
   */
  enum SkolemId
  {
    // a != "" ^ b = "ccccd" ^ a ++ "d" ++ a' = b ++ b' =>
    //    exists k. a = "cccc" + k
    SK_ID_C_SPT,
    SK_ID_C_SPT_REV,
    // a != "" ^ b = "c" ^ len(a)!=len(b) ^ a ++ a' = b ++ b' =>
    //    exists k. a = "c" ++ k
    SK_ID_VC_SPT,
    SK_ID_VC_SPT_REV,
    // a != "" ^ b = "cccccccc" ^ len(a)!=len(b) a ++ a' = b = b' =>
    //    exists k. a = "cccc" ++ k OR ( len(k) > 0 ^ "cccc" = a ++ k )
    SK_ID_VC_BIN_SPT,
    SK_ID_VC_BIN_SPT_REV,
    // a != "" ^ b != "" ^ len(a)!=len(b) ^ a ++ a' = b ++ b' =>
    //    exists k. len( k )>0 ^ ( a ++ k = b OR a = b ++ k )
    SK_ID_V_SPT,
    SK_ID_V_SPT_REV,
    // contains( a, b ) =>
    //    exists k_pre, k_post. a = k_pre ++ b ++ k_post
    SK_ID_CTN_PRE,
    SK_ID_CTN_POST,
    // a != ""  ^ b = "c" ^ a ++ a' != b ++ b' =>
    //    exists k, k_rem. 
    //         len( k ) = 1 ^ 
    //         ( ( a = k ++ k_rem ^ k != "c" ) OR ( a = "c" ++ k_rem ) )
    SK_ID_DC_SPT,
    SK_ID_DC_SPT_REM,
    // a != ""  ^ b != "" ^ len( a ) != len( b ) ^ a ++ a' != b ++ b' =>
    //    exists k_x k_y k_z.
    //         ( len( k_y ) = len( a ) ^ len( k_x ) = len( b ) ^ len( k_z) > 0
    //           ( a = k_x ++ k_z OR b = k_y ++ k_z ) ) 
    SK_ID_DEQ_X,
    SK_ID_DEQ_Y,
    SK_ID_DEQ_Z,
  };
  /**
   * Returns a skolem of type string that is cached for (a,b,id) and has
   * name c.
   */
  Node mkSkolemCached(Node a, Node b, SkolemId id, const char* c);
  /** Returns a (uncached) skolem of type string with name c */
  Node mkSkolem(const char* c);
  /** Returns true if n is a skolem allocated by this class */
  bool isSkolem(Node n) const;

 private:
  /** map from node pairs and identifiers to skolems */
  std::map<Node, std::map<Node, std::map<SkolemId, Node> > > d_skolem_cache;
  /** the set of all skolems we have generated */
  std::unordered_set<Node, NodeHashFunction> d_all_skolems;
};

}  // namespace strings
}  // namespace theory
}  // namespace CVC4

#endif /* __CVC4__THEORY__STRINGS__SKOLEM_CACHE_H */
