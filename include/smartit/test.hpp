#ifndef SMARTIT_TEST_HPP
#define SMARTIT_TEST_HPP

#include <iostream>
#include <utility>
#include <vector>

namespace smit {

  namespace core {

    class test_collector {

    public:

      test_collector(const char* name) : m_name{name} { };

      ~test_collector() {

        if (status()) {
          std::cerr << "Errors detected for collector \"" << m_name << "\":" << std::endl;
          for ( const auto &p : m_errors)
            std::cerr << p.first << ": " << p.second << std::endl;
        }
      }

      bool status() const { return m_errors.size() != 0; }

      template<class Function, class ... Args>
      void test_scope_function(const char* f, Function F, const Args& ... args) {

        try {
          F(args ...);
        }
        catch (const char* e) {
          m_errors.emplace_back(f, e);
        }
      }

    protected:

      const char* m_name;

      std::vector<std::pair<const char*, const char*>> m_errors;
    };

    #define SMARTIT_TEST_ASSERT(F, COMP, ARGS...) \
    {\
        if (F(ARGS) != COMP)\
          throw #F;\
    }\

    #define SMARTIT_TEST_SCOPE_FUNCTION(coll, F, ...) \
    coll.test_scope_function(#F, F, ## __VA_ARGS__);
  }
}

#endif // SMARTIT_TEST_HPP
