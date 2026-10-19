cmake_minimum_required(VERSION 3.16)
project(SuperJupiterSignalFilter VERSION 0.1.0 LANGUAGES CXX)

set(_superjupiter_signal_filter_top_level OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(_superjupiter_signal_filter_top_level ON)
endif()
option(SUPERJUPITER_SIGNAL_FILTER_BUILD_TESTS "Build SignalFilter tests" ${_superjupiter_signal_filter_top_level})
option(SUPERJUPITER_SIGNAL_FILTER_BUILD_EXAMPLES "Build SignalFilter host example" ${_superjupiter_signal_filter_top_level})
option(SUPERJUPITER_SIGNAL_FILTER_INSTALL "Generate SignalFilter install rules" ${_superjupiter_signal_filter_top_level})

add_library(SuperJupiterSignalFilter INTERFACE)
add_library(SuperJupiter::SignalFilter ALIAS SuperJupiterSignalFilter)
set_target_properties(SuperJupiterSignalFilter PROPERTIES EXPORT_NAME SignalFilter)
target_include_directories(SuperJupiterSignalFilter INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>)
target_compile_features(SuperJupiterSignalFilter INTERFACE cxx_std_11)

if(SUPERJUPITER_SIGNAL_FILTER_BUILD_TESTS)
  enable_testing()
  add_executable(superjupiter-signal-filter-tests tests/test.cpp)
  target_link_libraries(superjupiter-signal-filter-tests PRIVATE SuperJupiter::SignalFilter)
  add_test(NAME superjupiter-signal-filter-tests COMMAND superjupiter-signal-filter-tests)
  add_executable(superjupiter-signal-filter-namespace-test tests/namespace_isolation.cpp)
  target_link_libraries(superjupiter-signal-filter-namespace-test PRIVATE SuperJupiter::SignalFilter)
  add_test(NAME superjupiter-signal-filter-namespace-test COMMAND superjupiter-signal-filter-namespace-test)
endif()
if(SUPERJUPITER_SIGNAL_FILTER_BUILD_EXAMPLES)
  add_executable(superjupiter-signal-filter-replay tools/replay.cpp)
  target_link_libraries(superjupiter-signal-filter-replay PRIVATE SuperJupiter::SignalFilter)
endif()
if(SUPERJUPITER_SIGNAL_FILTER_INSTALL)
  include(CMakePackageConfigHelpers)
  set(_superjupiter_signal_filter_package_dir lib/cmake/SuperJupiterSignalFilter)
  install(TARGETS SuperJupiterSignalFilter EXPORT SuperJupiterSignalFilterTargets)
  install(FILES src/SuperJupiterSignalFilter.h DESTINATION include)
  install(EXPORT SuperJupiterSignalFilterTargets NAMESPACE SuperJupiter::
    DESTINATION ${_superjupiter_signal_filter_package_dir})
  configure_package_config_file(cmake/SuperJupiterSignalFilterConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterSignalFilterConfig.cmake
    INSTALL_DESTINATION ${_superjupiter_signal_filter_package_dir})
  write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterSignalFilterConfigVersion.cmake
    VERSION ${PROJECT_VERSION} COMPATIBILITY SameMinorVersion ARCH_INDEPENDENT)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterSignalFilterConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SuperJupiterSignalFilterConfigVersion.cmake
    DESTINATION ${_superjupiter_signal_filter_package_dir})
  install(FILES LICENSE DESTINATION share/licenses/SuperJupiterSignalFilter)
endif()
