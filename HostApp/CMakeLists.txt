cmake_minimum_required(VERSION 3.5)

project(HostApp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("Looking for Qt...")
# Qt modules (https://doc.qt.io/qt-5/qtmodules.html) you're using in your application

find_package(Qt5 COMPONENTS Widgets SerialPort REQUIRED)

if (${Qt5_FOUND})
    message("Found Qt " ${Qt5_VERSION})
else()
    message("Couldn't find Qt")
endif()

set(sources
  main.cpp
  maindialog.cpp
  maindialog.h
  maindialog.ui
)

add_executable(${CMAKE_PROJECT_NAME} ${sources})

target_link_libraries(HostApp PRIVATE Qt5::Widgets Qt5::SerialPort)
