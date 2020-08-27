#ifndef SMARTIT_VECTOR_HPP
#define SMARTIT_VECTOR_HPP

#include <vector>

#include "containers.hpp"

namespace smit {

  /** Vector class
   *
   */
  template <class Object> class vector : public core::vector_base_t<typename Object::types> {

  public:
    /// Base class
    using base_class = core::vector_base_t<typename Object::types>;
    /// Similar to std::vector
    using iterator = typename Object::vector_iterator_type;

    /// Default constructor
    vector() {}
    /// Construct the vector from a size
    vector(size_t n)
        : base_class{
              core::make_vector_tuple(n, typename Object::types{})} {};
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
