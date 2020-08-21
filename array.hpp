#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "utils.hpp"

namespace si {

  /** Array class
   *
   */
  template<class Object, size_t N>
  class array : public Object::template array_type<N> {

  public:

    using iterator = typename Object::template array_iterator_type<N>;

    /// Default constructor
    array() { }
    /// Destructor
    ~array() { }

    /// Get the size of the array
    size_t size() const {

      if constexpr (Object::number_of_fields == 0)
        return 0;
      else
	return std::get<0>(*this).size();
    }

    /// Begining of the array
    auto begin() {
      return this->begin_impl(std::make_index_sequence<Object::number_of_fields>{});
    }

    /// End of the array
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

#endif
