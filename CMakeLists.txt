cmake_minimum_required(VERSION 3.10)
project(Day2Vision)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

# 注意这里：生成名叫 yolo_test 的程序，编译 yolo_test.cpp
add_executable(yolo_test yolo_test.cpp)

# 注意这里：把 OpenCV 链接给 yolo_test
target_link_libraries(yolo_test ${OpenCV_LIBS})