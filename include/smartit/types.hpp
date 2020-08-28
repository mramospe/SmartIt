#ifndef SMARTIT_TYPES_HPP
#define SMARTIT_TYPES_HPP

#include <cmath>

#include "value.hpp"

namespace smit {

  /**
   * @brief Prototype class for a point in three dimensions
   *
   * @see smit::point_3d
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

  /**
   * @brief Alias for a point in three dimensions
   *
   * @see smit::point_3d_proto
   */
  template <typename Type>
  using point_3d = data_object<point_3d_proto, Type, Type, Type>;

  /// Dot product
  template <class T1, class T2>
  auto dot(point_3d_proto<T1> const &f, point_3d_proto<T2> const &s) {
    return f.x() * s.x() + f.y() * s.y() + f.z() * s.z();
  }

  /// Cross product
  template <class T1, class T2>
  build_value_type_t<point_3d_proto, T1, T2> cross(const T1 &f, const T2 &s) {
    return {
        f.y() * s.z() - f.z() * s.y(),
        f.z() * s.x() - f.x() * s.z(),
        f.x() * s.y() - f.y() * s.x(),
    };
  }

  /**
   * @brief Prototype class for a point and a vector in three dimensions
   *
   * @see smit::point_with_vector_3d
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

  /**
   * @brief Alias for a point and a vector in three dimensions
   *
   * @see smit::point_with_vector_3d_proto
   */
  template <typename Type>
  using point_with_vector_3d =
      data_object<point_with_vector_3d_proto, point_3d<Type>, point_3d<Type>>;
} // namespace smit

#endif // SMARTIT_TYPES_HPP
