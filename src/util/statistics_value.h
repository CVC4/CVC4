/*********************                                                        */
/*! \file statistics_value.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Gereon Kremer
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Statistic data classes
 **
 ** The statistic data classes that actually hold the data for the statistics.
 **
 ** Conceptually, every statistic consists of a data object and a proxy object.
 ** The data objects (statistic values) are derived from `StatisticBaseValue`
 ** and live in the `StatisticsRegistry`.
 ** They are solely exported to the proxy objects, which should be the sole
 ** way to manipulate the data of a data object.
 ** The data objects themselves need to implement printing (normal and safe) and
 ** conversion to the API type `Stat`.
 **/

#include "cvc4_private_library.h"

#ifndef CVC4__UTIL__STATISTICS_VALUE_H
#define CVC4__UTIL__STATISTICS_VALUE_H

#include <chrono>
#include <iomanip>
#include <map>
#include <optional>
#include <sstream>
#include <variant>
#include <vector>

#include "util/safe_print.h"

namespace CVC4 {

class StatisticsRegistry;

using StatExportData = std::variant<std::monostate, int64_t, double, std::string, std::map<std::string, uint64_t>>;

/**
 * Base class for all statistic values.
 * Requires three methods:
 * - `getViewer` converts to the API representation `Stat`
 * - `print` writes the data to a regular `std::ostream`
 * - `print_safe` safely writes the data to a file descriptor
 */
struct StatisticBaseValue
{
  virtual ~StatisticBaseValue();
  virtual StatExportData getViewer() const = 0;
  virtual void print(std::ostream&) const = 0;
  virtual void print_safe(int fd) const = 0;

  bool d_expert = true;
};
/** Writes the data to an output stream */
std::ostream& operator<<(std::ostream& out, const StatisticBaseValue& sbv);

/** Holds the data for an running average statistic */
struct StatisticAverageValue : StatisticBaseValue
{
  StatExportData getViewer() const override;
  void print(std::ostream& out) const override;
  void print_safe(int fd) const override;
  double get() const;

  /** Sum of added values */
  double d_sum;
  /** Number of added values */
  uint64_t d_count;
};

/**
 * Holds some value of type `T`.
 *
 * To convert to the API representation in `getViewer`, `T` can only be one
 * of the types listed in `Stat::d_data` (or be implicitly converted to
 * one of them).
 */
template <typename T>
struct StatisticBackedValue : StatisticBaseValue
{
  StatExportData getViewer() const override { return d_value; }
  void print(std::ostream& out) const override { out << d_value; }
  void print_safe(int fd) const override { safe_print<T>(fd, d_value); }

  T d_value;
};

/**
 * Holds the data for a histogram. We assume the type to be (convertible to)
 * integral, and we can thus use a std::vector<uint64_t> for fast storage.
 * The core idea is to track the minimum and maximum values `[a,b]` that have
 * been added to the histogram and maintain a vector with `b-a+1` values.
 * The vector is resized on demand to grow as necessary and supports negative
 * values as well.
 * Note that the template type needs to have a streaming operator to convert it
 * to a string in `getViewer`.
 */
template <typename Integral>
struct StatisticHistogramValue : StatisticBaseValue
{
  static_assert(std::is_integral<Integral>::value
                    || std::is_enum<Integral>::value,
                "Type should be a fundamental integral type.");

  /**
   * Convert the internal representation to a `std::map<std::string, uint64_t>`
   */
  StatExportData getViewer() const override
  {
    std::map<std::string, uint64_t> res;
    for (size_t i = 0, n = d_hist.size(); i < n; ++i)
    {
      if (d_hist[i] > 0)
      {
        std::stringstream ss;
        ss << static_cast<Integral>(i + d_offset);
        res.emplace(ss.str(), d_hist[i]);
      }
    }
    return res;
  }
  void print(std::ostream& out) const override
  {
    out << "[";
    bool first = true;
    for (size_t i = 0, n = d_hist.size(); i < n; ++i)
    {
      if (d_hist[i] > 0)
      {
        if (first)
        {
          first = false;
        }
        else
        {
          out << ", ";
        }
        out << "(" << static_cast<Integral>(i + d_offset) << " : " << d_hist[i]
            << ")";
      }
    }
    out << "]";
  }
  void print_safe(int fd) const override
  {
    safe_print(fd, "[");
    bool first = true;
    for (size_t i = 0, n = d_hist.size(); i < n; ++i)
    {
      if (d_hist[i] > 0)
      {
        if (first)
        {
          first = false;
        }
        else
        {
          safe_print(fd, ", ");
        }
        safe_print(fd, "(");
        safe_print<Integral>(fd, static_cast<Integral>(i + d_offset));
        safe_print(fd, " : ");
        safe_print<uint64_t>(fd, d_hist[i]);
        safe_print(fd, ")");
      }
    }
    safe_print(fd, "]");
  }

  /**
   * Add `val` to the histogram. Casts `val` to `int64_t`, then resizes and
   * moves the vector entries as necessary.
   */
  void add(Integral val)
  {
    int64_t v = static_cast<int64_t>(val);
    if (d_hist.empty())
    {
      d_offset = v;
    }
    if (v < d_offset)
    {
      d_hist.insert(d_hist.begin(), d_offset - v, 0);
      d_offset = v;
    }
    if (static_cast<size_t>(v - d_offset) >= d_hist.size())
    {
      d_hist.resize(v - d_offset + 1);
    }
    d_hist[v - d_offset]++;
  }

  /** Actual data */
  std::vector<uint64_t> d_hist;
  /** Offset of the entries. d_hist[i] corresponds to Interval(d_offset + i) */
  int64_t d_offset;
};

/**
 * Holds the data for a `ReferenceStat`.
 * When the `ReferenceStat` is destroyed the current value is copied into
 * `d_committed`. Once `d_committed` is set, this value is returned, even if
 * the reference is still valid.
 */
template <typename T>
struct StatisticReferenceValue : StatisticBaseValue
{
  StatExportData getViewer() const override
  {
    if (d_committed)
    {
      if constexpr (std::is_integral_v<T>)
      {
        return static_cast<int64_t>(*d_committed);
      }
      else
      {
        return *d_committed;
      }
    }
    else if (d_value != nullptr)
    {
      if constexpr (std::is_integral_v<T>)
      {
        return static_cast<int64_t>(*d_value);
      }
      else
      {
        return *d_value;
      }
    }
    else
    {
      return {};
    }
  }
  void print(std::ostream& out) const override
  {
    if (d_committed)
    {
      out << *d_committed;
    }
    else if (d_value != nullptr)
    {
      out << *d_value;
    }
    else
    {
      out << "<undefined>";
    }
  }
  void print_safe(int fd) const override
  {
    if (d_committed)
    {
      safe_print<T>(fd, *d_committed);
    }
    else if (d_value != nullptr)
    {
      safe_print<T>(fd, *d_value);
    }
    else
    {
      safe_print(fd, "<undefined>");
    }
  }
  void commit()
  {
    if (d_value != nullptr)
    {
      d_committed = *d_value;
    }
  }
  const T& get() const { return d_committed ? *d_committed : *d_value; }

  const T* d_value = nullptr;
  std::optional<T> d_committed;
};

/**
 * Holds the data for a `SizeStat`.
 * When the `SizeStat` is destroyed the current size is copied into
 * `d_committed`. Once `d_committed` is set, this value is returned, even if
 * the reference is still valid.
 */
template <typename T>
struct StatisticSizeValue : StatisticBaseValue
{
  StatExportData getViewer() const override
  {
    if (d_committed)
    {
      return static_cast<int64_t>(*d_committed);
    }
    else if (d_value != nullptr)
    {
      return static_cast<int64_t>(d_value->size());
    }
    else
    {
      return {};
    }
  }
  void print(std::ostream& out) const override
  {
    if (d_committed)
    {
      out << *d_committed;
    }
    else if (d_value != nullptr)
    {
      out << d_value->size();
    }
    else
    {
      out << "<undefined>";
    }
  }
  void print_safe(int fd) const override
  {
    if (d_committed)
    {
      safe_print(fd, *d_committed);
    }
    else if (d_value != nullptr)
    {
      safe_print(fd, d_value->size());
    }
    else
    {
      safe_print(fd, "<undefined>");
    }
  }
  void commit()
  {
    if (d_value != nullptr)
    {
      d_committed = d_value->size();
    }
  }
  size_t get() const { return d_committed ? *d_committed : d_value->size(); }

  const T* d_value = nullptr;
  std::optional<std::size_t> d_committed;
};

/**
 * Holds the data for a `TimerStat`.
 * Uses `std::chrono` to obtain the current time, store a time point and sum up
 * the total durations.
 */
struct StatisticTimerValue : StatisticBaseValue
{
  using clock = std::chrono::steady_clock;
  using time_point = clock::time_point;
  struct duration : public std::chrono::nanoseconds
  {
  };
  /** Returns the number of milliseconds */
  StatExportData getViewer() const override;
  /** Prints seconds in fixed-point format */
  void print(std::ostream& out) const override;
  /** Prints seconds in fixed-point format */
  void print_safe(int fd) const override;
  /** Make sure that we include the time of a currently running timer */
  duration get() const;

  duration d_duration;
  time_point d_start;
  bool d_running;
};

}  // namespace CVC4

#endif
