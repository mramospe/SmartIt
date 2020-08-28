#ifndef SMARTIT_VALUE_HPP
#define SMARTIT_VALUE_HPP

#include <tuple>

#include "utils.hpp"

namespace smit {

  /// Function to access a field of an object based on std::tuple
  template <class Object> auto &get_field(Object &obj);
  /// Function to access a field of an object based on std::tuple
  template <class Object> auto const &get_field(Object const &obj);

  namespace core {

    /**
     * @brief Base class to store a value type
     */
    template <class... Types>
    class __base_value_type : public std::tuple<Types...> {

    public:
      using types = utils::types_holder<Types...>;

      static const auto number_of_fields = sizeof...(Types);

      /// Get an element from a data object
      template <size_t Index> friend auto &get_field(__base_value_type &v) {
        return std::get<Index>(v);
      }

      /// Get an element from a constant data object
      template <size_t Index>
      friend auto const &get_field(__base_value_type const &v) {
        return std::get<Index>(v);
      }

      /// Inherit constructors
      using std::tuple<Types...>::tuple;
    };

    /**
     * @brief Base template for a container type
     */
    template <class Iterator> class __base_container_type {

    public:
      /// Link to the iterator instance
      Iterator &m_iter;

    public:
      /// Get an element from a container type
      template <size_t Index>
      friend auto &get_field(__base_container_type &ct) {
        return *std::get<Index>(ct.m_iter);
      }

      /// Get an element from a container type (constant)
      template <size_t Index>
      friend auto const &get_field(__base_container_type const &ct) {
        return *std::get<Index>(ct.m_iter);
      }

      using types = typename Iterator::types;

      /// Construct the class from the iterator instance
      __base_container_type(Iterator &it) : m_iter{it} {}
    };
  } // namespace core

  /**
   * @brief Template class to generate data object instances.
   *
   * Data objects are build using a prototype class, that must be a template
   * accepting a single argument. This prototype must provide the necessary
   * methods to extract all the fields of the object through the smit::get_field
   * function. Template functions can be later defined, taking two template
   * arguments as an input. The return type can be obtained using the
   * smit::common_value_type_t structure. The types of the fields are determined
   * with the following template arguments, and can be data objects too. An
   * example of a data object would be:
   *
   * \code{.cpp}
   #include "smartit/all.hpp"

   template <class T> class point_2d_proto : public T {

   public:
     using T::T; // Inherit constructors

     auto &x() { return smit::get_field<0>(*this); }
     auto const &x() const { return smit::get_field<0>(*this); }
     auto &y() const { return smit::get_field<1>(*this); }
     auto const &y() { return smit::get_field<1>(*this); }

     // Dot product as a member function
     template<class U>
     auto dot(const point_2d_proto<U> &other) const {
       return this->x() * other.x() + this->y() * other.y();
     }
   };

   // Define our new data object
   template <typename Type>
   using point_2d = smit::data_object<point_2d_proto, Type, Type>;

   // Dot product as an external function
   //template <class T1, class T2>
   //auto dot(const point_2d_proto<T1> &a, const point_2d_proto<T2> &b) {
   //  return a.x() * b.x() + a.y() * b.y();
   //}

   // Unitary vector as an external function
   template <class T> smit::extract_value_type_t<T> unit(const point_2d_proto<T>
   &a) { auto const m = std::sqrt(a.x() * a.x() + a.y() * a.y()); return {a.x()
   / m, a.y() / m};
   }


   int main() {

     point_2d<double> a{1, 1};
     point_2d<double> b{1, 0};

     //auto c = dot(a, b);

     auto c = unit(a);

     return 0;
   }
   * \endcode
   *
   * @see smit::common_value_type
   * @see smit::common_value_type_t
   * @see smit::get_field
   *
   */
  template <template <class> class Prototype, class... Types>
  using data_object = Prototype<core::__base_value_type<Types...>>;

  namespace core {

    template <template <class> class Prototype, class ValueType>
    constexpr auto
    _f_extract_prototype(utils::types_holder<Prototype<ValueType>>) {
      return utils::template_holder<Prototype>{};
    }

    template <class Object> struct extract_prototype {
      template <class ValueType>
      using type = typename decltype(_f_extract_prototype(
          utils::types_holder<Object>{}))::template type<ValueType>;
    };

    template <template <class> class Prototype, class... Types>
    constexpr auto _f_value_type(utils::types_holder<Types...>) {
      return data_object<Prototype, Types...>{};
    }
  } // namespace core

  /**
   * @brief Determine the value type of the template argument for a
   * smit::data_object
   *
   * @see smit::extract_value_type_t
   */
  template <class T> struct extract_value_type {
    using type =
        decltype(core::_f_value_type<core::extract_prototype<T>::template type>(
            typename T::types{}));
  };

  /**
   * @brief Type returned by smit::extract_value_type
   *
   * @see smit::extract_value_type
   */
  template <class T>
  using extract_value_type_t = typename extract_value_type<T>::type;

  /// Declaration of the container type
  template <template <class> class Prototype, class Iterator>
  using __container_type = Prototype<core::__base_container_type<Iterator>>;

  /**
   * @brief Get the value type of several template arguments to a prototype
   * class
   *
   * @see smit::build_value_type_t
   */
  template <template <class> class Prototype, class First, class... Last>
  struct build_value_type {
    using type = typename std::enable_if<
        (utils::is_same_template<
             core::extract_prototype<First>::template type,
             core::extract_prototype<Last>::template type>::value &&
         ...),
        extract_value_type_t<First>>::type;
  };

  /**
   * @brief Type returned by smit::build_value_type
   *
   * @see smit::build_value_type
   */
  template <template <class> class Prototype, class First, class... Last>
  using build_value_type_t =
      typename build_value_type<Prototype, First, Last...>::type;
} // namespace smit

#endif
