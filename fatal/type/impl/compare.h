/*
 *  Copyright (c) 2016, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_impl_compare_h
#define FATAL_INCLUDE_fatal_type_impl_compare_h

namespace fatal {
namespace impl_comp {

template <typename, typename, typename, int = 0> struct dc;

template <typename Less, typename LHS, typename RHS>
struct dc<Less, LHS, RHS, -1>: std::true_type {};

template <typename Less, typename LHS, typename RHS>
struct dc<Less, LHS, RHS, 1>: std::false_type {};

template <typename Less, template <typename...> class Variadic>
struct dc<Less, Variadic<>, Variadic<>, 0>: std::false_type {};

template <typename Less, template <typename...> class Variadic, typename... LHS>
struct dc<Less, Variadic<LHS...>, Variadic<>, 0>: std::false_type {};

template <typename Less, template <typename...> class Variadic, typename... RHS>
struct dc<Less, Variadic<>, Variadic<RHS...>, 0>: std::true_type {};

template <
  typename Less,
  template <typename...> class Variadic,
  typename L, typename... LHS,
  typename R, typename... RHS
>
struct dc<Less, Variadic<L, LHS...>, Variadic<R, RHS...>, 0>:
  dc<
    Less,
    Variadic<LHS...>,
    Variadic<RHS...>,
    Less::template apply<L, R>::value
      ? -1
      : Less::template apply<R, L>::value
        ? 1
        : 0
  >
{};

template <typename Less, template <typename V, V...> class Variadic, typename T>
struct dc<Less, Variadic<T>, Variadic<T>, 0>: std::false_type {};

template <
  typename Less,
  template <typename V, V...> class Variadic,
  typename T,
  T... LHS
>
struct dc<Less, Variadic<T, LHS...>, Variadic<T>, 0>: std::false_type {};

template <
  typename Less,
  template <typename V, V...> class Variadic,
  typename T,
  T... RHS
>
struct dc<Less, Variadic<T>, Variadic<T, RHS...>, 0>: std::true_type {};

template <
  typename Less,
  template <typename V, V...> class Variadic,
  typename T,
  T L, T... LHS,
  T R, T... RHS
>
struct dc<Less, Variadic<T, L, LHS...>, Variadic<T, R, RHS...>, 0>:
  dc<
    Less,
    Variadic<T, LHS...>,
    Variadic<T, RHS...>,
    (L < R) ? -1 : (R < L) ? 1 : 0 // TODO: USE PREDICATE
    // TODO: FIX PREDICATE FOR SEQUENCES
    /*
    Less::template apply<L, R>::value
      ? -1
      : Less::template apply<R, L>::value
        ? 1
        : 0
    */
  >
{};

} // namespace impl_comp {
} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_impl_compare_h
