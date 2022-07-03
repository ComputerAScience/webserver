cmake_minimum_required(VERSION 3.16)
project(webserver)

set(CMAKE_CXX_STANDARD 17)

set(SRCS
        main.cpp
        Buffer.cpp Buffer.h)
include_directories(${PROJECT_SOURCE_DIR}/base)


add_executable(webserver ${SRCS})
target_link_libraries(webserver libserver_base)


add_subdirectory(base)
