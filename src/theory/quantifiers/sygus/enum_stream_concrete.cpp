/*********************                                                        */
/*! \file enum_stream_concrete.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Haniel Barbosa
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief class for streaming concrete values from enumerated abstract ones
 **/

#include "theory/quantifiers/sygus/enum_stream_concrete.h"

#include "options/base_options.h"
#include "options/quantifiers_options.h"
#include "printer/printer.h"
#include "theory/quantifiers/sygus/term_database_sygus.h"

#include <numeric>  // for std::iota

using namespace CVC4::kind;

namespace CVC4 {
namespace theory {
namespace quantifiers {

StreamPermutation::StreamPermutation(Node value, quantifiers::TermDbSygus* tds)
    : d_tds(tds)
{
  d_value = value;
  // get variables in value's type
  TypeNode tn = value.getType();
  Node var_list = Node::fromExpr(tn.getDatatype().getSygusVarList());
  NodeManager* nm = NodeManager::currentNM();
  // get subtypes in value's type
  std::vector<TypeNode> sf_types;
  d_tds->getSubfieldTypes(tn, sf_types);
  for (const Node& v : var_list)
  {
    // collect constructors for variable in all subtypes
    std::vector<Node> cons;
    for (const TypeNode& stn : sf_types)
    {
      const Datatype& dt = stn.getDatatype();
      for (unsigned i = 0, size = dt.getNumConstructors(); i < size; ++i)
      {
        if (dt[i].getNumArgs() == 0 && Node::fromExpr(dt[i].getSygusOp()) == v)
        {
          cons.push_back(nm->mkNode(APPLY_CONSTRUCTOR, dt[i].getConstructor()));
          d_cons_var[cons.back()] = v;
        }
      }
    }
    d_var_cons[v] = cons;
  }
  // collect variables occurring in value
  std::unordered_set<Node, NodeHashFunction> visited;
  collectVars(value, d_vars, visited);
  // partition permutation variables
  d_first = true;
  d_curr_ind = 0;
  Trace("synth-stream-concrete") << " ..permutting vars :";
  for (const Node& cons_var : d_vars)
  {
    Assert(d_cons_var.find(cons_var) != d_cons_var.end());
    Node var = d_cons_var[cons_var];
    d_var_classes[d_tds->getSubclassForVar(tn, var)].push_back(cons_var);
  }
  for (const std::pair<unsigned, std::vector<Node>>& p : d_var_classes)
  {
    d_perm_state_class.push_back(PermutationState(p.second));
    if (Trace.isOn("synth-stream-concrete"))
    {
      Trace("synth-stream-concrete") << " " << p.first << " -> [";
      for (const Node& var : p.second)
      {
        std::stringstream ss;
        Printer::getPrinter(options::outputLanguage())->toStreamSygus(ss, var);
        Trace("synth-stream-concrete") << " " << ss.str();
      }
      Trace("synth-stream-concrete") << " ]";
    }
  }
  Trace("synth-stream-concrete") << "\n";
}

Node StreamPermutation::getNext()
{
  if (Trace.isOn("synth-stream-concrete"))
  {
    std::stringstream ss;
    Printer::getPrinter(options::outputLanguage())->toStreamSygus(ss, d_value);
    Trace("synth-stream-concrete")
        << " ....streaming next permutation for value : " << ss.str()
        << " with " << d_perm_state_class.size() << " permutation classes\n";
  }
  // initial value
  if (d_first)
  {
    d_first = false;
    Node bultin_value = d_tds->sygusToBuiltin(d_value, d_value.getType());
    d_perm_values.insert(
        d_tds->getExtRewriter()->extendedRewrite(bultin_value));
    return d_value;
  }
  unsigned n_classes = d_perm_state_class.size();
  Assert(n_classes > 0);
  Node perm_value, bultin_perm_value;
  do
  {
    bool new_perm = false;
    do
    {
      if (d_perm_state_class[d_curr_ind].getNextPermutation())
      {
        new_perm = true;
        Trace("synth-stream-concrete-debug2")
            << " ....class " << d_curr_ind << " has new perm\n";
        d_curr_ind = 0;
      }
      else
      {
        Trace("synth-stream-concrete-debug2")
            << " ....class " << d_curr_ind << " reset\n";
        d_perm_state_class[d_curr_ind].reset();
        d_curr_ind++;
      }
    } while (!new_perm && d_curr_ind < n_classes);
    // no new permutation
    if (!new_perm)
    {
      Trace("synth-stream-concrete") << " ....no new perm, return null\n";
      return Node::null();
    }
    // build sub
    std::vector<Node> sub;
    for (unsigned i = 0, size = d_perm_state_class.size(); i < size; ++i)
    {
      Trace("synth-stream-concrete") << " ..perm for class " << i << " is";
      std::vector<Node> raw_sub;
      d_perm_state_class[i].getLastPerm(raw_sub);
      // build proper substitution based on variables types and constructors
      unsigned curr_size = sub.size();
      for (unsigned j = 0, size_j = raw_sub.size(); j < size_j; ++j)
      {
        TypeNode perm_var_tn = d_vars[j + curr_size].getType();
        if (perm_var_tn == raw_sub[j].getType())
        {
          continue;
        }
        // variables in value are constructor nodes
        Assert(d_cons_var.find(raw_sub[j]) != d_cons_var.end());
        Node cons_sub_var = d_cons_var[raw_sub[j]];
        Assert(d_var_cons.find(cons_sub_var) != d_var_cons.end());
        Assert(!d_var_cons[cons_sub_var].empty());
        for (const Node& cons : d_var_cons[cons_sub_var])
        {
          if (cons.getType() == perm_var_tn)
          {
            Trace("synth-stream-concrete-debug2")
                << "\n ....{ replacing " << raw_sub[j] << " ["
                << raw_sub[j].getType() << "] by " << cons << " ["
                << cons.getType() << "] }";
            raw_sub[j] = cons;
            break;
          }
        }
      }
      sub.insert(sub.end(), raw_sub.begin(), raw_sub.end());
      Trace("synth-stream-concrete") << "\n";
    }
    Assert(d_vars.size() == sub.size());
    perm_value = d_value.substitute(
        d_vars.begin(), d_vars.end(), sub.begin(), sub.end());
    bultin_perm_value = d_tds->sygusToBuiltin(perm_value, perm_value.getType());
    Trace("synth-stream-concrete-debug")
        << " ......perm builtin is " << bultin_perm_value;
    bultin_perm_value =
        d_tds->getExtRewriter()->extendedRewrite(bultin_perm_value);
    Trace("synth-stream-concrete-debug")
        << " and rewrites to " << bultin_perm_value << "\n";
    // if permuted value is equivalent modulo rewriting to a previous one, look
    // for another
  } while (!d_perm_values.insert(bultin_perm_value).second);
  if (Trace.isOn("synth-stream-concrete"))
  {
    std::stringstream ss;
    Printer::getPrinter(options::outputLanguage())
        ->toStreamSygus(ss, perm_value);
    Trace("synth-stream-concrete")
        << " ....return new perm " << ss.str() << "\n";
  }
  return perm_value;
}

const std::vector<Node>& StreamPermutation::getVars() const { return d_vars; }

unsigned StreamPermutation::getVarClassSize(unsigned id) const
{
  std::map<unsigned, std::vector<Node>>::const_iterator it =
      d_var_classes.find(id);
  if (it == d_var_classes.end())
  {
    return 0;
  }
  return it->second.size();
}

void StreamPermutation::collectVars(
    Node n,
    std::vector<Node>& vars,
    std::unordered_set<Node, NodeHashFunction>& visited)
{
  if (visited.find(n) != visited.end())
  {
    return;
  }
  visited.insert(n);
  if (n.getNumChildren() > 0)
  {
    for (const Node& ni : n)
    {
      collectVars(ni, vars, visited);
    }
    return;
  }
  if (d_tds->sygusToBuiltin(n, n.getType()).getKind() == kind::BOUND_VARIABLE)
  {
    if (std::find(vars.begin(), vars.end(), n) == vars.end())
    {
      vars.push_back(n);
    }
    return;
  }
}

StreamPermutation::PermutationState::PermutationState(
    const std::vector<Node>& vars)
{
  d_vars = vars;
  d_curr_ind = 0;
  d_seq.resize(vars.size());
  std::fill(d_seq.begin(), d_seq.end(), 0);
  // initialize variable indices
  d_last_perm.resize(vars.size());
  std::iota(d_last_perm.begin(), d_last_perm.end(), 0);
}

void StreamPermutation::PermutationState::reset()
{
  d_curr_ind = 0;
  std::fill(d_seq.begin(), d_seq.end(), 0);
  std::iota(d_last_perm.begin(), d_last_perm.end(), 0);
}

void StreamPermutation::PermutationState::getLastPerm(std::vector<Node>& vars)
{
  for (unsigned i = 0, size = d_last_perm.size(); i < size; ++i)
  {
    if (Trace.isOn("synth-stream-concrete"))
    {
      std::stringstream ss;
      Printer::getPrinter(options::outputLanguage())
          ->toStreamSygus(ss, d_vars[d_last_perm[i]]);
      Trace("synth-stream-concrete") << " " << ss.str();
    }
    vars.push_back(d_vars[d_last_perm[i]]);
  }
}

bool StreamPermutation::PermutationState::getNextPermutation()
{
  // exhausted permutations
  if (d_curr_ind == d_vars.size())
  {
    Trace("synth-stream-concrete-debug2") << "exhausted perms, ";
    return false;
  }
  if (d_seq[d_curr_ind] >= d_curr_ind)
  {
    d_seq[d_curr_ind] = 0;
    d_curr_ind++;
    return getNextPermutation();
  }
  if (d_curr_ind % 2 == 0)
  {
    // swap with first element
    std::swap(d_last_perm[0], d_last_perm[d_curr_ind]);
  }
  else
  {
    // swap with element in index in sequence of current index
    std::swap(d_last_perm[d_seq[d_curr_ind]], d_last_perm[d_curr_ind]);
  }
  d_seq[d_curr_ind] += 1;
  d_curr_ind = 0;
  return true;
}

StreamCombination::StreamCombination(Node value, quantifiers::TermDbSygus* tds)
    : d_tds(tds), d_stream_permutations(value, tds)
{
  if (Trace.isOn("synth-stream-concrete"))
  {
    std::stringstream ss;
    Printer::getPrinter(options::outputLanguage())->toStreamSygus(ss, value);
    Trace("synth-stream-concrete")
        << " * Streaming concrete: registering value " << ss.str() << "\n";
  }
  // get variables in value's type
  TypeNode tn = value.getType();
  Node var_list = Node::fromExpr(tn.getDatatype().getSygusVarList());
  // get subtypes in value's type
  NodeManager* nm = NodeManager::currentNM();
  std::vector<TypeNode> sf_types;
  d_tds->getSubfieldTypes(tn, sf_types);
  std::vector<Node> vars;
  for (const Node& v : var_list)
  {
    vars.push_back(v);
    // collect constructors for variable in all subtypes
    std::vector<Node> cons;
    for (const TypeNode& stn : sf_types)
    {
      const Datatype& dt = stn.getDatatype();
      for (unsigned i = 0, size = dt.getNumConstructors(); i < size; ++i)
      {
        if (dt[i].getNumArgs() == 0 && Node::fromExpr(dt[i].getSygusOp()) == v)
        {
          cons.push_back(nm->mkNode(APPLY_CONSTRUCTOR, dt[i].getConstructor()));
        }
      }
    }
    d_var_cons[v] = cons;
  }
  // split initial variables into classes
  std::map<unsigned, std::vector<Node>> var_classes;
  for (const Node& var : vars)
  {
    Assert(d_tds->getSubclassForVar(tn, var) > 0);
    var_classes[d_tds->getSubclassForVar(tn, var)].push_back(var);
  }
  // initialize combination utils per class
  Trace("synth-stream-concrete") << " ..combining vars :";
  d_curr_ind = 0;
  for (const std::pair<unsigned, std::vector<Node>>& p : var_classes)
  {
    unsigned perm_var_class_sz = d_stream_permutations.getVarClassSize(p.first);
    d_comb_state_class.push_back(
        CombinationState(p.second.size(), perm_var_class_sz, p.second));
    if (Trace.isOn("synth-stream-concrete"))
    {
      Trace("synth-stream-concrete")
          << " " << p.first << " -> [" << perm_var_class_sz << " from";
      for (const Node& var : p.second)
      {
        std::stringstream ss;
        Printer::getPrinter(options::outputLanguage())->toStreamSygus(ss, var);
        Trace("synth-stream-concrete") << " " << ss.str();
      }
      Trace("synth-stream-concrete") << " ]";
    }
  }
  Trace("synth-stream-concrete") << "\n";
}

Node StreamCombination::getNext()
{
  const std::vector<Node>& perm_vars = d_stream_permutations.getVars();
  Trace("synth-stream-concrete")
      << " ..streaming next combination of " << perm_vars.size() << " vars\n";
  unsigned n_classes = d_comb_state_class.size();
  // if no variables
  if (perm_vars.size() == 0)
  {
    if (d_last.isNull())
    {
      d_last = d_stream_permutations.getNext();
      if (Trace.isOn("synth-stream-concrete"))
      {
        std::stringstream ss;
        Printer::getPrinter(options::outputLanguage())
            ->toStreamSygus(ss, d_last);
        Trace("synth-stream-concrete")
            << " ..only comb is " << ss.str() << "\n";
      }
      return d_last;
    }
    else
    {
      Trace("synth-stream-concrete") << " ..no new comb, return null\n";
      return Node::null();
    }
  }
  // intial case
  if (d_last.isNull())
  {
    d_last = d_stream_permutations.getNext();
  }
  else
  {
    bool new_comb = false;
    do
    {
      if (d_comb_state_class[d_curr_ind].getNextCombination())
      {
        new_comb = true;
        Trace("synth-stream-concrete-debug2")
            << " ....class " << d_curr_ind << " has new comb\n";
        d_curr_ind = 0;
      }
      else
      {
        Trace("synth-stream-concrete-debug2")
            << " ....class " << d_curr_ind << " reset\n";
        d_comb_state_class[d_curr_ind].reset();
        d_curr_ind++;
      }
    } while (!new_comb && d_curr_ind < n_classes);
    // no new combination
    if (!new_comb)
    {
      Trace("synth-stream-concrete")
          << " ..no new comb, get next permutation\n";
#ifdef CVC4_ASSERTIONS
      Trace("synth-stream-concrete")
          << " ....total combs until here : " << d_comb_values.size() << "\n";
#endif
      d_last = d_stream_permutations.getNext();
      // exhausted permutations
      if (d_last.isNull())
      {
        Trace("synth-stream-concrete") << " ..no new comb, return null\n";
        return Node::null();
      }
      // reset combination classes for next permutation
      d_curr_ind = 0;
      for (unsigned i = 0, size = d_comb_state_class.size(); i < size; ++i)
      {
        d_comb_state_class[i].reset();
      }
    }
  }
  // building substitution
  std::vector<Node> sub;
  for (unsigned i = 0, size = d_comb_state_class.size(); i < size; ++i)
  {
    Trace("synth-stream-concrete") << " ..comb for class " << i << " is";
    std::vector<Node> raw_sub;
    d_comb_state_class[i].getLastComb(raw_sub);
    // build proper substitution based on variables types and constructors
    unsigned curr_size = sub.size();
    for (unsigned j = 0, size_j = raw_sub.size(); j < size_j; ++j)
    {
      TypeNode perm_var_tn = perm_vars[j + curr_size].getType();
      if (perm_var_tn == raw_sub[j].getType())
      {
        continue;
      }
      Assert(d_var_cons.find(raw_sub[j]) != d_var_cons.end());
      Assert(!d_var_cons[raw_sub[j]].empty());
      for (const Node& cons : d_var_cons[raw_sub[j]])
      {
        if (cons.getType() == perm_var_tn)
        {
          Trace("synth-stream-concrete-debug2")
              << "\n ....{ replacing " << raw_sub[j] << " ["
              << raw_sub[j].getType() << "] by " << cons << " ["
              << cons.getType() << "] }";
          raw_sub[j] = cons;
          break;
        }
      }
    }
    sub.insert(sub.end(), raw_sub.begin(), raw_sub.end());
    Trace("synth-stream-concrete") << "\n";
  }
  Assert(perm_vars.size() == sub.size());
  Node comb_value = d_last.substitute(
      perm_vars.begin(), perm_vars.end(), sub.begin(), sub.end());
  if (Trace.isOn("synth-stream-concrete"))
  {
    std::stringstream ss;
    Printer::getPrinter(options::outputLanguage())
        ->toStreamSygus(ss, comb_value);
    Trace("synth-stream-concrete")
        << " ..return new comb " << ss.str() << "\n\n";
  }
#ifdef CVC4_ASSERTIONS
  // the new combination value should be fresh, modulo rewriting, by
  // construction (unless it's equiv to a constant, e.g. true / false)
  Node builtin_comb_value = d_tds->getExtRewriter()->extendedRewrite(
      d_tds->sygusToBuiltin(comb_value, comb_value.getType()));
  Assert(builtin_comb_value.isConst()
         || d_comb_values.insert(builtin_comb_value).second);
#endif
  return comb_value;
}

StreamCombination::CombinationState::CombinationState(
    unsigned n, unsigned k, const std::vector<Node>& vars)
    : d_n(n), d_k(k)
{
  Assert(k <= n);
  d_last_comb.resize(k);
  std::iota(d_last_comb.begin(), d_last_comb.end(), 0);
  d_vars = vars;
}

void StreamCombination::CombinationState::reset()
{
  std::iota(d_last_comb.begin(), d_last_comb.end(), 0);
}

void StreamCombination::CombinationState::getLastComb(std::vector<Node>& vars)
{
  for (unsigned i = 0, size = d_last_comb.size(); i < size; ++i)
  {
    if (Trace.isOn("synth-stream-concrete"))
    {
      std::stringstream ss;
      Printer::getPrinter(options::outputLanguage())
          ->toStreamSygus(ss, d_vars[d_last_comb[i]]);
      Trace("synth-stream-concrete") << " " << ss.str();
    }
    vars.push_back(d_vars[d_last_comb[i]]);
  }
}

bool StreamCombination::CombinationState::getNextCombination()
{
  // find what to increment
  bool new_comb = false;
  for (int i = d_k - 1; i >= 0; --i)
  {
    if (d_last_comb[i] < d_n - d_k + i)
    {
      unsigned j = d_last_comb[i] + 1;
      while (static_cast<unsigned>(i) <= d_k - 1)
      {
        d_last_comb[i++] = j++;
      }
      new_comb = true;
      break;
    }
  }
  return new_comb;
}

}  // namespace quantifiers
}  // namespace theory
}  // namespace CVC4
