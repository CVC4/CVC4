/*********************                                                        */
/*! \file symmetry_detect.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **  Paul Meng, Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Symmetry detection module
 **/

#include "preprocessing/passes/symmetry_detect.h"
#include "theory/quantifiers/term_util.h"
#include "theory/rewriter.h"

using namespace std;

namespace CVC4 {
namespace preprocessing {
namespace passes {

SymmetryDetect::Partition SymmetryDetect::detect(const vector<Node>& assertions)
{
  Partition p =
      findPartitions(NodeManager::currentNM()->mkNode(kind::AND, assertions));
  Trace("sym-dt") << endl
                  << "------------------------------ The Final Partition "
                     "------------------------------"
                  << endl;
  if(Trace.isOn("sym-dt"))
  {
    printPartition("sym-dt", p);
  }
  return p;
}

Node SymmetryDetect::getSymBreakVariable(TypeNode tn, unsigned index)
{
  std::map<TypeNode, std::vector<Node> >::iterator it = d_sb_vars.find(tn);
  if (it == d_sb_vars.end())
  {
    // initialize the variables for type tn
    d_sb_vars[tn].clear();
    it = d_sb_vars.find(tn);
  }
  while (it->second.size() <= index)
  {
    std::stringstream ss;
    ss << "sym_bk_" << tn;
    Node fresh_var = NodeManager::currentNM()->mkSkolem(ss.str(), tn);
    it->second.push_back(fresh_var);
  }
  return it->second[index];
}

void SymmetryDetect::getPartition(vector<vector<Node> >& parts,
                                  const vector<Node>& assertions)
{
  Partition p = detect(assertions);

  for (map<Node, vector<Node> >::const_iterator subvar_to_vars_it =
           p.d_subvar_to_vars.begin();
       subvar_to_vars_it != p.d_subvar_to_vars.end();
       ++subvar_to_vars_it)
  {
    parts.push_back(subvar_to_vars_it->second);
  }
}

SymmetryDetect::Partition SymmetryDetect::findPartitions(Node node)
{
  Trace("sym-dt-debug")
      << "------------------------------------------------------------" << endl;
  Trace("sym-dt-debug") << "[sym-dt] findPartitions gets a term: " << node
                        << endl;
  map<Node, Partition>::iterator partition = d_term_partition.find(node);

  // Return its partition from cache if we have processed the node before
  if (partition != d_term_partition.end())
  {
    Trace("sym-dt-debug") << "[sym-dt] We have seen the node " << node
                          << " before, thus we return its partition from cache."
                          << endl;
    return partition->second;
  }

  // The new partition for node
  Partition p;
  // If node is a variable
  if (node.isVar() && node.getKind() != kind::BOUND_VARIABLE)
  {
    vector<Node> vars;
    TypeNode type = node.getType();
    Node fresh_var = getSymBreakVariable(type, 0);
    vars.push_back(node);
    p.d_term = node;
    p.d_sterm = fresh_var;
    p.d_subvar_to_vars[fresh_var] = vars;
    p.d_var_to_subvar[node] = fresh_var;
    d_term_partition[node] = p;
    return p;
  }
  // If node is a constant
  else if (node.getNumChildren() == 0)
  {
    p.d_term = node;
    p.d_sterm = node;
    d_term_partition[node] = p;
    return p;
  }

  // Children of node
  vector<Node> children;
  // Partitions of children
  vector<Partition> partitions;

  // Get all children of node
  Trace("sym-dt-debug") << "[sym-dt] collectChildren for: " << node
                        << " with operator " << node.getKind() << endl;
  collectChildren(node, children);
  Trace("sym-dt-debug") << "[sym-dt] children: " << children << endl;

  // Create partitions for children
  std::unordered_set<unsigned> active_indices;
  for (vector<Node>::iterator children_it = children.begin();
       children_it != children.end();
       ++children_it)
  {
    active_indices.insert(partitions.size());
    partitions.push_back(findPartitions(*children_it));
    Trace("sym-dt-debug") << "  " << partitions.back().d_sterm << std::endl;
  }

  Trace("sym-dt-debug") << "----------------------------- Start processing "
                           "partitions -------------------------------"
                        << endl;

  if (theory::quantifiers::TermUtil::isComm(node.getKind()))
  {
    // map substituted terms to indices in partitions
    std::map<Node, std::vector<unsigned> > sterm_to_indices;
    for (unsigned j = 0, size = partitions.size(); j < size; j++)
    {
      sterm_to_indices[partitions[j].d_sterm].push_back(j);
    }

    for (const std::pair<Node, std::vector<unsigned> >& sti : sterm_to_indices)
    {
      Node cterm = sti.first;
      if (Trace.isOn("sym-dt-debug"))
      {
        Trace("sym-dt-debug") << "  indices[" << cterm << "] = ";
        for (unsigned i : sti.second)
        {
          Trace("sym-dt-debug") << i << " ";
        }
        Trace("sym-dt-debug") << std::endl;
      }
      // merge children, remove active indices
      mergePartitions(node.getKind(), partitions, sti.second, active_indices);
    }
  }

  // for all active indices
  unordered_set<Node, NodeHashFunction> all_vars;
  std::map<TypeNode, unsigned> type_index;
  std::vector<Node> schildren;
  std::vector<Partition> active_partitions;
  for (const unsigned& i : active_indices)
  {
    Trace("sym-dt-debug") << "Reconstruct partition for active index : " << i
                          << std::endl;
    Partition& pa = partitions[i];
    // ensure the variables of pa are fresh
    std::vector<Node> f_vars;
    std::vector<Node> f_subs;
    // add to overall list of variables
    for (const pair<const Node, vector<Node> >& pas : pa.d_subvar_to_vars)
    {
      Node v = pas.first;
      TypeNode tnv = v.getType();
      // ensure we use a new index for this variable
      unsigned new_index = 0;
      std::map<TypeNode, unsigned>::iterator itt = type_index.find(tnv);
      if (itt != type_index.end())
      {
        new_index = itt->second;
      }
      type_index[tnv] = new_index + 1;
      Node new_v = getSymBreakVariable(tnv, new_index);
      f_vars.push_back(v);
      f_subs.push_back(new_v);
      // add all vars to partition trie classifier
      all_vars.insert(pas.second.begin(), pas.second.end());
      for (const Node& x : pas.second)
      {
        pa.d_var_to_subvar[x] = new_v;
        Trace("sym-dt-debug")
            << "...set var to svar " << x << " -> " << new_v << std::endl;
      }
    }
    // reconstruct the partition
    for (unsigned j = 0, size = f_vars.size(); j < size; j++)
    {
      Node v = f_vars[j];
      Node new_v = f_subs[j];
      pa.d_subvar_to_vars[new_v].insert(pa.d_subvar_to_vars[new_v].end(),
                                        pa.d_subvar_to_vars[v].begin(),
                                        pa.d_subvar_to_vars[v].end());
      pa.d_subvar_to_vars.erase(v);
    }
    pa.d_sterm = pa.d_sterm.substitute(
        f_vars.begin(), f_vars.end(), f_subs.begin(), f_subs.end());
    schildren.push_back(pa.d_sterm);
    Trace("sym-dt-debug") << "...got : " << pa.d_sterm << std::endl;
    active_partitions.push_back(pa);
  }

  PartitionTrie pt;
  // Build the partition trie
  for (const Node& n : all_vars)
  {
    pt.addNode(n, active_partitions);
  }
  // Get the new partition
  pt.getNewPartition(p, pt);

  // Reconstruct the node
  Trace("sym-dt-debug") << "[sym-dt] Reconstructing node: " << node << endl;
  p.d_term = node;
  p.d_sterm = schildren.size() == 1
                  ? schildren[0]
                  : NodeManager::currentNM()->mkNode(node.getKind(), schildren);
  d_term_partition[node] = p;
  printPartition("sym-dt-debug", p);
  return p;
}

void SymmetryDetect::matches(vector<Partition>& partitions,
                             map<Node, Node>& subvar_to_var,
                             map<Node, Node>& subvar_to_expr)
{
  Trace("sym-dt-debug")
      << "[sym-dt] Start testing if singleton partitions can be merged!"
      << endl;

  if (!subvar_to_expr.empty())
  {
    // Replacement variables
    vector<Node> repls;
    // Variables that have been merged
    unordered_set<Node, NodeHashFunction> merged;
    // Put the variable in repls
    repls.push_back((subvar_to_expr.begin())->first);

    for (map<Node, Node>::iterator expr_it1 = subvar_to_expr.begin();
         expr_it1 != subvar_to_expr.end();
         ++expr_it1)
    {
      // Skip expr_it1->first, if it has been merged
      if (merged.find(expr_it1->first) != merged.end())
      {
        continue;
      }

      Partition p;
      vector<Node> subs;
      vector<Node> part;
      Node fst_var = subvar_to_var.find(expr_it1->first)->second;

      part.push_back(fst_var);
      subs.push_back(fst_var);
      merged.insert(expr_it1->first);
      p.d_var_to_subvar[fst_var] = expr_it1->first;
      Node sub_expr1 =
          (expr_it1->second)
              .substitute(subs.begin(), subs.end(), repls.begin(), repls.end());

      for (map<Node, Node>::iterator expr_it2 = subvar_to_expr.begin();
           expr_it2 != subvar_to_expr.end();
           ++expr_it2)
      {
        if (merged.find(expr_it2->first) != merged.end())
        {
          continue;
        }
        if ((expr_it1->second).getType() != (expr_it2->second).getType())
        {
          continue;
        }
        vector<Node> subs2;
        Node snd_var = subvar_to_var.find(expr_it2->first)->second;

        subs2.push_back(snd_var);
        Node sub_expr2 =
            (expr_it2->second)
                .substitute(
                    subs2.begin(), subs2.end(), repls.begin(), repls.end());
        Trace("sym-dt-debug") << "[sym-dt] Testing if " << sub_expr1
                              << " is equivalent to " << sub_expr2 << endl;

        if (sub_expr1 == sub_expr2)
        {
          Trace("sym-dt-debug") << "[sym-dt] Merge variable " << fst_var
                                << " with variable " << snd_var << endl;
          merged.insert(expr_it2->first);
          part.push_back(snd_var);
          p.d_var_to_subvar[snd_var] = expr_it1->first;
        }
      }
      p.d_subvar_to_vars[expr_it1->first] = part;
      Trace("sym-dt-debug")
          << "[sym-dt] Add a new partition after merging: " << endl;
      printPartition("sym-dt-debug", p);
      partitions.push_back(p);
    }
  }
}

void SymmetryDetect::processSingletonPartitions(
    vector<Partition>& partitions, unordered_set<Node, NodeHashFunction>& vars)
{
  Trace("sym-dt-debug")
      << "[sym-dt] Start post-processing partitions with size = "
      << partitions.size() << endl;

  // Mapping between the substitution variable to the actual variable
  map<Node, Node> subvar_to_var;
  // Mapping between the substitution variable to the actual expression
  map<Node, Node> subvar_to_expr;
  // Partitions that have 2 or more variables
  vector<Partition> new_partitions;

  // Collect singleton partitions: subvar_to_expr, subvar_to_var, and variables
  for (vector<Partition>::const_iterator part_it = partitions.begin();
       part_it != partitions.end();
       ++part_it)
  {
    if ((*part_it).d_var_to_subvar.size() == 1)
    {
      vars.insert(((*part_it).d_var_to_subvar.begin())->first);
      subvar_to_expr[((*part_it).d_var_to_subvar.begin())->second] =
          (*part_it).d_term;
      subvar_to_var[((*part_it).d_var_to_subvar.begin())->second] =
          ((*part_it).d_var_to_subvar.begin())->first;
    }
    else if ((*part_it).d_var_to_subvar.size() >= 2)
    {
      for (const pair<Node, Node>& var_to_subvar : (*part_it).d_var_to_subvar)
      {
        vars.insert(var_to_subvar.first);
      }
      // Only add partitions that have more than 1 variable
      new_partitions.push_back(*part_it);
    }
  }

  // Save all partitions that have more than 1 variable
  partitions = new_partitions;

  // Do matches on singleton terms
  if (!subvar_to_var.empty())
  {
    matches(partitions, subvar_to_var, subvar_to_expr);
  }
}

void SymmetryDetect::collectChildren(Node node, vector<Node>& children)
{
  Kind k = node.getKind();

  if(!theory::quantifiers::TermUtil::isAssoc(k))
  {
    children.insert(children.end(), node.begin(), node.end());
    return;
  }

  Node cur;
  vector<Node> visit;
  visit.push_back(node);
  unordered_set<Node, NodeHashFunction> visited;

  do
  {
    cur = visit.back();
    visit.pop_back();
    if (visited.find(cur) == visited.end())
    {
      visited.insert(cur);
      if (cur.getKind() == k)
      {
        for (const Node& cn : cur)
        {
          visit.push_back(cn);
        }
      }
      else
      {
        children.push_back(cur);
      }
    }
  } while (!visit.empty());
}

bool SymmetryDetect::mergePartitions(
    Kind k,
    std::vector<Partition>& partitions,
    const std::vector<unsigned>& indices,
    std::unordered_set<unsigned>& active_indices)
{
  Assert(!indices.empty());
  unsigned first_index = indices[0];

  unsigned num_sb_vars = partitions[first_index].d_subvar_to_vars.size();
  if (num_sb_vars > 1)
  {
    // cannot handle multiple symmetry breaking variables
    return false;
  }
  // separate by number of variables
  // for each n, nv_indices[n] contains the indices of partitions of the form
  // { w1 -> { x1, ..., xn } }
  std::map<unsigned, std::vector<unsigned> > nv_indices;
  for (unsigned j = 0, size = indices.size(); j < size; j++)
  {
    unsigned index = indices[j];
    Assert(partitions[index].d_subvar_to_vars.size() == 1);
    unsigned num_vars = partitions[index].d_var_to_subvar.size();
    nv_indices[num_vars].push_back(index);
  }
  bool merged = false;
  for (const std::pair<const unsigned, std::vector<unsigned> >& nvi :
       nv_indices)
  {
    unsigned num_vars = nvi.first;
    if (Trace.isOn("sym-dt-debug"))
    {
      Trace("sym-dt-debug") << "    nv_indices[" << num_vars << "] = ";
      for (unsigned i : nvi.second)
      {
        Trace("sym-dt-debug") << i << " ";
      }
      Trace("sym-dt-debug") << std::endl;
    }
    std::unordered_set<unsigned> include_indices;
    unsigned curr_index = 0;
    std::unordered_set<Node, NodeHashFunction> curr_variables;
    Trace("sym-dt-debug") << "    try merge partitions..." << std::endl;
    if (mergePartitions(k,
                        include_indices,
                        curr_index,
                        curr_variables,
                        num_vars,
                        partitions,
                        nvi.second,
                        active_indices))
    {
      merged = true;
    }
  }
  return merged;
}

unsigned nChoosek(unsigned n, unsigned k)
{
  if (k > n) return 0;
  if (k * 2 > n) k = n - k;
  if (k == 0) return 1;

  int result = n;
  for (int i = 2; i <= static_cast<int>(k); ++i)
  {
    result *= (n - i + 1);
    result /= i;
  }
  return result;
}

bool SymmetryDetect::mergePartitions(
    Kind k,
    std::unordered_set<unsigned>& include_indices,
    unsigned curr_index,
    std::unordered_set<Node, NodeHashFunction>& curr_variables,
    unsigned num_vars,
    std::vector<Partition>& partitions,
    const std::vector<unsigned>& indices,
    std::unordered_set<unsigned>& active_indices)
{
  Trace("sym-dt-debug") << "merge " << curr_index << " / " << indices.size()
                        << std::endl;
  if (curr_index == indices.size())
  {
    if (include_indices.size() <= 1)
    {
      return false;
    }
    // found a symmetry
    // merge the partitions
    std::vector<Node> children;
    std::vector<Node> schildren;
    bool set_master_index = false;
    unsigned master_index = 0;
    Trace("sym-dt-debug") << "found symmetry : { ";
    for (const unsigned& i : include_indices)
    {
      Trace("sym-dt-debug") << i << " ";
      const Partition& p = partitions[i];
      children.push_back(p.d_term);
      schildren.push_back(p.d_sterm);
      Assert(active_indices.find(i) != active_indices.end());
      if (!set_master_index)
      {
        set_master_index = true;
        master_index = i;
      }
      else
      {
        active_indices.erase(i);
      }
    }
    Trace("sym-dt-debug") << "}" << std::endl;
    Trace("sym-dt-debug") << "Reconstruct master index " << master_index
                          << std::endl;
    Partition& p = partitions[master_index];
    // reconstruct the master partition
    p.d_term = NodeManager::currentNM()->mkNode(k, children);
    p.d_sterm = NodeManager::currentNM()->mkNode(k, schildren);
    Assert(p.d_subvar_to_vars.size() == 1);
    Node sb_v = p.d_subvar_to_vars.begin()->first;
    p.d_subvar_to_vars[sb_v].clear();
    for (const Node& v : curr_variables)
    {
      p.d_var_to_subvar[v] = sb_v;
      p.d_subvar_to_vars[sb_v].push_back(v);
      Trace("sym-dt-debug")
          << "- set var to svar " << v << " -> " << sb_v << std::endl;
    }
    return true;
  }
  // try to include this index
  unsigned index = indices[curr_index];
  Partition& p = partitions[index];
  std::unordered_set<Node, NodeHashFunction> new_variables;
  new_variables.insert(curr_variables.begin(), curr_variables.end());
  Assert(p.d_subvar_to_vars.size() == 1);
  for (const std::pair<const Node, std::vector<Node> >& s : p.d_subvar_to_vars)
  {
    new_variables.insert(s.second.begin(), s.second.end());
  }
  // are there too many variables?
  bool var_ok = false;
  unsigned curr_nvars = new_variables.size();
  Assert(curr_nvars >= num_vars);
  if (curr_nvars == num_vars)
  {
    var_ok = true;
  }
  else
  {
    // the maximum number of partitions we can include at this point
    unsigned npart_max = include_indices.size() + (indices.size() - curr_index);
    // the number of patitions we need, which is (choose curr_nvars num_vars)
    unsigned npart_need = nChoosek(curr_nvars, num_vars);
    Trace("sym-dt-debug") << "...curr num vars = " << curr_nvars << std::endl;
    Trace("sym-dt-debug") << "  npart_max = " << npart_max << std::endl;
    Trace("sym-dt-debug") << "  npart_need = " << npart_need << std::endl;
    // max must be greater than or equal to need
    var_ok = npart_max >= npart_need;
  }
  if (var_ok)
  {
    include_indices.insert(index);
    if (mergePartitions(k,
                        include_indices,
                        curr_index + 1,
                        new_variables,
                        num_vars,
                        partitions,
                        indices,
                        active_indices))
    {
      return true;
    }
    include_indices.erase(index);
  }
  // try not including this index
  return mergePartitions(k,
                         include_indices,
                         curr_index + 1,
                         curr_variables,
                         num_vars,
                         partitions,
                         indices,
                         active_indices);
}

void SymmetryDetect::PartitionTrie::getNewPartition(Partition& part,
                                                    PartitionTrie& pt)
{
  if (!pt.d_variables.empty())
  {
    vector<Node> vars;
    Node fresh_var = NodeManager::currentNM()->mkSkolem(
        "sym_bk", pt.d_variables[0].getType());
    Trace("sym-dt-debug")
        << "[sym-dt] A partition from leaves of the partition trie:{";

    for (vector<Node>::iterator var_it = pt.d_variables.begin();
         var_it != pt.d_variables.end();
         ++var_it)
    {
      Trace("sym-dt-debug") << " " << *var_it;
      vars.push_back(*var_it);
      part.d_var_to_subvar[*var_it] = fresh_var;
    }
    Trace("sym-dt-debug") << " }" << endl;
    part.d_subvar_to_vars[fresh_var] = vars;
  }
  else
  {
    for (map<Node, PartitionTrie>::iterator part_it = pt.d_children.begin();
         part_it != pt.d_children.end();
         ++part_it)
    {
      getNewPartition(part, part_it->second);
    }
  }
}

void SymmetryDetect::PartitionTrie::addNode(Node target_var,
                                            vector<Partition>& partitions)
{
  Trace("sym-dt-debug") << "[sym-dt] Add a variable {" << target_var
                        << "} to the partition trie..." << endl;
  vector<Node> subvars;

  for (vector<Partition>::iterator part_it = partitions.begin();
       part_it != partitions.end();
       ++part_it)
  {
    map<Node, Node>::iterator var_sub_it =
        (*part_it).d_var_to_subvar.find(target_var);

    if (var_sub_it != (*part_it).d_var_to_subvar.end())
    {
      for (map<Node, vector<Node> >::iterator sub_vars_it =
               (*part_it).d_subvar_to_vars.begin();
           sub_vars_it != (*part_it).d_subvar_to_vars.end();
           ++sub_vars_it)
      {
        if (var_sub_it->second == sub_vars_it->first)
        {
          subvars.push_back(var_sub_it->second);
        }
        else
        {
          subvars.push_back(Node::null());
        }
      }
    }
    else
    {
      subvars.resize(subvars.size()+(*part_it).d_subvar_to_vars.size());
    }
  }

  Trace("sym-dt-debug") << "[sym-dt] Substitution variables for the variable "
                        << target_var << ": " << subvars << endl;
  addNode(target_var, subvars);
}

void SymmetryDetect::PartitionTrie::addNode(Node target_var,
                                            vector<Node>& subvars)
{
  if (subvars.empty())
  {
    d_variables.push_back(target_var);
  }
  else
  {
    vector<Node> new_subs;
    map<Node, PartitionTrie>::iterator children_it =
        d_children.find(subvars[0]);

    if (subvars.begin() + 1 < subvars.end())
    {
      new_subs.insert(new_subs.begin(), subvars.begin() + 1, subvars.end());
    }
    if (children_it != d_children.end())
    {
      (children_it->second).addNode(target_var, new_subs);
    }
    else
    {
      PartitionTrie pt;

      pt.addNode(target_var, new_subs);
      d_children[subvars[0]] = pt;
    }
  }
}

void SymmetryDetect::printPartition(const char* c, Partition p)
{
  for (map<Node, vector<Node> >::iterator sub_vars_it =
           p.d_subvar_to_vars.begin();
       sub_vars_it != p.d_subvar_to_vars.end();
       ++sub_vars_it)
  {
    Trace(c) << "[sym-dt] Partition: " << sub_vars_it->first << " -> {";

    for (vector<Node>::iterator part_it = (sub_vars_it->second).begin();
         part_it != (sub_vars_it->second).end();
         ++part_it)
    {
      Trace(c) << " " << *part_it;
    }
    Trace(c) << " }" << endl;
  }
}
}  // namespace passes
}  // namespace preprocessing
}  // namespace CVC4
