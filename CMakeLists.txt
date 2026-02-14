cmake_minimum_required(VERSION 3.19)
project(Tucil1_13524053 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Tucil1_13524053
    WIN32 MACOSX_BUNDLE

    # GUI
    src/GUI/main.cpp
    src/GUI/mainwindow.cpp
    src/GUI/mainwindow.h
    src/GUI/mainwindow.ui

    # CORE
    src/core/Board.cpp
    src/core/Solver.cpp
    src/GUI/mainwindow.h src/GUI/mainwindow.cpp src/GUI/mainwindow.ui
)

target_include_directories(Tucil1_13524053 PRIVATE
    src
)

target_link_libraries(Tucil1_13524053
    PRIVATE
        Qt::Core
        Qt::Widgets
)
