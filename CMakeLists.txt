cmake_minimum_required(VERSION 3.14)
project(LLSpiderNet)

message("-- 开始载入项目LLSpiderNet")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

#全局变量设制
message("---------------------------------------")
message("---- 全局变量:")
set(USE_DEBUG_CODE ON)
message("-- USE_DEBUG_CODE : ${USE_DEBUG_CODE}")
message("-- INSTALL_SWITCH : ${INSTALL_SWITCH}")

#子项目
message("---------------------------------------")
message("-- 加载子项目")
message("---------------------------------------")
message("载入Network模块")
add_subdirectory(./src/Network)
message("载入ToolKit模块")
add_subdirectory(./ToolKit)
message("---------------------------------------")

set(link_d ${link_d}
)
set(link_d_shared ${link_d_shared}
                  network
                  Event_T_Callback
)
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
add_library(button STATIC ./src/Button.cc)
target_link_libraries(button ${FC_DEP_LIBS} network Event_T_Callback)
target_link_libraries(tray ${FC_DEP_LIBS} network button Event_T_Callback)


#动态库编译
set(link_d_shared ${link_d_shared})

#链接文件
set(link_d ${link_d} 
           ${link_d_shared}
            tray 
            button
)
#生成目标文件
add_executable(LLSpiderNet main.cc)
 
#add_definitions (${GTK3_CFLAGS_OTHER})
message(STATUS "----- ${GTK3_CFLAGS_OTHER}")
# -pthread
set(CMAKE_CXX_FLAGS "-pthread")
if(USE_DEBUG_CODE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
#链接
message("----------------------")
message("-- 库链接:${link_d}")

target_link_libraries(LLSpiderNet ${link_d})


#install
if(INSTALL_SWITCH) 
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
  set_target_properties(LLSpiderNet PROPERTIES INSTALL_RPATH "/usr/local/lib/LLSpiderNet")
  install(TARGETS LLSpiderNet ${link_d_shared}
          EXPORT LLSpiderNet
          LIBRARY DESTINATION lib/LLSpiderNet #动态库
          #ARCHIVE DESTINATION lib/LLSpiderNet  #静态库
          RUNTIME DESTINATION bin  #可制行文件
          #PUBLIC_HEADER DESTINATION include #头文件
  )
  install(SCRIPT ./install.cmake)
  add_definitions(-DINSTALLED="/usr/local/share/LLSpiderNet/")
endif()
