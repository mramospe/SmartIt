#ifndef SMARTIT_VALUE_HPP
#define SMARTIT_VALUE_HPP

#include "utils.hpp"

namespace smit {

  // Forward declaration of access functions
  template <class Object> auto &get_field(Object &obj);

  template <class Object> auto const &get_field(Object const &obj);

  /** Value type
   *
   */
  template <template <class> class Prototype, class... Types>
  class __base_value_type : public std::tuple<Types...> {

  public:
    template <class T> using prototype = Prototype<T>;
    using types = core::types_holder<Types...>;

    static const auto number_of_fields = sizeof...(Types);

    /// Get an element from a data object
    template <size_t Index> friend auto &get_field(__base_value_type &v) {
      return std::get<Index>(v);
    }

    /// Get an element from a constant data object
    template <size_t Index>
    friend auto const &get_field(__base_value_type const &v) {
      return std::get<Index>(v);
    }

    /// Inherit constructors
    using std::tuple<Types...>::tuple;
  };

  /// Declare the value type
  template <template <class> class Prototype, class... Types>
  using __value_type = Prototype<__base_value_type<Prototype, Types...>>;

  /** Container type
   *
   */
  template <template <class> class Prototype, class Iterator> class __base_container_type {

  public:
    /// Link to the iterator instance
    Iterator &m_iter;

  public:
    /// Get an element from a container type
    template <size_t Index> friend auto &get_field(__base_container_type &ct) {
      return *std::get<Index>(ct.m_iter);
    }

    /// Get an element from a container type (constant)
    template <size_t Index>
    friend auto const &get_field(__base_container_type const &ct) {
      return *std::get<Index>(ct.m_iter);
    }

    template <class T> using prototype = Prototype<T>;
    using types = typename Iterator::types;

    /// Construct the class from the iterator instance
    __base_container_type(Iterator &it) : m_iter{it} {}
  };

  /// Declare the container type
  template <template <class> class Prototype, class Iterator>
  using __container_type = Prototype<__base_container_type<Prototype, Iterator>>;
} // namespace smit

#endif
