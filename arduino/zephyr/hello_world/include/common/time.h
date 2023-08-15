// Copyright (c) 2023, Henrik Söderlund

#pragma once

#include <zephyr/kernel.h>

#include "common/assert.h"

namespace time {
struct Frequency;

/**
 * @brief Class that holds a time value\n
 **/
struct TimeUnit {
  explicit TimeUnit(const TimeUnit& from)
    : _val(static_cast<float>(from)), conversion_factor(from.conversion_factor) {}

  constexpr TimeUnit(float val, uint64_t conversion_factor)
    : _val(val), conversion_factor(conversion_factor) {}

  constexpr TimeUnit(uint64_t val, uint64_t conversion_factor)
    : _val(static_cast<float>(val)), conversion_factor(conversion_factor) {}

  TimeUnit(const TimeUnit& from, uint64_t conversion_factor)
    : _val(0), conversion_factor(conversion_factor) {
    if (conversion_factor < from.conversion_factor) {
      ASSERT(from.conversion_factor != 0UL);
      _val = static_cast<float>(from) * static_cast<float>(conversion_factor) /
             from.conversion_factor;
    } else {
      ASSERT(conversion_factor != 0UL);
      _val = static_cast<float>(from) * from.conversion_factor / conversion_factor;
    }
  }

  TimeUnit(const Frequency& from, uint64_t conversion_factor);

  operator float() const {
    return _val;
  }

  operator uint64_t() const {
    return static_cast<uint64_t>(_val);
  }

  float          _val;
  const uint64_t conversion_factor;
};

/**
 * @brief Class that holds a time value in seconds\n
 **/
struct Seconds : public TimeUnit {
  constexpr explicit Seconds(float val) : TimeUnit(val, 1UL) {}
  explicit           Seconds(const TimeUnit& from) : TimeUnit(from, 1UL) {}
  explicit           Seconds(const Frequency& from) : TimeUnit(from, 1UL) {}
};

/**
 * @brief Class that holds a time value in milliseconds\n
 **/
struct Milliseconds : public TimeUnit {
  constexpr explicit Milliseconds(uint64_t val) : TimeUnit(val, 1000UL) {}
  explicit           Milliseconds(const TimeUnit& from) : TimeUnit(from, 1000UL) {}
  explicit           Milliseconds(const Frequency& from) : TimeUnit(from, 1000UL) {}
};

/**
 * @brief Class that holds a time value in microseconds\n
 **/
struct Microseconds : public TimeUnit {
  constexpr explicit Microseconds(uint64_t val) : TimeUnit(val, 1000000UL) {}
  explicit           Microseconds(const TimeUnit& from) : TimeUnit(from, 1000000UL) {}
  explicit           Microseconds(const Frequency& from) : TimeUnit(from, 1000000UL) {}
};

/**
 * @brief Class that holds a frequency value in Hz\n
 **/
struct Frequency {
  explicit Frequency(const Frequency& from)
    : _val(static_cast<uint64_t>(from)) {}

  constexpr explicit Frequency(uint64_t val) : _val(val) {}
  constexpr explicit Frequency(float val) : _val(static_cast<uint64_t>(val)) {}

  explicit Frequency(const TimeUnit& from)
    : _val(1.0F / static_cast<float>(Seconds(from))) {}

  operator float() const {
    return static_cast<float>(_val);
  }

  operator uint64_t() const {
    return _val;
  }

  uint64_t _val;
};

inline TimeUnit::TimeUnit(const Frequency& from, uint64_t conversion_factor)
  : _val(0), conversion_factor(conversion_factor) {
  _val = static_cast<float>(conversion_factor) / static_cast<float>(from);
}
}  // namespace time
