/***************************************************************************************[Solver.cc]
Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
Copyright (c) 2007-2010, Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#include <math.h>

#include "mtl/Sort.h"
#include "core/Solver.h"
#include <vector>
#include <iostream>

#include "util/output.h"
#include "util/utility.h"
#include "util/exception.h"
#include "theory/bv/options.h"
#include "theory/interrupted.h"

#include "proof/proof_manager.h"
#include "proof/bitvector_proof.h"
#include "proof/sat_proof.h"
#include "proof/sat_proof_implementation.h"

using namespace BVMinisat;
using namespace CVC4;
namespace BVMinisat {

#define OUTPUT_TAG "bvminisat: [a=" << assumptions.size() << ",l=" << decisionLevel() << "] "

std::ostream& operator << (std::ostream& out, const BVMinisat::Lit& l) {
  out << (sign(l) ? "-" : "") << var(l) + 1;
  return out;
}

std::ostream& operator << (std::ostream& out, const BVMinisat::Clause& c) {
  for (int i = 0; i < c.size(); i++) {
    if (i > 0) {
      out << " ";
    }
    out << c[i];
  }
  return out;
}

}

//=================================================================================================
// Options:


static const char* _cat = "CORE";

// static DoubleOption  opt_var_decay         (_cat, "var-decay",   "The variable activity decay factor",            0.95,     DoubleRange(0, false, 1, false));
// static DoubleOption  opt_clause_decay      (_cat, "cla-decay",   "The clause activity decay factor",              0.999,    DoubleRange(0, false, 1, false));
// static DoubleOption  opt_random_var_freq   (_cat, "rnd-freq",    "The frequency with which the decision heuristic tries to choose a random variable", 0.0, DoubleRange(0, true, 1, true));
// static DoubleOption  opt_random_seed       (_cat, "rnd-seed",    "Used by the random variable selection",         91648253, DoubleRange(0, false, HUGE_VAL, false));
// static IntOption     opt_ccmin_mode        (_cat, "ccmin-mode",  "Controls conflict clause minimization (0=none, 1=basic, 2=deep)", 0, IntRange(0, 2));
// static IntOption     opt_phase_saving      (_cat, "phase-saving", "Controls the level of phase saving (0=none, 1=limited, 2=full)", 2, IntRange(0, 2));
// static BoolOption    opt_rnd_init_act      (_cat, "rnd-init",    "Randomize the initial activity", false);
// static BoolOption    opt_luby_restart      (_cat, "luby",        "Use the Luby restart sequence", true);
// static IntOption     opt_restart_first     (_cat, "rfirst",      "The base restart interval", 100, IntRange(1, INT32_MAX)); 
// static DoubleOption  opt_restart_inc       (_cat, "rinc",        "Restart interval increase factor", 1.5, DoubleRange(1, false, HUGE_VAL, false));
// static DoubleOption  opt_garbage_frac      (_cat, "gc-frac",     "The fraction of wasted memory allowed before a garbage collection is triggered",  0.20, DoubleRange(0, false, HUGE_VAL, false));


static DoubleOption  opt_var_decay         (_cat, "var-decay",   "The variable activity decay factor",            0.95,     DoubleRange(0, false, 1, false));
static DoubleOption  opt_clause_decay      (_cat, "cla-decay",   "The clause activity decay factor",              0.999,    DoubleRange(0, false, 1, false));
static DoubleOption  opt_random_var_freq   (_cat, "rnd-freq",    "The frequency with which the decision heuristic tries to choose a random variable", 0, DoubleRange(0, true, 1, true));
static DoubleOption  opt_random_seed       (_cat, "rnd-seed",    "Used by the random variable selection",         91648253, DoubleRange(0, false, HUGE_VAL, false));
static IntOption     opt_ccmin_mode        (_cat, "ccmin-mode",  "Controls conflict clause minimization (0=none, 1=basic, 2=deep)", 2, IntRange(0, 2));
static IntOption     opt_phase_saving      (_cat, "phase-saving", "Controls the level of phase saving (0=none, 1=limited, 2=full)", 2, IntRange(0, 2));
static BoolOption    opt_rnd_init_act      (_cat, "rnd-init",    "Randomize the initial activity", false);
static BoolOption    opt_luby_restart      (_cat, "luby",        "Use the Luby restart sequence", true);
static IntOption     opt_restart_first     (_cat, "rfirst",      "The base restart interval", 25, IntRange(1, INT32_MAX));
static DoubleOption  opt_restart_inc       (_cat, "rinc",        "Restart interval increase factor", 3, DoubleRange(1, false, HUGE_VAL, false));
static DoubleOption  opt_garbage_frac      (_cat, "gc-frac",     "The fraction of wasted memory allowed before a garbage collection is triggered",  0.20, DoubleRange(0, false, HUGE_VAL, false));

//=================================================================================================
// Proof declarations
CRef Solver::TCRef_Undef = CRef_Undef;
CRef Solver::TCRef_Lazy = CRef_Undef - 1; // no real lazy ref here


//=================================================================================================
// Constructor/Destructor:


Solver::Solver(CVC4::context::Context* c) :

    // Parameters (user settable):
    //
    c(c)
  , verbosity        (0)
  , var_decay        (opt_var_decay)
  , clause_decay     (opt_clause_decay)
  , random_var_freq  (opt_random_var_freq)
  , random_seed      (opt_random_seed)
  , luby_restart     (opt_luby_restart)
  , ccmin_mode       (opt_ccmin_mode)
  , phase_saving     (opt_phase_saving)
  , rnd_pol          (false)
  , rnd_init_act     (opt_rnd_init_act)
  , garbage_frac     (opt_garbage_frac)
  , restart_first    (opt_restart_first)
  , restart_inc      (opt_restart_inc)

    // Parameters (the rest):
    //
  , learntsize_factor((double)1/(double)3), learntsize_inc(1.5)

    // Parameters (experimental):
    //
  , learntsize_adjust_start_confl (100)
  , learntsize_adjust_inc         (1.5)

    // Statistics: (formerly in 'SolverStats')
    //
  , solves(0), starts(0), decisions(0), rnd_decisions(0), propagations(0), conflicts(0)
  , dec_vars(0), clauses_literals(0), learnts_literals(0), max_literals(0), tot_literals(0)

  , need_to_propagate(false)
  , only_bcp(false)
  , clause_added(false)
  , ok                 (true)
  , cla_inc            (1)
  , var_inc            (1)
  , watches            (WatcherDeleted(ca))
  , qhead              (0)
  , simpDB_assigns     (-1)
  , simpDB_props       (0)
  , order_heap         (VarOrderLt(activity))
  , progress_estimate  (0)
  , remove_satisfied   (true)

  , ca                 ()

  // even though these are temporaries and technically should be set
  // before calling, lets intialize them. this will reduces chances of
  // non-determinism in portfolio (parallel) solver if variables are
  // being (incorrectly) used without initialization.
  , seen(),  analyze_stack(), analyze_toclear(), add_tmp()
  , max_learnts(0.0), learntsize_adjust_confl(0.0), learntsize_adjust_cnt(0)

    // Resource constraints:
    //
  , conflict_budget    (-1)
  , propagation_budget (-1)
  , asynch_interrupt   (false)
{
  PROOF(ProofManager::getBitVectorProof()->initSatProof(this););
  
  // Create the constant variables
  varTrue = newVar(true, false);
  varFalse = newVar(false, false);

  // Assert the constants
  uncheckedEnqueue(mkLit(varTrue, false));
  uncheckedEnqueue(mkLit(varFalse, true));
  PROOF( ProofManager::getBitVectorProof()->getSatProof()->registerUnitClause(mkLit(varTrue, false), INPUT); )
  PROOF( ProofManager::getBitVectorProof()->getSatProof()->registerUnitClause(mkLit(varFalse, true), INPUT); )
}


Solver::~Solver()
{
}


//=================================================================================================
// Minor methods:


// Creates a new SAT variable in the solver. If 'decision' is cleared, variable will not be
// used as a decision variable (NOTE! This has effects on the meaning of a SATISFIABLE result).
//
Var Solver::newVar(bool sign, bool dvar)
{
    int v = nVars();
    watches  .init(mkLit(v, false));
    watches  .init(mkLit(v, true ));
    assigns  .push(l_Undef);
    vardata  .push(mkVarData(CRef_Undef, 0));
    marker   .push(0);
    //activity .push(0);
    activity .push(rnd_init_act ? drand(random_seed) * 0.00001 : 0);
    seen     .push(0);
    polarity .push(sign);
    decision .push();
    trail    .capacity(v+1);
    setDecisionVar(v, dvar);

    return v;
}


bool Solver::addClause_(vec<Lit>& ps)
{
    if (decisionLevel() > 0) {
      cancelUntil(0);
    }
    
    if (!ok) return false;

    // Check if clause is satisfied and remove false/duplicate literals:
    sort(ps);
    Lit p; int i, j;
    for (i = j = 0, p = lit_Undef; i < ps.size(); i++)
        if (value(ps[i]) == l_True || ps[i] == ~p)
            return true;
        else if (value(ps[i]) != l_False && ps[i] != p)
            ps[j++] = p = ps[i];
    ps.shrink(i - j);

    clause_added = true;

    // TODO // PROOF unit conflict?
    if (ps.size() == 0)
        return ok = false;
    else if (ps.size() == 1){
        uncheckedEnqueue(ps[0]);
        PROOF( ProofManager::getBitVectorProof()->getSatProof()->registerUnitClause(ps[0], INPUT););
        return ok = (propagate() == CRef_Undef);
    } else {
        CRef cr = ca.alloc(ps, false);
        clauses.push(cr);
        attachClause(cr);
        PROOF( ProofManager::getBitVectorProof()->getSatProof()->registerClause(cr, INPUT););
     }
    return ok; 
}

void Solver::attachClause(CRef cr) {
    const Clause& c = ca[cr];
    assert(c.size() > 1);
    watches[~c[0]].push(Watcher(cr, c[1]));
    watches[~c[1]].push(Watcher(cr, c[0]));
    if (c.learnt()) learnts_literals += c.size();
    else            clauses_literals += c.size(); }


void Solver::detachClause(CRef cr, bool strict) {
    const Clause& c = ca[cr];
    PROOF( ProofManager::getBitVectorProof()->getSatProof()->markDeleted(cr); );
    
    assert(c.size() > 1);
    
    if (strict){
        remove(watches[~c[0]], Watcher(cr, c[1]));
        remove(watches[~c[1]], Watcher(cr, c[0]));
    }else{
        // Lazy detaching: (NOTE! Must clean all watcher lists before garbage collecting this clause)
        watches.smudge(~c[0]);
        watches.smudge(~c[1]);
    }

    if (c.learnt()) learnts_literals -= c.size();
    else            clauses_literals -= c.size(); }


void Solver::removeClause(CRef cr) {
    Clause& c = ca[cr];
    detachClause(cr);
    // Don't leave pointers to free'd memory!
    if (locked(c)) vardata[var(c[0])].reason = CRef_Undef;
    c.mark(1); 
    ca.free(cr);
}


bool Solver::satisfied(const Clause& c) const {
    for (int i = 0; i < c.size(); i++)
        if (value(c[i]) == l_True)
            return true;
    return false; }


// Revert to the state at given level (keeping all assignment at 'level' but not beyond).
//
void Solver::cancelUntil(int level) {
    if (decisionLevel() > level){
      Debug("bvminisat::explain") << OUTPUT_TAG << " backtracking to " << level << std::endl;
      for (int c = trail.size()-1; c >= trail_lim[level]; c--){
            Var      x  = var(trail[c]);
            assigns [x] = l_Undef;
            if (marker[x] == 2) marker[x] = 1;
            if (phase_saving > 1 || (phase_saving == 1) && c > trail_lim.last())
                polarity[x] = sign(trail[c]);
            insertVarOrder(x); }
        qhead = trail_lim[level];
        trail.shrink(trail.size() - trail_lim[level]);
        trail_lim.shrink(trail_lim.size() - level);
    }
}


//=================================================================================================
// Major methods:


Lit Solver::pickBranchLit()
{
    Var next = var_Undef;

    // Random decision:
    if (drand(random_seed) < random_var_freq && !order_heap.empty()){
        next = order_heap[irand(random_seed,order_heap.size())];
        if (value(next) == l_Undef && decision[next])
            rnd_decisions++; }

    // Activity based decision:
    while (next == var_Undef || value(next) != l_Undef || !decision[next])
        if (order_heap.empty()){
            next = var_Undef;
            break;
        }else
            next = order_heap.removeMin();

    return next == var_Undef ? lit_Undef : mkLit(next, rnd_pol ? drand(random_seed) < 0.5 : polarity[next]);
}


/*_________________________________________________________________________________________________
|
|  analyze : (confl : Clause*) (out_learnt : vec<Lit>&) (out_btlevel : int&)  ->  [void]
|  
|  Description:
|    Analyze conflict and produce a reason clause.
|  
|    Pre-conditions:
|      * 'out_learnt' is assumed to be cleared.
|      * Current decision level must be greater than root level.
|  
|    Post-conditions:
|      * 'out_learnt[0]' is the asserting literal at level 'out_btlevel'.
|      * If out_learnt.size() > 1 then 'out_learnt[1]' has the greatest decision level of the 
|        rest of literals. There may be others from the same level though.
|  
|________________________________________________________________________________________________@*/
void Solver::analyze(CRef confl, vec<Lit>& out_learnt, int& out_btlevel, UIP uip)
{
    int pathC = 0;
    Lit p     = lit_Undef;

    // Generate conflict clause:
    //
    out_learnt.push();      // (leave room for the asserting literal)
    int index   = trail.size() - 1;

    bool done = false;
    
    PROOF( ProofManager::getBitVectorProof()->getSatProof()->startResChain(confl); )

    do{
        assert(confl != CRef_Undef); // (otherwise should be UIP)
        Clause& c = ca[confl];

        if (c.learnt())
            claBumpActivity(c);

        for (int j = (p == lit_Undef) ? 0 : 1; j < c.size(); j++){
            Lit q = c[j];

            if (!seen[var(q)] && level(var(q)) > 0) {
                varBumpActivity(var(q));
                seen[var(q)] = 1;
                if (level(var(q)) >= decisionLevel())
                    pathC++;
                else
                    out_learnt.push(q);
            }
            
            if (level(var(q)) == 0) {
              PROOF( ProofManager::getBitVectorProof()->getSatProof()->resolveOutUnit(q); );
            }
        }
        
        // Select next clause to look at:
        while (!seen[var(trail[index--])]);
        p     = trail[index+1];
        confl = reason(var(p));
        seen[var(p)] = 0;
        pathC--;

        if ( pathC > 0 && confl != CRef_Undef ) {
          PROOF( ProofManager::getBitVectorProof()->getSatProof()->addResolutionStep(p, confl, sign(p)););
        }

        switch (uip) {
        case UIP_FIRST:
          done = pathC == 0;
          break;
        case UIP_LAST:
          done = confl == CRef_Undef || (pathC == 0 && marker[var(p)] == 2);
          break;
        default:
          Unreachable();
          break;
        }
    } while (!done);
    out_learnt[0] = ~p;

    // Simplify conflict clause:
    //
    int i, j;
    out_learnt.copyTo(analyze_toclear);
    if (ccmin_mode == 2){
        uint32_t abstract_level = 0;
        for (i = 1; i < out_learnt.size(); i++)
            abstract_level |= abstractLevel(var(out_learnt[i])); // (maintain an abstraction of levels involved in conflict)

        for (i = j = 1; i < out_learnt.size(); i++) {
            if (reason(var(out_learnt[i])) == CRef_Undef) {
                out_learnt[j++] = out_learnt[i];
            } else {
              // Check if the literal is redundant
              if (!litRedundant(out_learnt[i], abstract_level)) {
                // Literal is not redundant
                out_learnt[j++] = out_learnt[i];
              } else {
                PROOF( ProofManager::getBitVectorProof()->getSatProof()->storeLitRedundant(out_learnt[i]); )
              }
            }
        }
        
    }else if (ccmin_mode == 1){
        Unreachable();
        for (i = j = 1; i < out_learnt.size(); i++){
            Var x = var(out_learnt[i]);

            if (reason(x) == CRef_Undef)
                out_learnt[j++] = out_learnt[i];
            else{
                Clause& c = ca[reason(var(out_learnt[i]))];
                for (int k = 1; k < c.size(); k++)
                    if (!seen[var(c[k])] && level(var(c[k])) > 0){
                        out_learnt[j++] = out_learnt[i];
                        break; }
            }
        }
    }else
        i = j = out_learnt.size();

    max_literals += out_learnt.size();
    out_learnt.shrink(i - j);
    tot_literals += out_learnt.size();

    for (int i = 0; i < out_learnt.size(); ++ i) {
      if (marker[var(out_learnt[i])] == 0) {
        break;
      }
    }

    // Find correct backtrack level:
    //
    if (out_learnt.size() == 1) {
      out_btlevel = 0;
    }
    else{
        int max_i = 1;
        // Find the first literal assigned at the next-highest level:
        for (int i = 2; i < out_learnt.size(); i++)
            if (level(var(out_learnt[i])) > level(var(out_learnt[max_i])))
                max_i = i;
        // Swap-in this literal at index 1:
        Lit p             = out_learnt[max_i];
        out_learnt[max_i] = out_learnt[1];
        out_learnt[1]     = p;
        out_btlevel       = level(var(p));
    }

    for (int j = 0; j < analyze_toclear.size(); j++) seen[var(analyze_toclear[j])] = 0;    // ('seen[]' is now cleared)
}


// Check if 'p' can be removed. 'abstract_levels' is used to abort early if the algorithm is
// visiting literals at levels that cannot be removed later.
bool Solver::litRedundant(Lit p, uint32_t abstract_levels)
{
    analyze_stack.clear(); analyze_stack.push(p);
    int top = analyze_toclear.size();
    while (analyze_stack.size() > 0){
        CRef c_reason = reason(var(analyze_stack.last()));
        assert(c_reason != CRef_Undef);
        Clause& c = ca[c_reason];
        int c_size = c.size();
        analyze_stack.pop();

        for (int i = 1; i < c_size; i++){
            Lit p  = c[i];
            if (!seen[var(p)] && level(var(p)) > 0){
                if (reason(var(p)) != CRef_Undef && (abstractLevel(var(p)) & abstract_levels) != 0){
                    seen[var(p)] = 1;
                    analyze_stack.push(p);
                    analyze_toclear.push(p);
                }else{
                    for (int j = top; j < analyze_toclear.size(); j++)
                        seen[var(analyze_toclear[j])] = 0;
                    analyze_toclear.shrink(analyze_toclear.size() - top);
                    return false;
                }
            }
        }
    }

    return true;
}

/** 
 * Specialized analyzeFinal procedure where we test the consistency
 * of the assumptions before backtracking bellow the assumption level.
 * 
 * @param p the original uip (may be unit)
 * @param confl_clause the conflict clause
 * @param out_conflict the conflict in terms of assumptions we are building
 */
void Solver::analyzeFinal2(Lit p, CRef confl_clause, vec<Lit>& out_conflict) {
  assert (confl_clause != CRef_Undef);
  assert (decisionLevel() == assumptions.size());
  assert (level(var(p)) == assumptions.size());

  out_conflict.clear(); 
  
  Clause& cl = ca[confl_clause];
  for (int i = 0; i < cl.size(); ++i) {
    seen[var(cl[i])] = 1;
  }

  for (int i = trail.size() - 1; i >= trail_lim[0]; i--) {
    Var x = var(trail[i]);
    if (seen[x]) {
      if (reason(x) == CRef_Undef) {
        // we skip p if was a learnt unit
        if (x != var(p)) {
          assert (marker[x] == 2);
          assert (level(x) > 0);
          out_conflict.push(~trail[i]);
        }
      } else {
        Clause& c = ca[reason(x)];
        PROOF(ProofManager::getBitVectorProof()->getSatProof()->addResolutionStep(trail[i],reason(x), sign(trail[i])););

        for (int j = 1; j < c.size(); j++)
          if (level(var(c[j])) > 0)
            seen[var(c[j])] = 1;
      }
      seen[x] = 0;
    }
    assert (seen[x] == 0); 
  }
  assert (out_conflict.size()); 
}

/*_________________________________________________________________________________________________
|
|  analyzeFinal : (p : Lit)  ->  [void]
|  
|  Description:
|    Specialized analysis procedure to express the final conflict in terms of assumptions.
|    Calculates the (possibly empty) set of assumptions that led to the assignment of 'p', and
|    stores the result in 'out_conflict'.
|________________________________________________________________________________________________@*/
void Solver::analyzeFinal(Lit p, vec<Lit>& out_conflict)
{
    out_conflict.clear();
    if (marker[var(p)] == 2) {
      out_conflict.push(p);
    }

    if (decisionLevel() == 0)
        return;

    seen[var(p)] = 1;

    for (int i = trail.size()-1; i >= trail_lim[0]; i--){
        Var x = var(trail[i]);
        if (seen[x]) {
            if (reason(x) == CRef_Undef) {
              assert(marker[x] == 2);
              assert(level(x) > 0);
              out_conflict.push(~trail[i]);
            } else {
              Clause& c = ca[reason(x)];
              // TODO add resolution step here?
              PROOF(ProofManager::getBitVectorProof()->getSatProof()->addResolutionStep(trail[i], reason(x), sign(trail[i])));
              for (int j = 1; j < c.size(); j++)
                if (level(var(c[j])) > 0) {
                  seen[var(c[j])] = 1;
                }
            }
            seen[x] = 0;
        }
    }

    seen[var(p)] = 0;
    assert (out_conflict.size());
}


void Solver::uncheckedEnqueue(Lit p, CRef from)
{
    assert(value(p) == l_Undef);
    assigns[var(p)] = lbool(!sign(p));
    vardata[var(p)] = mkVarData(from, decisionLevel());
    trail.push_(p);
    if (decisionLevel() <= assumptions.size() && marker[var(p)] == 1) {
      if (notify) {
        Debug("bvminisat::explain") << OUTPUT_TAG << "propagating " << p << std::endl;
        notify->notify(p);
      }
    }
}

void Solver::popAssumption() {
    assumptions.pop();
    conflict.clear();
    cancelUntil(assumptions.size());
}

lbool Solver::propagateAssumptions() {
  only_bcp = true;
  ccmin_mode = 0;
  return search(-1);
}

lbool Solver::assertAssumption(Lit p, bool propagate) {
  // TODO need to somehow mark the assumption as unit in the current context?
  // it's not always unit though, but this would be useful for debugging
  
  // assert(marker[var(p)] == 1);
  
  if (decisionLevel() > assumptions.size()) {
    cancelUntil(assumptions.size());
  }

  conflict.clear();

  // add to the assumptions
  if (c->getLevel() > 0) {
    assumptions.push(p);
  } else {
    if (!addClause(p)) {
      conflict.push(~p);
      return l_False;
    }
  }

  // run the propagation
  if (propagate) {
    only_bcp = true;
    ccmin_mode = 0; 
    lbool result = search(-1);
    return result; 
  } else {
    return l_True;
  }
}

/*_________________________________________________________________________________________________
|
|  propagate : [void]  ->  [Clause*]
|  
|  Description:
|    Propagates all enqueued facts. If a conflict arises, the conflicting clause is returned,
|    otherwise CRef_Undef.
|  
|    Post-conditions:
|      * the propagation queue is empty, even if there was a conflict.
|________________________________________________________________________________________________@*/
CRef Solver::propagate()
{
    CRef    confl     = CRef_Undef;
    int     num_props = 0;
    watches.cleanAll();

    while (qhead < trail.size()){
        Lit            p   = trail[qhead++];     // 'p' is enqueued fact to propagate.
        vec<Watcher>&  ws  = watches[p];
        Watcher        *i, *j, *end;
        num_props++;

        for (i = j = (Watcher*)ws, end = i + ws.size();  i != end;){
            // Try to avoid inspecting the clause:
            Lit blocker = i->blocker;
            if (value(blocker) == l_True){
                *j++ = *i++; continue; }

            // Make sure the false literal is data[1]:
            CRef     cr        = i->cref;
            Clause&  c         = ca[cr];
            Lit      false_lit = ~p;
            if (c[0] == false_lit)
                c[0] = c[1], c[1] = false_lit;
            assert(c[1] == false_lit);
            i++;

            // If 0th watch is true, then clause is already satisfied.
            Lit     first = c[0];
            Watcher w     = Watcher(cr, first);
            if (first != blocker && value(first) == l_True){
                *j++ = w; continue; }

            // Look for new watch:
            for (int k = 2; k < c.size(); k++)
                if (value(c[k]) != l_False){
                    c[1] = c[k]; c[k] = false_lit;
                    watches[~c[1]].push(w);
                    goto NextClause; }

            // Did not find watch -- clause is unit under assignment:
            *j++ = w;
            if (value(first) == l_False){
                confl = cr;
                qhead = trail.size();
                // Copy the remaining watches:
                while (i < end)
                    *j++ = *i++;
            }else
                uncheckedEnqueue(first, cr);

        NextClause:;
        }
        ws.shrink(i - j);
    }
    propagations += num_props;
    simpDB_props -= num_props;

    return confl;
}


/*_________________________________________________________________________________________________
|
|  reduceDB : ()  ->  [void]
|  
|  Description:
|    Remove half of the learnt clauses, minus the clauses locked by the current assignment. Locked
|    clauses are clauses that are reason to some assignment. Binary clauses are never removed.
|________________________________________________________________________________________________@*/
struct reduceDB_lt { 
    ClauseAllocator& ca;
    reduceDB_lt(ClauseAllocator& ca_) : ca(ca_) {}
    bool operator () (CRef x, CRef y) { 
        return ca[x].size() > 2 && (ca[y].size() == 2 || ca[x].activity() < ca[y].activity()); } 
};
void Solver::reduceDB()
{
    int     i, j;
    double  extra_lim = cla_inc / learnts.size();    // Remove any clause below this activity

    sort(learnts, reduceDB_lt(ca));
    // Don't delete binary or locked clauses. From the rest, delete clauses from the first half
    // and clauses with activity smaller than 'extra_lim':
    for (i = j = 0; i < learnts.size(); i++){
        Clause& c = ca[learnts[i]];
        if (c.size() > 2 && !locked(c) && (i < learnts.size() / 2 || c.activity() < extra_lim))
            removeClause(learnts[i]);
        else
            learnts[j++] = learnts[i];
    }
    learnts.shrink(i - j);
    checkGarbage();
}


void Solver::removeSatisfied(vec<CRef>& cs)
{
    int i, j;
    for (i = j = 0; i < cs.size(); i++){
        Clause& c = ca[cs[i]];
        if (satisfied(c)) {
          if (locked(c)) {
            // store a resolution of the literal c propagated
            PROOF( ProofManager::getBitVectorProof()->getSatProof()->storeUnitResolution(c[0]); )
          }
            removeClause(cs[i]);
        }
        else
            cs[j++] = cs[i];
    }
    cs.shrink(i - j);
}


void Solver::rebuildOrderHeap()
{
    vec<Var> vs;
    for (Var v = 0; v < nVars(); v++)
        if (decision[v] && value(v) == l_Undef)
            vs.push(v);
    order_heap.build(vs);
}


/*_________________________________________________________________________________________________
|
|  simplify : [void]  ->  [bool]
|  
|  Description:
|    Simplify the clause database according to the current top-level assigment. Currently, the only
|    thing done here is the removal of satisfied clauses, but more things can be put here.
|________________________________________________________________________________________________@*/
bool Solver::simplify()
{
    assert(decisionLevel() == 0);

    if (!ok || propagate() != CRef_Undef)
        return ok = false;

    if (nAssigns() == simpDB_assigns || (simpDB_props > 0))
        return true;

    // Remove satisfied clauses:
    removeSatisfied(learnts);
    if (remove_satisfied)        // Can be turned off.
        removeSatisfied(clauses);
    checkGarbage();
    rebuildOrderHeap();

    simpDB_assigns = nAssigns();
    simpDB_props   = clauses_literals + learnts_literals;   // (shouldn't depend on stats really, but it will do for now)

    return true;
}


/*_________________________________________________________________________________________________
|
|  search : (nof_conflicts : int) (params : const SearchParams&)  ->  [lbool]
|  
|  Description:
|    Search for a model the specified number of conflicts. 
|    NOTE! Use negative value for 'nof_conflicts' indicate infinity.
|  
|  Output:
|    'l_True' if a partial assigment that is consistent with respect to the clauseset is found. If
|    all variables are decision variables, this means that the clause set is satisfiable. 'l_False'
|    if the clause set is unsatisfiable. 'l_Undef' if the bound on number of conflicts is reached.
|________________________________________________________________________________________________@*/
lbool Solver::search(int nof_conflicts, UIP uip)
{
    assert(ok);
    int         backtrack_level;
    int         conflictC = 0;
    vec<Lit>    learnt_clause;
    starts++;

    for (;;){
        CRef confl = propagate();
        if (confl != CRef_Undef){
            // CONFLICT
            conflicts++; conflictC++;

            if (decisionLevel() == 0) {
              // can this happen for bv?
              PROOF( ProofManager::getBitVectorProof()->getSatProof()->finalizeProof(confl););
              return l_False;
            }

            learnt_clause.clear();
            analyze(confl, learnt_clause, backtrack_level, uip);

            Lit p = learnt_clause[0];
            //bool assumption = marker[var(p)] == 2;

            CRef cr = CRef_Undef;
            if (learnt_clause.size() > 1) {
              cr = ca.alloc(learnt_clause, true);
              learnts.push(cr);
              attachClause(cr);
              claBumpActivity(ca[cr]);
              PROOF( ProofManager::getBitVectorProof()->getSatProof()->endResChain(cr); );
            }
            
            if (learnt_clause.size() == 1) {
              // learning a unit clause
              PROOF( ProofManager::getBitVectorProof()->getSatProof()->endResChain(learnt_clause[0]););
              // TODO: start new resolution chain if unit?
            }
            
            //  if the uip was an assumption we are unsat
            if (level(var(p)) <= assumptions.size()) {
              for (int i = 0; i < learnt_clause.size(); ++i) {
                assert (level(var(learnt_clause[i])) <= decisionLevel()); 
                seen[var(learnt_clause[i])] = 1;
              }

              // Starting new resolution chain for bit-vector proof
              Assert (cr != CRef_Undef); // This will probably fail
              PROOF (ProofManager::getBitVectorProof()->startBVConflict(cr));
              analyzeFinal(p, conflict);
              PROOF (ProofManager::getBitVectorProof()->endBVConflict(conflict););
              Debug("bvminisat::search") << OUTPUT_TAG << " conflict on assumptions " << std::endl;
              return l_False;
            }

            if (!CVC4::options::bvEagerExplanations()) {
              // check if uip leads to a conflict 
              if (backtrack_level < assumptions.size()) {
                cancelUntil(assumptions.size());
                uncheckedEnqueue(p, cr);
              
                CRef new_confl = propagate();
                if (new_confl != CRef_Undef) {
                  // we have a conflict we now need to explain it
                  // TODO: proof for analyzeFinal2
                  PROOF (ProofManager::getBitVectorProof()->startBVConflict(new_confl););
                  analyzeFinal2(p, new_confl, conflict);
                  PROOF (ProofManager::getBitVectorProof()->endBVConflict(conflict););
                  return l_False;
                }
              }
            }

            cancelUntil(backtrack_level);
            uncheckedEnqueue(p, cr);
    
         
            varDecayActivity();
            claDecayActivity();

            if (--learntsize_adjust_cnt == 0){
                learntsize_adjust_confl *= learntsize_adjust_inc;
                learntsize_adjust_cnt    = (int)learntsize_adjust_confl;
                max_learnts             *= learntsize_inc;

                if (verbosity >= 1)
                    printf("| %9d | %7d %8d %8d | %8d %8d %6.0f | %6.3f %% |\n", 
                           (int)conflicts, 
                           (int)dec_vars - (trail_lim.size() == 0 ? trail.size() : trail_lim[0]), nClauses(), (int)clauses_literals, 
                           (int)max_learnts, nLearnts(), (double)learnts_literals/nLearnts(), progressEstimate()*100);
            }

        }else{
            // NO CONFLICT
            bool isWithinBudget;
            try {
              isWithinBudget = withinBudget(); 
            }
            catch (const CVC4::theory::Interrupted& e) {
              // do some clean-up and rethrow 
              cancelUntil(assumptions.size()); 
              throw e; 
            }
            
            if (decisionLevel() > assumptions.size() && nof_conflicts >= 0 && conflictC >= nof_conflicts ||
                !isWithinBudget) {
                // Reached bound on number of conflicts:
                Debug("bvminisat::search") << OUTPUT_TAG << " restarting " << std::endl;
                progress_estimate = progressEstimate();
                cancelUntil(assumptions.size());
                return l_Undef;
            }
 
            // Simplify the set of problem clauses:
            if (decisionLevel() == 0 && !simplify()) {
                Debug("bvminisat::search") << OUTPUT_TAG << " base level conflict, we're unsat" << std::endl;
                return l_False;
            }

            // We can't erase clauses if there is unprocessed assumptions, there might be some
            // propagationg we need to redu
            if (decisionLevel() >= assumptions.size() && learnts.size()-nAssigns() >= max_learnts) {
                // Reduce the set of learnt clauses:
                Debug("bvminisat::search") << OUTPUT_TAG << " cleaning up database" << std::endl;
                reduceDB();
            }

            Lit next = lit_Undef;
            while (decisionLevel() < assumptions.size()){
                // Perform user provided assumption:
                Lit p = assumptions[decisionLevel()];
                if (value(p) == l_True){
                    // Dummy decision level:
                    newDecisionLevel();
                }else if (value(p) == l_False){
                    marker[var(p)] = 2;
                    // TODO Start resolution chain with reason for var(p)?
                    
                    PROOF ( ProofManager::getBitVectorProof()->startBVConflict(reason(var(p))););
                    analyzeFinal(~p, conflict);
                    PROOF (ProofManager::getBitVectorProof()->endBVConflict(conflict););
                    Debug("bvminisat::search") << OUTPUT_TAG << " assumption false, we're unsat" << std::endl;
                    return l_False;
                }else{
                    marker[var(p)] = 2;
                    next = p;
                    break;
                }
            }

            if (next == lit_Undef){

                if (only_bcp) {
                  Debug("bvminisat::search") << OUTPUT_TAG << " only bcp, skipping rest of the problem" << std::endl;
                  return l_True;
                }

                // New variable decision:
                decisions++;
                next = pickBranchLit();

                if (next == lit_Undef) {
                    Debug("bvminisat::search") << OUTPUT_TAG << " satisfiable" << std::endl;
                    // Model found:
                    return l_True;
                }
            }

            // Increase decision level and enqueue 'next'
            newDecisionLevel();
            uncheckedEnqueue(next);
        }
    }
}


double Solver::progressEstimate() const
{
    double  progress = 0;
    double  F = 1.0 / nVars();

    for (int i = 0; i <= decisionLevel(); i++){
        int beg = i == 0 ? 0 : trail_lim[i - 1];
        int end = i == decisionLevel() ? trail.size() : trail_lim[i];
        progress += pow(F, i) * (end - beg);
    }

    return progress / nVars();
}

/*
  Finite subsequences of the Luby-sequence:

  0: 1
  1: 1 1 2
  2: 1 1 2 1 1 2 4
  3: 1 1 2 1 1 2 4 1 1 2 1 1 2 4 8
  ...


 */

static double luby(double y, int x){

    // Find the finite subsequence that contains index 'x', and the
    // size of that subsequence:
    int size, seq;
    for (size = 1, seq = 0; size < x+1; seq++, size = 2*size+1);

    while (size-1 != x){
        size = (size-1)>>1;
        seq--;
        x = x % size;
    }

    return pow(y, seq);
}

// NOTE: assumptions passed in member-variable 'assumptions'.
lbool Solver::solve_()
{
    Debug("bvminisat") <<"BVMinisat::Solving learned clauses " << learnts.size() <<"\n";
    Debug("bvminisat") <<"BVMinisat::Solving assumptions " << assumptions.size() <<"\n";

    model.clear();
    conflict.clear();

    ccmin_mode = 0;
    
    if (!ok) return l_False;

    solves++;

    max_learnts               = nClauses() * learntsize_factor;
    learntsize_adjust_confl   = learntsize_adjust_start_confl;
    learntsize_adjust_cnt     = (int)learntsize_adjust_confl;
    lbool   status            = l_Undef;

    if (verbosity >= 1){
        printf("============================[ Search Statistics ]==============================\n");
        printf("| Conflicts |          ORIGINAL         |          LEARNT          | Progress |\n");
        printf("|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |\n");
        printf("===============================================================================\n");
    }

    // Search:
    int curr_restarts = 0;
    while (status == l_Undef){
        double rest_base = luby_restart ? luby(restart_inc, curr_restarts) : pow(restart_inc, curr_restarts);
        status = search(rest_base * restart_first);
        if (!withinBudget()) break;
        curr_restarts++;
    }

    if (verbosity >= 1)
        printf("===============================================================================\n");

    if (status == l_True){
        // Extend & copy model:
        // model.growTo(nVars());
        // for (int i = 0; i < nVars(); i++) model[i] = value(i);
    }else if (status == l_False && conflict.size() == 0)
        ok = false;

    return status;
}

//=================================================================================================
// Bitvector propagations
// 

void Solver::explain(Lit p, std::vector<Lit>& explanation) {
  vec<Lit> queue;
  queue.push(p);

  Debug("bvminisat::explain") << OUTPUT_TAG << "starting explain of " << p << std::endl;

   __gnu_cxx::hash_set<Var> visited;
  visited.insert(var(p));
  
  while(queue.size() > 0) {
    Lit l = queue.last();

    Debug("bvminisat::explain") << OUTPUT_TAG << "processing " << l << std::endl;

    assert(value(l) == l_True);
    queue.pop();
    if (reason(var(l)) == CRef_Undef) {
      if (level(var(l)) == 0) continue;
      Assert(marker[var(l)] == 2);
      explanation.push_back(l);
      visited.insert(var(l));
    } else {
      Clause& c = ca[reason(var(l))];
      for (int i = 1; i < c.size(); ++i) {
        if (level(var(c[i])) > 0 && visited.count(var(c[i])) == 0) {
          queue.push(~c[i]);
          visited.insert(var(c[i]));
        }
      }
    }
  }
}

  

//=================================================================================================
// Writing CNF to DIMACS:
// 
// FIXME: this needs to be rewritten completely.

static Var mapVar(Var x, vec<Var>& map, Var& max)
{
    if (map.size() <= x || map[x] == -1){
        map.growTo(x+1, -1);
        map[x] = max++;
    }
    return map[x];
}


void Solver::toDimacs(FILE* f, Clause& c, vec<Var>& map, Var& max)
{
    if (satisfied(c)) return;

    for (int i = 0; i < c.size(); i++)
        if (value(c[i]) != l_False)
            fprintf(f, "%s%d ", sign(c[i]) ? "-" : "", mapVar(var(c[i]), map, max)+1);
    fprintf(f, "0\n");
}


void Solver::toDimacs(const char *file, const vec<Lit>& assumps)
{
    FILE* f = fopen(file, "wr");
    if (f == NULL)
        fprintf(stderr, "could not open file %s\n", file), exit(1);
    toDimacs(f, assumps);
    fclose(f);
}


void Solver::toDimacs(FILE* f, const vec<Lit>& assumps)
{
    // Handle case when solver is in contradictory state:
    if (!ok){
        fprintf(f, "p cnf 1 2\n1 0\n-1 0\n");
        return; }

    vec<Var> map; Var max = 0;

    // Cannot use removeClauses here because it is not safe
    // to deallocate them at this point. Could be improved.
    int cnt = 0;
    for (int i = 0; i < clauses.size(); i++)
        if (!satisfied(ca[clauses[i]]))
            cnt++;
        
    for (int i = 0; i < clauses.size(); i++)
        if (!satisfied(ca[clauses[i]])){
            Clause& c = ca[clauses[i]];
            for (int j = 0; j < c.size(); j++)
                if (value(c[j]) != l_False)
                    mapVar(var(c[j]), map, max);
        }

    // Assumptions are added as unit clauses:
    cnt += assumps.size();

    fprintf(f, "p cnf %d %d\n", max, cnt);

    for (int i = 0; i < assumps.size(); i++){
        assert(value(assumps[i]) != l_False);
        fprintf(f, "%s%d 0\n", sign(assumps[i]) ? "-" : "", mapVar(var(assumps[i]), map, max)+1);
    }

    for (int i = 0; i < clauses.size(); i++)
        toDimacs(f, ca[clauses[i]], map, max);

    if (verbosity > 0)
        printf("Wrote %d clauses with %d variables.\n", cnt, max);
}


//=================================================================================================
// Garbage Collection methods:

void Solver::relocAll(ClauseAllocator& to)
{
    // All watchers:
    //
    // for (int i = 0; i < watches.size(); i++)
    watches.cleanAll();
    for (int v = 0; v < nVars(); v++)
        for (int s = 0; s < 2; s++){
            Lit p = mkLit(v, s);
            // printf(" >>> RELOCING: %s%d\n", sign(p)?"-":"", var(p)+1);
            vec<Watcher>& ws = watches[p];
            for (int j = 0; j < ws.size(); j++)
              ca.reloc(ws[j].cref, to, NULLPROOF( ProofManager::getBitVectorProof()->getSatProof()->getProxy() ));
        }

    // All reasons:
    //
    for (int i = 0; i < trail.size(); i++){
        Var v = var(trail[i]);

        if (reason(v) != CRef_Undef && (ca[reason(v)].reloced() || locked(ca[reason(v)])))
            ca.reloc(vardata[v].reason, to, NULLPROOF( ProofManager::getBitVectorProof()->getSatProof()->getProxy() ));
    }

    // All learnt:
    //
    for (int i = 0; i < learnts.size(); i++)
        ca.reloc(learnts[i], to, NULLPROOF( ProofManager::getBitVectorProof()->getSatProof()->getProxy() ));

    // All original:
    //
    for (int i = 0; i < clauses.size(); i++)
        ca.reloc(clauses[i], to, NULLPROOF( ProofManager::getBitVectorProof()->getSatProof()->getProxy() ));
	
    PROOF( ProofManager::getBitVectorProof()->getSatProof()->finishUpdateCRef(); );
}


void Solver::garbageCollect()
{
    // Initialize the next region to a size corresponding to the estimated utilization degree. This
    // is not precise but should avoid some unnecessary reallocations for the new region:
    ClauseAllocator to(ca.size() - ca.wasted()); 
    Debug("bvminisat") << " BVMinisat::Garbage collection \n"; 
    relocAll(to);
    if (verbosity >= 2)
        printf("|  Garbage collection:   %12d bytes => %12d bytes             |\n", 
               ca.size()*ClauseAllocator::Unit_Size, to.size()*ClauseAllocator::Unit_Size);
    to.moveTo(ca);
}

void ClauseAllocator::reloc(CRef& cr, ClauseAllocator& to, CVC4::BVProofProxy* proxy)
{
  CRef old = cr;  // save the old reference

  Clause& c = operator[](cr);
  if (c.reloced()) { cr = c.relocation(); return; }
  
  cr = to.alloc(c, c.learnt());
  c.relocate(cr);
  if (proxy) {
    proxy->updateCRef(old, cr); 
  }
  
  // Copy extra data-fields: 
  // (This could be cleaned-up. Generalize Clause-constructor to be applicable here instead?)
  to[cr].mark(c.mark());
  if (to[cr].learnt())         to[cr].activity() = c.activity();
  else if (to[cr].has_extra()) to[cr].calcAbstraction();
}
