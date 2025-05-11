cmake_minimum_required(VERSION 3.10)
project(Rasterizer)

find_package(OpenCV REQUIRED)

find_package(Eigen3 REQUIRED)

set(CMAKE_CXX_STANDARD 17)



add_executable(BezierCurve main.cpp)

target_include_directories(BezierCurve PRIVATE
    ${EIGEN3_INCLUDE_DIRS}  # Eigen3 头文件路径
    ${OpenCV_INCLUDE_DIRS}  # OpenCV 头文件路径
)

target_link_libraries(BezierCurve PRIVATE
    ${OpenCV_LIBRARIES}     # OpenCV 库
)

#target_compile_options(Rasterizer PUBLIC -Wall -Wextra -pedantic)