*/********************                                                        */
/*! \file sygus_enumerator.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief sygus_enumerator
 **/

#include "cvc4_private.h"

#ifndef __CVC4__THEORY__QUANTIFIERS__SYGUS_ENUMERATOR_H
#define __CVC4__THEORY__QUANTIFIERS__SYGUS_ENUMERATOR_H

#include <map>
#include <unordered_set>
#include "theory/quantifiers/sygus/term_database_sygus.h"
#include "theory/quantifiers/sygus/synth_conjecture.h"

namespace CVC4 {
namespace theory {
namespace quantifiers {

/** SygusEnumerator
 * 
 */
class SygusEnumerator : public EnumValGenerator
{
public:
  SygusEnumerator(TermDbSygus* tds);
  ~SygusEnumerator(){}
  /** initialize this class with enumerator e */
  void initialize(Node e) override;
  /** Inform this generator that abstract value v was enumerated. */
  void addValue(Node v) override;
  /** Get the next concrete value generated by this class. */
  Node getNext() override;
private:
  /** pointer to term database sygus */
  TermDbSygus * d_tds;
class TermCache
{
public:
  TermCache();
  /** initialize this cache */
  void initialize(TypeNode tn, TermDbSygus * tds);
  /** 
   * Add sygus term n to this cache, return true if the term was unique based
   * on the redundancy criteria used by this class. 
   */
  bool addTerm(Node n);
  /** 
   * Indicate to this cache that we are finished enumerating terms of the
   * current size 
   */
  void pushEnumSize();
  /** Get the current size of terms that we are enumerating */
  unsigned getEnumSize() { return d_sizeEnum; }
  /** get the index at which size s terms start */
  unsigned getIndexForSize( unsigned s ) const;
private:
  /** the sygus type of terms in this cache */
  TypeNode d_type;
  /** pointer to term database sygus */
  TermDbSygus * d_tds;
  /** the list of sygus terms we have enumerated */
std::vector< Node > d_terms;
/** the set of builtin terms corresponding to the above list */
std::unordered_set< Node, NodeHashFunction > d_bterms;
/** the index of first term of each size, if it exists */
std::map< unsigned, unsigned > d_lastSizeIndex;
/** the maximum size of terms we have stored in this cache so far */
unsigned d_sizeEnum;
};
/** above cache for each sygus type */
std::map< TypeNode, TermCache > d_tcache;
/** initialize term cache for type tn */
void initializeTermCache( TypeNode tn );

class TermEnum
{
public:
TermEnum();
void initialize(SygusEnumerator * se, TypeNode tn, unsigned sizeLim, bool sizeExact);
Node getCurrent();
unsigned getCurrentSize();
bool increment();
private:
  /** pointer to the sygus enumerator class */
SygusEnumerator * d_se;
/** are we a "master" enumerator?
 * 
 */
bool d_isMaster;
/** the (sygus) type of terms we are enumerating */
TypeNode d_type;
/** the current size of terms we are enumerating */
unsigned d_currSize;

//----------------------------------------------- for master enumerators
/** the current constructor we are using */
unsigned d_consNum;
/** the child enumerators for this enumerator */
std::map< unsigned, TermEnum > d_children;
//----------------------------------------------- end for master enumerators
//----------------------------------------------- for non-master enumerators
/** the current index in the term cache we are considering */
unsigned d_index;
/** the end index in the term cache */
unsigned d_indexEnd;
//----------------------------------------------- end for non-master enumerators
};
/** the top-level enumerator for this class */
//TermEnum d_enum;
};

} /* CVC4::theory::quantifiers namespace */
} /* CVC4::theory namespace */
} /* CVC4 namespace */

#endif /* __CVC4__THEORY__QUANTIFIERS__SYGUS_ENUMERATOR_H */
