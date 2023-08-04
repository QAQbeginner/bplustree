# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo)

# 
include_directories(${CMAKE_SOURCE_DIR}/include)

# 指定生成目标
add_executable(Demo main.cpp include/bplustree.cpp include/CTool.cpp)

SET(CMAKE_BUILD_TYPE "Debug")