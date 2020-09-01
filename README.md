# SmartIt

[![Build Status](https://travis-ci.org/mramospe/SmartIt.svg?branch=master)](https://travis-ci.org/mramospe/SmartIt)

[![Documentation](https://img.shields.io/badge/documentation-link-blue.svg)](https://mramospe.github.io/SmartIt)

This package provides a way to define data objects based on a C++ tuple structure, so Struct-Of-Arrays (SOA) instances can be easily constructed.
Users only need to declare a prototype class and a set of functions and member functions acting on it.
SmartIt will declare the apropriate value and container types, in such a way that operations between vector or array iterators with value types can be done without needing to duplicate code.

```cpp

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

     auto d = dot(a, b);

     auto u = unit(a);

     return 0;
    }
```

The package is provided as a header-only C++ library, and can be installed using [CMake](https://cmake.org).
To install it simply do:

```bash
   git clone git@github.com:mramospe/SmartIt.git smartit
   mkdir smartit-build
   cd smartit-build
   cmake .. -DINSTALL_TESTS=ON
   make
```

If you just want to use the headers without installation, add *smartit/include* path to your set of include directories.
