# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo)

# 
include_directories(${CMAKE_SOURCE_DIR}/include)

# 指定生成目标
add_executable(Demo main.cpp rsc/bplustree.cpp rsc/CTool.cpp rsc/del.cpp rsc/test.cpp rsc/save.cpp rsc/insert.cpp)

SET(CMAKE_BUILD_TYPE "Debug")