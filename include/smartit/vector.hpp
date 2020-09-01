#ifndef SMARTIT_VECTOR_HPP
#define SMARTIT_VECTOR_HPP

#include <vector>

#include "iterator.hpp"

namespace smit {

  // Forward declaration of the vector class
  template <class Object, template <class> class Alloc> class vector;
  namespace core {

    /// Proxy for a vector, storing its type and iterator
    template <class Type, template <class> class Alloc, class Enable = void>
    struct vector_proxy {}; // primary template

    template <class Type, template <class> class Alloc>
    struct vector_proxy<
        Type, Alloc,
        typename std::enable_if<std::is_arithmetic<Type>::value>::type> {
      using type = std::vector<Type, Alloc<Type>>;
      using iterator = typename type::iterator;
      using const_iterator = typename type::const_iterator;
    };

    template <class Type, template <class> class Alloc>
    struct vector_proxy<
        Type, Alloc,
        typename std::enable_if<!std::is_arithmetic<Type>::value>::type> {
      using type = vector<Type, Alloc>;
      using iterator = typename type::iterator;
      using const_iterator = typename type::const_iterator;
    };

    template <class Type, template <class> class Alloc>
    using vector_proxy_t = typename vector_proxy<Type, Alloc>::type;

    // Auxiliar function to determine the vector type
    template <template <class> class Alloc, class... Types>
    constexpr auto _f_vector_base(utils::types_holder<Types...>) {
      return std::tuple<vector_proxy_t<Types, Alloc>...>{};
    }

    /// Container of the base type for vector objects
    template <class H, template <class> class Alloc> struct vector_base {
      using type = decltype(_f_vector_base<Alloc>(H{}));
    };

    /// Base type for vector objects
    template <class H, template <class> class Alloc>
    using vector_base_t = typename vector_base<H, Alloc>::type;

    template <template <class> class Alloc> struct alloc_vector_proxy {
      template <class Type> using type = vector_proxy_t<Type, Alloc>;
    };

    /// Create a tuple of vectors with the given size
    template <template <class> class Alloc, class... Types>
    constexpr std::tuple<vector_proxy_t<Types, Alloc>...>
    make_vector_tuple(size_t n, utils::types_holder<Types...>) {
      return {vector_proxy_t<Types, Alloc>(n)...};
    }
  } // namespace core

  /**
   * @brief Definition of a vector based on the std::vector class
   */
  template <class Object, template <class> class Alloc = std::allocator>
  class vector : public core::vector_base_t<typename Object::types, Alloc> {

  public:
    /// Base class
    using base_class = core::vector_base_t<typename Object::types, Alloc>;
    /// Vector iterator
    using iterator =
        core::__iterator<core::alloc_vector_proxy<Alloc>::template type,
                         Object>;
    /// Vector constant iterator
    using const_iterator =
        core::__const_iterator<core::alloc_vector_proxy<Alloc>::template type,
                               Object>;

    /// Default constructor
    vector() {}
    /// Construct the vector from a size
    vector(size_t n)
        : base_class{
              core::make_vector_tuple<Alloc>(n, typename Object::types{})} {};
    /// Destructor
    ~vector() {}

    inline typename iterator::value_type &operator[](size_t i) {
      return this->at(i);
    }

    inline typename iterator::value_type const &operator[](size_t i) const {
      return this->at(i);
    }

    /// Returns a reference at position i in the vector
    typename iterator::value_type &at(size_t i) {
      return this->at_impl(
          i, std::make_index_sequence<Object::number_of_fields>{});
    }

    /// Returns a reference at position i in the vector (constant)
    typename iterator::value_type const &at(size_t i) const {
      return this->at_impl(
          i, std::make_index_sequence<Object::number_of_fields>{});
    }

    /// Test whether the vector is empty
    inline bool empty() const { return this->size() == 0; }

    /// Requests that the vector capacity of each field be at least enough to
    /// contain n elements.
    void reserve(size_t n) {
      this->reserve_impl(n,
                         std::make_index_sequence<Object::number_of_fields>{});
    }

    /// Change size
    void resize(size_t n) {
      this->resize_impl(n,
                        std::make_index_sequence<Object::number_of_fields>{});
    }

    /// Get the size of the vector
    inline size_t size() const {

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

    /// Begining of the vector (constant)
    auto begin() const {
      return this->cbegin_impl(
          std::make_index_sequence<Object::number_of_fields>{});
    }

    /// Begining of the vector (constant)
    auto cbegin() {
      return this->cbegin_impl(
          std::make_index_sequence<Object::number_of_fields>{});
    }

    /// End of the vector
    auto end() {
      return this->end_impl(
          std::make_index_sequence<Object::number_of_fields>{});
    }

    /// End of the vector (constant)
    auto end() const {
      return this->cend_impl(
          std::make_index_sequence<Object::number_of_fields>{});
    }

    /// End of the vector (constant)
    auto cend() {
      return this->cend_impl(
          std::make_index_sequence<Object::number_of_fields>{});
    }

  private:
    /// Implementation of the at function
    template <size_t... I>
    typename iterator::value_type &at_impl(size_t i,
                                           std::index_sequence<I...>) {
      return *(this->begin() + i);
    }

    /// Implementation of the at function (constant)
    template <size_t... I>
    typename iterator::value_type const &
    at_impl(size_t i, std::index_sequence<I...>) const {
      return *(this->cbegin() + i);
    }

    /// Implementation of the begin function
    template <size_t... I> iterator begin_impl(std::index_sequence<I...>) {

      return {std::begin(std::get<I>(*this))...};
    };

    /// Implementation of the cbegin function
    template <size_t... I>
    const_iterator cbegin_impl(std::index_sequence<I...>) const {

      return {std::cbegin(std::get<I>(*this))...};
    };

    /// Implementation of the end function
    template <size_t... I> iterator end_impl(std::index_sequence<I...>) {

      return {std::end(std::get<I>(*this))...};
    };

    /// Implementation of the cend function
    template <size_t... I>
    const_iterator cend_impl(std::index_sequence<I...>) const {

      return {std::cend(std::get<I>(*this))...};
    };

    /// Implementation of the reserve function
    template <size_t... I>
    inline void reserve_impl(size_t n, std::index_sequence<I...>) {
      (std::get<I>(*this).reserve(n), ...);
    }

    /// Implementation of the resize function
    template <size_t... I>
    inline void resize_impl(size_t n, std::index_sequence<I...>) {
      (std::get<I>(*this).resize(n), ...);
    }
  };
} // namespace smit

#endif // SMARTIT_VECTOR_HPP
