cmake_minimum_required(VERSION 3.14)
project(LLSpiderNet)

message("-- 开始载入项目LLSpiderNet")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

#子项目
message("-- 加载子项目")
message("---------------------------------------")
message("载入Network模块")
add_subdirectory(./src/Network)
message("---------------------------------------")

set(link_d ${link_d} network)
message("-- 子项目加载完成,进入主项目")

#GTK3编译选项获取
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
message(STATUS "----- GTK3_INCLUDE_DIRS: ${GTK3_INCLUDE_DIRS}")
message(STATUS "----- GKT3_LIBRARIES: ${GTK3_LIBRARIES}")
message(STATUS "----- GTK3_LINK_LIBRARIES: ${GTK3_LINK_LIBRARIES}")
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
list(APPEND FC_DEP_LIBS ${GTK3_LIBRARIES})
 
#载入头文件
include_directories(./include)

#静态库编译
add_library(tray STATIC ./src/tray.cc)
target_link_libraries(tray ${FC_DEP_LIBS})

add_library(debug_tools STATIC ./src/debug_tools.cc)

#动态库编译

#链接文件
set(link_d ${link_d} tray debug_tools)
#生成目标文件
add_executable(LLSpiderNet main.cc)
 
#add_definitions (${GTK3_CFLAGS_OTHER})
message(STATUS "----- ${GTK3_CFLAGS_OTHER}")
# -pthread
set(CMAKE_CXX_FLAGS "-pthread")
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
#链接
message("----------------------")
message("-- 库链结:${link_d}")

target_link_libraries(LLSpiderNet ${link_d})

