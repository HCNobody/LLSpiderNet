cmake_minimum_required (VERSION 3.14)
project (LLSpiderNet)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
message(STATUS "----- GTK3_INCLUDE_DIRS: ${GTK3_INCLUDE_DIRS}")
message(STATUS "----- GKT3_LIBRARIES: ${GTK3_LIBRARIES}")
message(STATUS "----- GTK3_LINK_LIBRARIES: ${GTK3_LINK_LIBRARIES}")
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
list(APPEND FC_DEP_LIBS ${GTK3_LIBRARIES})
 

include_directories(./include)

add_library(tray STATIC ./src/tray.cc)
target_link_libraries(tray ${FC_DEP_LIBS})

add_library(debug_tools STATIC ./src/debug_tools.cc)

set(link_d tray debug_tools)
add_executable (LLSpiderNet main.cc)
 
#add_definitions (${GTK3_CFLAGS_OTHER})
message(STATUS "----- ${GTK3_CFLAGS_OTHER}")
# -pthread

target_link_libraries (LLSpiderNet  ${link_d})

