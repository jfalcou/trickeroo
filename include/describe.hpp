//==================================================================================================
/**
  Copyright : Joel FALCOU
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

//==================================================================================================
/**
  describe::type<T> returns a type containing the name of T accessible as a string_view.
  describe::value<V> returns a type containing the value of V accessible as a string_view.
  describe::id<"S"> returns a an object whose type depends on "S".

  Usage: https://gcc.godbolt.org/z/fnKjo7T3M
  Thanks @beached for the improvement
**/
//==================================================================================================
#include <array>
#include <string_view>
#include <utility>

namespace describe
{
  namespace detail 
  {
    template <typename T>
    constexpr auto typer() noexcept 
    {
    #if defined(__clang__)
      constexpr auto prefix = std::string_view("auto describe::detail::typer() [T = ").size();
      constexpr auto suffix = std::string_view("]").size();
      constexpr auto raw    = std::string_view(__PRETTY_FUNCTION__);
    #elif defined(__GNUC__)
      constexpr auto prefix = std::string_view("constexpr auto describe::detail::typer() [with T = ").size();
      constexpr auto suffix = std::string_view("]").size();
      constexpr auto raw    = std::string_view(__PRETTY_FUNCTION__);
    #elif defined(_MSC_VER)
      constexpr auto prefix = std::string_view("auto __cdecl describe::detail::typer<").size();
      constexpr auto suffix = std::string_view(">(void)").size();
      constexpr auto raw    = std::string_view(__FUNCSIG__);
    #endif

      auto value = raw;
      value.remove_prefix(prefix);
      value.remove_suffix(suffix);

      constexpr auto size = raw.size() - (prefix + suffix);
      auto fn = [&]<std::size_t... Is>(std::index_sequence<Is...>) 
      {
        return std::array<char const, sizeof...(Is) + 1>{value[Is]...};
      };
      
      return fn(std::make_index_sequence<size>{});
    }

    template <auto T>
    constexpr auto valuer() noexcept 
    {
    #if defined(__clang__)
      constexpr auto prefix = std::string_view("auto describe::detail::valuer() [T = ").size();
      constexpr auto suffix = std::string_view("]").size();
      constexpr auto raw    = std::string_view(__PRETTY_FUNCTION__);
    #elif defined(__GNUC__)
      constexpr auto prefix = std::string_view("constexpr auto describe::detail::valuer() [with auto T = ").size();
      constexpr auto suffix = std::string_view("]").size();
      constexpr auto raw    = std::string_view(__PRETTY_FUNCTION__);
    #elif defined(_MSC_VER)
      constexpr auto prefix = std::string_view("auto __cdecl describe::detail::valuer<").size();
      constexpr auto suffix = std::string_view(">(void)").size();
      constexpr auto raw    = std::string_view(__FUNCSIG__);
    #endif

      auto value = raw;
      value.remove_prefix(prefix);
      value.remove_suffix(suffix);

      constexpr auto size = raw.size() - (prefix + suffix);
      auto fn = [&]<std::size_t... Is>(std::index_sequence<Is...>) 
      {
        return std::array<char const, sizeof...(Is) + 1>{value[Is]...};
      };
      
      return fn(std::make_index_sequence<size>{});
    }

    template <typename T>
    inline constexpr auto type_array = typer<T>();

    template <auto V>
    inline constexpr auto value_array = valuer<V>();

    template<std::size_t N> struct str_
    {
      std::array<char,N> data;

      template <std::size_t... Is>
      constexpr str_(const char (&str)[N], std::index_sequence<Is...>) :data{str[Is]...} {}
      constexpr str_(const char (&str)[N]) : str_{str, std::make_index_sequence<N>{}} {}

      static constexpr  auto  size()  { return N; }
      auto value() const { return std::string_view(&data[0],N); }
    };

    template<std::size_t N> str_(const char (&str)[N]) -> str_<N>;
  }

  template<typename T, auto ID = detail::type_array<T>>
  struct type_t 
  {
    static constexpr auto value() { return std::string_view(ID.data(), ID.size());}
  };

  template<auto V, auto ID = detail::value_array<V>>
  struct value_t 
  {
    static constexpr auto value() { return std::string_view(ID.data(), ID.size());}
  };

  template<detail::str_ S>
  struct id_t 
  {
    static constexpr auto value() { return S.value();}
  };
  
  template<typename T>      inline constexpr auto type  = type_t<T>{};
  template<auto V>          inline constexpr auto value = value_t<V>{};
  template<detail::str_ V>  inline constexpr auto id    = id_t<V>{};
}
