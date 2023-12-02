cmake_minimum_required(VERSION 3.0.0)
project(efp_parser VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_library(efp_parser INTERFACE)
target_include_directories(efp_parser INTERFACE include)
target_link_libraries(efp_parser INTERFACE efp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(lib)
add_subdirectory(test)
