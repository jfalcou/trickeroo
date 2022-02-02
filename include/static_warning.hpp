//==================================================================================================
/**
  Copyright : Joel FALCOU
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once
#include <type_traits>

//==================================================================================================
/**
  static_warning triggers a compiler warning with a custom message based on the fact a given
  constexpr condition is satisfied or not.

  It's basically static_assert but emitting a warning.

  Usage sample: https://godbolt.org/z/fY6qPTWe4
**/
//==================================================================================================
#define static_warning(Message, ...)                                                                \
static_assert ([] () {                                                                              \
  struct warn                                                                                       \
  {                                                                                                 \
    [[deprecated ("\n*** WARNING ***\n" Message "\n*** WARNING ***\n")]]                            \
    constexpr bool f ( ::std::false_type) { return true; }                                          \
    constexpr bool f ( ::std::true_type)  { return true; }                                          \
  };                                                                                                \
  return warn{}.f(std::bool_constant<__VA_ARGS__>{});                                               \
}())                                                                                                \
/**/
