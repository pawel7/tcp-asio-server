cmake_minimum_required(VERSION 3.10)

# set the project name
project(tcp_server)

file(GLOB SERVER_FILES server/*.cpp)
set(SOURCES app/main.cpp asio-1.20.0/include/asio.hpp ${SERVER_FILES})

# add_subdirectory( asio-1.20.0 )

add_executable(tcp_server ${SOURCES})

set(LIB_SOURCES asio/include/*.*)
#add_library(asio STATIC ${LIB_SOURCES})

#target_include_directories(tcp_server src/server)

