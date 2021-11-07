#pragma once

template <typename T> T abs(T val) { return val < 0 ? -1 * val : val; }

template <typename T> T signOf(T val) {
  return ((val < 0) ? -1 : ((val > 0) ? 1 : 0));
}

template <typename T> T max(T a, T b) { return (a >= b) ? a : b; }

template <typename T> T min(T a, T b) { return (a <= b) ? a : b; }
