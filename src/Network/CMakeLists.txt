cmake_minimum_required(VERSION 3.14)
project(network)

include_directories(../../include/Network)


# -pthread
set(CMAKE_CXX_FLAGS "-pthread")
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

add_library(network SHARED ./network.cc)
