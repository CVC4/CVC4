/*********************                                                        */
/*! \file type_node.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Morgan Deters, Andrew Reynolds, Kshitij Bansal
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2017 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Reference-counted encapsulation of a pointer to node information.
 **
 ** Reference-counted encapsulation of a pointer to node information.
 **/
#include "expr/type_node.h"

#include <vector>

#include "base/portability.h"
#include "expr/node_manager_attributes.h"
#include "expr/type_properties.h"
#include "options/base_options.h"
#include "options/expr_options.h"
#include "options/quantifiers_options.h"

using namespace std;

namespace CVC4 {

TypeNode TypeNode::s_null( &expr::NodeValue::null() );

TypeNode TypeNode::substitute(const TypeNode& type,
                              const TypeNode& replacement,
                              std::unordered_map<TypeNode, TypeNode, HashFunction>& cache) const {
  // in cache?
  std::unordered_map<TypeNode, TypeNode, HashFunction>::const_iterator i = cache.find(*this);
  if(i != cache.end()) {
    return (*i).second;
  }

  // otherwise compute
  NodeBuilder<> nb(getKind());
  if(getMetaKind() == kind::metakind::PARAMETERIZED) {
    // push the operator
    nb << TypeNode(d_nv->d_children[0]);
  }
  for(TypeNode::const_iterator i = begin(),
        iend = end();
      i != iend;
      ++i) {
    if(*i == type) {
      nb << replacement;
    } else {
      (*i).substitute(type, replacement);
    }
  }

  // put in cache
  TypeNode tn = nb.constructTypeNode();
  cache[*this] = tn;
  return tn;
}

Cardinality TypeNode::getCardinality() const {
  return kind::getCardinality(*this);
}

bool TypeNode::isInterpretedFinite() const {
  if( getCardinality().isFinite() ){
    return true;
  }else{
    if( options::finiteModelFind() ){
      if( isSort() ){
        return true;
      }else if( isDatatype() ){
        TypeNode tn = *this;
        const Datatype& dt = getDatatype();
        return dt.isInterpretedFinite( tn.toType() );
      }else if( isArray() ){
        return getArrayIndexType().isInterpretedFinite() && getArrayConstituentType().isInterpretedFinite();
      }else if( isSet() ) {
        return getSetElementType().isInterpretedFinite();
      }
    }
    return false;
  }
}

bool TypeNode::isWellFounded() const {
  return kind::isWellFounded(*this);
}

Node TypeNode::mkGroundTerm() const {
  return kind::mkGroundTerm(*this);
}

bool TypeNode::isSubtypeOf(TypeNode t) const {
  if(*this == t) {
    return true;
  }
  if(getKind() == kind::TYPE_CONSTANT) {
    switch(getConst<TypeConstant>()) {
    case INTEGER_TYPE:
      return t.getKind() == kind::TYPE_CONSTANT && t.getConst<TypeConstant>() == REAL_TYPE;
    default:
      return false;
    }
  }
  if(isTuple() && t.isTuple()) {
    const Datatype& dt1 = getDatatype();
    const Datatype& dt2 = t.getDatatype();
    if( dt1[0].getNumArgs()!=dt2[0].getNumArgs() ){
      return false;
    }
    // r1's fields must be subtypes of r2's, in order
    for( unsigned i=0; i<dt1[0].getNumArgs(); i++ ){
      if( !dt1[0][i].getRangeType().isSubtypeOf( dt2[0][i].getRangeType() ) ){
        return false;
      }
    }
    return true;
  }else if( t.isRecord() && t.isRecord() ){
    //records are not subtypes of each other in current implementation
  }
  if(isFunction()) {
    // A function is a subtype of another if the args are the same type, and
    // the return type is a subtype of the other's.  This is enough for now
    // (and it's necessary for model generation, since a Real-valued function
    // might return a constant Int and thus the model value is typed differently).
    return t.isFunction() && getArgTypes() == t.getArgTypes() && getRangeType().isSubtypeOf(t.getRangeType());
  }
  if(isParametricDatatype() && t.isParametricDatatype()) {
    Assert(getKind() == kind::PARAMETRIC_DATATYPE);
    Assert(t.getKind() == kind::PARAMETRIC_DATATYPE);
    if((*this)[0] != t[0] || getNumChildren() != t.getNumChildren()) {
      return false;
    }
    for(size_t i = 1; i < getNumChildren(); ++i) {
      if(!((*this)[i].isSubtypeOf(t[i]))) {
        return false;
      }
    }
    return true;
  }
  if(isSet() && t.isSet()) {
    return getSetElementType().isSubtypeOf(t.getSetElementType());
  }
  if(isArray() && t.isArray()) {
    //reverse for index type
    return t.getArrayIndexType().isSubtypeOf(getArrayIndexType()) && 
           getArrayConstituentType().isSubtypeOf(t.getArrayConstituentType());
  }
  return false;
}

bool TypeNode::isComparableTo(TypeNode t) const {
  if(*this == t) {
    return true;
  }
  if(isSubtypeOf(NodeManager::currentNM()->realType())) {
    return t.isSubtypeOf(NodeManager::currentNM()->realType());
  }
  if(isTuple() && t.isTuple()) {
    const Datatype& dt1 = getDatatype();
    const Datatype& dt2 = t.getDatatype();
    if( dt1[0].getNumArgs()!=dt2[0].getNumArgs() ){
      return false;
    }
    // r1's fields must be subtypes of r2's, in order
    for( unsigned i=0; i<dt1[0].getNumArgs(); i++ ){
      if( !dt1[0][i].getRangeType().isComparableTo( dt2[0][i].getRangeType() ) ){
        return false;
      }
    }
    return true;
  //}else if( isRecord() && t.isRecord() ){
    //record types are incomparable in current implementation
  } else if(isParametricDatatype() && t.isParametricDatatype()) {
    Assert(getKind() == kind::PARAMETRIC_DATATYPE);
    Assert(t.getKind() == kind::PARAMETRIC_DATATYPE);
    if((*this)[0] != t[0] || getNumChildren() != t.getNumChildren()) {
      return false;
    }
    for(size_t i = 1; i < getNumChildren(); ++i) {
      if(!((*this)[i].isComparableTo(t[i]))) {
        return false;
      }
    }
    return true;
  } else if(isSet() && t.isSet()) {
    return getSetElementType().isComparableTo(t.getSetElementType());
  }
  if(isArray() && t.isArray()) {
    return getArrayIndexType().isComparableTo(t.getArrayIndexType()) && getArrayConstituentType().isComparableTo(t.getArrayConstituentType());
  }
  return false;
}

TypeNode TypeNode::getBaseType() const {
  TypeNode realt = NodeManager::currentNM()->realType();
  if (isSubtypeOf(realt)) {
    return realt;
  } else if (isParametricDatatype()) {
    vector<Type> v;
    for(size_t i = 1; i < getNumChildren(); ++i) {
      v.push_back((*this)[i].getBaseType().toType());
    }
    TypeNode tn = TypeNode::fromType((*this)[0].getDatatype().getDatatypeType(v));
    return tn;
  }
  return *this;
}

std::vector<TypeNode> TypeNode::getArgTypes() const {
  vector<TypeNode> args;
  if(isTester()) {
    Assert(getNumChildren() == 1);
    args.push_back((*this)[0]);
  } else {
    Assert(isFunction() || isConstructor() || isSelector());
    for(unsigned i = 0, i_end = getNumChildren() - 1; i < i_end; ++i) {
      args.push_back((*this)[i]);
    }
  }
  return args;
}

std::vector<TypeNode> TypeNode::getParamTypes() const {
  vector<TypeNode> params;
  Assert(isParametricDatatype());
  for(unsigned i = 1, i_end = getNumChildren(); i < i_end; ++i) {
    params.push_back((*this)[i]);
  }
  return params;
}


/** Is this a tuple type? */
bool TypeNode::isTuple() const {
  return ( getKind() == kind::DATATYPE_TYPE && getDatatype().isTuple() );
}

/** Is this a record type? */
bool TypeNode::isRecord() const {
  return ( getKind() == kind::DATATYPE_TYPE && getDatatype().isRecord() );
}

size_t TypeNode::getTupleLength() const {
  Assert(isTuple());
  const Datatype& dt = getDatatype();
  Assert(dt.getNumConstructors()==1);
  return dt[0].getNumArgs();
}

vector<TypeNode> TypeNode::getTupleTypes() const {
  Assert(isTuple());
  const Datatype& dt = getDatatype();
  Assert(dt.getNumConstructors()==1);
  vector<TypeNode> types;
  for(unsigned i = 0; i < dt[0].getNumArgs(); ++i) {
    types.push_back(TypeNode::fromType(dt[0][i].getRangeType()));
  }
  return types;
}

const Record& TypeNode::getRecord() const {
  Assert(isRecord());
  const Datatype & dt = getDatatype();
  return *(dt.getRecord());
  //return getAttribute(expr::DatatypeRecordAttr()).getConst<Record>();
}

vector<TypeNode> TypeNode::getSExprTypes() const {
  Assert(isSExpr());
  vector<TypeNode> types;
  for(unsigned i = 0, i_end = getNumChildren(); i < i_end; ++i) {
    types.push_back((*this)[i]);
  }
  return types;
}

/** Is this an instantiated datatype type */
bool TypeNode::isInstantiatedDatatype() const {
  if(getKind() == kind::DATATYPE_TYPE) {
    return true;
  }
  if(getKind() != kind::PARAMETRIC_DATATYPE) {
    return false;
  }
  const Datatype& dt = (*this)[0].getDatatype();
  unsigned n = dt.getNumParameters();
  Assert(n < getNumChildren());
  for(unsigned i = 0; i < n; ++i) {
    if(TypeNode::fromType(dt.getParameter(i)) == (*this)[i + 1]) {
      return false;
    }
  }
  return true;
}

/** Is this an instantiated datatype parameter */
bool TypeNode::isParameterInstantiatedDatatype(unsigned n) const {
  AssertArgument(getKind() == kind::PARAMETRIC_DATATYPE, *this);
  const Datatype& dt = (*this)[0].getDatatype();
  AssertArgument(n < dt.getNumParameters(), *this);
  return TypeNode::fromType(dt.getParameter(n)) != (*this)[n + 1];
}

TypeNode TypeNode::leastCommonTypeNode(TypeNode t0, TypeNode t1){
  return commonTypeNode( t0, t1, true );
}

TypeNode TypeNode::mostCommonTypeNode(TypeNode t0, TypeNode t1){
  return commonTypeNode( t0, t1, false );
}

TypeNode TypeNode::commonTypeNode(TypeNode t0, TypeNode t1, bool isLeast) {
  Assert( NodeManager::currentNM() != NULL )
      << "There is no current CVC4::NodeManager associated to this thread." << std::endl
      << "Perhaps a public-facing function is missing a NodeManagerScope ?" << std::endl;

  Assert(!t0.isNull());
  Assert(!t1.isNull());

  if(__CVC4__expect( (t0 == t1), true )) {
    return t0;
  }

  // t0 != t1 &&
  if(t0.getKind() == kind::TYPE_CONSTANT) {
    switch(t0.getConst<TypeConstant>()) {
    case INTEGER_TYPE:
      if(t1.isInteger()) {
        // t0 == IntegerType && t1.isInteger()
        return t0; //IntegerType
      } else if(t1.isReal()) {
        // t0 == IntegerType && t1.isReal() && !t1.isInteger()
        return isLeast ? t1 : t0; // RealType
      } else {
        return TypeNode(); // null type
      }
    case REAL_TYPE:
      if(t1.isReal()) {
        return isLeast ? t0 : t1; // RealType
      } else {
        return TypeNode(); // null type
      }
    default:
      return TypeNode(); // null type
    }
  } else if(t1.getKind() == kind::TYPE_CONSTANT) {
    return commonTypeNode(t1, t0, isLeast); // decrease the number of special cases
  }

  // t0 != t1 &&
  // t0.getKind() == kind::TYPE_CONSTANT &&
  // t1.getKind() == kind::TYPE_CONSTANT
  switch(t0.getKind()) {
  case kind::BITVECTOR_TYPE:
  case kind::SORT_TYPE:
  case kind::CONSTRUCTOR_TYPE:
  case kind::SELECTOR_TYPE:
  case kind::TESTER_TYPE:
    return TypeNode();
  case kind::FUNCTION_TYPE:
    return TypeNode(); // Not sure if this is right
  case kind::SET_TYPE: {
    // take the least common subtype of element types
    TypeNode elementType;
    if(t1.isSet() && !(elementType = commonTypeNode(t0[0], t1[0], isLeast)).isNull() ) {
      return NodeManager::currentNM()->mkSetType(elementType);
    } else {
      return TypeNode();
    }
  }
  case kind::ARRAY_TYPE: {
    TypeNode indexType, elementType;
    if(t1.isArray() &&
       ! (indexType = commonTypeNode(t0[0], t1[0], !isLeast)).isNull() && 
       ! (elementType = commonTypeNode(t0[1], t1[1], isLeast)).isNull() ) {
      return NodeManager::currentNM()->mkArrayType(indexType, elementType);
    } else {
      return TypeNode();
    }
  }
  case kind::SEXPR_TYPE:
    Unimplemented() << "haven't implemented leastCommonType for symbolic expressions yet" << std::endl;
    return TypeNode();
  case kind::DATATYPE_TYPE:
    if( t0.isTuple() && t1.isTuple() ){
      const Datatype& dt1 = t0.getDatatype();
      const Datatype& dt2 = t1.getDatatype();
      if( dt1[0].getNumArgs()==dt2[0].getNumArgs() ){
        std::vector< TypeNode > lc_types;
        for( unsigned i=0; i<dt1[0].getNumArgs(); i++ ){
          TypeNode tc = commonTypeNode( TypeNode::fromType( dt1[0][i].getRangeType() ), TypeNode::fromType( dt2[0][i].getRangeType() ), isLeast );
          if( tc.isNull() ){
            return tc;
          }else{
            lc_types.push_back( tc );
          }
        }
        return NodeManager::currentNM()->mkTupleType( lc_types );
      }
    }
    //else if( t0.isRecord() && t1.isRecord() ){
      //record types are not related in current implementation
    //}
    // otherwise no common ancestor
    return TypeNode();
  case kind::PARAMETRIC_DATATYPE: {
    if(!t1.isParametricDatatype()) {
      return TypeNode();
    }
    if(t0[0] != t1[0] || t0.getNumChildren() != t1.getNumChildren()) {
      return TypeNode();
    }
    vector<Type> v;
    for(size_t i = 1; i < t0.getNumChildren(); ++i) {
      v.push_back(commonTypeNode(t0[i], t1[i], isLeast).toType());
    }
    return TypeNode::fromType(t0[0].getDatatype().getDatatypeType(v));
  }
  default:
    Unimplemented() << "don't have a commonType for types `" << t0.toString() << "' and `" << t1.toString() << "'" << std::endl;
    return TypeNode();
  }
}

Node TypeNode::getEnsureTypeCondition( Node n, TypeNode tn ) {
  TypeNode ntn = n.getType();
  Assert( ntn.isComparableTo( tn ) );
  if( !ntn.isSubtypeOf( tn ) ){
    if( tn.isInteger() ){
      if( tn.isSubtypeOf( ntn ) ){
        return NodeManager::currentNM()->mkNode( kind::IS_INTEGER, n );
      }
    }else if( tn.isDatatype() && ntn.isDatatype() ){
      if( tn.isTuple() && ntn.isTuple() ){
        const Datatype& dt1 = tn.getDatatype();
        const Datatype& dt2 = ntn.getDatatype();
        if( dt1[0].getNumArgs()==dt2[0].getNumArgs() ){
          std::vector< Node > conds;
          for( unsigned i=0; i<dt2[0].getNumArgs(); i++ ){
            Node s = NodeManager::currentNM()->mkNode( kind::APPLY_SELECTOR_TOTAL, Node::fromExpr( dt2[0][i].getSelector() ), n );
            Node etc = getEnsureTypeCondition( s, TypeNode::fromType( dt1[0][i].getRangeType() ) );
            if( etc.isNull() ){
              return Node::null();
            }else{
              conds.push_back( etc );
            }
          }
          if( conds.empty() ){
            return NodeManager::currentNM()->mkConst( true );
          }else if( conds.size()==1 ){
            return conds[0];
          }else{
            return NodeManager::currentNM()->mkNode( kind::AND, conds );
          }
        }
      }
    }
    return Node::null();
  }else{
    return NodeManager::currentNM()->mkConst( true );
  }
}

/** Is this a sort kind */
bool TypeNode::isSort() const {
  return ( getKind() == kind::SORT_TYPE && !hasAttribute(expr::SortArityAttr()) );
}

/** Is this a sort constructor kind */
bool TypeNode::isSortConstructor() const {
  return getKind() == kind::SORT_TYPE && hasAttribute(expr::SortArityAttr());
}

std::string TypeNode::toString() const {
  std::stringstream ss;
  OutputLanguage outlang = (this == &s_null) ? language::output::LANG_AUTO : options::outputLanguage();
  d_nv->toStream(ss, -1, false, 0, outlang);
  return ss.str();
}


}/* CVC4 namespace */
