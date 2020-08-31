#ifndef SMARTIT_VALUE_HPP
#define SMARTIT_VALUE_HPP

#include <tuple>

#include "traits.hpp"
#include "utils.hpp"

namespace smit {

  namespace core {

    /**
     * @brief Base class to store a value type
     */
    template <class... Types>
    class __base_value_type : public std::tuple<Types...> {

    public:
      using types = utils::types_holder<Types...>;

      static const auto number_of_fields = sizeof...(Types);

      /// Inherit constructors
      using std::tuple<Types...>::tuple;
    };

    /**
     * @brief Base template for a container type
     */
    template <class... Iterators>
    class __base_container_type : public std::tuple<Iterators &...> {

    public:
      using base_class = std::tuple<Iterators &...>;

      /// Construct the class from the iterator instance
      __base_container_type(std::tuple<Iterators...> &it)
          : base_class{std::move(utils::vtuple_to_rtuple(it))} {}
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

    template <template <class> class Prototype, class... Iterators>
    constexpr auto _f_container_type(utils::types_holder<Iterators...>) {
      return utils::type_wrapper<
          Prototype<__base_container_type<Iterators...>>>{};
    }

    /// Declaration of the container type
    template <template <class> class Prototype, class IterTypes>
    using __container_type =
        typename decltype(_f_container_type<Prototype>(IterTypes{}))::type;

    template <template <class> class Prototype, class... Types>
    constexpr auto _f_value_type(utils::types_holder<Types...>) {
      return data_object<Prototype, Types...>{};
    }
  } // namespace core

  /// Function to access a field of an object based on std::tuple
  template <size_t I, class... Types>
  inline utils::tuple_element_t<I, Types...> &
  get_field(core::__base_value_type<Types...> &obj) {
    return std::get<I>(obj);
  }

  /// Function to access a field of an object based on std::tuple
  template <size_t I, class... Types>
  inline utils::tuple_element_t<I, Types...> const &
  get_field_const(core::__base_value_type<Types...> const &obj) {
    return std::get<I>(obj);
  }

  /// Function to access a field of an object based on std::tuple
  template <size_t I, class... Iterators>
  inline typename utils::tuple_element_t<I, Iterators...>::value_type &
  get_field(core::__base_container_type<Iterators...> &obj) {
    return *std::get<I>(obj);
  }

  /// Function to access a field of an object based on std::tuple
  template <size_t I, class... Iterators>
  inline typename utils::tuple_element_t<I, Iterators...>::value_type const &
  get_field_const(core::__base_container_type<Iterators...> const &obj) {
    return *std::get<I>(obj);
  }

  /**
   * @brief Determine the value type of the template argument for a
   * smit::data_object
   *
   * @see smit::extract_value_type_t
   */
  template <class T> struct extract_value_type {
    using type = decltype(
        core::_f_value_type<traits::extract_prototype<T>::template type>(
            typename T::types{}));
  };

  /**
   * @brief Type returned by smit::extract_value_type
   *
   * @see smit::extract_value_type
   */
  template <class T>
  using extract_value_type_t = typename extract_value_type<T>::type;

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
             traits::extract_prototype<First>::template type,
             traits::extract_prototype<Last>::template type>::value &&
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
