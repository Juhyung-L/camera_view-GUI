cmake_minimum_required(VERSION 3.5)

project(camera_view VERSION 0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)
find_package(OpenCV REQUIRED)
include_directories(include src)

file(GLOB SRC_FILES src/*.cpp)
file(GLOB HEADER_FILES include/*.h)

add_executable(camera_view ${SRC_FILES} ${HEADER_FILES})
target_link_libraries(camera_view PRIVATE Qt5::Widgets Qt5::Core Qt5::Gui ${OpenCV_LIBS} stdc++fs)

