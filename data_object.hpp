#ifndef DATA_OBJECT_HPP
#define DATA_OBJECT_HPP

#include <array>
#include <vector>
#include <tuple>

#include "utils.hpp"

namespace si {

  template <typename Descriptor, typename ... Types>
  class data_object : public std::tuple<Types ...> {

  public:
    
    using std::tuple<Types ...>::tuple;
    using types = types_holder<Types ...>;

    static size_t const number_of_fields = sizeof ... (Types);

    /** Iterator class
     *
     */
    template<class ... Iterators>
    class iterator : public std::tuple<Iterators ...> {

    public:

      using class_type = iterator<Iterators...>;
      using base_data_type = std::tuple<Iterators ...>;
      
      /** Container type
       *
       */
      class container_type : public Descriptor {

      private:

	/// Link to the iterator instance
	class_type &iter;

      public:

	/// Construct the class from the iterator instance
	container_type(class_type &it) : iter{it} { };

	/// Get an element from a container type
	template<size_t Index> friend
	typename tuple_element<Index, Types ...>::type& get_field(container_type& ct ) {
	  return *std::get<Index>(ct.iter);
	}

      };

      // Similar constructors to those of std::tuple
      template<class ... Args>
      iterator( Args ... args ) : m_container{*this}, std::tuple<Iterators ...>(args ...) { };

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

    // Container types
    template<size_t N>
    using array_type = std::tuple<std::array<Types, N> ...>;
    using vector_type = std::tuple<std::vector<Types> ...>;

    // Iterator types
    template<size_t N>
    using array_iterator_type = iterator<typename std::array<Types, N>::iterator ...>;
    using vector_iterator_type = iterator<typename std::vector<Types>::iterator ...>;
  };

  /// Get an element from a data object
  template<size_t Index, typename Descriptor, typename ... Types>
  typename tuple_element<Index, Types ...>::type& get_field(data_object<Descriptor, Types ...>& t ) {
    return std::get<Index>(t);
  }

  /// Get an element from a constant data object
  template<size_t Index, typename Descriptor, typename ... Types>
  typename tuple_element<Index, Types ...>::type const& get_field(data_object<Descriptor, Types ...> const& t ) {
    return std::get<Index>(t);
  }

}

#endif // DATA_OBJECT_HPP
