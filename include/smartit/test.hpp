#ifndef SMARTIT_TEST_HPP
#define SMARTIT_TEST_HPP

#include <iostream>
#include <utility>
#include <vector>

#include "value.hpp"

namespace smit {

  namespace test {

    /// Get the combined status for different collectors
    template <class... Status> bool combined_status(Status... s) {
      return (s || ...);
    }

    /**
     * @brief Class to collect test results and display them on error
     */
    class test_collector {

    public:
      /// Construct the class from the collector's name
      test_collector(const char *name) : m_name{name} {};
      /// On destruction, the collector displays the errors
      ~test_collector() {

        if (status()) {
          std::cerr << "Errors detected for collector \"" << m_name
                    << "\":" << std::endl;
          for (const auto &p : m_errors)
            std::cerr << p.first << ": " << p.second << std::endl;
        }
      }

      /// Status code of the collector
      bool status() const { return m_errors.size() != 0; }

      /// Call a function and check whether it throws and exception or not
      template <class Function, class... Args>
      void test_scope_function(const char *f, Function F,
                               const Args &... args) {

        std::cout << "Within the context of test collector \"" << m_name << "\""
                  << std::endl;

        try {
          F(args...);
          std::cout << "\"" << f << "\" (success)" << std::endl;
        } catch (const char *e) {
          std::cerr << "\"" << f << "\" (error)" << std::endl;
          m_errors.emplace_back(f, e);
        }
      }

    protected:
      const char *m_name; //!< Name of the collector

      std::vector<std::pair<const char *, const char *>>
          m_errors; //!< Container for the error messages
    };

// Definition of the "assert" function for the tests
#define SMARTIT_TEST_ASSERT(F, COMP, ARGS...)                                  \
  {                                                                            \
    if (F(ARGS) != COMP)                                                       \
      throw #F;                                                                \
  }

// Associate a function call to a given scope (running it)
#define SMARTIT_TEST_SCOPE_FUNCTION(coll, F, ...)                              \
  coll.test_scope_function(#F, F, ##__VA_ARGS__);

    /**
     * @brief Simple data object prototype with a single value
     */
    template <class T> class single_value_proto : public T {

    public:
      using T::T;

      /// Access the value
      auto &value() { return get_field<0>(*this); }
      /// Access the value
      auto const &value() const { return get_field_const<0>(*this); }
    };

    /// Simple data object with a single value
    template <typename Type>
    using single_value = data_object<single_value_proto, Type>;

    /**
     * @brief Data object prototype composed by another two
     */
    template <class T> class two_single_values_proto : public T {

    public:
      using T::T;

      /// Access the first element
      auto &first() { return get_field<0>(*this); }
      /// Access the first element
      auto const &first() const { return get_field_const<0>(*this); }

      /// Access the second element
      auto &second() { return get_field<1>(*this); }
      /// Access the second element
      auto const &second() const { return get_field_const<1>(*this); }
    };

    /// Simple data object with two single values
    template <typename Type>
    using two_single_values =
        data_object<two_single_values_proto, single_value<Type>,
                    single_value<Type>>;
  } // namespace test
} // namespace smit

#endif // SMARTIT_TEST_HPP
