cmake_minimum_required(VERSION 3.3)

set(TARGET tray_timer)
project(${TARGET} CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CLANG_CXX_FLAGS "-fcolor-diagnostics -Wno-deprecated-register")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_CXX_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SRCS
    main_window.cpp
    progress_icon.cpp
    timer_menu.cpp
    running_menu.cpp
    suspended_menu.cpp
    context_menu.cpp
    timer.cpp
    tray_timer.cpp
    tray_timer.qrc
    main.cpp
    actions.cpp
    settings.cpp
)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt4 REQUIRED QtGui)

add_executable(${TARGET} ${SRCS})
target_link_libraries(${TARGET} Qt4::QtGui)
