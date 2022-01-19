cmake_minimum_required(VERSION 3.17)
project(homework3)

set(CMAKE_CXX_STANDARD 14)

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(homework3 main.cpp)
target_link_libraries(homework3 ${OpenCV_LIBS})