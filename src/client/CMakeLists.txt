cmake_minimum_required(VERSION 3.10)
project(client)

include_directories(../common ../asio/asio/include ../httpparser/)

add_definitions(-DASIO_ENABLE_HANDLER_TRACKING)
add_executable(${PROJECT_NAME} my_client.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)