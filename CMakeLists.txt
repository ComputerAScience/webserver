cmake_minimum_required(VERSION 3.16)
project(webserver)

set(CMAKE_CXX_STANDARD 17)

set(SRCS
        main.cpp)
include_directories(${PROJECT_SOURCE_DIR}/base)
include_directories(${PROJECT_SOURCE_DIR}/net)
include_directories(${PROJECT_SOURCE_DIR}/http
        )

add_executable(webserver ${SRCS})
target_link_libraries(webserver libserver_base libserver_net)


add_subdirectory(base)
add_subdirectory(net)
