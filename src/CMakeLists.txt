cmake_minimum_required(VERSION 3.10)
project(tcp_server)

find_package (Threads)

set(SOURCES app/main.cpp 
            server/Server.cpp)

include_directories(server asio asio/include)
#include_directories(server asio asio/include ../vendor/cpp-netlib)

#add_subdirectory(/home/pawel/pro/tcp_server/vendor/cpp-netlib)

add_definitions(-DASIO_STANDALONE)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
#target_link_libraries(${PROJECT_NAME} PUBLIC pthread CPP-NETLIB)