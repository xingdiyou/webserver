add_executable(http_app http_app.cpp)

target_link_libraries(http_app PRIVATE webserver glog::glog)
