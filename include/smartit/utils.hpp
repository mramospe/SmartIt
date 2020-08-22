#ifndef UTILS_HPP
#define UTILS_HPP

namespace smit {
  namespace core{

    /// Hold types
    template <typename ... Types>
    struct types_holder { };

    /// Determines the type of an element in a tuple
    template <int N, typename... T>
    struct tuple_element;

    template <typename T0, typename... T>
    struct tuple_element<0, T0, T...> {
      typedef T0 type;
    };
    template <int N, typename T0, typename... T>
    struct tuple_element<N, T0, T...> {
      typedef typename tuple_element<N - 1, T...>::type type;
    };
  }
}

#endif
