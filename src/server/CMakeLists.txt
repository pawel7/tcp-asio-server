cmake_minimum_required(VERSION 3.10)
project(server)

include_directories(../asio/asio/include ../common ../httpparser)

add_definitions(-DASIO_ENABLE_HANDLER_TRACKING)
add_executable(${PROJECT_NAME} my_server.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)