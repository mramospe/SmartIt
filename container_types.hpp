#include <array>
#include <vector>

#include "array.hpp"
#include "vector.hpp"

namespace si {

  namespace core {

    template<class Type, size_t N, class Enable=void>
    struct array_t { }; // primary template

    template<class Type, size_t N>
    struct array_t<Type, N, typename std::enable_if_t<std::is_arithmetic<Type>::value>::type> {
      using type = std::array<Type, N>;
      using iterator = typename type::iterator;
    };

    template<class Type, size_t N>
    struct array_t<Type, N, typename std::enable_if_t<!std::is_arithmetic<Type>::value>::type> {
      using type = std::array<Type, N>;
      using iterator = typename type::iterator;
    };

    template<class Type, class Enable=void>
    struct vector_t { }; // primary template

    template<class Type>
    struct vector_t<Type, typename std::enable_if<std::is_arithmetic<Type>::value>::type> {
      using type = std::vector<Type>;   
      using iterator = typename type::iterator;
    };

    template<class Type>
    struct vector_t<Type, typename std::enable_if_t<!std::is_arithmetic<Type>::value>::type> {
      using type = vector<Type>;
      using iterator = typename type::iterator;
    };

  }
}
