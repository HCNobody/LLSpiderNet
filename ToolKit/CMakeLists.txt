cmake_minimum_required(VERSION 3.14)
project(ToolKit)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

include_directories(./heads)


# -pthread
set(CMAKE_CXX_FLAGS "-pthread")
if(USE_DEBUG_CODE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

message("-- -- Thread_Pool")
add_library(Thread_Pool SHARED ./src/Thread_Pool.cc)


message("-- -- Event_T_Callback")
add_library(Event_T_Callback SHARED ./src/Event_T_Callback.cc)
link_directories(Event_T_Callback Thread_Pool)



