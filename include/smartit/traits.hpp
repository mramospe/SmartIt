#ifndef SMARTIT_TRAITS_HPP
#define SMARTIT_TRAITS_HPP

#include "utils.hpp"

namespace smit {
  namespace traits {

    template <template <class> class Prototype, class ValueType>
    constexpr auto
    _f_extract_prototype(utils::types_holder<Prototype<ValueType>>) {
      return utils::template_holder<Prototype>{};
    }

    template <class Object> struct extract_prototype {
      template <class ValueType>
      using type = typename decltype(_f_extract_prototype(
          utils::types_holder<Object>{}))::template type<ValueType>;
    };

    template <template <class> class Proxy, class... Types>
    constexpr auto _f_iterator_proxies(utils::types_holder<Types...>) {
      return utils::types_holder<typename Proxy<Types>::iterator...>{};
    }

    template <template <class> class Proxy, class... Types>
    constexpr auto _f_iterator_base(utils::types_holder<Types...>) {
      return std::tuple<typename Proxy<Types>::iterator...>{};
    }

    template <template <class> class Proxy, class H> struct _s_iterator_base {
      using type = decltype(_f_iterator_base<Proxy>(H{}));
    };

    /// Type of the iterator for the given container proxy and types holder
    template <template <class> class Proxy, class H>
    using iterator_base = typename _s_iterator_base<Proxy, H>::type;

    template <template <class> class Proxy, class... Types>
    constexpr auto _f_const_iterator_proxies(utils::types_holder<Types...>) {
      return utils::types_holder<typename Proxy<Types>::const_iterator...>{};
    }

    template <template <class> class Proxy, class... Types>
    constexpr auto _f_const_iterator_base(utils::types_holder<Types...>) {
      return std::tuple<typename Proxy<Types>::const_iterator...>{};
    }

    template <template <class> class Proxy, class H>
    struct _s_const_iterator_base {
      using type = decltype(_f_const_iterator_base<Proxy>(H{}));
    };

    /// Type of the const_iterator for the given container proxy and types
    /// holder
    template <template <class> class Proxy, class H>
    using const_iterator_base = typename _s_const_iterator_base<Proxy, H>::type;

    template <size_t I, class... IterTypes>
    constexpr auto _f_deref_iterator(utils::types_holder<IterTypes...>) {
      return utils::type_wrapper<utils::tuple_element_t<I, IterTypes...>>{};
    }

    /// Determine the type of the iterator values at the given tuple position
    template <size_t I, class Iterator>
    using deref_iterator_t = typename decltype(_f_deref_iterator<I>(
        typename Iterator::iterator_types{}))::type::value_type;

    /// Determine the type of the const_iterator values at the given tuple
    /// position
    template <size_t I, class Iterator>
    using deref_const_iterator_t = typename decltype(_f_deref_iterator<I>(
        typename Iterator::const_iterator_types{}))::type::value_type;
  } // namespace traits
} // namespace smit

#endif
