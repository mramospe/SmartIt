#ifndef SMARTIT_TYPES_HPP
#define SMARTIT_TYPES_HPP

#include <cmath>

#include "data_object.hpp"

namespace smit {

  /** Prototype class for a point in three dimensions
   *
   */
  template <class T> class point_3d_proto : public T {

  public:
    /// Constructors inherited from the parent class
    using T::T;

    /// X coordinate
    const auto &x() const { return get_field<0>(*this); }
    /// X coordinate
    auto &x() { return get_field<0>(*this); }

    /// Y coordinate
    auto const &y() const { return get_field<1>(*this); }
    /// Y coordinate
    auto &y() { return get_field<1>(*this); }

    /// Z coordinate
    auto const &z() const { return get_field<2>(*this); }
    /// Z coordinate
    auto &z() { return get_field<2>(*this); }

    /// Square of the module
    auto mod2() const { return x() * x() + y() * y() + z() * z(); }

    /// Angle with respect to the X axis
    auto phi() const {
      return std::acos(x() / std::sqrt(x() * x() + y() * y()));
    }

    /// Angle with respect to the Z axis
    auto theta() const { return std::acos(z() / std::sqrt(mod2())); }
  };

  /// Alias for a point in three dimensions
  template <typename Type>
  using point_3d =
      typename data_object<point_3d_proto, Type, Type, Type>::value_type;

  /// Dot product
  template <class T1, class T2>
  auto dot(point_3d_proto<T1> const &f, point_3d_proto<T2> const &s) {
    return f.x() * s.x() + f.y() * s.y() + f.z() * s.z();
  }

  /// Cross product
  template <class T1, class T2>
  typename common_value_type<T1, T2>::type cross(const point_3d_proto<T1> &f,
                                                 const point_3d_proto<T2> &s) {
    return {
        f.y() * s.z() - f.z() * s.y(),
        f.z() * s.x() - f.x() * s.z(),
        f.x() * s.y() - f.y() * s.x(),
    };
  }

  /** Prototype class for a point and a vector in three dimensions
   *
   */
  template <class T> class point_with_vector_3d_proto : public T {

  public:
    /// Constructors inherited from the parent class
    using T::T;

    /// Access the point
    const auto &point() const { return get_field<0>(*this); }
    /// Access the point
    auto &point() { return get_field<0>(*this); }

    /// Access the vector
    const auto &vector() const { return get_field<1>(*this); }
    /// Access the vector
    auto &vector() { return get_field<1>(*this); }
  };

  /// Alias for a point and a vector in three dimensions
  template <typename Type>
  using point_with_vector_3d =
      typename data_object<point_with_vector_3d_proto, point_3d<Type>,
                           point_3d<Type>>::value_type;
} // namespace smit

#endif // SMARTIT_TYPES_HPP
