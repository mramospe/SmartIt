#ifndef BASIC_TYPES_HPP
#define BASIC_TYPES_HPP

#include "data_object.hpp"

namespace si {

  template<typename Type>
  class point3d : public data_object<point3d<Type>, Type, Type, Type> {

  public:

    using data_object<point3d<Type>, Type, Type, Type>::data_object;
    
    const auto& x() const { return get_field<0>(*this); }
    auto& x() { return get_field<0>(*this); }

    const auto& y() const { return get_field<1>(*this); }
    auto& y() { return get_field<1>(*this); }

    const auto& z() const { return get_field<2>(*this); }
    auto& z() { return get_field<2>(*this); }
  };
}

#endif // BASIC_TYPES_HPP
