/*********************                                                        */
/*! \file dtype.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief A class representing a datatype definition
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__DATATYPES__DTYPE_H
#define CVC4__THEORY__DATATYPES__DTYPE_H

#include <map>
#include <string>
#include <vector>
#include "expr/node.h"
#include "expr/type_node.h"
#include "theory/datatypes/dtype_cons.h"
#include "theory/datatypes/dtype_cons_arg.h"

namespace CVC4 {

// messy; Node needs DType (because it's the payload of a
// CONSTANT-kinded expression), and DType needs Node.
// class  DType;
class DTypeIndexConstant;

class NodeManager;

class DTypeConstructor;
class DTypeConstructorArg;

class DTypeConstructorIterator
{
  const std::vector<DTypeConstructor>* d_v;
  size_t d_i;

  friend class DType;

  DTypeConstructorIterator(const std::vector<DTypeConstructor>& v, bool start)
      : d_v(&v), d_i(start ? 0 : v.size())
  {
  }

 public:
  typedef const DTypeConstructor& value_type;
  const DTypeConstructor& operator*() const { return (*d_v)[d_i]; }
  const DTypeConstructor* operator->() const { return &(*d_v)[d_i]; }
  DTypeConstructorIterator& operator++()
  {
    ++d_i;
    return *this;
  }
  DTypeConstructorIterator operator++(int)
  {
    DTypeConstructorIterator i(*this);
    ++d_i;
    return i;
  }
  bool operator==(const DTypeConstructorIterator& other) const
  {
    return d_v == other.d_v && d_i == other.d_i;
  }
  bool operator!=(const DTypeConstructorIterator& other) const
  {
    return d_v != other.d_v || d_i != other.d_i;
  }
}; /* class DTypeConstructorIterator */

/**
 * The representation of an inductive datatype.
 *
 * This is far more complicated than it first seems.  Consider this
 * datatype definition:
 *
 *   DATATYPE nat =
 *     succ(pred: nat)
 *   | zero
 *   END;
 *
 * You cannot define "nat" until you have a Type for it, but you
 * cannot have a Type for it until you fill in the type of the "pred"
 * selector, which needs the Type.  So we have a chicken-and-egg
 * problem.  It's even more complicated when we have mutual recursion
 * between datatypes, since the CVC presentation language does not
 * require forward-declarations.  Here, we define trees of lists that
 * contain trees of lists (etc):
 *
 *   DATATYPE
 *     tree = node(left: tree, right: tree) | leaf(list),
 *     list = cons(car: tree, cdr: list) | nil
 *   END;
 *
 * Note that while parsing the "tree" definition, we have to take it
 * on faith that "list" is a valid type.  We build DType objects to
 * describe "tree" and "list", and their constructors and constructor
 * arguments, but leave any unknown types (including self-references)
 * in an "unresolved" state.  After parsing the whole DATATYPE block,
 * we create a TypeNode through
 * NodeManager::mkMutualTypeNodes().  The NodeManager creates a
 * TypeNode for each, but before "releasing" this type into the
 * wild, it does a round of in-place "resolution" on each DType by
 * calling DType::resolve() with a map of string -> TypeNode to
 * allow the datatype to construct the necessary testers and
 * selectors.
 *
 * An additional point to make is that we want to ease the burden on
 * both the parser AND the users of the CVC4 API, so this class takes
 * on the task of generating its own selectors and testers, for
 * instance.  That means that, after reifying the DType with the
 * NodeManager, the parser needs to go through the (now-resolved)
 * DType and request the constructor, selector, and tester terms.
 * See src/parser/parser.cpp for how this is done.  For API usage
 * ideas, see test/unit/util/datatype_black.h.
 *
 * DTypes may also be defined parametrically, such as this example:
 *
 *  DATATYPE
 *    list[T] = cons(car : T, cdr : list[T]) | null,
 *    tree = node(children : list[tree]) | leaf
 *  END;
 *
 * Here, the definition of the parametric datatype list, where T is a type
 * variable. In other words, this defines a family of types list[C] where C is
 * any concrete type.  DTypes can be parameterized over multiple type variables
 * using the syntax sym[ T1, ..., Tn ] = ...,
 *
 */
class DType
{
  friend class DTypeConstructor;

 public:
  /**
   * Get the datatype of a constructor, selector, or tester operator.
   */
  static const DType& datatypeOf(Node item);

  /**
   * Get the index of a constructor or tester in its datatype, or the
   * index of a selector in its constructor.  (Zero is always the
   * first index.)
   */
  static size_t indexOf(Node item);

  /**
   * Get the index of constructor corresponding to selector.  (Zero is
   * always the first index.)
   */
  static size_t cindexOf(Node item);

  /**
   * Same as above, but without checks. These methods should be used by
   * internal (Node-level) code.
   */
  static size_t indexOfInternal(Node item);
  static size_t cindexOfInternal(Node item);

  /** The type for iterators over constructors. */
  typedef DTypeConstructorIterator iterator;
  /** The (const) type for iterators over constructors. */
  typedef DTypeConstructorIterator const_iterator;

  /** Create a new DType of the given name. */
  explicit DType(std::string name, bool isCo = false);

  /**
   * Create a new DType of the given name, with the given
   * parameterization.
   */
  DType(std::string name,
        const std::vector<TypeNode>& params,
        bool isCo = false);

  ~DType();

  /** Add a constructor to this DType.
   *
   * Notice that constructor names need not
   * be unique; they are for convenience and pretty-printing only.
   */
  void addConstructor(const DTypeConstructor& c);

  /** set sygus
   *
   * This marks this datatype as a sygus datatype.
   * A sygus datatype is one that represents terms of type st
   * via a deep embedding described in Section 4 of
   * Reynolds et al. CAV 2015. We say that this sygus datatype
   * "encodes" its sygus type st in the following.
   *
   * st : the type this datatype encodes (this can be Int, Bool, etc.),
   * bvl : the list of arguments for the synth-fun
   * allow_const : whether all constants are (implicitly) allowed by the
   * datatype
   * allow_all : whether all terms are (implicitly) allowed by the datatype
   *
   * Notice that allow_const/allow_all do not reflect the constructors
   * for this datatype, and instead are used solely for relaxing constraints
   * when doing solution reconstruction (Figure 5 of Reynolds et al.
   * CAV 2015).
   */
  void setSygus(TypeNode st, Node bvl, bool allow_const, bool allow_all);
  /** add sygus constructor
   *
   * This adds a sygus constructor to this datatype, where
   * this datatype should be currently unresolved.
   *
   * op : the builtin operator, constant, or variable that
   *      this constructor encodes
   * cname : the name of the constructor (for printing only)
   * cargs : the arguments of the constructor
   * spc : an (optional) callback that is used for custom printing. This is
   *       to accomodate user-provided grammars in the sygus format.
   *
   * It should be the case that cargs are sygus datatypes that
   * encode the arguments of op. For example, a sygus constructor
   * with op = PLUS should be such that cargs.size()>=2 and
   * the sygus type of cargs[i] is Real/Int for each i.
   *
   * weight denotes the value added by the constructor when computing the size
   * of datatype terms. Passing a value <0 denotes the default weight for the
   * constructor, which is 0 for nullary constructors and 1 for non-nullary
   * constructors.
   */
  void addSygusConstructor(
      Node op,
      const std::string& cname,
      const std::vector<TypeNode>& cargs,
      std::shared_ptr<SygusPrintCallbackInternal> spc = nullptr,
      int weight = -1);

  /** set that this datatype is a tuple */
  void setTuple();

  /** Get the name of this DType. */
  std::string getName() const;

  /** Get the number of constructors (so far) for this DType. */
  size_t getNumConstructors() const;

  /** Is this datatype parametric? */
  bool isParametric() const;

  /** Get the nubmer of type parameters */
  size_t getNumParameters() const;

  /** Get parameter */
  TypeNode getParameter(unsigned int i) const;

  /** Get parameters */
  std::vector<TypeNode> getParameters() const;

  /** is this a co-datatype? */
  bool isCodatatype() const;

  /** is this a sygus datatype? */
  bool isSygus() const;

  /** is this a tuple datatype? */
  bool isTuple() const;

  /** get the record representation for this datatype */
  Record* getRecord() const;

  /**
   * Return the cardinality of this datatype.
   * The DType must be resolved.
   *
   * The version of this method that takes type t is required
   * for parametric datatypes, where t is an instantiated
   * parametric datatype type whose datatype is this class.
   */
  Cardinality getCardinality(TypeNode t) const;
  Cardinality getCardinality() const;

  /**
   * Return true iff this DType has finite cardinality. If the
   * datatype is not well-founded, this method returns false. The
   * DType must be resolved or an exception is thrown.
   *
   * The version of this method that takes type t is required
   * for parametric datatypes, where t is an instantiated
   * parametric datatype type whose datatype is this class.
   */
  bool isFinite(TypeNode t) const;
  bool isFinite() const;

  /**
   * Return true iff this  DType is finite (all constructors are
   * finite, i.e., there  are finitely  many ground terms) under the
   * assumption unintepreted sorts are finite. If the
   * datatype is  not well-founded, this method returns false.  The
   * DType must be resolved or an exception is thrown.
   *
   * The versions of these methods that takes type t is required
   * for parametric datatypes, where t is an instantiated
   * parametric datatype type whose datatype is this class.
   */
  bool isInterpretedFinite(TypeNode t) const;
  bool isInterpretedFinite() const;

  /** is well-founded
   *
   * Return true iff this datatype is well-founded (there exist finite
   * values of this type).
   * This datatype must be resolved or an exception is thrown.
   */
  bool isWellFounded() const;

  /** is recursive singleton
   *
   * Return true iff this datatype is a recursive singleton
   * (a recursive singleton is a recursive datatype with only
   * one infinite value). For details, see Reynolds et al. CADE 2015.
   *
   * The versions of these methods that takes type t is required
   * for parametric datatypes, where t is an instantiated
   * parametric datatype type whose datatype is this class.
   */
  bool isRecursiveSingleton(TypeNode t) const;
  bool isRecursiveSingleton() const;

  /** recursive single arguments
   *
   * Get recursive singleton argument types (uninterpreted sorts that the
   * cardinality of this datatype is dependent upon). For example, for :
   *   stream :=  cons( head1 : U1, head2 : U2, tail : stream )
   * Then, the recursive singleton argument types of stream are { U1, U2 },
   * since if U1 and U2 have cardinality one, then stream has cardinality
   * one as well.
   *
   * The versions of these methods that takes Type t is required
   * for parametric datatypes, where t is an instantiated
   * parametric datatype type whose datatype is this class.
   */
  unsigned getNumRecursiveSingletonArgTypes(TypeNode t) const;
  TypeNode getRecursiveSingletonArgType(TypeNode t, unsigned i) const;
  unsigned getNumRecursiveSingletonArgTypes() const;
  TypeNode getRecursiveSingletonArgType(unsigned i) const;

  /**
   * Construct and return a ground term of this DType.  The
   * DType must be both resolved and well-founded, or else an
   * exception is thrown.
   *
   * This method takes a type t, which is a datatype type whose
   * datatype is this class, which may be an instantiated datatype
   * type if this datatype is parametric.
   */
  Node mkGroundTerm(TypeNode t) const;
  /** Make ground value
   *
   * Same as above, but constructs a constant value instead of a ground term.
   * These two notions typically coincide. However, for uninterpreted sorts,
   * they do not: mkGroundTerm returns a fresh variable whereas mkValue returns
   * an uninterpreted constant. The motivation for mkGroundTerm is that
   * unintepreted constants should never appear in lemmas. The motivation for
   * mkGroundValue is for things like type enumeration and model construction.
   */
  Node mkGroundValue(TypeNode t) const;

  /**
   * Get the TypeNode associated to this DType.  Can only be
   * called post-resolution.
   */
  TypeNode getTypeNode() const;

  /**
   * Get the TypeNode associated to this (parameterized) DType.  Can only be
   * called post-resolution.
   */
  TypeNode getTypeNode(const std::vector<TypeNode>& params) const;

  /**
   * Return true iff the two DTypes are the same.
   *
   * We need == for mkConst(DType) to properly work---since if the
   * DType Node requested is the same as an already-existing one,
   * we need to return that one.  For that, we have a hash and
   * operator==.  We provide != for symmetry.  We don't provide
   * operator<() etc. because given two DType Nodes, you could
   * simply compare those rather than the (bare) DTypes.  This
   * means, though, that DType cannot be stored in a sorted list or
   * RB tree directly, so maybe we can consider adding these
   * comparison operators later on.
   */
  bool operator==(const DType& other) const;
  /** Return true iff the two DTypes are not the same. */
  bool operator!=(const DType& other) const;

  /** Return true iff this DType has already been resolved. */
  bool isResolved() const;

  /** Get the beginning iterator over DTypeConstructors. */
  iterator begin();
  /** Get the ending iterator over DTypeConstructors. */
  iterator end();
  /** Get the beginning const_iterator over DTypeConstructors. */
  const_iterator begin() const;
  /** Get the ending const_iterator over DTypeConstructors. */
  const_iterator end() const;

  /** Get the ith DTypeConstructor. */
  const DTypeConstructor& operator[](size_t index) const;

  /**
   * Get the DTypeConstructor named.  This is a linear search
   * through the constructors, so in the case of multiple,
   * similarly-named constructors, the first is returned.
   */
  const DTypeConstructor& operator[](std::string name) const;

  /**
   * Get the constructor operator for the named constructor.
   * This is a linear search through the constructors, so in
   * the case of multiple, similarly-named constructors, the
   * first is returned.
   *
   * This DType must be resolved.
   */
  Node getConstructor(std::string name) const;

  /** get sygus type
   * This gets the built-in type associated with
   * this sygus datatype, i.e. the type of the
   * term that this sygus datatype encodes.
   */
  TypeNode getSygusType() const;

  /** get sygus var list
   * This gets the variable list of the function
   * to synthesize using this sygus datatype.
   * For example, if we are synthesizing a binary
   * function f where solutions are of the form:
   *   f = (lambda (xy) t[x,y])
   * In this case, this method returns the
   * bound variable list containing x and y.
   */
  Node getSygusVarList() const;
  /** get sygus allow constants
   *
   * Does this sygus datatype allow constants?
   * Notice that this is not a property of the
   * constructors of this datatype. Instead, it is
   * an auxiliary flag (provided in the call
   * to setSygus).
   */
  bool getSygusAllowConst() const;
  /** get sygus allow all
   *
   * Does this sygus datatype allow all terms?
   * Notice that this is not a property of the
   * constructors of this datatype. Instead, it is
   * an auxiliary flag (provided in the call
   * to setSygus).
   */
  bool getSygusAllowAll() const;

  /** involves external type
   * Get whether this datatype has a subfield
   * in any constructor that is not a datatype type.
   */
  bool involvesExternalType() const;
  /** involves uninterpreted type
   * Get whether this datatype has a subfield
   * in any constructor that is an uninterpreted type.
   */
  bool involvesUninterpretedType() const;

  /**
   * Get the list of constructors.
   */
  const std::vector<DTypeConstructor>* getConstructors() const;

  /** prints this datatype to stream */
  void toStream(std::ostream& out) const;

 private:
  /** name of this datatype */
  std::string d_name;
  /** the type parameters of this datatype (if this is a parametric datatype)
   */
  std::vector<TypeNode> d_params;
  /** whether the datatype is a codatatype. */
  bool d_isCo;
  /** whether the datatype is a tuple */
  bool d_isTuple;
  /** the constructors of this datatype */
  std::vector<DTypeConstructor> d_constructors;
  /** whether this datatype has been resolved */
  bool d_resolved;
  TypeNode d_self;
  /** cache for involves external type */
  bool d_involvesExt;
  /** cache for involves uninterpreted type */
  bool d_involvesUt;
  /** the builtin type that this sygus type encodes */
  TypeNode d_sygus_type;
  /** the variable list for the sygus function to synthesize */
  Node d_sygus_bvl;
  /** whether all constants are allowed as solutions */
  bool d_sygus_allow_const;
  /** whether all terms are allowed as solutions */
  bool d_sygus_allow_all;

  /** the cardinality of this datatype
   * "mutable" because computing the cardinality can be expensive,
   * and so it's computed just once, on demand---this is the cache
   */
  mutable Cardinality d_card;

  /** is this type a recursive singleton type?
   * The range of this map stores
   * 0 if the field has not been computed,
   * 1 if this datatype is a recursive singleton type,
   * -1 if this datatype is not a recursive singleton type.
   * For definition of (co)recursive singleton, see
   * Section 2 of Reynolds et al. CADE 2015.
   */
  mutable std::map<TypeNode, int> d_card_rec_singleton;
  /** if d_card_rec_singleton is true,
   * This datatype has infinite cardinality if at least one of the
   * following uninterpreted sorts having cardinality > 1.
   */
  mutable std::map<TypeNode, std::vector<TypeNode> > d_card_u_assume;
  /** cache of whether this datatype is well-founded */
  mutable int d_well_founded;
  /** cache of ground term for this datatype */
  mutable std::map<TypeNode, Node> d_ground_term;
  /** cache of ground values for this datatype */
  mutable std::map<TypeNode, Node> d_ground_value;
  /** cache of shared selectors for this datatype */
  mutable std::map<TypeNode, std::map<TypeNode, std::map<unsigned, Node> > >
      d_shared_sel;

  /**
   * DTypes refer to themselves, recursively, and we have a
   * chicken-and-egg problem.  The TypeNode around the DType
   * cannot exist until the DType is finalized, and the DType
   * cannot refer to the TypeNode representing itself until it
   * exists.  resolve() is called by the NodeManager when a type is
   * ultimately requested of the DType specification (that is, when
   * NodeManager::mkTypeNode() or NodeManager::mkMutualTypeNodes()
   * is called).  Has the effect of freezing the object, too; that is,
   * addConstructor() will fail after a call to resolve().
   *
   * The basic goal of resolution is to assign constructors, selectors,
   * and testers.  To do this, any UnresolvedType/SelfType references
   * must be cleared up.  This is the purpose of the "resolutions" map;
   * it includes any mutually-recursive datatypes that are currently
   * under resolution.  The four vectors come in two pairs (so, really
   * they are two maps).  placeholders->replacements give type variables
   * that should be resolved in the case of parametric datatypes.
   *
   * @param em the NodeManager at play
   * @param resolutions a map of strings to TypeNodes currently under
   * resolution
   * @param placeholders the types in these DTypes under resolution that must
   * be replaced
   * @param replacements the corresponding replacements
   * @param paramTypes the sort constructors in these DTypes under resolution
   * that must be replaced
   * @param paramReplacements the corresponding (parametric) TypeNodes
   */
  void resolve(NodeManager* em,
               const std::map<std::string, TypeNode>& resolutions,
               const std::vector<TypeNode>& placeholders,
               const std::vector<TypeNode>& replacements,
               const std::vector<SortConstructorType>& paramTypes,
               const std::vector<TypeNode>& paramReplacements);
  friend class NodeManager;  // for access to resolve()

  /** compute the cardinality of this datatype */
  Cardinality computeCardinality(TypeNode t,
                                 std::vector<TypeNode>& processing) const;
  /** compute whether this datatype is a recursive singleton */
  bool computeCardinalityRecSingleton(TypeNode t,
                                      std::vector<TypeNode>& processing,
                                      std::vector<TypeNode>& u_assume) const;
  /** compute whether this datatype is well-founded */
  bool computeWellFounded(std::vector<TypeNode>& processing) const;
  /** compute ground term
   *
   * This method checks if there is a term of this datatype whose type is t
   * that is finitely constructable. As needed, it traverses its subfield types.
   *
   * The argument processing is the set of datatype types we are currently
   * traversing.
   *
   * The argument isValue is whether we are constructing a constant value. If
   * this flag is false, we are constructing a canonical ground term that is
   * not necessarily constant.
   */
  Node computeGroundTerm(TypeNode t,
                         std::vector<TypeNode>& processing,
                         bool isValue) const;
  /** Get the shared selector
   *
   * This returns the index^th (constructor-agnostic)
   * selector for type t. The type dtt is the datatype
   * type whose datatype is this class, where this may
   * be an instantiated parametric datatype.
   *
   * In the terminology of "DTypes with Shared Selectors",
   * this returns the term sel_{dtt}^{t,index}.
   */
  Node getSharedSelector(TypeNode dtt, TypeNode t, unsigned index) const;
  /**
   * Helper for mkGroundTerm and mkGroundValue above.
   */
  Node mkGroundTermInternal(TypeNode t, bool isValue) const;
}; /* class DType */

/**
 * A hash function for DTypes.  Needed to store them in hash sets
 * and hash maps.
 */
struct DTypeHashFunction
{
  size_t operator()(const DType& dt) const
  {
    return std::hash<std::string>()(dt.getName());
  }
  size_t operator()(const DType* dt) const
  {
    return std::hash<std::string>()(dt->getName());
  }
  size_t operator()(const DTypeConstructor& dtc) const
  {
    return std::hash<std::string>()(dtc.getName());
  }
  size_t operator()(const DTypeConstructor* dtc) const
  {
    return std::hash<std::string>()(dtc->getName());
  }
}; /* struct DTypeHashFunction */

/* stores an index to DType residing in NodeManager */
class DTypeIndexConstant
{
 public:
  DTypeIndexConstant(unsigned index);

  unsigned getIndex() const { return d_index; }
  bool operator==(const DTypeIndexConstant& uc) const
  {
    return d_index == uc.d_index;
  }
  bool operator!=(const DTypeIndexConstant& uc) const { return !(*this == uc); }
  bool operator<(const DTypeIndexConstant& uc) const
  {
    return d_index < uc.d_index;
  }
  bool operator<=(const DTypeIndexConstant& uc) const
  {
    return d_index <= uc.d_index;
  }
  bool operator>(const DTypeIndexConstant& uc) const { return !(*this <= uc); }
  bool operator>=(const DTypeIndexConstant& uc) const { return !(*this < uc); }

 private:
  const unsigned d_index;
}; /* class DTypeIndexConstant */

std::ostream& operator<<(std::ostream& out, const DTypeIndexConstant& dic);

struct DTypeIndexConstantHashFunction
{
  size_t operator()(const DTypeIndexConstant& dic) const
  {
    return IntegerHashFunction()(dic.getIndex());
  }
}; /* struct DTypeIndexConstantHashFunction */

std::ostream& operator<<(std::ostream& os, const DType& dt);

}  // namespace CVC4

#endif
