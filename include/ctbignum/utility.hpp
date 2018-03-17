#ifndef CT_UTILITY_HPP
#define CT_UTILITY_HPP

#include <array>
#include <cstddef>
#include <ctbignum/bigint.hpp>
#include <limits>
#include <utility>

namespace cbn {
namespace detail {


template <size_t N, typename T>
constexpr auto tight_length(big_int<N, T> num) {
  // count the effective number of limbs
  // (ignoring zero-limbs at the most-significant-limb side)
  size_t L = N;
  while (L > 0 && num[L - 1] == 0)
    --L;

  return L;
}

template <typename T, T... Is>
constexpr auto tight_length(std::integer_sequence<T, Is...>)

{
  // count the effective number of limbs
  // (ignoring zero-limbs at the most-significant-limb side)

  size_t L = sizeof...(Is);
  std::array<T, sizeof...(Is)> num{Is...};
  while (L > 0 && num[L - 1] == 0)
    --L;

  return L;
}

template <std::size_t N, typename T>
constexpr auto bit_length(big_int<N, T> num) {
  auto L = tight_length(num);
  size_t bitlen = L * std::numeric_limits<T>::digits;
  T msb = num[L - 1];
  while ((msb & (static_cast<T>(1) << (std::numeric_limits<T>::digits - 1))) == 0) {
    msb <<= 1;
    --bitlen;
  }
  return bitlen;
}

template <std::size_t N1, std::size_t N2, typename T>
constexpr void assign(big_int<N1, T>& dst, big_int<N2, T> src) {
  // assignment for the scenario where N1 >= N2

  static_assert(N1 >= N2,
                "cannot assign: destination has smaller size than source");

  for (auto i = 0; i < N1; ++i)
    dst[i] = src[i];
  for (auto i = N1; i < N2; ++i)
    dst[i] = 0;
}

} // end of detail namespace
} // end of cbn namespace

#endif
