cmake_minimum_required(VERSION 3.10)
project(Day2Vision)
set(CMAKE_CEX_STANDARD 11)

find_package(OpenCV REQUIRED)

add_executable(vision_app main.cpp ImageProcessor.cpp NMS.cpp)

target_link_libraries(vision_app ${OpenCV_LIBS})