cmake_minimum_required(VERSION 2.6)
project(lineana)
set( CMAKE_BUILD_TYPE Debug)
set( CMAKE_CXX_FLAGS "-std=c++11 -O0" )


find_package( OpenCV 3.1 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )



add_executable(lineana main.cpp)
target_link_libraries( lineana ${OpenCV_LIBS} )
install(TARGETS lineana RUNTIME DESTINATION bin)
