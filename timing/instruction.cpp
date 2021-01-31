#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "smartit/all.hpp"

#define DEFAULT_ENTRIES 1000
#define DEFAULT_REPETITIONS 10000

namespace options {
  /// Definition of the namespace to use as backend
  enum Namespace { STD, SMIT };
  /// Actions to do on the containers
  enum Action { iterate, reset };

  /// Return a namespace from a string
  Namespace process_namespace(std::string const &n) {
    if (n == "std")
      return STD;
    else if (n == "smit")
      return SMIT;
    else
      throw std::runtime_error(std::string{"Unknown namespace: "} + n);
  }

  /// Return an action from a string
  Action process_action(std::string const &a) {
    if (a == "iterate")
      return iterate;
    else if (a == "reset")
      return reset;
    else
      throw std::runtime_error(std::string{"Unknown action: "} + a);
  }

  /// Return a namespace from a string
} // namespace options

/// Action of iterating over the container
template <options::Namespace N> struct iterate {};
template <> struct iterate<options::STD> {
  template <class Iterator> static void call(Iterator &it) { ++it; }
};
template <> struct iterate<options::SMIT> {
  template <class Iterator> static void call(Iterator &it) { ++it; }
};

/// Action of setting values on the container
template <options::Namespace N> struct reset {};
template <> struct reset<options::STD> {
  template <class Iterator> static void call(Iterator &it) { *it++ = *it; }
};
template <> struct reset<options::SMIT> {
  template <class Iterator> static void call(Iterator &it) { *it++ = *it; }
};

/// Call the action
template <class Vector, class Action> void run(size_t N, size_t R) {
  for (std::size_t r = 0; r < R; ++r) {
    Vector v(1000);
    auto it = v.begin();
    for (std::size_t i = 0; i < N; ++i)
      Action::call(it);
  }
}

/// Switch by namespace
template <template <options::Namespace> class Action>
void namespace_switcher(options::Namespace s, std::size_t n, std::size_t r) {
  switch (s) {
  case options::STD:
    run<std::vector<smit::point_3d<float>>, Action<options::STD>>(n, r);
    return;
  case options::SMIT:
    run<smit::vector<smit::point_3d<float>>, Action<options::SMIT>>(n, r);
    return;
  default:
    throw std::runtime_error("Internal error");
  }
}

/// Switch by user action
void action_switcher(options::Action a, options::Namespace s, std::size_t n,
                     std::size_t r) {
  switch (a) {
  case options::iterate:
    namespace_switcher<iterate>(s, n, r);
    return;
  case options::reset:
    namespace_switcher<reset>(s, n, r);
    return;
  default:
    throw std::runtime_error("Internal error");
  }
}

// Alias
const auto &process_options = &action_switcher;

/// Display the help message
void display_help() {
  std::cout << "Execute an action repeatedly on a container. The options are:"
            << std::endl;
  std::cout << "- namespace: (std, smit), default: smit" << std::endl;
  std::cout << "- action: (iterate, rest), default: iterate" << std::endl;
  std::cout << "- entries: (int), default: " << DEFAULT_ENTRIES << std::endl;
  std::cout << "- repetitions: (int), default: " << DEFAULT_REPETITIONS
            << std::endl;
}

/// Main function
int main(int argc, const char *argv[]) {

  // Process the possible call to --help and check for invalid options
  if (argc == 2) {
    if (argv[1] == std::string{"--help"}) {
      display_help();
      return 0;
    } else {
      std::cerr << "ERROR: Invalid arguments" << std::endl;
      display_help();
      return 1;
    }
  }

  if ((argc - 1) % 2 != 0) {
    std::cerr << "ERROR: Invalid arguments" << std::endl;
    display_help();
    return 1;
  }

  // Defaults
  options::Action action = options::iterate;
  options::Namespace nspc = options::SMIT;
  std::size_t entries = DEFAULT_ENTRIES;
  std::size_t repetitions = DEFAULT_REPETITIONS;

  // Process the options
  for (int i = 1; i < argc; i += 2) {
    try {
      if (argv[i] == std::string{"--action"})
        action = options::process_action(argv[i + 1]);
      else if (argv[i] == std::string{"--namespace"})
        nspc = options::process_namespace(argv[i + 1]);
      else if (argv[i] == std::string{"--entries"})
        entries = std::stoi(argv[i + 1]);
      else if (argv[i] == std::string{"--repetitions"})
        repetitions = std::stoi(argv[i + 1]);
      else {
        std::cerr << "ERROR: Unknown option: \"" << argv[i] << '\"'
                  << std::endl;
        return 1;
      }
    } catch (std::runtime_error const &e) {
      std::cerr << "ERROR: " << e.what() << std::endl;
      return 1;
    }
  }

  // Call the function
  process_options(action, nspc, entries, repetitions);

  return 0;
}
