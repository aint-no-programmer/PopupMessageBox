cmake_minimum_required(VERSION 3.5)

project(popup_messagebox LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(popup_messagebox
    main.cpp
    mainwindow.cpp
    mainwindow.h
    popup.cpp
    popup.h
    mainwindow.ui
)

target_link_libraries(popup_messagebox PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
