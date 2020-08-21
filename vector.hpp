#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <vector>

#include "utils.hpp"

namespace si {

  namespace core {
    /// Create a tuple of vectors with the given size
    template <class ... Types>
    constexpr std::tuple<std::vector<Types> ...> make_vector_tuple(size_t n, types_holder<Types ...>) {
      return {std::vector<Types>(n) ...};
    }
  }

  /** Vector class
   *
   */
  template<class Object>
  class vector : public Object::vector_type {

  public:

    using iterator = typename Object::vector_iterator_type;

    /// Default constructor
    vector() { }
    /// Construct the vector from a size
    vector(size_t n) : Object::vector_type{core::make_vector_tuple(n, typename Object::types{})} {
      
    };
    /// Destructor
    ~vector() { }

    /// Get the size of the vector
    size_t size() const {

      if constexpr (Object::number_of_fields == 0)
        return 0;
      else
	return std::get<0>(*this).size();
    }

    /// Begining of the vector
    auto begin() {
      return this->begin_impl(std::make_index_sequence<Object::number_of_fields>{});
    }

    /// End of the vector
    auto end() {
      return this->end_impl(std::make_index_sequence<Object::number_of_fields>{});
    }

  private:

    /// Implementation of the begining function
    template<size_t ... I>
    iterator begin_impl(std::index_sequence<I ...>) {

      return {std::begin(std::get<I>(*this)) ...};
    };

    /// Implementation of the end function
    template<size_t ... I>
    iterator end_impl(std::index_sequence<I ...>) {

      return {std::end(std::get<I>(*this)) ...};
    };
  };
}

#endif // VECTOR_HPP
