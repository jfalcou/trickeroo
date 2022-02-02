//==================================================================================================
/**
  Copyright : Joel FALCOU
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once
#include <string_view>

//==================================================================================================
/**
  describe<T>() returns a std::string_view containing the name of T.

  Usage: https://godbolt.org/z/EKK1sqWcW
**/
//==================================================================================================
template <typename T> constexpr auto describe() noexcept
{
  std::string_view name, prefix, suffix;
#ifdef __clang__
  name = __PRETTY_FUNCTION__;
  prefix = "auto describe() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
  name = __PRETTY_FUNCTION__;
  prefix = "constexpr auto describe() [with T = ";
  suffix = "]";
#elif defined(_MSC_VER)
  name = __FUNCSIG__;
  prefix = "auto __cdecl describe<";
  suffix = ">(void)";
#endif
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}
