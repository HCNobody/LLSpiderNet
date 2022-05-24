cmake_minimum_required (VERSION 3.14)
project (LLSpiderNet)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

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
add_library(network SHARED ./src/network.cc)

#链接文件
set(link_d tray debug_tools network)
#生成目标文件
add_executable(LLSpiderNet main.cc)
 
#add_definitions (${GTK3_CFLAGS_OTHER})
message(STATUS "----- ${GTK3_CFLAGS_OTHER}")
# -pthread
#链接
target_link_libraries(LLSpiderNet  ${link_d})

