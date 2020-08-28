#ifndef SMARTIT_VECTOR_HPP
#define SMARTIT_VECTOR_HPP

#include <vector>

#include "iterator.hpp"

namespace smit {

  // Forward declaration of the vector class
  template <class Object> class vector;
  namespace core {

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

    template <class Type>
    using vector_proxy_t = typename vector_proxy<Type>::type;

    // Auxiliar function to determine the vector type
    template <class... Types>
    constexpr auto _f_vector_base(utils::types_holder<Types...>) {
      return std::tuple<vector_proxy_t<Types>...>{};
    }

    /// Container of the base type for vector objects
    template <class H> struct vector_base {
      using type = decltype(_f_vector_base(H{}));
    };

    /// Base type for vector objects
    template <class H> using vector_base_t = typename vector_base<H>::type;

    /// Create a tuple of vectors with the given size
    template <class... Types>
    constexpr std::tuple<vector_proxy_t<Types>...>
    make_vector_tuple(size_t n, utils::types_holder<Types...>) {
      return {vector_proxy_t<Types>(n)...};
    }
  } // namespace core

  /**
   * @brief Definition of a vector based on the std::vector class
   */
  template <class Object>
  class vector : public core::vector_base_t<typename Object::types> {

  public:
    /// Base class
    using base_class = core::vector_base_t<typename Object::types>;
    /// Similar to std::vector
    using iterator = core::__iterator<core::vector_proxy, Object>;

    /// Default constructor
    vector() {}
    /// Construct the vector from a size
    vector(size_t n)
        : base_class{core::make_vector_tuple(n, typename Object::types{})} {};
    /// Destructor
    ~vector() {}

    /// Get the size of the vector
    size_t size() const {

      if constexpr (Object::number_of_fields == 0)
        return 0;
      else
        return std::get<0>(*this).size();
    }

    /// Begining of the vector
    auto begin() {
      return this->begin_impl(
          std::make_index_sequence<Object::number_of_fields>{});
    }

    /// End of the vector
    auto end() {
      return this->end_impl(
          std::make_index_sequence<Object::number_of_fields>{});
    }

  private:
    /// Implementation of the begin function
    template <size_t... I> iterator begin_impl(std::index_sequence<I...>) {

      return {std::begin(std::get<I>(*this))...};
    };

    /// Implementation of the end function
    template <size_t... I> iterator end_impl(std::index_sequence<I...>) {

      return {std::end(std::get<I>(*this))...};
    };
  };
} // namespace smit

#endif // SMARTIT_VECTOR_HPP
