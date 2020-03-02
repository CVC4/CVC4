/*********************                                                        */
/*! \file regexp.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds, Tim King, Tianyi Liang
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "cvc4_public.h"

#ifndef CVC4__REGEXP_H
#define CVC4__REGEXP_H

#include <cstddef>
#include <functional>
#include <ostream>
#include <string>
#include <vector>
#include "util/rational.h"

namespace CVC4 {

/** The CVC4 string class
 *
 * This data structure is the domain of values for the string type. It can also
 * be used as a generic utility for representing strings.
 */
class CVC4_PUBLIC String {
 public:
  /**
   * The start ASCII code. In our string representation below, we represent
   * characters using a vector d_str of unsigned integers. We refer to this as
   * the "internal representation" for the string.
   *
   * We make unsigned integer 0 correspond to the 65th character ("A") in the
   * ASCII alphabet to make models intuitive. In particular, say if we have
   * a set of string variables that are distinct but otherwise unconstrained,
   * then the model may assign them "A", "B", "C", ...
   */
  static inline unsigned start_code() { return 65; }
  /**
   * This is the cardinality of the alphabet that is representable by this
   * class. Notice that this must be greater than or equal to the cardinality
   * of the alphabet that the string theory reasons about.
   *
   * This must be strictly less than std::numeric_limits<unsigned>::max().
   */
  static inline unsigned num_codes() { return 256; }
  /**
   * Convert unsigned char to the unsigned used in the internal representation
   * in d_str below.
   */
  static unsigned convertCharToUnsignedInt(unsigned char c);
  /** Convert the internal unsigned to a unsigned char. */
  static unsigned char convertUnsignedIntToChar(unsigned i);
  /** Does the internal unsigned correspond to a printable character? */
  static bool isPrintable(unsigned i);
  /** get the internal unsigned for ASCII code c. */
  static unsigned convertCodeToUnsignedInt(unsigned c);
  /** get the ASCII code number that internal unsigned i corresponds to. */
  static unsigned convertUnsignedIntToCode(unsigned i);
  /** Escape sequence mode */
  enum EscSeqMode
  {
    /**
     * Handle escape sequences, including \n, \t, \v, \b, \r, \f, \a, \\,
     * \x[N] and octal escape sequences of the form \[c1]([c2]([c3])?)? where
     * c1, c2, c3 are digits from 0 to 7.
     */
    ESC_SEQ_AD_HOC,
    /**
     * Handle only unicode escape sequences of the form:
     *  \u d_3 d_2 d_1 d_0
     *  \u{d_0}
     *  \u{d_1 d_0}
     *  \u{d_2 d_1 d_0}
     *  \u{d_3 d_2 d_1 d_0}
     *  \u{d_4 d_3 d_2 d_1 d_0}
     * where d_0 ... d_4 are hexidecimal digits.
     */
    ESC_SEQ_UNICODE_STD,
    /** Handle no escape sequences */
    ESC_SEQ_NONE,
  };
  /** constructors for String
   *
   * Internally, a CVC4::String is represented by a vector of unsigned
   * integers (d_str), where the correspondence between C++ characters
   * to and from unsigned integers is determined by
   * by convertCharToUnsignedInt and convertUnsignedIntToChar.
   *
   * Argument esmode determines which escape sequences to process, as
   * described above.
   *
   * If esmode is ESC_SEQUENCE_NONE, then the characters of the constructed
   * CVC4::String correspond one-to-one with the input string.
   */
  String() = default;
  explicit String(const std::string& s,
                  EscSeqMode esmode = ESC_SEQ_UNICODE_STD)
      : d_str(toInternal(s, esmode))
  {
  }
  explicit String(const char* s,
                  EscSeqMode esmode = ESC_SEQ_UNICODE_STD)
      : d_str(toInternal(std::string(s), esmode))
  {
  }
  explicit String(const std::vector<unsigned>& s);

  String& operator=(const String& y) {
    if (this != &y) {
      d_str = y.d_str;
    }
    return *this;
  }

  String concat(const String& other) const;

  bool operator==(const String& y) const { return cmp(y) == 0; }
  bool operator!=(const String& y) const { return cmp(y) != 0; }
  bool operator<(const String& y) const { return cmp(y) < 0; }
  bool operator>(const String& y) const { return cmp(y) > 0; }
  bool operator<=(const String& y) const { return cmp(y) <= 0; }
  bool operator>=(const String& y) const { return cmp(y) >= 0; }

  bool strncmp(const String& y, const std::size_t np) const;
  bool rstrncmp(const String& y, const std::size_t np) const;

  /* toString
  * Converts this string to a std::string.
  *
  * If useEscSequences is true, then unprintable characters
  * are converted to escape sequences. The escape sequences
  * \n, \t, \v, \b, \r, \f, \a, \\ are printed in this way.
  * For all other unprintable characters, we print \x[N] where
  * [N] is the 2 digit hexidecimal corresponding to value of
  * the character.
  *
  * If useEscSequences is false, the returned std::string's characters
  * map one-to-one with the characters in this string.
  * Notice that for all std::string s, we have that
  *    CVC4::String( s ).toString() = s.
  */
  std::string toString(bool useEscSequences = false) const;
  /** is this the empty string? */
  bool empty() const { return d_str.empty(); }
  /** is this the empty string? */
  bool isEmptyString() const { return empty(); }
  /** is less than or equal to string y */
  bool isLeq(const String& y) const;
  /** Return the length of the string */
  std::size_t size() const { return d_str.size(); }

  bool isRepeated() const;
  bool tailcmp(const String& y, int& c) const;

  /**
   * Return the first position y occurs in this string, or std::string::npos
   * otherwise.
   */
  std::size_t find(const String& y, const std::size_t start = 0) const;
  /**
   * Return the first position y occurs in this string searching from the end,
   * or std::string::npos otherwise.
   */
  std::size_t rfind(const String& y, const std::size_t start = 0) const;
  /** Returns true if y is a prefix of this */
  bool hasPrefix(const String& y) const;
  /** Returns true if y is a suffix of this */
  bool hasSuffix(const String& y) const;
  /** Replace the first occurrence of s in this string with t */
  String replace(const String& s, const String& t) const;
  /** Return the substring of this string starting at index i */
  String substr(std::size_t i) const;
  /** Return the substring of this string starting at index i with size at most
   * j */
  String substr(std::size_t i, std::size_t j) const;
  /** Return the prefix of this string of size at most i */
  String prefix(std::size_t i) const { return substr(0, i); }
  /** Return the suffix of this string of size at most i */
  String suffix(std::size_t i) const { return substr(size() - i, i); }

  /**
   * Checks if there is any overlap between this string and another string. This
   * corresponds to checking whether one string contains the other and whether a
   * substring of one is a prefix of the other and vice-versa.
   *
   * @param y The other string
   * @return True if there is an overlap, false otherwise
   */
  bool noOverlapWith(const String& y) const;

  /** string overlap
  *
  * if overlap returns m>0,
  * then the maximal suffix of this string that is a prefix of y is of length m.
  *
  * For example, if x is "abcdef", then:
  * x.overlap("defg") = 3
  * x.overlap("ab") = 0
  * x.overlap("d") = 0
  * x.overlap("bcdefdef") = 5
  */
  std::size_t overlap(const String& y) const;
  /** string reverse overlap
  *
  * if roverlap returns m>0,
  * then the maximal prefix of this string that is a suffix of y is of length m.
  *
  * For example, if x is "abcdef", then:
  * x.roverlap("aaabc") = 3
  * x.roverlap("def") = 0
  * x.roverlap("d") = 0
  * x.roverlap("defabcde") = 5
  *
  * Notice that x.overlap(y) = y.roverlap(x)
  */
  std::size_t roverlap(const String& y) const;

  /**
   * Returns true if this string corresponds in text to a number, for example
   * this returns true for strings "7", "12", "004", "0" and false for strings
   * "abc", "4a", "-4", "".
   */
  bool isNumber() const;
  /** Returns the corresponding rational for the text of this string. */
  Rational toNumber() const;
  /** get the internal unsigned representation of this string */
  const std::vector<unsigned>& getVec() const { return d_str; }
  /** get the internal unsigned value of the first character in this string */
  unsigned front() const;
  /** get the internal unsigned value of the last character in this string */
  unsigned back() const;
  /** is the unsigned a digit?
  * The input should be the same type as the element type of d_str
  */
  static bool isDigit(unsigned character);
  /** is the unsigned a digit?
   * The input should be the same type as the element type of d_str
   */
  static bool isHexDigit(unsigned character);

  /**
   * Returns the maximum length of string representable by this class.
   * Corresponds to the maximum size of d_str.
   */
  static size_t maxSize();
 private:
  // guarded
  static unsigned char hexToDec(unsigned char c);

  /** 
   * Helper for toInternal: add character ch to vector vec, storing a string in
   * internal format. This throws an error if ch is not a printable character,
   * since non-printable characters must be escaped.
   */
  static void addCharToInternal(unsigned char ch, std::vector<unsigned>& vec);
  /** 
   * Convert the string s to the internal format based on escaped sequence
   * mode esmode.
   */
  static std::vector<unsigned> toInternal(const std::string& s,
                                          EscSeqMode esmode);

  /**
   * Returns a negative number if *this < y, 0 if *this and y are equal and a
   * positive number if *this > y.
   */
  int cmp(const String& y) const;

  std::vector<unsigned> d_str;
}; /* class String */

namespace strings {

struct CVC4_PUBLIC StringHashFunction {
  size_t operator()(const ::CVC4::String& s) const {
    return std::hash<std::string>()(s.toString());
  }
}; /* struct StringHashFunction */

}  // namespace strings

std::ostream& operator<<(std::ostream& os, const String& s) CVC4_PUBLIC;

}  // namespace CVC4

#endif /* CVC4__REGEXP_H */
