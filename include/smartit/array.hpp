#ifndef SMARTIT_ARRAY_HPP
#define SMARTIT_ARRAY_HPP

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
    template <size_t N, class... Types> struct array_base {
      using type = std::tuple<typename array_proxy<Types, N>::type...>;
    };
  } // namespace core

  /**
   * @brief Definition of an array based on the std::array class
   */
  template <class Object, size_t N>
  class array : public Object::template array_type<N> {

  public:
    using iterator = typename Object::template array_iterator_type<N>;

    /// Default constructor
    array() {}
    /// Destructor
    ~array() {}

    /// Get the size of the array
    size_t size() const {

      if constexpr (Object::number_of_fields == 0)
        return 0;
      else
        return std::get<0>(*this).size();
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
