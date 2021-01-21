/*********************                                                        */
/*! \file inst_match_generator.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Morgan Deters, Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/
#include "theory/quantifiers/ematching/inst_match_generator.h"

#include "options/datatypes_options.h"
#include "options/quantifiers_options.h"
#include "theory/datatypes/theory_datatypes_utils.h"
#include "theory/quantifiers/ematching/candidate_generator.h"
#include "theory/quantifiers/ematching/trigger.h"
#include "theory/quantifiers/instantiate.h"
#include "theory/quantifiers/term_database.h"
#include "theory/quantifiers/term_util.h"
#include "theory/quantifiers_engine.h"

using namespace std;
using namespace CVC4;
using namespace CVC4::kind;
using namespace CVC4::context;
using namespace CVC4::theory;

namespace CVC4 {
namespace theory {
namespace inst {

bool IMGenerator::sendInstantiation(Trigger* tparent, InstMatch& m)
{
  return tparent->sendInstantiation(m);
}

InstMatchGenerator::InstMatchGenerator( Node pat ){
  d_cg = nullptr;
  d_needsReset = true;
  d_active_add = true;
  Assert(quantifiers::TermUtil::hasInstConstAttr(pat));
  d_pattern = pat;
  d_match_pattern = pat;
  d_match_pattern_type = pat.getType();
  d_next = nullptr;
  d_independent_gen = false;
}

InstMatchGenerator::InstMatchGenerator() {
  d_cg = nullptr;
  d_needsReset = true;
  d_active_add = true;
  d_next = nullptr;
  d_independent_gen = false;
}

InstMatchGenerator::~InstMatchGenerator()
{
  for( unsigned i=0; i<d_children.size(); i++ ){
    delete d_children[i];
  }
  delete d_cg;
}

void InstMatchGenerator::setActiveAdd(bool val){
  d_active_add = val;
  if (d_next != nullptr)
  {
    d_next->setActiveAdd(val);
  }
}

int InstMatchGenerator::getActiveScore( QuantifiersEngine * qe ) {
  if( d_match_pattern.isNull() ){
    return -1;
  }else if( Trigger::isAtomicTrigger( d_match_pattern ) ){
    Node f = qe->getTermDatabase()->getMatchOperator( d_match_pattern );
    unsigned ngt = qe->getTermDatabase()->getNumGroundTerms( f );
    Trace("trigger-active-sel-debug") << "Number of ground terms for " << f << " is " << ngt << std::endl;
    return ngt;
  }else if( d_match_pattern.getKind()==INST_CONSTANT ){
    TypeNode tn = d_match_pattern.getType();
    unsigned ngtt = qe->getTermDatabase()->getNumTypeGroundTerms( tn );
    Trace("trigger-active-sel-debug") << "Number of ground terms for " << tn << " is " << ngtt << std::endl;
    return ngtt;
  }
  return -1;
}

void InstMatchGenerator::initialize(Node q,
                                    QuantifiersEngine* qe,
                                    std::vector<InstMatchGenerator*>& gens)
{
  if (d_pattern.isNull())
  {
    gens.insert( gens.end(), d_children.begin(), d_children.end() );
    return;
  }
  Trace("inst-match-gen") << "Initialize, pattern term is " << d_pattern
                          << std::endl;
  if (d_match_pattern.getKind() == NOT)
  {
    Assert(d_pattern.getKind() == NOT);
    // we want to add the children of the NOT
    d_match_pattern = d_match_pattern[0];
  }

  if (d_pattern.getKind() == NOT && d_match_pattern.getKind() == EQUAL
      && d_match_pattern[0].getKind() == INST_CONSTANT
      && d_match_pattern[1].getKind() == INST_CONSTANT)
  {
    // special case: disequalities between variables x != y will match ground
    // disequalities.
  }
  else if (d_match_pattern.getKind() == EQUAL
           || d_match_pattern.getKind() == GEQ)
  {
    // We are one of the following cases:
    //   f(x)~a, f(x)~y, x~a, x~y
    // If we are the first or third case, we ensure that f(x)/x is on the left
    // hand side of the relation d_pattern, d_match_pattern is f(x)/x and
    // d_eq_class_rel (indicating the equivalence class that we are related
    // to) is set to a.
    for (size_t i = 0; i < 2; i++)
    {
      Node mp = d_match_pattern[i];
      Node mpo = d_match_pattern[1 - i];
      // If this side has free variables, and the other side does not or
      // it is a free variable, then we will match on this side of the
      // relation.
      if (quantifiers::TermUtil::hasInstConstAttr(mp)
          && (!quantifiers::TermUtil::hasInstConstAttr(mpo)
              || mpo.getKind() == INST_CONSTANT))
      {
        if (i == 1)
        {
          if (d_match_pattern.getKind() == GEQ)
          {
            d_pattern = NodeManager::currentNM()->mkNode(kind::GT, mp, mpo);
            d_pattern = d_pattern.negate();
          }
          else
          {
            d_pattern = NodeManager::currentNM()->mkNode(
                d_match_pattern.getKind(), mp, mpo);
          }
        }
        d_eq_class_rel = mpo;
        d_match_pattern = mp;
        // we won't find a term in the other direction
        break;
      }
    }
  }
  d_match_pattern_type = d_match_pattern.getType();
  Trace("inst-match-gen") << "Pattern is " << d_pattern << ", match pattern is "
                          << d_match_pattern << std::endl;
  d_match_pattern_op = qe->getTermDatabase()->getMatchOperator(d_match_pattern);

  // now, collect children of d_match_pattern
  Kind mpk = d_match_pattern.getKind();
  if (mpk == INST_CONSTANT)
  {
    d_children_types.push_back(
        d_match_pattern.getAttribute(InstVarNumAttribute()));
  }
  else
  {
    for (size_t i = 0, size = d_match_pattern.getNumChildren(); i < size; i++)
    {
      Node pat = d_match_pattern[i];
      Node qa = quantifiers::TermUtil::getInstConstAttr(pat);
      if (!qa.isNull())
      {
        if (pat.getKind() == INST_CONSTANT && qa == q)
        {
          d_children_types.push_back(pat.getAttribute(InstVarNumAttribute()));
        }
        else
        {
          InstMatchGenerator* cimg = getInstMatchGenerator(q, pat);
          if (cimg)
          {
            d_children.push_back(cimg);
            d_children_index.push_back(i);
            d_children_types.push_back(-2);
          }
          else
          {
            d_children_types.push_back(-1);
          }
        }
      }
      else
      {
        d_children_types.push_back(-1);
      }
    }
  }

  // create candidate generator
  if (mpk == APPLY_SELECTOR)
  {
    // candidates for apply selector are a union of correctly and incorrectly
    // applied selectors
    d_cg = new inst::CandidateGeneratorSelector(qe, d_match_pattern);
  }
  else if (Trigger::isAtomicTriggerKind(mpk))
  {
    if (mpk == APPLY_CONSTRUCTOR)
    {
      // 1-constructors have a trivial way of generating candidates in a
      // given equivalence class
      const DType& dt = d_match_pattern.getType().getDType();
      if (dt.getNumConstructors() == 1)
      {
        d_cg = new inst::CandidateGeneratorConsExpand(qe, d_match_pattern);
      }
    }
    if (d_cg == nullptr)
    {
      CandidateGeneratorQE* cg = new CandidateGeneratorQE(qe, d_match_pattern);
      // we will be scanning lists trying to find ground terms whose operator
      // is the same as d_match_operator's.
      d_cg = cg;
      // if matching on disequality, inform the candidate generator not to
      // match on eqc
      if (d_pattern.getKind() == NOT && d_pattern[0].getKind() == EQUAL)
      {
        cg->excludeEqc(d_eq_class_rel);
        d_eq_class_rel = Node::null();
      }
    }
  }
  else if (mpk == INST_CONSTANT)
  {
    if (d_pattern.getKind() == APPLY_SELECTOR_TOTAL)
    {
      Node selectorExpr = qe->getTermDatabase()->getMatchOperator(d_pattern);
      size_t selectorIndex = datatypes::utils::cindexOf(selectorExpr);
      const DType& dt = datatypes::utils::datatypeOf(selectorExpr);
      const DTypeConstructor& c = dt[selectorIndex];
      Node cOp = c.getConstructor();
      Trace("inst-match-gen")
          << "Purify dt trigger " << d_pattern << ", will match terms of op "
          << cOp << std::endl;
      d_cg = new inst::CandidateGeneratorQE(qe, cOp);
    }else{
      d_cg = new CandidateGeneratorQEAll(qe, d_match_pattern);
    }
  }
  else if (mpk == EQUAL)
  {
    // we will be producing candidates via literal matching heuristics
    if (d_pattern.getKind() == NOT)
    {
      // candidates will be all disequalities
      d_cg = new inst::CandidateGeneratorQELitDeq(qe, d_match_pattern);
    }
  }
  else
  {
    Trace("inst-match-gen-warn")
        << "(?) Unknown matching pattern is " << d_match_pattern << std::endl;
  }
  gens.insert( gens.end(), d_children.begin(), d_children.end() );
}

/** get match (not modulo equality) */
int InstMatchGenerator::getMatch(
    Node f, Node t, InstMatch& m, QuantifiersEngine* qe, Trigger* tparent)
{
  Trace("matching") << "Matching " << t << " against pattern " << d_match_pattern << " ("
                    << m << ")" << ", " << d_children.size() << ", pattern is " << d_pattern << std::endl;
  Assert(!d_match_pattern.isNull());
  if (d_cg == nullptr)
  {
    Trace("matching-fail") << "Internal error for match generator." << std::endl;
    return -2;
  }
  EqualityQuery* q = qe->getEqualityQuery();
  bool success = true;
  std::vector<int> prev;
  // if t is null
  Assert(!t.isNull());
  Assert(!quantifiers::TermUtil::hasInstConstAttr(t));
  // notice that t may have a different kind or operator from our match
  // pattern, e.g. for APPLY_SELECTOR triggers.
  // first, check if ground arguments are not equal, or a match is in conflict
  Trace("matching-debug2") << "Setting immediate matches..." << std::endl;
  for (size_t i = 0, size = d_match_pattern.getNumChildren(); i < size; i++)
  {
    int ct = d_children_types[i];
    if (ct >= 0)
    {
      Trace("matching-debug2")
          << "Setting " << ct << " to " << t[i] << "..." << std::endl;
      bool addToPrev = m.get(ct).isNull();
      if (!m.set(q, ct, t[i]))
      {
        // match is in conflict
        Trace("matching-fail")
            << "Match fail: " << m.get(ct) << " and " << t[i] << std::endl;
        success = false;
        break;
      }
      else if (addToPrev)
      {
        Trace("matching-debug2") << "Success." << std::endl;
        prev.push_back(ct);
      }
    }
    else if (ct == -1)
    {
      if (!q->areEqual(d_match_pattern[i], t[i]))
      {
        Trace("matching-fail") << "Match fail arg: " << d_match_pattern[i]
                               << " and " << t[i] << std::endl;
        // ground arguments are not equal
        success = false;
        break;
      }
    }
  }
  Trace("matching-debug2") << "Done setting immediate matches, success = "
                           << success << "." << std::endl;
  // for variable matching
  if (d_match_pattern.getKind() == INST_CONSTANT)
  {
    bool addToPrev = m.get(d_children_types[0]).isNull();
    if (!m.set(q, d_children_types[0], t))
    {
      success = false;
    }
    else
    {
      if (addToPrev)
      {
        prev.push_back(d_children_types[0]);
      }
    }
  }
  // for relational matching
  if (!d_eq_class_rel.isNull() && d_eq_class_rel.getKind() == INST_CONSTANT)
  {
    NodeManager* nm = NodeManager::currentNM();
    int v = d_eq_class_rel.getAttribute(InstVarNumAttribute());
    // also must fit match to equivalence class
    bool pol = d_pattern.getKind() != NOT;
    Node pat = d_pattern.getKind() == NOT ? d_pattern[0] : d_pattern;
    Node t_match;
    if (pol)
    {
      if (pat.getKind() == GT)
      {
        t_match = nm->mkNode(MINUS, t, nm->mkConst(Rational(1)));
      }else{
        t_match = t;
      }
    }
    else
    {
      if (pat.getKind() == EQUAL)
      {
        if (t.getType().isBoolean())
        {
          t_match = nm->mkConst(!q->areEqual(nm->mkConst(true), t));
        }
        else
        {
          Assert(t.getType().isReal());
          t_match = nm->mkNode(PLUS, t, nm->mkConst(Rational(1)));
        }
      }
      else if (pat.getKind() == GEQ)
      {
        t_match = nm->mkNode(PLUS, t, nm->mkConst(Rational(1)));
      }
      else if (pat.getKind() == GT)
      {
        t_match = t;
      }
    }
    if (!t_match.isNull())
    {
      bool addToPrev = m.get(v).isNull();
      if (!m.set(q, v, t_match))
      {
        success = false;
      }
      else if (addToPrev)
      {
        prev.push_back(v);
      }
    }
  }
  int ret_val = -1;
  if (success)
  {
    Trace("matching-debug2") << "Reset children..." << std::endl;
    // now, fit children into match
    // we will be requesting candidates for matching terms for each child
    for (size_t i = 0, size = d_children.size(); i < size; i++)
    {
      if (!d_children[i]->reset(t[d_children_index[i]], qe))
      {
        success = false;
        break;
      }
    }
    if (success)
    {
      Trace("matching-debug2") << "Continue next " << d_next << std::endl;
      ret_val = continueNextMatch(f, m, qe, tparent);
    }
  }
  if (ret_val < 0)
  {
    for (int& pv : prev)
    {
      m.d_vals[pv] = Node::null();
    }
  }
  return ret_val;
}

int InstMatchGenerator::continueNextMatch(Node q,
                                          InstMatch& m,
                                          QuantifiersEngine* qe,
                                          Trigger* tparent)
{
  if( d_next!=NULL ){
    return d_next->getNextMatch(q, m, qe, tparent);
  }
  if (d_active_add)
  {
    return sendInstantiation(tparent, m) ? 1 : -1;
  }
  return 1;
}

/** reset instantiation round */
void InstMatchGenerator::resetInstantiationRound( QuantifiersEngine* qe ){
  if( !d_match_pattern.isNull() ){
    Trace("matching-debug2") << this << " reset instantiation round." << std::endl;
    d_needsReset = true;
    if( d_cg ){
      d_cg->resetInstantiationRound();
    }
  }
  if( d_next ){
    d_next->resetInstantiationRound( qe );
  }
  d_curr_exclude_match.clear();
}

bool InstMatchGenerator::reset( Node eqc, QuantifiersEngine* qe ){
  if (d_cg == nullptr)
  {
    // we did not properly initialize the candidate generator, thus we fail
    return false;
  }
  eqc = qe->getEqualityQuery()->getRepresentative( eqc );
  Trace("matching-debug2") << this << " reset " << eqc << "." << std::endl;
  if( !d_eq_class_rel.isNull() && d_eq_class_rel.getKind()!=INST_CONSTANT ){
    d_eq_class = d_eq_class_rel;
  }else if( !eqc.isNull() ){
    d_eq_class = eqc;
  }
  //we have a specific equivalence class in mind
  //we are producing matches for f(E) ~ t, where E is a non-ground vector of terms, and t is a ground term
  //just look in equivalence class of the RHS
  d_cg->reset( d_eq_class );
  d_needsReset = false;
  
  //generate the first candidate preemptively
  d_curr_first_candidate = Node::null();
  Node t;
  do {
    t = d_cg->getNextCandidate();
    if( d_curr_exclude_match.find( t )==d_curr_exclude_match.end() ){
      d_curr_first_candidate = t;
    }
  }while( !t.isNull() && d_curr_first_candidate.isNull() );
  Trace("matching-summary") << "Reset " << d_match_pattern << " in " << eqc << " returns " << !d_curr_first_candidate.isNull() << "." << std::endl;

  return !d_curr_first_candidate.isNull();
}

int InstMatchGenerator::getNextMatch(Node f,
                                     InstMatch& m,
                                     QuantifiersEngine* qe,
                                     Trigger* tparent)
{
  if( d_needsReset ){
    Trace("matching") << "Reset not done yet, must do the reset..." << std::endl;
    reset( d_eq_class, qe );
  }
  d_curr_matched = Node::null();
  Trace("matching") << this << " " << d_match_pattern << " get next match " << m << " in eq class " << d_eq_class << std::endl;
  int success = -1;
  Node t = d_curr_first_candidate;
  do{
    Trace("matching-debug2") << "Matching candidate : " << t << std::endl;
    Assert(!qe->inConflict());
    //if t not null, try to fit it into match m
    if( !t.isNull() ){
      if( d_curr_exclude_match.find( t )==d_curr_exclude_match.end() ){
        Assert(t.getType().isComparableTo(d_match_pattern_type));
        Trace("matching-summary") << "Try " << d_match_pattern << " : " << t << std::endl;
        success = getMatch(f, t, m, qe, tparent);
        if( d_independent_gen && success<0 ){
          Assert(d_eq_class.isNull() || !d_eq_class_rel.isNull());
          d_curr_exclude_match[t] = true;
        }
      }
      //get the next candidate term t
      if( success<0 ){
        t = qe->inConflict() ? Node::null() : d_cg->getNextCandidate();
      }else{
        d_curr_first_candidate = d_cg->getNextCandidate();
      }
    }
  }while( success<0 && !t.isNull() );
  d_curr_matched = t;
  if( success<0 ){
    Trace("matching-summary") << "..." << d_match_pattern << " failed, reset." << std::endl;
    Trace("matching") << this << " failed, reset " << d_eq_class << std::endl;
    //we failed, must reset
    reset( d_eq_class, qe );
  }else{
    Trace("matching-summary") << "..." << d_match_pattern << " success." << std::endl;
  }
  return success;
}

unsigned InstMatchGenerator::addInstantiations(Node f,
                                               QuantifiersEngine* qe,
                                               Trigger* tparent)
{
  //try to add instantiation for each match produced
  unsigned addedLemmas = 0;
  InstMatch m( f );
  while (getNextMatch(f, m, qe, tparent) > 0)
  {
    if( !d_active_add ){
      if (sendInstantiation(tparent, m))
      {
        addedLemmas++;
        if( qe->inConflict() ){
          break;
        }
      }
    }else{
      addedLemmas++;
      if( qe->inConflict() ){
        break;
      }
    }
    m.clear();
  }
  //return number of lemmas added
  return addedLemmas;
}


InstMatchGenerator* InstMatchGenerator::mkInstMatchGenerator( Node q, Node pat, QuantifiersEngine* qe ) {
  std::vector< Node > pats;
  pats.push_back( pat );
  std::map< Node, InstMatchGenerator * > pat_map_init;
  return mkInstMatchGenerator( q, pats, qe, pat_map_init );
}

InstMatchGenerator* InstMatchGenerator::mkInstMatchGeneratorMulti( Node q, std::vector< Node >& pats, QuantifiersEngine* qe ) {
  Assert(pats.size() > 1);
  InstMatchGeneratorMultiLinear * imgm = new InstMatchGeneratorMultiLinear( q, pats, qe );
  std::vector< InstMatchGenerator* > gens;
  imgm->initialize(q, qe, gens);
  Assert(gens.size() == pats.size());
  std::vector< Node > patsn;
  std::map< Node, InstMatchGenerator * > pat_map_init;
  for (InstMatchGenerator* g : gens){
    Node pn = g->d_match_pattern;
    patsn.push_back( pn );
    pat_map_init[pn] = g;
  }
  //return mkInstMatchGenerator( q, patsn, qe, pat_map_init );
  imgm->d_next = mkInstMatchGenerator( q, patsn, qe, pat_map_init );
  return imgm;
}

InstMatchGenerator* InstMatchGenerator::mkInstMatchGenerator( Node q, std::vector< Node >& pats, QuantifiersEngine* qe, 
                                                              std::map< Node, InstMatchGenerator * >& pat_map_init ) {
  size_t pCounter = 0;
  InstMatchGenerator* prev = NULL;
  InstMatchGenerator* oinit = NULL;
  while( pCounter<pats.size() ){
    size_t counter = 0;
    std::vector< InstMatchGenerator* > gens;
    InstMatchGenerator* init;
    std::map< Node, InstMatchGenerator * >::iterator iti = pat_map_init.find( pats[pCounter] );
    if( iti==pat_map_init.end() ){
      init = new InstMatchGenerator(pats[pCounter]);
    }else{
      init = iti->second;
    }
    if(pCounter==0){
      oinit = init;
    }
    gens.push_back(init);
    //chain the resulting match generators together
    while (counter<gens.size()) {
      InstMatchGenerator* curr = gens[counter];
      if( prev ){
        prev->d_next = curr;
      }
      curr->initialize(q, qe, gens);
      prev = curr;
      counter++;
    }
    pCounter++;
  }
  return oinit;
}

InstMatchGenerator* InstMatchGenerator::getInstMatchGenerator(Node q, Node n)
{
  if (n.getKind() != INST_CONSTANT)
  {
    Trace("var-trigger-debug")
        << "Is " << n << " a variable trigger?" << std::endl;
    Node x;
    if (options::purifyTriggers())
    {
      Node xi = Trigger::getInversionVariable(n);
      if (!xi.isNull())
      {
        Node qa = quantifiers::TermUtil::getInstConstAttr(xi);
        if (qa == q)
        {
          x = xi;
        }
      }
    }
    if (!x.isNull())
    {
      Node s = Trigger::getInversion(n, x);
      VarMatchGeneratorTermSubs* vmg = new VarMatchGeneratorTermSubs(x, s);
      Trace("var-trigger") << "Term substitution trigger : " << n
                           << ", var = " << x << ", subs = " << s << std::endl;
      return vmg;
    }
  }
  return new InstMatchGenerator(n);
}

VarMatchGeneratorTermSubs::VarMatchGeneratorTermSubs( Node var, Node subs ) :
  InstMatchGenerator(), d_var( var ), d_subs( subs ), d_rm_prev( false ){
  d_children_types.push_back(d_var.getAttribute(InstVarNumAttribute()));
  d_var_type = d_var.getType();
}

int VarMatchGeneratorTermSubs::getNextMatch(Node q,
                                            InstMatch& m,
                                            QuantifiersEngine* qe,
                                            Trigger* tparent)
{
  int ret_val = -1;
  if( !d_eq_class.isNull() ){
    Trace("var-trigger-matching") << "Matching " << d_eq_class << " against " << d_var << " in " << d_subs << std::endl;
    TNode tvar = d_var;
    Node s = d_subs.substitute(tvar, d_eq_class);
    s = Rewriter::rewrite( s );
    Trace("var-trigger-matching") << "...got " << s << ", " << s.getKind() << std::endl;
    d_eq_class = Node::null();
    //if( s.getType().isSubtypeOf( d_var_type ) ){
    d_rm_prev = m.get(d_children_types[0]).isNull();
    if (!m.set(qe->getEqualityQuery(), d_children_types[0], s))
    {
      return -1;
    }else{
      ret_val = continueNextMatch(q, m, qe, tparent);
      if( ret_val>0 ){
        return ret_val;
      }
    }
  }
  if( d_rm_prev ){
    m.d_vals[d_children_types[0]] = Node::null();
    d_rm_prev = false;
  }
  return -1;
}

InstMatchGeneratorMultiLinear::InstMatchGeneratorMultiLinear( Node q, std::vector< Node >& pats, QuantifiersEngine* qe ) {
  //order patterns to maximize eager matching failures
  std::map< Node, std::vector< Node > > var_contains;
  for (const Node& pat : pats)
  {
    quantifiers::TermUtil::computeInstConstContainsForQuant(
        q, pat, var_contains[pat]);
  }
  std::map< Node, std::vector< Node > > var_to_node;
  for( std::pair< const Node, std::vector< Node > >& vc : var_contains ){
    for (const Node& n : vc.second)
    {
      var_to_node[n].push_back(vc.first);
    }
  }
  std::vector< Node > pats_ordered;
  std::vector< bool > pats_ordered_independent;
  std::map< Node, bool > var_bound;
  size_t patsSize = pats.size();
  while (pats_ordered.size() < patsSize)
  {
    // score is lexographic ( bound vars, shared vars )
    int score_max_1 = -1;
    int score_max_2 = -1;
    unsigned score_index = 0;
    bool set_score_index = false;
    for (size_t i = 0; i < patsSize; i++)
    {
      Node p = pats[i];
      if( std::find( pats_ordered.begin(), pats_ordered.end(), p )==pats_ordered.end() ){
        int score_1 = 0;
        int score_2 = 0;
        for( unsigned j=0; j<var_contains[p].size(); j++ ){
          Node v = var_contains[p][j];
          if( var_bound.find( v )!=var_bound.end() ){
            score_1++;
          }else if( var_to_node[v].size()>1 ){
            score_2++;
          }
        }
        if (!set_score_index || score_1 > score_max_1
            || (score_1 == score_max_1 && score_2 > score_max_2))
        {
          score_index = i;
          set_score_index = true;
          score_max_1 = score_1;
          score_max_2 = score_2;
        }
      }
    }
    Assert(set_score_index);
    //update the variable bounds
    Node mp = pats[score_index];
    std::vector<Node>& vcm = var_contains[mp];
    for (const Node& vc : vcm){
      var_bound[vc] = true;
    }
    pats_ordered.push_back( mp );
    pats_ordered_independent.push_back( score_max_1==0 );
  }
  
  Trace("multi-trigger-linear") << "Make children for linear multi trigger." << std::endl;
  for (size_t i = 0, poSize = pats_ordered.size(); i < poSize; i++)
  {
    Node po = pats_ordered[i];
    Trace("multi-trigger-linear") << "...make for " << po << std::endl;
    InstMatchGenerator* cimg = getInstMatchGenerator(q, po);
    Assert(cimg != nullptr);
    d_children.push_back( cimg );
    // this could be improved
    if (i == 0)
    {
      cimg->setIndependent();
    }
  }
}

int InstMatchGeneratorMultiLinear::resetChildren( QuantifiersEngine* qe ){
  for (InstMatchGenerator* c : d_children)
  {
    if (!c->reset(Node::null(), qe))
    {
      return -2;
    }
  }
  return 1;
}

bool InstMatchGeneratorMultiLinear::reset( Node eqc, QuantifiersEngine* qe ) {
  Assert(eqc.isNull());
  if( options::multiTriggerLinear() ){
    return true;
  }
  return resetChildren(qe) > 0;
}

int InstMatchGeneratorMultiLinear::getNextMatch(Node q,
                                                InstMatch& m,
                                                QuantifiersEngine* qe,
                                                Trigger* tparent)
{
  Trace("multi-trigger-linear-debug") << "InstMatchGeneratorMultiLinear::getNextMatch : reset " << std::endl;
  if( options::multiTriggerLinear() ){
    //reset everyone
    int rc_ret = resetChildren( qe );
    if( rc_ret<0 ){
      return rc_ret;
    }
  }
  Trace("multi-trigger-linear-debug") << "InstMatchGeneratorMultiLinear::getNextMatch : continue match " << std::endl;
  Assert(d_next != nullptr);
  int ret_val = continueNextMatch(q, m, qe, tparent);
  if( ret_val>0 ){
    Trace("multi-trigger-linear") << "Successful multi-trigger instantiation." << std::endl;
    if( options::multiTriggerLinear() ){
      // now, restrict everyone
      for( size_t i=0, csize = d_children.size(); i<csize; i++ ){
        Node mi = d_children[i]->getCurrentMatch();
        Trace("multi-trigger-linear") << "   child " << i << " match : " << mi << std::endl;
        d_children[i]->excludeMatch( mi );
      }
    }
  }
  return ret_val;
}


/** constructors */
InstMatchGeneratorMulti::InstMatchGeneratorMulti(Node q,
                                                 std::vector<Node>& pats,
                                                 QuantifiersEngine* qe)
    : d_quant(q)
{
  Trace("multi-trigger-cache") << "Making smart multi-trigger for " << q << std::endl;
  std::map< Node, std::vector< Node > > var_contains;
  for (const Node& pat : pats)
  {
    quantifiers::TermUtil::computeInstConstContainsForQuant(
        q, pat, var_contains[pat]);
  }
  //convert to indicies
  for( std::pair< const Node, std::vector< Node > >& vc : var_contains ){
    Trace("multi-trigger-cache") << "Pattern " << vc.first << " contains: ";
    for (const Node& vcn : vc.second){
      Trace("multi-trigger-cache") << vcn << " ";
      uint64_t index = vcn.getAttribute(InstVarNumAttribute());
      d_var_contains[ vc.first ].push_back( index );
      d_var_to_node[ index ].push_back( vc.first );
    }
    Trace("multi-trigger-cache") << std::endl;
  }
  size_t patsSize = pats.size();
  for (size_t i = 0; i < patsSize; i++)
  {
    Node n = pats[i];
    //make the match generator
    InstMatchGenerator* img =
        InstMatchGenerator::mkInstMatchGenerator(q, n, qe);
    img->setActiveAdd(false);
    d_children.push_back(img);
    //compute unique/shared variables
    std::vector< uint64_t > unique_vars;
    std::map< uint64_t, bool > shared_vars;
    unsigned numSharedVars = 0;
    std::vector<uint64_t>& vctn = d_var_contains[n];
    for (size_t j = 0, vctnSize = vctn.size(); j < vctnSize; j++)
    {
      if (d_var_to_node[vctn[j]].size() == 1)
      {
        Trace("multi-trigger-cache")
            << "Var " << vctn[j] << " is unique to " << pats[i] << std::endl;
        unique_vars.push_back(vctn[j]);
      }else{
        shared_vars[vctn[j]] = true;
        numSharedVars++;
      }
    }
    //we use the latest shared variables, then unique variables
    std::vector< uint64_t > vars;
    size_t index = i == 0 ? pats.size() - 1 : (i - 1);
    while( numSharedVars>0 && index!=i ){
      for( std::map< uint64_t, bool >::iterator it = shared_vars.begin(); it != shared_vars.end(); ++it ){
        if( it->second ){
          std::vector<uint64_t>& vctni = d_var_contains[pats[index]];
          if (std::find(vctni.begin(), vctni.end(), it->first) != vctni.end())
          {
            vars.push_back( it->first );
            shared_vars[ it->first ] = false;
            numSharedVars--;
          }
        }
      }
      index = index == 0 ? patsSize - 1 : (index - 1);
    }
    vars.insert( vars.end(), unique_vars.begin(), unique_vars.end() );
    if (Trace.isOn("multi-trigger-cache"))
    {
      Trace("multi-trigger-cache") << "   Index[" << i << "]: ";
      for( uint64_t v : vars ){
        Trace("multi-trigger-cache") << v << " ";
      }
      Trace("multi-trigger-cache") << std::endl;
    }
    //make ordered inst match trie
    d_imtio[i] = new InstMatchTrie::ImtIndexOrder;
    d_imtio[i]->d_order.insert( d_imtio[i]->d_order.begin(), vars.begin(), vars.end() );
    d_children_trie.push_back( InstMatchTrieOrdered( d_imtio[i] ) );
  }
}

InstMatchGeneratorMulti::~InstMatchGeneratorMulti()
{
  for (size_t i = 0, csize = d_children.size(); i < csize; i++)
  {
    delete d_children[i];
  }
  for (std::pair<const unsigned, InstMatchTrie::ImtIndexOrder*>& i : d_imtio)
  {
    delete i.second;
  }
}

/** reset instantiation round (call this whenever equivalence classes have changed) */
void InstMatchGeneratorMulti::resetInstantiationRound( QuantifiersEngine* qe ){
  for (InstMatchGenerator* c : d_children)
  {
    c->resetInstantiationRound(qe);
  }
}

/** reset, eqc is the equivalence class to search in (any if eqc=null) */
bool InstMatchGeneratorMulti::reset( Node eqc, QuantifiersEngine* qe ){
  for (InstMatchGenerator* c : d_children)
  {
    if (!c->reset(eqc, qe))
    {
      // do not return false here
    }
  }
  return true;
}

unsigned InstMatchGeneratorMulti::addInstantiations(Node q,
                                                    QuantifiersEngine* qe,
                                                    Trigger* tparent)
{
  unsigned addedLemmas = 0;
  Trace("multi-trigger-cache") << "Process smart multi trigger" << std::endl;
  for (size_t i = 0, csize = d_children.size(); i < csize; i++)
  {
    Trace("multi-trigger-cache") << "Calculate matches " << i << std::endl;
    std::vector< InstMatch > newMatches;
    InstMatch m( q );
    while (d_children[i]->getNextMatch(q, m, qe, tparent) > 0)
    {
      //m.makeRepresentative( qe );
      newMatches.push_back( InstMatch( &m ) );
      m.clear();
    }
    Trace("multi-trigger-cache") << "Made " << newMatches.size() << " new matches for index " << i << std::endl;
    for (size_t j = 0, nmatches = newMatches.size(); j < nmatches; j++)
    {
      Trace("multi-trigger-cache2") << "...processing " << j << " / " << newMatches.size() << ", #lemmas = " << addedLemmas << std::endl;
      processNewMatch(qe, tparent, newMatches[j], i, addedLemmas);
      if( qe->inConflict() ){
        return addedLemmas;
      }
    }
  }
  return addedLemmas;
}

void InstMatchGeneratorMulti::processNewMatch(QuantifiersEngine* qe,
                                              Trigger* tparent,
                                              InstMatch& m,
                                              size_t fromChildIndex,
                                              unsigned& addedLemmas)
{
  //see if these produce new matches
  d_children_trie[fromChildIndex].addInstMatch(qe, d_quant, m);
  //possibly only do the following if we know that new matches will be produced?
  //the issue is that instantiations are filtered in quantifiers engine, and so there is no guarentee that
  // we can safely skip the following lines, even when we have already produced this match.
  Trace("multi-trigger-cache-debug") << "Child " << fromChildIndex << " produced match " << m << std::endl;
  //process new instantiations
  size_t childIndex = (fromChildIndex + 1) % d_children.size();
  processNewInstantiations(qe,
                           tparent,
                           m,
                           addedLemmas,
                           d_children_trie[childIndex].getTrie(),
                           0,
                           childIndex,
                           fromChildIndex,
                           true);
}

void InstMatchGeneratorMulti::processNewInstantiations(QuantifiersEngine* qe,
                                                       Trigger* tparent,
                                                       InstMatch& m,
                                                       unsigned& addedLemmas,
                                                       InstMatchTrie* tr,
                                                       size_t trieIndex,
                                                       size_t childIndex,
                                                       size_t endChildIndex,
                                                       bool modEq)
{
  Assert(!qe->inConflict());
  if( childIndex==endChildIndex ){
    // m is an instantiation
    if (sendInstantiation(tparent, m))
    {
      addedLemmas++;
      Trace("multi-trigger-cache-debug") << "-> Produced instantiation " << m
                                         << std::endl;
    }
    return;
  }
  InstMatchTrie::ImtIndexOrder* iio = d_children_trie[childIndex].getOrdering();
  if (trieIndex < iio->d_order.size())
  {
    size_t curr_index = iio->d_order[trieIndex];
    // Node curr_ic = qe->getTermUtil()->getInstantiationConstant( d_quant,
    // curr_index );
    Node n = m.get( curr_index );
    if( n.isNull() ){
      // add to InstMatch
      for (std::pair<const Node, InstMatchTrie>& d : tr->d_data)
      {
        InstMatch mn(&m);
        mn.setValue(curr_index, d.first);
        processNewInstantiations(qe,
                                 tparent,
                                 mn,
                                 addedLemmas,
                                 &(d.second),
                                 trieIndex + 1,
                                 childIndex,
                                 endChildIndex,
                                 modEq);
        if (qe->inConflict())
        {
          break;
        }
      }
    }
    // shared and set variable, try to merge
    std::map<Node, InstMatchTrie>::iterator it = tr->d_data.find(n);
    if (it != tr->d_data.end())
    {
      processNewInstantiations(qe,
                               tparent,
                               m,
                               addedLemmas,
                               &(it->second),
                               trieIndex + 1,
                               childIndex,
                               endChildIndex,
                               modEq);
    }
    if (!modEq)
    {
      return;
    }
    eq::EqualityEngine* ee = qe->getEqualityQuery()->getEngine();
    // check modulo equality for other possible instantiations
    if (!ee->hasTerm(n))
    {
      return;
    }
    eq::EqClassIterator eqc(ee->getRepresentative(n), ee);
    while (!eqc.isFinished())
    {
      Node en = (*eqc);
      if (en != n)
      {
        std::map<Node, InstMatchTrie>::iterator itc = tr->d_data.find(en);
        if (itc != tr->d_data.end())
        {
          processNewInstantiations(qe,
                                   tparent,
                                   m,
                                   addedLemmas,
                                   &(itc->second),
                                   trieIndex + 1,
                                   childIndex,
                                   endChildIndex,
                                   modEq);
          if (qe->inConflict())
          {
            break;
          }
        }
      }
      ++eqc;
    }
  }else{
    size_t newChildIndex = (childIndex + 1) % d_children.size();
    processNewInstantiations(qe,
                             tparent,
                             m,
                             addedLemmas,
                             d_children_trie[newChildIndex].getTrie(),
                             0,
                             newChildIndex,
                             endChildIndex,
                             modEq);
  }
}

InstMatchGeneratorSimple::InstMatchGeneratorSimple(Node q,
                                                   Node pat,
                                                   QuantifiersEngine* qe)
    : d_quant(q), d_match_pattern(pat)
{
  if( d_match_pattern.getKind()==NOT ){
    d_match_pattern = d_match_pattern[0];
    d_pol = false;
  }else{
    d_pol = true;
  }
  if( d_match_pattern.getKind()==EQUAL ){
    d_eqc = d_match_pattern[1];
    d_match_pattern = d_match_pattern[0];
    Assert(!quantifiers::TermUtil::hasInstConstAttr(d_eqc));
  }
  Assert(Trigger::isSimpleTrigger(d_match_pattern));
  for( unsigned i=0; i<d_match_pattern.getNumChildren(); i++ ){
    if( d_match_pattern[i].getKind()==INST_CONSTANT ){
      if( !options::cegqi() || quantifiers::TermUtil::getInstConstAttr(d_match_pattern[i])==q ){
        d_var_num[i] = d_match_pattern[i].getAttribute(InstVarNumAttribute());
      }else{
        d_var_num[i] = -1;
      }
    }
    d_match_pattern_arg_types.push_back( d_match_pattern[i].getType() );
  }
  d_op = qe->getTermDatabase()->getMatchOperator( d_match_pattern );
}

void InstMatchGeneratorSimple::resetInstantiationRound( QuantifiersEngine* qe ) {
  
}
unsigned InstMatchGeneratorSimple::addInstantiations(Node q,
                                                     QuantifiersEngine* qe,
                                                     Trigger* tparent)
{
  unsigned addedLemmas = 0;
  TNodeTrie* tat;
  if( d_eqc.isNull() ){
    tat = qe->getTermDatabase()->getTermArgTrie( d_op );
  }else{
    if( d_pol ){
      tat = qe->getTermDatabase()->getTermArgTrie( d_eqc, d_op );
    }else{
      //iterate over all classes except r
      tat = qe->getTermDatabase()->getTermArgTrie( Node::null(), d_op );
      if (tat && !qe->inConflict())
      {
        Node r = qe->getEqualityQuery()->getRepresentative(d_eqc);
        for (std::pair<const TNode, TNodeTrie>& t : tat->d_data)
        {
          if (t.first != r)
          {
            InstMatch m( q );
            addInstantiations(m, qe, addedLemmas, 0, &(t.second));
            if( qe->inConflict() ){
              break;
            }
          }
        }
      }
      tat = nullptr;
    }
  }
  Debug("simple-trigger-debug") << "Adding instantiations based on " << tat << " from " << d_op << " " << d_eqc << std::endl;
  if (tat && !qe->inConflict())
  {
    InstMatch m( q );
    addInstantiations( m, qe, addedLemmas, 0, tat );
  }
  return addedLemmas;
}

void InstMatchGeneratorSimple::addInstantiations(InstMatch& m,
                                                 QuantifiersEngine* qe,
                                                 unsigned& addedLemmas,
                                                 unsigned argIndex,
                                                 TNodeTrie* tat)
{
  Debug("simple-trigger-debug") << "Add inst " << argIndex << " " << d_match_pattern << std::endl;
  if (argIndex == d_match_pattern.getNumChildren())
  {
    Assert(!tat->d_data.empty());
    TNode t = tat->getData();
    Debug("simple-trigger") << "Actual term is " << t << std::endl;
    //convert to actual used terms
    for (const std::pair<unsigned, int>& v : d_var_num)
    {
      if (v.second >= 0)
      {
        Assert(v.first < t.getNumChildren());
        Debug("simple-trigger")
            << "...set " << v.second << " " << t[v.first] << std::endl;
        m.setValue(v.second, t[v.first]);
      }
    }
    // we do not need the trigger parent for simple triggers (no post-processing
    // required)
    if (qe->getInstantiate()->addInstantiation(d_quant, m))
    {
      addedLemmas++;
      Debug("simple-trigger") << "-> Produced instantiation " << m << std::endl;
    }
    return;
  }
  if (d_match_pattern[argIndex].getKind() == INST_CONSTANT)
  {
    int v = d_var_num[argIndex];
    if (v != -1)
    {
      for (std::pair<const TNode, TNodeTrie>& tt : tat->d_data)
      {
        Node t = tt.first;
        Node prev = m.get(v);
        // using representatives, just check if equal
        Assert(t.getType().isComparableTo(d_match_pattern_arg_types[argIndex]));
        if (prev.isNull() || prev == t)
        {
          m.setValue(v, t);
          addInstantiations(m, qe, addedLemmas, argIndex + 1, &(tt.second));
          m.setValue(v, prev);
          if (qe->inConflict())
          {
            break;
          }
        }
      }
      return;
    }
    // inst constant from another quantified formula, treat as ground term TODO:
    // remove this?
  }
  Node r = qe->getEqualityQuery()->getRepresentative(d_match_pattern[argIndex]);
  std::map<TNode, TNodeTrie>::iterator it = tat->d_data.find(r);
  if (it != tat->d_data.end())
  {
    addInstantiations(m, qe, addedLemmas, argIndex + 1, &(it->second));
  }
}

int InstMatchGeneratorSimple::getActiveScore( QuantifiersEngine * qe ) {
  Node f = qe->getTermDatabase()->getMatchOperator( d_match_pattern );
  size_t ngt = qe->getTermDatabase()->getNumGroundTerms(f);
  Trace("trigger-active-sel-debug") << "Number of ground terms for (simple) " << f << " is " << ngt << std::endl;
  return static_cast<int>(ngt);
}


}/* CVC4::theory::inst namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */
