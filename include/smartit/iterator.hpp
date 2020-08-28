#ifndef SMARTIT_ITERATOR_HPP
#define SMARTIT_ITERATOR_HPP

#include <tuple>

#include "value.hpp"

namespace smit {

  namespace core {
    template <template <class> class Proxy, class... Types>
    constexpr auto _f_iterator_base_type(types_holder<Types...>) {
      return std::tuple<typename Proxy<Types>::iterator...>{};
    }

    template <template <class> class Proxy, class H>
    struct __iterator_base_type {
      using type = decltype(_f_iterator_base_type<Proxy>(H{}));
    };

    template <template <class> class Proxy, class H>
    using iterator_base_type_t = typename __iterator_base_type<Proxy, H>::type;
  } // namespace core

  /** Iterator type
   *
   */
  template <template <class> class Proxy, class Object>
  class __iterator
      : public core::iterator_base_type_t<Proxy, typename Object::types> {

  public:
    /// Tuple type
    using base_class =
        core::iterator_base_type_t<Proxy, typename Object::types>;
    using container_type =
        __container_type<Object::template prototype, __iterator>;
    using value_type = Object;
    using types = typename Object::types;
    /// Number of fields
    static const auto number_of_fields = Object::number_of_fields;

    explicit __iterator() : base_class{}, m_container{*this} { }

    // Similar constructors to those of std::tuple
    template <class... Iterators>
    explicit __iterator(Iterators const&... args)
        : base_class{args...}, m_container{*this} {}

    /// Constructors for cases with no arguments (default) and with
    /// iterators
    template <class... UIterators>
    __iterator(UIterators &&... args)
        : base_class{args...}, m_container{*this} {}

    __iterator(const __iterator &other)
        : base_class(other), m_container{*this} {}
    __iterator(__iterator &&other) : base_class(other), m_container{*this} {}

    __iterator &operator=(const __iterator &other) {
      base_class::operator=(other);
      return *this;
    }

    __iterator &operator=(__iterator &&other) {
      base_class::operator=(other);
      return *this;
    }

    /// Access operator
    container_type *operator->() { return &m_container; }

    /// Dereference operator
    container_type &operator*() { return m_container; }

    /// Dereference operator (constant)
    container_type const &operator*() const { return m_container; }

    /// Access operator
    container_type const *operator->() const { return &m_container; }

    /// Increment operator
    __iterator &operator++() {

      this->next_impl(std::make_index_sequence<number_of_fields>{});

      return *this;
    }

    /// Increment operator (copy)
    __iterator operator++(int) {

      __iterator copy{*this};

      this->next_impl(std::make_index_sequence<number_of_fields>{});

      return copy;
    }

    /// Decrement operator
    __iterator &operator--() {

      this->prev_impl(std::make_index_sequence<number_of_fields>{});

      return *this;
    }

    /// Decrement operator (copy)
    __iterator operator--(int) {

      __iterator copy{*this};

      this->prev_impl(std::make_index_sequence<number_of_fields>{});

      return copy;
    }

    /// Comparison operator (equality)
    template <class Iterator>
    typename std::enable_if<(number_of_fields == Iterator::number_of_fields),
                            bool>::type
    operator==(Iterator const &other) const {

      if constexpr (number_of_fields == 0)
        return true;
      else
        return std::get<0>(*this) == std::get<0>(other);
    }

    /// Comparison operator (inequality)
    template <class Iterator>
    typename std::enable_if<(number_of_fields == Iterator::number_of_fields),
                            bool>::type
    operator!=(Iterator const &other) const {

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
    template <size_t... I> inline void next_impl(std::index_sequence<I...>) {

      (++std::get<I>(*this), ...);
    }

    /// Implementation of the function to access the previous element
    template <size_t... I> inline void prev_impl(std::index_sequence<I...>) {

      (--std::get<I>(*this), ...);
    }
  };
} // namespace smit

#endif
