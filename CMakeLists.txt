cmake_minimum_required(VERSION 3.5)

project(camera_view VERSION 0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(PROJECT_SOURCES
        main.cpp
        mainWindow.cpp
        tabs.cpp
        cameraStream.cpp
        videoWindow.cpp
        videoWidget.cpp
        cameraViewWindow.cpp
        calibrationWindow.cpp
        fisheyeCameraModel.cpp
        )

set(PROJECT_HEADERS
	mainWindow.h
	tabs.h
	settings.h
	cameraStream.h
	videoWindow.h
	videoWidget.h
	cameraViewWindow.h
	calibrationWindow.h
	fisheyeCameraModel.h
	)

add_executable(camera_view ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_link_libraries(camera_view PRIVATE Qt5::Widgets ${OpenCV_LIBS} stdc++fs)

