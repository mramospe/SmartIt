#ifndef SMARTIT_CONTAINERS_HPP
#define SMARTIT_CONTAINERS_HPP

#include <array>
#include <tuple>
#include <type_traits>
#include <vector>

#include "utils.hpp"

namespace smit {

  // Forward declaration of container classes
  template <class Object, size_t N> class array;
  template <class Object> class vector;

  namespace core {

    /// Proxy for an array storing its type and iterator
    template <class Type, size_t N, class Enable = void>
    struct array_proxy {}; // primary template

    template <class Type, size_t N>
    struct array_proxy<
        Type, N,
        typename std::enable_if_t<std::is_arithmetic<Type>::value>::type> {
      using type = std::array<Type, N>;
      using iterator = typename type::iterator;
    };

    template <class Type, size_t N>
    struct array_proxy<
        Type, N,
        typename std::enable_if_t<!std::is_arithmetic<Type>::value>::type> {
      using type = std::array<Type, N>;
      using iterator = typename type::iterator;
    };

    /// Base type for array objects
    template <size_t N, class ... Types>
    struct array_base {
      using type = std::tuple<typename array_proxy<Types, N>::type...>;
    };

    /// Proxy for a vector, storing its type and iterator
    template <class Type, class Enable = void>
    struct vector_proxy {}; // primary template

    template <class Type>
    struct vector_proxy<
        Type, typename std::enable_if<std::is_arithmetic<Type>::value>::type> {
      using type = std::vector<Type>;
      using iterator = typename type::iterator;
    };

    template <class Type>
    struct vector_proxy<
        Type, typename std::enable_if<!std::is_arithmetic<Type>::value>::type> {
      using type = vector<Type>;
      using iterator = typename type::iterator;
    };


    // Auxiliar function to determine the vector type
    template<class ... Types>
    constexpr auto _f_vector_base(types_holder<Types ...>) {
      return std::tuple<typename vector_proxy<Types>::type ...>{};
    }

    /// Container of the base type for vector objects
    template<class H>
    struct vector_base {
      using type = decltype(_f_vector_base(H{}));
    };

    /// Base type for vector objects
    template<class H>
    using vector_base_t = typename vector_base<H>::type;

    /// Create a tuple of vectors with the given size
    template <class... Types>
    constexpr std::tuple<typename vector_proxy<Types>::type...>
    make_vector_tuple(size_t n, types_holder<Types...>) {
      return {typename vector_proxy<Types>::type(n)...};
    }
  } // namespace core
} // namespace smit

#endif // SMARTIT_CONTAINERS_HPP
