cmake_minimum_required(VERSION 3.10)
project(zmq)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cppzmq CONFIG REQUIRED)
find_package(ZeroMQ  CONFIG  REQUIRED)
find_package(nlohmann_json  CONFIG  REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/include)

# 收集头文件和源文件
file(GLOB HEADERS
    ${CMAKE_SOURCE_DIR}/include/*.hpp
)

file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)   


# 添加可执行文件
add_executable(zmq ${HEADERS} ${SOURCES} main.cpp)

# 链接 ZeroMQ 库
target_link_libraries(zmq PRIVATE nlohmann_json cppzmq libzmq)