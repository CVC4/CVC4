/*********************                                                        */
/*! \file drat_proof_black.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Alex Ozdemir
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2018 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Black box testing of the DRAT proof class
 **
 ** In particular, tests DRAT binary parsing.
 **/


#include <cxxtest/TestSuite.h>

#include "proof/drat/drat_proof.h"

using namespace CVC4::proof::drat;

class DratProofBlack : public CxxTest::TestSuite
{
 public:
  void setUp() override {}
  void tearDown() override {}

  void testParseOneAdd();
  void testParseOneMediumAdd();
  void testParseOneBigAdd();
  void testParseLiteralIsTooBig();
  void testParseLiteralOverflow();
  void testParseClauseOverflow();

  void testParseTwo();
};

void DratProofBlack::testParseOneAdd()
{
  // a 1;
  std::string input("a\x02\x00", 3);
  DratProof proof = DratProof::fromBinary(input);


  TS_ASSERT_EQUALS(proof.getInstructions()[0].d_kind, ADDITION);
  TS_ASSERT_EQUALS(proof.getInstructions()[0].d_clause.size(), 1);
  TS_ASSERT_EQUALS(proof.getInstructions()[0].d_clause[0], SatLiteral(0, false));
}

void DratProofBlack::testParseOneMediumAdd()
{
  // a -255;
  std::string input("a\xff\x01\x00", 4);
  DratProof proof = DratProof::fromBinary(input);


  TS_ASSERT_EQUALS(proof.getInstructions()[0].d_kind, ADDITION);
  TS_ASSERT_EQUALS(proof.getInstructions()[0].d_clause.size(), 1);
  TS_ASSERT_EQUALS(proof.getInstructions()[0].d_clause[0], SatLiteral(126, true));
}

void DratProofBlack::testParseOneBigAdd()
{
  // a -2199023255551;
  std::string input("a\xff\xff\xff\xff\xff\x7f\x00", 8);
  DratProof proof = DratProof::fromBinary(input);


  TS_ASSERT_EQUALS(proof.getInstructions()[0].d_kind, ADDITION);
  TS_ASSERT_EQUALS(proof.getInstructions()[0].d_clause.size(), 1);
  TS_ASSERT_EQUALS(proof.getInstructions()[0].d_clause[0], SatLiteral(2199023255550, true));
}

void DratProofBlack::testParseLiteralIsTooBig()
{
  std::string input("a\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x7f\x00", 14);
  TS_ASSERT_THROWS(DratProof::fromBinary(input), InvalidDratProofException);
}

void DratProofBlack::testParseLiteralOverflow()
{
  std::string input("a\x80", 2);
  TS_ASSERT_THROWS(DratProof::fromBinary(input), InvalidDratProofException);
}

void DratProofBlack::testParseClauseOverflow()
{
  std::string input("a\x80\x01", 3);
  TS_ASSERT_THROWS(DratProof::fromBinary(input), InvalidDratProofException);
}

void DratProofBlack::testParseTwo()
{
  // a -255;
  std::string input("\x64\x7f\x83\x80\x01\x00\x61\x82\x02\xff\x7f\x00", 12);
  DratProof proof = DratProof::fromBinary(input);


  TS_ASSERT_EQUALS(proof.getInstructions()[0].d_kind, DELETION);
  TS_ASSERT_EQUALS(proof.getInstructions()[0].d_clause.size(), 2);
  TS_ASSERT_EQUALS(proof.getInstructions()[0].d_clause[0], SatLiteral(62, true));
  TS_ASSERT_EQUALS(proof.getInstructions()[0].d_clause[1], SatLiteral(8192, true));

  TS_ASSERT_EQUALS(proof.getInstructions()[1].d_kind, ADDITION);
  TS_ASSERT_EQUALS(proof.getInstructions()[1].d_clause.size(), 2);
  TS_ASSERT_EQUALS(proof.getInstructions()[1].d_clause[0], SatLiteral(128, false));
  TS_ASSERT_EQUALS(proof.getInstructions()[1].d_clause[1], SatLiteral(8190, true));
}
