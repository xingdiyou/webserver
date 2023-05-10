enable_testing()

set(test_libraries gtest gtest_main webserver)

function(add_webserver_test test_name test_source)
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name} ${test_libraries})
    add_test(
        NAME ${test_name}
        COMMAND valgrind --tool=memcheck ./${test_name}
    )
endfunction()

set(test_sources
    thread_pool_test.cpp
    http_request_parser_test.cpp
)

foreach(test_source ${test_sources})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_webserver_test(${test_name} ${test_source})
endforeach()
