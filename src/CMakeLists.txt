add_library(webserver STATIC)

target_sources(webserver
    PRIVATE
    thread_pool.cpp
    acceptor.cpp
    dispatcher.cpp
    http_request_handler.cpp
    circular_buffer.cpp
)

target_include_directories(webserver
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(webserver
    PUBLIC
)
