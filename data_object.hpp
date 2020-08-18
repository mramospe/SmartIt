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

    static const size_t number_of_fields = sizeof ... (Types);

    /** Container type
     *
     */
    /*
    class container_type : public Descriptor {

      container_type() {

      }
    };
    */


    /** Iterator class
     *
     */
    template<class ... Iterators>
    class iterator : public Descriptor, std::tuple<Iterators ...> {

    public:

      // Inherit constructors
      using std::tuple<Iterators ...>::tuple;

      using base_data_type = std::tuple<Iterators ...>;

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
      operator==(const Iterator& other) const {

	if constexpr (number_of_fields == 0)
	   return true;
	else
	  return std::get<0>(*static_cast<base_data_type const*>(this)) == std::get<0>(*static_cast<typename Iterator::base_data_type const*>(&other));
      }

      /// Comparison operator (inequality)
      template<class Iterator>
      typename std::enable_if<(number_of_fields == Iterator::number_of_fields), bool>::type
      operator!=(const Iterator& other) const {

	if constexpr (number_of_fields == 0)
	   return true;
	else
	  return std::get<0>(*static_cast<base_data_type const*>(this)) != std::get<0>(*static_cast<typename Iterator::base_data_type const*>(&other));
      }
      
    private:

      /// Implementation of the function to access the next element
      template<size_t ... I>
      void next_impl(std::index_sequence<I ...>) {

	(++std::get<I>(*static_cast<base_data_type*>(this)), ...);
      }

      /// Implementation of the function to access the previous element
      template<size_t ... I>
      void prev_impl(std::index_sequence<I ...>) {

	(--std::get<I>(*static_cast<base_data_type*>(this)), ...);
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
  const typename tuple_element<Index, Types ...>::type& get_field(const data_object<Descriptor, Types ...>& t ) {
    return std::get<Index>(t);
  }

  /// Get an element from an iterator
  template<size_t Index, typename Descriptor, typename ... Types, typename ... Iterators>
  typename tuple_element<Index, Types ...>::type&
  get_field(typename data_object<Descriptor, Types ...>::template iterator<Iterators ...>& it ) {
    return *std::get<Index>(*static_cast<typename data_object<Descriptor, Types ...>::template iterator<Iterators ...>*>(it));
  }
}

#endif // DATA_OBJECT_HPP
