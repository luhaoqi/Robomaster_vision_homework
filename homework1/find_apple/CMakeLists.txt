cmake_minimum_required(VERSION 3.10)
project(opencv_test)


set(CMAKE_CXX_STANDARD 11)

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(opencv_test main.cpp vedio.cpp)
target_link_libraries(  opencv_test ${OpenCV_LIBS}  )