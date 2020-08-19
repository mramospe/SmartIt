#ifndef BASIC_TYPES_HPP
#define BASIC_TYPES_HPP

#include <cmath>

#include "data_object.hpp"

namespace si {

  /** Prototype class for a point in three dimensions
   *
   */
  template<class T>
  class point3d_proto : public T {

  public:

    /// Constructors inherited from the parent class
    using T::T;

    /// X coordinate
    const auto& x() const { return get_field<0>(*this); }
    /// X coordinate
    auto& x() { return get_field<0>(*this); }

    /// Y coordinate
    const auto& y() const { return get_field<1>(*this); }
    /// Y coordinate
    auto& y() { return get_field<1>(*this); }

    /// Z coordinate
    const auto& z() const { return get_field<2>(*this); }
    /// Z coordinate
    auto& z() { return get_field<2>(*this); }

    /// Square of the module
    auto mod2() const {
      return x() * x() + y() * y() + z() * z();
    }

    /// Angle with respect to the X axis
    auto phi() const {
      return std::acos(x() / std::sqrt(x() * x() + y() * y()));
    }

    /// Angle with respect to the Z axis
    auto theta() const {
      return std::acos(z() / std::sqrt(mod2()));
    }
  };

  /// Alias for a point in three dimensions
  template<typename Type>
  using point3d = typename data_object<point3d_proto, Type, Type, Type>::value_type;

  /// Dot product
  template<class T1, class T2>
  auto dot(point3d_proto<T1> const& f, point3d_proto<T2> const& s) {
    return f.x() * s.x() + f.y() * s.y() + f.z() * s.z();
  }

  /// Cross product
  template<class T1, class T2>
  typename common_value_type<T1, T2>::type
  cross(const point3d_proto<T1>& f, const point3d_proto<T2>& s) {
    return {
      f.y() * s.z() - f.z() * s.y(),
      f.z() * s.x() - f.x() * s.z(),
      f.x() * s.y() - f.y() * s.x(),
    };
  }
}

#endif // BASIC_TYPES_HPP
