#include "smartit/all.hpp"

template <class T> class point_2d_proto : public T {

public:
  using T::T; // Inherit constructors

  auto &x() { return smit::get_field<0>(*this); }
  auto const &x() const { return smit::get_field_const<0>(*this); }
  auto &y() { return smit::get_field<1>(*this); }
  auto const &y() const { return smit::get_field_const<1>(*this); }

  // Dot product as a member function
  template <class U> auto dot(const point_2d_proto<U> &other) const {
    return this->x() * other.x() + this->y() * other.y();
  }
};

// Define our new data object
template <typename Type>
using point_2d = smit::data_object<point_2d_proto, Type, Type>;

// Dot product as an external function
template <class T1, class T2>
auto dot(const point_2d_proto<T1> &a, const point_2d_proto<T2> &b) {
  return a.x() * b.x() + a.y() * b.y();
}

// Unitary vector as an external function
template <class T> smit::extract_value_type_t<T> unit(const T &a) {
  auto const m = std::sqrt(a.x() * a.x() + a.y() * a.y());
  return {a.x() / m, a.y() / m};
}

int main() {

  point_2d<double> a{1, 1};
  point_2d<double> b{1, 0};

  dot(a, b);

  unit(a);

  return 0;
}
