add_library(webserver STATIC)

target_sources(webserver
    PRIVATE
    event_handler.cpp
    http_server.cpp
    acceptor.cpp
    reactor.cpp
    epoll_reactor.cpp
    thread_pool.cpp
    socket.cpp
    http_connection.cpp
    http_request.cpp
    http_response.cpp
    http_request_parser.cpp
)

target_include_directories(webserver
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(webserver
    PRIVATE
    glog::glog
    fmt::fmt
)
