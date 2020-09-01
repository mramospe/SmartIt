#ifndef SMARTIT_ARRAY_HPP
#define SMARTIT_ARRAY_HPP

#include <array>

#include "iterator.hpp"

namespace smit {

  // Forward declaration of the array class
  template <class Object, size_t N> class array;

  namespace core {
    /// Proxy for an array storing its type and iterator
    template <class Type, size_t N, class Enable = void>
    struct array_proxy {}; // primary template

    template <class Type, size_t N>
    struct array_proxy<
        Type, N,
        typename std::enable_if<std::is_arithmetic<Type>::value>::type> {
      using type = std::array<Type, N>;
      using iterator = typename type::iterator;
    };

    template <class Type, size_t N>
    struct array_proxy<
        Type, N,
        typename std::enable_if<!std::is_arithmetic<Type>::value>::type> {
      using type = array<Type, N>;
      using iterator = typename type::iterator;
    };

    template <class Type, size_t N>
    using array_proxy_t = typename array_proxy<Type, N>::type;

    // Auxiliar function to determine the array type
    template <size_t N, class... Types>
    constexpr auto _f_array_base(utils::types_holder<Types...>) {
      return utils::type_wrapper<std::tuple<array_proxy_t<Types, N>...>>{};
    }

    /// Container of the base type for array objects
    template <class H, size_t N> struct array_base {
      using type = typename decltype(_f_array_base<N>(H{}))::type;
    };

    /// Base type for array objects
    template <class H, size_t N>
    using array_base_t = typename array_base<H, N>::type;

    template <size_t N> struct sized_array_proxy {
      template <class Type> using type = array_proxy_t<Type, N>;
    };
  } // namespace core

  /**
   * @brief Definition of an array based on the std::array class
   */
  template <class Object, size_t N>
  class array : public core::array_base_t<typename Object::types, N> {

  public:
    using base_class = core::array_base_t<typename Object::types, N>;
    using iterator =
        core::__iterator<core::sized_array_proxy<N>::template type, Object>;

    /// Default constructor
    array() : base_class{} {}
    /// Destructor
    ~array() {}

    /// Get the size of the array
    size_t size() const {

      if constexpr (Object::number_of_fields == 0)
        return 0;
      else
        return N;
    }

    /// Begining of the array
    auto begin() {
      return this->begin_impl(
          std::make_index_sequence<Object::number_of_fields>{});
    }

    /// End of the array
    auto end() {
      return this->end_impl(
          std::make_index_sequence<Object::number_of_fields>{});
    }

  private:
    /// Implementation of the begining function
    template <size_t... I> iterator begin_impl(std::index_sequence<I...>) {

      return {std::begin(std::get<I>(*this))...};
    };

    /// Implementation of the end function
    template <size_t... I> iterator end_impl(std::index_sequence<I...>) {

      return {std::end(std::get<I>(*this))...};
    };
  };
} // namespace smit

#endif // SMARTIT_ARRAY_HPP
