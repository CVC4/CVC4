/*********************                                                        */
/*! \file theory_strings_preprocess.cpp
 ** \verbatim
 ** Original author: Tianyi Liang
 ** Major contributors: Morgan Deters
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Strings Preprocess
 **
 ** Strings Preprocess.
 **/

#include "theory/strings/theory_strings_preprocess.h"
#include "expr/kind.h"
#include "theory/strings/options.h"
#include "smt/logic_exception.h"

namespace CVC4 {
namespace theory {
namespace strings {

void StringsPreprocess::simplifyRegExp( Node s, Node r, std::vector< Node > &ret, std::vector< Node > &nn ) {
	int k = r.getKind();
	switch( k ) {
		case kind::STRING_TO_REGEXP:
		{
			Node eq = NodeManager::currentNM()->mkNode( kind::EQUAL, s, r[0] );
			ret.push_back( eq );
		}
			break;
		case kind::REGEXP_CONCAT:
		{
			std::vector< Node > cc;
			for(unsigned i=0; i<r.getNumChildren(); ++i) {
				if(r[i].getKind() == kind::STRING_TO_REGEXP) {
					cc.push_back( r[i][0] );
				} else {
					Node sk = NodeManager::currentNM()->mkSkolem( "recsym_$$", s.getType(), "created for regular expression concat" );
					simplifyRegExp( sk, r[i], ret, nn );
					cc.push_back( sk );
				}
			}
			Node cc_eq = NodeManager::currentNM()->mkNode( kind::EQUAL, s, 
						NodeManager::currentNM()->mkNode( kind::STRING_CONCAT, cc ) );
			nn.push_back( cc_eq );
		}
			break;
		case kind::REGEXP_OR:
		{
			std::vector< Node > c_or;
			for(unsigned i=0; i<r.getNumChildren(); ++i) {
				simplifyRegExp( s, r[i], c_or, nn );
			}
			Node eq = NodeManager::currentNM()->mkNode( kind::OR, c_or );
			ret.push_back( eq );
		}
			break;
		case kind::REGEXP_INTER:
			for(unsigned i=0; i<r.getNumChildren(); ++i) {
				simplifyRegExp( s, r[i], ret, nn );
			}
			break;
		case kind::REGEXP_STAR:
		{
			Node eq = NodeManager::currentNM()->mkNode( kind::STRING_IN_REGEXP, s, r );
			ret.push_back( eq );
		}
			break;
		default:
			//TODO: special sym: sigma, none, all
			Trace("strings-preprocess") << "Unsupported term: " << r << " in simplifyRegExp." << std::endl;
			Assert( false );
			break;
	}
}

bool StringsPreprocess::checkStarPlus( Node t ) {
	if( t.getKind() != kind::REGEXP_STAR && t.getKind() != kind::REGEXP_PLUS ) {
		for( unsigned i = 0; i<t.getNumChildren(); ++i ) {
			if( checkStarPlus(t[i]) ) return true;
		}
		return false;
	} else {
		return true;
	}
}

Node StringsPreprocess::simplify( Node t, std::vector< Node > &new_nodes ) {
    std::hash_map<TNode, Node, TNodeHashFunction>::const_iterator i = d_cache.find(t);
    if(i != d_cache.end()) {
      return (*i).second.isNull() ? t : (*i).second;
    }

	Trace("strings-preprocess") << "StringsPreprocess::simplify: " << t << std::endl;
	Node retNode = t;
	if( t.getKind() == kind::STRING_IN_REGEXP ) {
		// t0 in t1
		Node t0 = simplify( t[0], new_nodes );
		
		//if(!checkStarPlus( t[1] )) {
			//rewrite it
			std::vector< Node > ret;
			std::vector< Node > nn;
			simplifyRegExp( t0, t[1], ret, nn );
			new_nodes.insert( new_nodes.end(), nn.begin(), nn.end() );

			Node n = ret.size() == 1 ? ret[0] : NodeManager::currentNM()->mkNode( kind::AND, ret );
			d_cache[t] = (t == n) ? Node::null() : n;
			retNode = n;
		//} else {
			// TODO
		//	return (t0 == t[0]) ? t : NodeManager::currentNM()->mkNode( kind::STRING_IN_REGEXP, t0, t[1] );
		//}
	} else if( t.getKind() == kind::STRING_SUBSTR ){
		if(options::stringExp()) {
			Node sk1 = NodeManager::currentNM()->mkSkolem( "st1_$$", t.getType(), "created for substr" );
			Node sk2 = NodeManager::currentNM()->mkSkolem( "st2_$$", t.getType(), "created for substr" );
			Node sk3 = NodeManager::currentNM()->mkSkolem( "st3_$$", t.getType(), "created for substr" );
			Node x = simplify( t[0], new_nodes );
			Node x_eq_123 = NodeManager::currentNM()->mkNode( kind::EQUAL,
								NodeManager::currentNM()->mkNode( kind::STRING_CONCAT, sk1, sk2, sk3 ), x );
			new_nodes.push_back( x_eq_123 );
			Node len_sk1_eq_i = NodeManager::currentNM()->mkNode( kind::EQUAL, t[1],
									NodeManager::currentNM()->mkNode( kind::STRING_LENGTH, sk1 ) );
			new_nodes.push_back( len_sk1_eq_i );
			Node len_sk2_eq_j = NodeManager::currentNM()->mkNode( kind::EQUAL, t[2],
									NodeManager::currentNM()->mkNode( kind::STRING_LENGTH, sk2 ) );
			new_nodes.push_back( len_sk2_eq_j );

			d_cache[t] = sk2;
			retNode = sk2;
		} else {
			throw LogicException("substring not supported in this release");
		}
	} else if( t.getKind() == kind::STRING_CHARAT ){
		if(options::stringExp()) {
			Node sk1 = NodeManager::currentNM()->mkSkolem( "ca1_$$", t.getType(), "created for charat" );
			Node sk2 = NodeManager::currentNM()->mkSkolem( "ca2_$$", t.getType(), "created for charat" );
			Node sk3 = NodeManager::currentNM()->mkSkolem( "ca3_$$", t.getType(), "created for charat" );
			Node x = simplify( t[0], new_nodes );
			Node x_eq_123 = NodeManager::currentNM()->mkNode( kind::EQUAL,
								NodeManager::currentNM()->mkNode( kind::STRING_CONCAT, sk1, sk2, sk3 ), x );
			new_nodes.push_back( x_eq_123 );
			Node len_sk1_eq_i = NodeManager::currentNM()->mkNode( kind::EQUAL, t[1],
									NodeManager::currentNM()->mkNode( kind::STRING_LENGTH, sk1 ) );
			new_nodes.push_back( len_sk1_eq_i );
			Node len_sk2_eq_1 = NodeManager::currentNM()->mkNode( kind::EQUAL, NodeManager::currentNM()->mkConst( Rational( 1 ) ),
									NodeManager::currentNM()->mkNode( kind::STRING_LENGTH, sk2 ) );
			new_nodes.push_back( len_sk2_eq_1 );

			d_cache[t] = sk2;
			retNode = sk2;
		} else {
			throw LogicException("string char at not supported in this release");
		}
	} else if( t.getKind() == kind::STRING_STRIDOF ){
		if(options::stringExp()) {
			Node sk1 = NodeManager::currentNM()->mkSkolem( "io1_$$", t[0].getType(), "created for indexof" );
			Node sk2 = NodeManager::currentNM()->mkSkolem( "io2_$$", t[0].getType(), "created for indexof" );
			Node sk3 = NodeManager::currentNM()->mkSkolem( "io3_$$", t[0].getType(), "created for indexof" );
			Node sk4 = NodeManager::currentNM()->mkSkolem( "io4_$$", t[0].getType(), "created for indexof" );
			Node skk = NodeManager::currentNM()->mkSkolem( "iok_$$", t[2].getType(), "created for indexof" );
			Node eq = t[0].eqNode( NodeManager::currentNM()->mkNode( kind::STRING_CONCAT, sk1, sk2, sk3, sk4 ) );
			new_nodes.push_back( eq );
			Node negone = NodeManager::currentNM()->mkConst( ::CVC4::Rational(-1) );
			Node krange = NodeManager::currentNM()->mkNode( kind::AND,
							NodeManager::currentNM()->mkNode( kind::GEQ, skk, negone ),
							NodeManager::currentNM()->mkNode( kind::GT, 
								NodeManager::currentNM()->mkNode( kind::STRING_LENGTH, t[0] ),
								skk));
			new_nodes.push_back( krange );

			//str.len(s1) < y + str.len(s2)
			Node c1 = NodeManager::currentNM()->mkNode( kind::GT, NodeManager::currentNM()->mkNode( kind::PLUS, t[2], NodeManager::currentNM()->mkNode( kind::STRING_LENGTH, t[1] )),
							NodeManager::currentNM()->mkNode( kind::STRING_LENGTH, t[0] ));
			//str.len(t1) = y
			Node c2 = t[2].eqNode( NodeManager::currentNM()->mkNode( kind::STRING_LENGTH, sk1 ) );
			//~contain(t234, s2)
			Node c3 = NodeManager::currentNM()->mkNode( kind::STRING_STRCTN,
						NodeManager::currentNM()->mkNode( kind::STRING_CONCAT, sk2, sk3, sk4), t[1] ).negate();
			//left
			Node left = NodeManager::currentNM()->mkNode( kind::OR, c1, NodeManager::currentNM()->mkNode( kind::AND, c2, c3 ) );
			//t3 = s2
			Node c4 = t[1].eqNode( sk3 );
			//~contain(t2, s2)
			Node c5 = NodeManager::currentNM()->mkNode( kind::STRING_STRCTN, sk2, t[1] ).negate();
			//k=str.len(s1, s2)
			Node c6 = skk.eqNode( NodeManager::currentNM()->mkNode( kind::STRING_LENGTH,
									NodeManager::currentNM()->mkNode( kind::STRING_CONCAT, sk1, sk2 )));
			//right
			Node right = NodeManager::currentNM()->mkNode( kind::AND, c2, c4, c5, c6 );
			Node cond = skk.eqNode( negone );
			Node rr = NodeManager::currentNM()->mkNode( kind::ITE, cond, left, right );
			new_nodes.push_back( rr );
			d_cache[t] = skk;
			retNode = skk;
		} else {
			throw LogicException("string indexof not supported in this release");
		}
	} else if( t.getKind() == kind::STRING_STRREPL ){
		if(options::stringExp()) {
			Node zero = NodeManager::currentNM()->mkConst( ::CVC4::Rational(0) );
			Node x = t[0];
			Node y = t[1];
			Node z = t[2];
			Node sk1 = NodeManager::currentNM()->mkSkolem( "rp1_$$", t[0].getType(), "created for replace" );
			Node sk2 = NodeManager::currentNM()->mkSkolem( "rp2_$$", t[0].getType(), "created for replace" );
			Node skw = NodeManager::currentNM()->mkSkolem( "rpw_$$", t[0].getType(), "created for replace" );
			Node iof = NodeManager::currentNM()->mkNode( kind::STRING_STRIDOF, x, y, zero );
			Node igeq0 = NodeManager::currentNM()->mkNode( kind::GEQ, iof, zero );
			Node c1 = x.eqNode( NodeManager::currentNM()->mkNode( kind::STRING_CONCAT, sk1, y, sk2 ) );
			Node c2 = skw.eqNode( NodeManager::currentNM()->mkNode( kind::STRING_CONCAT, sk1, z, sk2 ) );
			Node c3 = iof.eqNode( NodeManager::currentNM()->mkNode( kind::STRING_LENGTH, sk1 ) );
			Node rr = NodeManager::currentNM()->mkNode( kind::ITE, igeq0,
							NodeManager::currentNM()->mkNode( kind::AND, c1, c2, c3 ),
							skw.eqNode(x) );
			new_nodes.push_back( rr );
			d_cache[t] = skw;
			retNode = skw;
		} else {
			throw LogicException("string replace not supported in this release");
		}
	} else if( t.getNumChildren()>0 ){
		std::vector< Node > cc;
		if (t.getMetaKind() == kind::metakind::PARAMETERIZED) {
			cc.push_back(t.getOperator());
		}
		bool changed = false;
		for( unsigned i=0; i<t.getNumChildren(); i++ ){
			Node tn = simplify( t[i], new_nodes );
			cc.push_back( tn );
			changed = changed || tn!=t[i];
		}
		if(changed) {
			Node n = NodeManager::currentNM()->mkNode( t.getKind(), cc );
			d_cache[t] = n;
			retNode = n;
		} else {
			d_cache[t] = Node::null();
			retNode = t;
		}
	}else{
		d_cache[t] = Node::null();
		retNode = t;
	}

	Trace("strings-preprocess") << "StringsPreprocess::simplify returns: " << retNode << std::endl;
	if(!new_nodes.empty()) {
		Trace("strings-preprocess") << " ... new nodes:";
		for(unsigned int i=0; i<new_nodes.size(); ++i) {
			Trace("strings-preprocess") << " " << new_nodes[i];
		}
		Trace("strings-preprocess") << std::endl;
	}

	return retNode;
}

void StringsPreprocess::simplify(std::vector< Node > &vec_node) {
	std::vector< Node > new_nodes;
	for( unsigned i=0; i<vec_node.size(); i++ ){
		vec_node[i] = simplify( vec_node[i], new_nodes );
	}
	vec_node.insert( vec_node.end(), new_nodes.begin(), new_nodes.end() );
}

}/* CVC4::theory::strings namespace */
}/* CVC4::theory namespace */
}/* CVC4 namespace */
