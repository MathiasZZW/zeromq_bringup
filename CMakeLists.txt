cmake_minimum_required(VERSION 3.10)
project(zmq_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH  "${CMAKE_SOURCE_DIR}/external/packages/")
 
find_package(cppzmq  CONFIG  REQUIRED)
# 添加可执行文件
add_executable(zmq_demo main.cpp) 

# 链接 ZeroMQ 库
target_link_libraries(zmq_demo PRIVATE  cppzmq)