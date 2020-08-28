#ifndef SMARTIT_UTILS_HPP
#define SMARTIT_UTILS_HPP

namespace smit {
  namespace utils {

    /// Hold types
    template <typename... Types> struct types_holder {};

    /// Determines the type of an element in a tuple
    template <int N, typename... T> struct tuple_element;

    template <typename T0, typename... T> struct tuple_element<0, T0, T...> {
      using type = T0;
    };

    template <int N, typename T0, typename... T>
    struct tuple_element<N, T0, T...> {
      using type = typename tuple_element<N - 1, T...>::type;
    };

    template <int N, typename T0, typename... T>
    using tuple_element_t = typename tuple_element<N, T0, T...>::type;
  } // namespace utils
} // namespace smit

#endif // SMARTIT_UTILS_HPP
