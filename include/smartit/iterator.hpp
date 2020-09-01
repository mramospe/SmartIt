#ifndef SMARTIT_ITERATOR_HPP
#define SMARTIT_ITERATOR_HPP

#include <iterator>
#include <tuple>

#include "traits.hpp"
#include "value.hpp"

namespace smit {

  namespace core {

    /**
     * @brief Iterator type
     */
    template <template <class> class Proxy, class Object>
    class __iterator
        : public traits::iterator_base<Proxy, typename Object::types> {

    public:
      /// Base class
      using base_class = traits::iterator_base<Proxy, typename Object::types>;
      using iterator_types = decltype(
          traits::_f_iterator_proxies<Proxy>(typename Object::types{}));
      using types = typename Object::types;

      /// Type of the container
      using value_type = core::__container_type<
          traits::extract_prototype<Object>::template type, iterator_types>;
      using difference_type = ptrdiff_t;
      using pointer_type = value_type *;
      using reference_type = value_type &;
      using iterator_category = std::random_access_iterator_tag;
      /// Number of fields
      static const auto number_of_fields = Object::number_of_fields;

      explicit __iterator() : base_class{}, m_container{*this} {}

      // Similar constructors to those of std::tuple
      template <class... Iterators>
      explicit __iterator(Iterators const &... args)
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
      pointer_type operator->() { return &m_container; }

      /// Access operator (constant)
      pointer_type operator->() const { return &m_container; }

      /// Dereference operator
      reference_type operator*() { return m_container; }

      /// Dereference operator (constant)
      reference_type const operator*() const { return m_container; }

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

      /// Add operator
      __iterator operator+(size_t n) const {
        __iterator copy{*this};
        return copy += n;
      }

      /// Add operator (inplace)
      __iterator &operator+=(size_t n) {
        return this->add_impl(n, std::make_index_sequence<number_of_fields>{});
      }

      /// Subtract operator
      __iterator operator-(size_t n) const {
        __iterator copy{*this};
        return copy += n;
      }

      /// Add operator (inplace)
      __iterator &operator-=(size_t n) {
        return this->sub_impl(n, std::make_index_sequence<number_of_fields>{});
      }

      /// Distance to another iterator
      difference_type operator-(__iterator const &other) const {
        if constexpr (number_of_fields == 0)
          return 0;
        else
          return std::get<0>(*this) - std::get<0>(other);
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

      template <class Iterator> bool operator<(Iterator const &other) {
        if constexpr (number_of_fields == 0)
          return true;
        else
          return std::get<0>(*this) < std::get<0>(other);
      }

      template <class Iterator> bool operator<=(Iterator const &other) {
        if constexpr (number_of_fields == 0)
          return true;
        else
          return std::get<0>(*this) <= std::get<0>(other);
      }

      template <class Iterator> bool operator>(Iterator const &other) {
        if constexpr (number_of_fields == 0)
          return true;
        else
          return std::get<0>(*this) > std::get<0>(other);
      }

      template <class Iterator> bool operator>=(Iterator const &other) {
        if constexpr (number_of_fields == 0)
          return true;
        else
          return std::get<0>(*this) >= std::get<0>(other);
      }

    protected:
      /// Container instance
      value_type m_container;

    private:
      /// Implementation of the add function
      template <size_t... I>
      inline __iterator &add_impl(size_t n, std::index_sequence<I...>) {
        (std::next(std::get<I>(*this), n), ...);
        return *this;
      }

      /// Implementation of the subtract function
      template <size_t... I>
      inline __iterator &sub_impl(size_t n, std::index_sequence<I...>) {
        (std::prev(std::get<I>(*this), n), ...);
        return *this;
      }

      /// Implementation of the function to access the next element
      template <size_t... I>
      constexpr void next_impl(std::index_sequence<I...>) {
        (++std::get<I>(*this), ...);
      }

      /// Implementation of the function to access the previous element
      template <size_t... I>
      constexpr void prev_impl(std::index_sequence<I...>) {
        (--std::get<I>(*this), ...);
      }
    };

    /**
     * @brief Constat iterator type
     */
    template <template <class> class Proxy, class Object>
    class __const_iterator
        : public traits::const_iterator_base<Proxy, typename Object::types> {

    public:
      /// Base class
      using base_class =
          traits::const_iterator_base<Proxy, typename Object::types>;
      using const_iterator_types = decltype(
          traits::_f_const_iterator_proxies<Proxy>(typename Object::types{}));
      using types = typename Object::types;
      /// Type of the container
      using value_type = core::__container_type<
          traits::extract_prototype<Object>::template type,
          const_iterator_types>;
      using difference_type = ptrdiff_t;
      using pointer_type = value_type const *;
      using reference_type = value_type const &;
      using iterator_category = std::random_access_iterator_tag;
      /// Number of fields
      static const auto number_of_fields = Object::number_of_fields;

      explicit __const_iterator() : base_class{}, m_container{*this} {}

      // Similar constructors to those of std::tuple
      template <class... Iterators>
      explicit __const_iterator(Iterators const &... args)
          : base_class{args...}, m_container{*this} {}

      /// Constructors for cases with no arguments (default) and with
      /// iterators
      template <class... UIterators>
      __const_iterator(UIterators &&... args)
          : base_class{args...}, m_container{*this} {}

      __const_iterator(const __const_iterator &other)
          : base_class(other), m_container{*this} {}
      __const_iterator(__const_iterator &&other)
          : base_class(other), m_container{*this} {}

      __const_iterator &operator=(const __const_iterator &other) {
        base_class::operator=(other);
        return *this;
      }

      __const_iterator &operator=(__const_iterator &&other) {
        base_class::operator=(other);
        return *this;
      }

      /// Dereference operator
      reference_type operator*() const { return m_container; }

      /// Access operator
      pointer_type operator->() const { return &m_container; }

      /// Increment operator
      __const_iterator &operator++() {

        this->next_impl(std::make_index_sequence<number_of_fields>{});

        return *this;
      }

      /// Increment operator (copy)
      __const_iterator operator++(int) {

        __const_iterator copy{*this};

        this->next_impl(std::make_index_sequence<number_of_fields>{});

        return copy;
      }

      /// Decrement operator
      __const_iterator &operator--() {

        this->prev_impl(std::make_index_sequence<number_of_fields>{});

        return *this;
      }

      /// Decrement operator (copy)
      __const_iterator operator--(int) {

        __const_iterator copy{*this};

        this->prev_impl(std::make_index_sequence<number_of_fields>{});

        return copy;
      }

      /// Add operator
      __const_iterator operator+(size_t n) const {
        __const_iterator copy{*this};
        return copy += n;
      }

      /// Add operator (inplace)
      __const_iterator &operator+=(size_t n) {
        return this->add_impl(n, std::make_index_sequence<number_of_fields>{});
      }

      /// Subtract operator
      __const_iterator operator-(size_t n) const {
        __const_iterator copy{*this};
        return copy += n;
      }

      /// Distance to another iterator
      difference_type operator-(__const_iterator const &other) const {
        if constexpr (number_of_fields == 0)
          return 0;
        else
          return std::get<0>(*this) - std::get<0>(other);
      }

      /// Add operator (inplace)
      __const_iterator &operator-=(size_t n) {
        return this->sub_impl(n, std::make_index_sequence<number_of_fields>{});
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

      template <class Iterator> bool operator<(Iterator const &other) {
        if constexpr (number_of_fields == 0)
          return true;
        else
          return std::get<0>(*this) < std::get<0>(other);
      }

      template <class Iterator> bool operator<=(Iterator const &other) {
        if constexpr (number_of_fields == 0)
          return true;
        else
          return std::get<0>(*this) <= std::get<0>(other);
      }

      template <class Iterator> bool operator>(Iterator const &other) {
        if constexpr (number_of_fields == 0)
          return true;
        else
          return std::get<0>(*this) > std::get<0>(other);
      }

      template <class Iterator> bool operator>=(Iterator const &other) {
        if constexpr (number_of_fields == 0)
          return true;
        else
          return std::get<0>(*this) >= std::get<0>(other);
      }

    protected:
      /// Container instance
      value_type m_container;

    private:
      /// Implementation of the add function
      template <size_t... I>
      inline __const_iterator &add_impl(size_t n, std::index_sequence<I...>) {
        (std::next(std::get<I>(*this), n), ...);
        return *this;
      }

      /// Implementation of the subtract function
      template <size_t... I>
      inline __const_iterator &sub_impl(size_t n, std::index_sequence<I...>) {
        (std::prev(std::get<I>(*this), n), ...);
        return *this;
      }

      /// Implementation of the function to access the next element
      template <size_t... I>
      constexpr void next_impl(std::index_sequence<I...>) {

        (++std::get<I>(*this), ...);
      }

      /// Implementation of the function to access the previous element
      template <size_t... I>
      constexpr void prev_impl(std::index_sequence<I...>) {

        (--std::get<I>(*this), ...);
      }
    };
  } // namespace core
} // namespace smit
#endif
