#ifndef SMARTIT_UTILS_HPP
#define SMARTIT_UTILS_HPP

#include <tuple>
#include <utility>

namespace smit {
  namespace utils {

    template <typename Type> struct type_wrapper { using type = Type; };

    /// Hold types
    template <typename... Types> struct types_holder {};

    /// Hold templates
    template <template <class...> class T> struct template_holder {
      template <class... U> using type = T<U...>;
    };

    /// Check if two templates are the same
    template <template <typename...> class, template <typename...> class>
    struct is_same_template : std::false_type {};

    template <template <typename...> class T>
    struct is_same_template<T, T> : std::true_type {};

    /// Determines the type of an element in a tuple
    template <size_t N, typename... T> struct tuple_element;

    template <typename T0, typename... T> struct tuple_element<0, T0, T...> {
      using type = T0;
    };

    template <size_t N, typename T0, typename... T>
    struct tuple_element<N, T0, T...> {
      using type = typename tuple_element<N - 1, T...>::type;
    };

    /// Type of smit::utils::tuple_element
    template <size_t N, typename... T>
    using tuple_element_t = typename tuple_element<N, T...>::type;

    template <size_t I, template <class...> class Dependent, class... Types>
    constexpr auto _f_tuple_element_for(types_holder<Dependent<Types...>>) {
      return tuple_element<I, Types...>{};
    }

    /// Access the type of an element of a dependent type
    template <size_t I, class Dependent> struct tuple_element_for {
      using type = typename decltype(
          _f_tuple_element_for(types_holder<Dependent>{}))::type;
    };

    /// Type of smit::utils::tuple_element_for
    template <size_t I, class Dependent>
    using tuple_element_for_t = typename tuple_element_for<I, Dependent>::type;

    template <size_t... I, class... T>
    std::tuple<T &...> _vtuple_to_rtuple(std::tuple<T...> &t,
                                         std::index_sequence<I...>) {
      return {std::get<I>(t)...};
    }

    /// Convert a tuple of values to a tuple of references
    template <class... T>
    std::tuple<T &...> vtuple_to_rtuple(std::tuple<T...> &t) {
      return _vtuple_to_rtuple(t, std::make_index_sequence<sizeof...(T)>());
    }

    template <class...> struct which_type;
  } // namespace utils
} // namespace smit

#endif // SMARTIT_UTILS_HPP
