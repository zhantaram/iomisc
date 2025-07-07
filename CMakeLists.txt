cmake_minimum_required(VERSION 4.0)

project(iomisc LANGUAGES CXX)

#------------------------------------------------------------------------------
# Library target
add_library(${PROJECT_NAME}
  src/dummy.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

#------------------------------------------------------------------------------
# Thirdparty
find_package(stdexec)
find_package(PkgConfig)
pkg_search_module(Asio REQUIRED IMPORTED_TARGET asio)
target_link_libraries(${PROJECT_NAME}
PUBLIC
  PkgConfig::Asio
  STDEXEC::stdexec
)

#------------------------------------------------------------------------------
# Tests
include(CTest)
if (PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
  find_package(GTest)
  add_executable(test_${PROJECT_NAME}
    tests/dummy.cpp
  )
  target_link_libraries(test_${PROJECT_NAME} PUBLIC ${PROJECT_NAME} GTest::gtest_main)
  gtest_discover_tests(test_${PROJECT_NAME})
endif()
