cmake_minimum_required(VERSION 3.17)
project(homework2)


set(CMAKE_CXX_STANDARD 14)

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(homework2 main.cpp)

target_link_libraries(homework2 ${OpenCV_LIBS})