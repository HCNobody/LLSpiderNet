cmake_minimum_required(VERSION 3.14)
project(network)

include_directories(../../include/Network)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 


# -pthread
set(CMAKE_CXX_FLAGS "-pthread")
if(USE_DEBUG_CODE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

add_library(ThreadPool STATIC ./ThreadPool.cc)
add_library(Getip STATIC ./Getip.cc)

add_library(network SHARED ./network.cc)
target_link_libraries(network ThreadPool Getip Event_T_Callback)
