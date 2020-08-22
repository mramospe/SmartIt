#ifndef SMARTIT_CONTAINERS_HPP
#define SMARTIT_CONTAINERS_HPP

#include <array>
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

    /// Create a tuple of vectors with the given size
    template <class... Types>
    constexpr std::tuple<typename vector_proxy<Types>::type...>
    make_vector_tuple(size_t n, types_holder<Types...>) {
      return {typename vector_proxy<Types>::type(n)...};
    }
  } // namespace core
} // namespace smit

#endif // SMARTIT_CONTAINERS_HPP
