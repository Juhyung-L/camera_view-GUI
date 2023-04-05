cmake_minimum_required(VERSION 3.5)

project(test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(PROJECT_SOURCES
        main.cpp
        mainWindow.h mainWindow.cpp
        tabs.h tabs.cpp
        settings.h
        cameraStream.h cameraStream.cpp
        videoWindow.h videoWindow.cpp
        videoWidget.h videoWidget.cpp
        cameraViewWindow.h cameraViewWindow.cpp
        calibrationWindow.h calibrationWindow.cpp
        fisheyeCameraModel.h fisheyeCameraModel.cpp
        )

add_executable(test ${PROJECT_SOURCES})

target_link_libraries(test PRIVATE Qt5::Widgets ${OpenCV_LIBS})

set_target_properties(test PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
