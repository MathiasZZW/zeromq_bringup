cmake_minimum_required(VERSION 3.10)
project(zmq)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH  "${CMAKE_SOURCE_DIR}/external/packages/")

find_package(cppzmq CONFIG REQUIRED)
find_package(ZeroMQ CONFIG REQUIRED)
# 添加可执行文件
add_executable(zmq main.cpp)

# 链接 ZeroMQ 库
target_link_libraries(zmq PRIVATE cppzmq libzmq)