include(GoogleTest)

add_executable(thread_pool_test thread_pool_test.cpp)

target_link_libraries(thread_pool_test gtest gtest_main webserver)

gtest_discover_tests(thread_pool_test)

add_executable(acceptor_test acceptor_test.cpp)

target_link_libraries(acceptor_test gtest gtest_main webserver)

gtest_discover_tests(acceptor_test)

add_executable(circular_buffer_test circular_buffer_test.cpp)

target_link_libraries(circular_buffer_test gtest gtest_main webserver)

gtest_discover_tests(circular_buffer_test)
