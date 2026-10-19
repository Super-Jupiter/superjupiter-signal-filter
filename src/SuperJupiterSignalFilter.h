// SPDX-License-Identifier: MIT
// Copyright (c) 2026 SuperJupiter
#ifndef SUPERJUPITER_SIGNAL_FILTER_H
#define SUPERJUPITER_SIGNAL_FILTER_H
#if defined(__FAST_MATH__) || (defined(__FINITE_MATH_ONLY__) && __FINITE_MATH_ONLY__ > 0)
#error "SuperJupiter requires IEEE non-finite checks; disable fast-math"
#endif
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#define SUPERJUPITER_SIGNAL_FILTER_VERSION "0.1.0"
namespace superjupiter {
namespace signal_filter {
enum class Status {
  Seeded,
  Filtered,
  GapReset,
  InvalidConfig,
  InvalidInput,
  InvalidTime,
  NumericError
};
struct Config {
  double tauMs;
  uint32_t maxGapMs;
  Config(double tau = 1000, uint32_t gap = 10000) : tauMs(tau), maxGapMs(gap) {}
};
struct Result {
  Status status;
  double value;
};
class SignalFilter {
  Config config_;
  bool seeded_;
  double value_;
  uint32_t time_;

public:
  explicit SignalFilter(Config config = Config())
      : config_(config), seeded_(false), value_(0), time_(0) {}
  void reset() {
    seeded_ = false;
    value_ = 0;
    time_ = 0;
  }
  Result push(double raw, uint32_t atMs) {
    if (!std::isfinite(config_.tauMs) || config_.tauMs <= 0 || config_.maxGapMs == 0 ||
        config_.maxGapMs >= 0x80000000UL)
      return Result{Status::InvalidConfig, value_};
    if (!std::isfinite(raw))
      return Result{Status::InvalidInput, value_};
    if (!seeded_) {
      seeded_ = true;
      value_ = raw;
      time_ = atMs;
      return Result{Status::Seeded, value_};
    }
    const uint32_t dt = atMs - time_;
    if (dt == 0 || dt >= 0x80000000UL)
      return Result{Status::InvalidTime, value_};
    if (dt > config_.maxGapMs) {
      value_ = raw;
      time_ = atMs;
      return Result{Status::GapReset, value_};
    }
    const double alpha = -std::expm1(-static_cast<double>(dt) / config_.tauMs);
    const double next = (1 - alpha) * value_ + alpha * raw;
    if (!std::isfinite(next))
      return Result{Status::NumericError, value_};
    value_ = next;
    time_ = atMs;
    return Result{Status::Filtered, value_};
  }
};
} // namespace signal_filter
} // namespace superjupiter
#endif
