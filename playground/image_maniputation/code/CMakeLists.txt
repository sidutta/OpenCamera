cmake_minimum_required(VERSION 2.8)
project( detect_skew )
find_package( OpenCV )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( detect_skew detect_skew.cpp )
target_link_libraries( detect_skew ${OpenCV_LIBS} )

