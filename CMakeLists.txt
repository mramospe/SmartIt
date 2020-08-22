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
 ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(EXPORT ${PROJECT_NAME}_Targets
       FILE ${PROJECT_NAME}Targets.cmake
       NAMESPACE ${PROJECT_NAME}::
       DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
             "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
       DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/smartit DESTINATION include)

#
# Installation of the test scripts
#
if(INSTALL_TESTS)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test)
    include_directories(include)
    add_executable(${PROJECT_NAME}_Test test/test.cpp)
    set_target_properties(${PROJECT_NAME}_Test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
    target_compile_features(${PROJECT_NAME}_Test INTERFACE cxx_std_17)
endif(INSTALL_TESTS)
