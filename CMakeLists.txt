cmake_minimum_required(VERSION 3.0)
project("SmartIt" VERSION 0.0.0
DESCRIPTION "A header only c++ library that provides an easy way to work with value types and iterators in a struct-of-arrays paradigm")

option(INSTALL_TESTS "Whether to install the test scripts or not" OFF)

#
# Installation of the library
#
add_library(${PROJECT_NAME} INTERFACE)

include(GNUInstallDirs)

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

#
# Create the configuration file
#
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

configure_package_config_file(
 "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
 "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
 INSTALL_DESTINATION
 ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/cmake)

install(EXPORT ${PROJECT_NAME}_Targets
       FILE ${PROJECT_NAME}Targets.cmake
       NAMESPACE ${PROJECT_NAME}::
       DESTINATION ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
             "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
       DESTINATION ${PROJECT_BINARY_DIR}/${PROJECT_NAME}/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/smartit DESTINATION ${PROJECT_BINARY_DIR}/include)

#
# Installation of the test scripts
#
if(INSTALL_TESTS)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test)
    include_directories(include)
    file(GLOB TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)
    set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")
    foreach(testsourcefile ${TEST_SOURCES})
      get_filename_component(testname ${testsourcefile} NAME_WE)
      add_executable(${testname} ${testsourcefile})
      set_target_properties(${testname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
    endforeach(testsourcefile ${TEST_SOURCES})
endif(INSTALL_TESTS)
