//==================================================================================================
/**
  Copyright : Joel FALCOU
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

//==================================================================================================
/**
  describe::type<T> returns a type containing the name of T accessible as a string_view.
  describe::value<V> returns a type containing the value of V accessible as a string_view.
  describe::id<"S"> returns a an object whose type depends on "S".

  Usage: https://gcc.godbolt.org/z/fnKjo7T3M
  Thanks @beached for the improvements
**/
//==================================================================================================
#include <array>
#include <string_view>
#include <utility>

#if defined(__clang__)
#define DSC_FUNCTION __PRETTY_FUNCTION__
#define DSC_SETUP(NAME,SEP)                                                                \
  constexpr auto pfx = std::string_view("auto describe::detail::" NAME "() [T = ").size(); \
  constexpr auto sfx = std::string_view("]").size();                                       \
  constexpr auto raw = std::string_view(DSC_FUNCTION)                                      \
  /**/
#elif defined(__GNUC__)
#define DSC_FUNCTION __PRETTY_FUNCTION__
#define DSC_SETUP(NAME,SEP)                                                                                      \
  constexpr auto pfx = std::string_view("constexpr auto describe::detail::" NAME "() [with " SEP "T = ").size(); \
  constexpr auto sfx = std::string_view("]").size();                                                             \
  constexpr auto raw = std::string_view(DSC_FUNCTION)                                                            \
  /**/
#elif defined(_MSC_VER)
#define DSC_FUNCTION __FUNCSIG__
#define DSC_SETUP(NAME,SEP)                                                                 \
  constexpr auto pfx = std::string_view("auto __cdecl describe::detail::" NAME "<").size(); \
  constexpr auto sfx = std::string_view(">(void)").size();                                  \
  constexpr auto raw = std::string_view(FN)                                                 \
  /**/
#endif

namespace describe
{
  namespace detail 
  {
    template <typename T> constexpr auto typer() noexcept 
    {
      DSC_SETUP("typer","");
      auto value = raw;
      value.remove_prefix(pfx);
      value.remove_suffix(sfx);

      constexpr auto size = raw.size() - (pfx + sfx);
      auto fn = [&]<std::size_t... Is>(std::index_sequence<Is...>) 
      {
        return std::array<char const, sizeof...(Is) + 1>{value[Is]...};
      };
      
      return fn(std::make_index_sequence<size>{});
    }

    template <auto T> constexpr auto valuer() noexcept 
    {
      DSC_SETUP("valuer","auto ");
      auto value = raw;
      value.remove_prefix(pfx);
      value.remove_suffix(sfx);

      constexpr auto size = raw.size() - (pfx + sfx);
      auto fn = [&]<std::size_t... Is>(std::index_sequence<Is...>) 
      {
        return std::array<char const, sizeof...(Is) + 1>{value[Is]...};
      };

      return fn(std::make_index_sequence<size>{});
    }

    template<typename T>  inline constexpr auto type_array  = typer<T>();
    template<auto V>      inline constexpr auto value_array = valuer<V>();

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
  
  template<typename T>      inline constexpr auto type  = type_t<T>{};
  template<auto V>          inline constexpr auto value = value_t<V>{};
  template<detail::str_ S>  inline constexpr auto id    = S;

  namespace literals
  {
    template<detail::str_ ID> constexpr auto operator""_id() noexcept { return id<ID>; }
  }
}
