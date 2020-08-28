#ifndef SMARTIT_DATA_OBJECT_HPP
#define SMARTIT_DATA_OBJECT_HPP

#include <tuple>
#include <type_traits>

#include "containers.hpp"
#include "utils.hpp"
#include "value.hpp"

namespace smit {

  /** Data object
   *
   */
  template <template <class> class Prototype, typename... Types>
  class data_object {

  public:
    /// Types of the fields
    using types = core::types_holder<Types...>;
    /// Number of fields
    static auto const number_of_fields = sizeof...(Types);
    /// Value type (user interface)
    using value_type = __value_type<Prototype, Types...>;
  }; // namespace smit

  namespace core {
    template <template <class> class Prototype, class... Types>
    constexpr auto _f_value_type(types_holder<Types...>) {
      return __value_type<Prototype, Types...>{};
    }

    template <class T> struct _s_value_type {
      using type = decltype(
          core::_f_value_type<T::template prototype>(typename T::types{}));
    };

    template <class T> using _s_value_type_t = typename _s_value_type<T>::type;
  } // namespace core

  /// Value type of the data object build with the provided arguments
  template <template <class> class Prototype, typename... Types>
  using data_object_t = typename data_object<Prototype, Types...>::value_type;

  /// Determine the value type of two or more template arguments to a
  /// prototype class
  template <class First, class... Last> struct common_value_type {
    using type = typename std::enable_if<
        (std::is_same<typename core::_s_value_type_t<First>,
                      typename core::_s_value_type_t<Last>>::value &&
         ...),
        typename core::_s_value_type_t<First>>::type;
  };

  /// Type returned by @common_value_type
  template <class First, class... Last>
  using common_value_type_t = typename common_value_type<First, Last...>::type;
} // namespace smit
#endif // SMARTIT_DATA_OBJECT_HPP
