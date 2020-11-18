/*********************                                                        */
/*! \file proof_generator.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Gereon Kremer
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of CAD proof generator
 **/

#include "theory/arith/nl/cad/proof_generator.h"

#ifdef CVC4_POLY_IMP

namespace CVC4 {
namespace theory {
namespace arith {
namespace nl {
namespace cad {

namespace {
/**
 * Retrieves the root indices of the sign-invariant region of v.
 *
 * We assume that roots holds a sorted list of roots from one polynomial.
 * If v is equal to one of these roots, we return (id,id) where id is the index
 * of this root within roots. Otherwise, we return the id of the largest root
 * below v and the id of the smallest root above v. To make sure a smaller root
 * and a larger root always exist, we implicitly extend the roots by -infty and
 * infty.
 *
 * ATTENTION: if we return id, the corresponding root is:
 *   - id = 0: -infty
 *   - 0 < id <= roots.size(): roots[id-1]
 *   - id = roots.size() + 1: infty
 */
std::pair<std::size_t, std::size_t> getRootIDs(
    const std::vector<poly::Value>& roots, const poly::Value& v)
{
  for (std::size_t i = 0; i < roots.size(); ++i)
  {
    if (roots[i] == v)
    {
      return {i + 1, i + 1};
    }
    if (roots[i] > v)
    {
      return {i, i + 1};
    }
  }
  return {roots.size(), roots.size() + 1};
}

/**
 * Constructs an IndexedRootExpression:
 *   var ~rel~ root_k(poly)
 * where root_k(poly) is "the k'th root of the polynomial".
 *
 * @param var The variable that is bounded
 * @param rel The relation for this constraint
 * @param zero A node representing Rational(0)
 * @param k The index of the root (starting with 1)
 * @param poly The polynomial whose root shall be considered
 * @param vm A variable mapper from CVC4 to libpoly variables
 */
Node mkIRP(const Node& var,
           Kind rel,
           const Node& zero,
           std::size_t k,
           const poly::Polynomial& poly,
           VariableMapper& vm)
{
  auto* nm = NodeManager::currentNM();
  auto op = nm->mkConst<IndexedRootPredicate>(IndexedRootPredicate(k));
  return nm->mkNode(Kind::INDEXED_ROOT_PREDICATE,
                    op,
                    nm->mkNode(rel, var, zero),
                    as_cvc_polynomial(poly, vm));
}

}  // namespace

CADProofGenerator::CADProofGenerator(context::Context* ctx,
                                     ProofNodeManager* pnm)
    : d_pnm(pnm), d_proofs(ctx)
{
  d_false = NodeManager::currentNM()->mkConst<bool>(false);
  d_zero = NodeManager::currentNM()->mkConst<Rational>(0);
}

std::shared_ptr<ProofNode> CADProofGenerator::getProof() const
{
  return d_proofs.back()->getProof();
}

void CADProofGenerator::startNewProof()
{
  d_proofs.push_back(std::make_shared<LazyTreeProofGenerator>(d_pnm));
}
void CADProofGenerator::startRecursive() { d_proofs.back()->openChild(); }
void CADProofGenerator::endRecursive()
{
  d_proofs.back()->setCurrent(
      PfRule::ARITH_NL_CAD_RECURSIVE, {}, {d_false}, d_false);
  d_proofs.back()->closeChild();
}
void CADProofGenerator::startScope()
{
  d_proofs.back()->openChild();
  d_proofs.back()->getCurrent().d_rule = PfRule::SCOPE;
}
void CADProofGenerator::endScope(const std::vector<Node>& args)
{
  d_proofs.back()->setCurrent(PfRule::SCOPE, {}, args, d_false);
  d_proofs.back()->closeChild();
}

ProofGenerator* CADProofGenerator::getProofGenerator() const
{
  return d_proofs.back().get();
}

void CADProofGenerator::addDirect(Node var,
                                  VariableMapper& vm,
                                  const poly::Polynomial& poly,
                                  const poly::Assignment& a,
                                  poly::SignCondition& sc,
                                  const poly::Interval& interval,
                                  Node constraint)
{
  if (is_minus_infinity(get_lower(interval))
      && is_plus_infinity(get_upper(interval)))
  {
    // "Full conflict", constraint excludes (-inf,inf)
    d_proofs.back()->openChild();
    d_proofs.back()->setCurrent(
        PfRule::ARITH_NL_CAD_DIRECT, {constraint}, {d_false}, d_false);
    d_proofs.back()->closeChild();
    return;
  }
  std::vector<Node> res;
  auto roots = poly::isolate_real_roots(poly, a);
  if (get_lower(interval) == get_upper(interval))
  {
    // Excludes a single point only
    auto ids = getRootIDs(roots, get_lower(interval));
    Assert(ids.first == ids.second);
    res.emplace_back(mkIRP(var, Kind::EQUAL, d_zero, ids.first, poly, vm));
  }
  else
  {
    // Excludes an open interval
    if (!is_minus_infinity(get_lower(interval)))
    {
      // Interval has lower bound that is not -inf
      auto ids = getRootIDs(roots, get_lower(interval));
      Assert(ids.first == ids.second);
      Kind rel = poly::get_lower_open(interval) ? Kind::GT : Kind::GEQ;
      res.emplace_back(mkIRP(var, rel, d_zero, ids.first, poly, vm));
    }
    if (!is_plus_infinity(get_upper(interval)))
    {
      // Interval has upper bound that is not inf
      auto ids = getRootIDs(roots, get_upper(interval));
      Assert(ids.first == ids.second);
      Kind rel = poly::get_upper_open(interval) ? Kind::LT : Kind::LEQ;
      res.emplace_back(mkIRP(var, rel, d_zero, ids.first, poly, vm));
    }
  }
  // Add to proof manager
  startScope();
  d_proofs.back()->openChild();
  d_proofs.back()->setCurrent(
      PfRule::ARITH_NL_CAD_DIRECT, {constraint}, {d_false}, d_false);
  d_proofs.back()->closeChild();
  endScope(res);
}

std::vector<Node> CADProofGenerator::constructCell(Node var,
                                                   const CACInterval& i,
                                                   const poly::Assignment& a,
                                                   const poly::Value& s,
                                                   VariableMapper& vm)
{
  if (is_minus_infinity(get_lower(i.d_interval))
      && is_plus_infinity(get_upper(i.d_interval)))
  {
    // "Full conflict", constraint excludes (-inf,inf)
    return {};
  }

  std::vector<Node> res;

  // Just use bounds for all polynomials
  for (const auto& poly : i.d_mainPolys)
  {
    auto roots = poly::isolate_real_roots(poly, a);
    auto ids = getRootIDs(roots, s);
    if (ids.first == ids.second)
    {
      // Excludes a single point only
      res.emplace_back(mkIRP(var, Kind::EQUAL, d_zero, ids.first, poly, vm));
    }
    else
    {
      // Excludes an open interval
      if (ids.first > 0)
      {
        // Interval has lower bound that is not -inf
        res.emplace_back(mkIRP(var, Kind::GT, d_zero, ids.first, poly, vm));
      }
      if (ids.second <= roots.size())
      {
        // Interval has upper bound that is not inf
        res.emplace_back(mkIRP(var, Kind::LT, d_zero, ids.first, poly, vm));
      }
    }
  }

  return res;
}

std::ostream& operator<<(std::ostream& os, const CADProofGenerator& proof)
{
  return os << *proof.d_proofs.back();
}

}  // namespace cad
}  // namespace nl
}  // namespace arith
}  // namespace theory
}  // namespace CVC4

#endif
