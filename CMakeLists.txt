cmake_minimum_required(VERSION 3.5.0)
project(efp_parser VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

include(FetchContent)

find_package(efp QUIET)

if(NOT efp_FOUND)
    FetchContent_Declare(
        efp
        GIT_REPOSITORY http://github.com/cwahn/efp
        GIT_TAG main
    )
    FetchContent_MakeAvailable(efp)
endif()

add_library(efp_parser INTERFACE)
target_include_directories(efp_parser INTERFACE include)
target_link_libraries(efp_parser INTERFACE efp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# add_subdirectory(lib)
add_subdirectory(test)
