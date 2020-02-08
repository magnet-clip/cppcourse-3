cmake_minimum_required(VERSION 3.1.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project (main)
include_directories(${PROJECT_SOURCE_DIR})
add_executable(main main.cpp stats.cpp)
set_property(TARGET main PROPERTY CXX_STANDARD 17)
