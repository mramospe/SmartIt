#ifndef DATA_OBJECT_HPP
#define DATA_OBJECT_HPP

#include <array>
#include <vector>
#include <tuple>
#include <type_traits>

#include "container_types.hpp"
#include "utils.hpp"

namespace smit {

  // Forward declaration of access functions
  template<class Object>
  auto& get_field(Object &obj);

  template<class Object>
  auto const& get_field(Object const &obj);

  /** Data object
   *
   */
  template <template <class> class Prototype, typename ... Types>
  class data_object {

  public:

    using data_object_type = data_object<Prototype, Types ...>;
    using types = core::types_holder<Types ...>;
    static size_t const number_of_fields = sizeof ... (Types);

    /** Iterator type
     *
     */
    template<class ... Iterators>
    class iterator : public std::tuple<Iterators ...> {

    public:

      /// Iterator type
      using iterator_type = iterator<Iterators ...>;
      /// Tuple type
      using tuple_type = std::tuple<Iterators ...>;
      /// Data object type
      using data_object_type = data_object::data_object_type;
      /// Types of the fields
      using types = data_object::types;
      /// Number of fields
      static const auto number_of_fields = data_object::number_of_fields;
      
      /** Container type
       *
       */
      class __base_container_type {

      public:

	/// Link to the iterator instance
	iterator_type &m_iter;

      public:

	/// Data object type
	using data_object_type = data_object::data_object_type;
	/// Types of the fields
	using types = data_object::types;
	/// Number of fields
	static const auto number_of_fields = data_object::number_of_fields;

	/// Construct the class from the iterator instance
	__base_container_type(iterator_type &it) : m_iter{it} { }

	/// Get an element from a container type
	template<size_t Index> friend
	auto& get_field(__base_container_type& ct) {
	  return *std::get<Index>(ct.m_iter);
	}

	/// Get an element from a container type (constant)
	template<size_t Index> friend
	auto const& get_field(__base_container_type const& ct) {
	  return *std::get<Index>(ct.m_iter);
	}
      };

      /// Declare the container type
      using container_type = Prototype<__base_container_type>;

      // Similar constructors to those of std::tuple
      explicit iterator( const Iterators& ... args ) : tuple_type{args ...}, m_container{*this} { }

      /// Constructors for cases with no arguments (default) and with iterators
      template<class ... UIterators>
      iterator( UIterators&& ... args ) : tuple_type{args ...}, m_container{*this} { }

      iterator(const iterator& other) : tuple_type(other), m_container{*this} { }
      iterator(iterator&& other) : tuple_type(other), m_container{*this} { }

      iterator& operator=(const iterator& other) {
	tuple_type::operator=(other);
	return *this;
      }

      iterator& operator=(iterator&& other) {
	tuple_type::operator=(other);
	return *this;
      }

      /// Access operator
      container_type* operator->() {

	return &m_container;
      }

      /// Dereference operator
      container_type& operator*() {

	return m_container;
      }

      /// Dereference operator (constant)
      container_type const& operator*() const {

	return m_container;
      }

      /// Access operator
      container_type const* operator->() const {

	return &m_container;
      }

      /// Increment operator
      iterator<Iterators ...>& operator++() {

	this->next_impl(std::make_index_sequence<sizeof ... (Iterators)>{});

	return *this;
      }

      /// Increment operator (copy)
      iterator<Iterators ...> operator++(int) {

	iterator<Iterators ...> copy{*this};

	this->next_impl(std::make_index_sequence<sizeof ... (Iterators)>{});

	return copy;
      }

      /// Decrement operator
      iterator<Iterators ...>& operator--() {

	this->prev_impl(std::make_index_sequence<sizeof ... (Iterators)>{});

	return *this;
      }

      /// Decrement operator (copy)
      iterator<Iterators ...> operator--(int) {

	iterator<Iterators ...> copy{*this};

	this->prev_impl(std::make_index_sequence<sizeof ... (Iterators)>{});

	return copy;
      }

      /// Comparison operator (equality)
      template<class Iterator>
      typename std::enable_if<(number_of_fields == Iterator::number_of_fields), bool>::type
      operator==(Iterator const& other) const {

	if constexpr (number_of_fields == 0)
	   return true;
	else
	  return std::get<0>(*this) == std::get<0>(other);
      }

      /// Comparison operator (inequality)
      template<class Iterator>
      typename std::enable_if<(number_of_fields == Iterator::number_of_fields), bool>::type
      operator!=(Iterator const& other) const {

	if constexpr (number_of_fields == 0)
	   return true;
	else
	  return std::get<0>(*this) != std::get<0>(other);
      }

    protected:

      /// Container instance
      container_type m_container;

    private:

      /// Implementation of the function to access the next element
      template<size_t ... I>
      inline void next_impl(std::index_sequence<I ...>) {

	(++std::get<I>(*this), ...);
      }

      /// Implementation of the function to access the previous element
      template<size_t ... I>
      inline void prev_impl(std::index_sequence<I ...>) {

	(--std::get<I>(*this), ...);
      }
    };

    /** Value type
     *
     */
    class __base_value_type : public std::tuple<Types ...> {

    public:

      /// Data object type
      using data_object_type = data_object::data_object_type;
      /// Types of the fields
      using types = data_object::types;
      /// Number of fields
      static const auto number_of_fields = data_object::number_of_fields;

      // Container types
      template<size_t N>
      using array_type = std::tuple<typename core::array_t<Types, N>::type ...>;
      using vector_type = std::tuple<typename core::vector_t<Types>::type ...>;

      // Iterator types
      template<size_t N>
      using array_iterator_type = iterator<typename core::array_t<Types, N>::iterator ...>;
      using vector_iterator_type = iterator<typename core::vector_t<Types>::iterator ...>;

      /// Inherit constructors
      using std::tuple<Types ...>::tuple;

      /// Get an element from a data object
      template<size_t Index> friend
      auto& get_field(__base_value_type& v) {
	return std::get<Index>(v);
      }

      /// Get an element from a constant data object
      template<size_t Index> friend
      auto const& get_field(__base_value_type const& v) {
	return std::get<Index>(v);
      }
    };

    /// Declare the value type
    using value_type = Prototype<__base_value_type>;
  };

  /// Determine the value type of two or more template arguments to a prototype class
  template<class First, class ... Last>
  struct common_value_type {
    using type = typename std::enable_if<
      (std::is_same<typename First::data_object_type::value_type,
       typename Last::data_object_type::value_type>::value && ...),
      typename First::data_object_type::value_type>::type;
  };
}

#endif // DATA_OBJECT_HPP
