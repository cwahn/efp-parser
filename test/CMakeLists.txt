add_executable(efp_parser_test efp_parser_test.cpp)
target_link_libraries(efp_parser_test
    PRIVATE
    Catch2::Catch2WithMain
    efp_parser)

catch_discover_tests(efp_parser_test)