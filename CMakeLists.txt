cmake_minimum_required(VERSION 3.16)
project(BigIntCalculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    REQUIRED
)

add_executable(BigIntCalculator
    src/main.cpp
    src/bigint.cpp
    src/bigint.h
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
)

target_link_libraries(BigIntCalculator
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)