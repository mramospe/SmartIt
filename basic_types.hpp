#ifndef BASIC_TYPES_HPP
#define BASIC_TYPES_HPP

#include "data_object.hpp"

namespace si {

  template<class T>
  class point3d_proto : public T {

  public:

    using T::T;

    const auto& x() const { return get_field<0>(*this); }
    auto& x() { return get_field<0>(*this); }

    const auto& y() const { return get_field<1>(*this); }
    auto& y() { return get_field<1>(*this); }

    const auto& z() const { return get_field<2>(*this); }
    auto& z() { return get_field<2>(*this); }    
  };

  template<typename Type>
  using point3d = typename data_object<point3d_proto, Type, Type, Type>::value_type;
}

#endif // BASIC_TYPES_HPP
