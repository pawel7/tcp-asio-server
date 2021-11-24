cmake_minimum_required(VERSION 3.10)
project(client)

include_directories(../common ../asio/asio/include ../httpparser/)

add_definitions(-DASIO_STANDALONE)
add_executable(${PROJECT_NAME} my_client.cpp)
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WIN32 strcasecmp=_stricmp strncasecmp=_strnicmp)
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
endif()