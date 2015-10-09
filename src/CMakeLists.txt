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
    actions.cpp
    appearance_setting_widget.cpp
    context_menu.cpp
    main.cpp
    progress_icon.cpp
    property_dialog.cpp
    running_menu.cpp
    settings.cpp
    suspended_menu.cpp
    timer.cpp
    timer_list_widget.cpp
    timer_list_model.cpp
    timer_list_actions.cpp
    timer_menu.cpp
    tray_timer.cpp
    tray_timer.qrc
    action_button.cpp
    timer_dialog.cpp
)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt4 REQUIRED QtGui)

add_definitions(-DQT_NO_KEYWORDS)

add_executable(${TARGET} ${SRCS})
target_link_libraries(${TARGET} Qt4::QtGui)

install(TARGETS ${TARGET} RUNTIME DESTINATION bin)

set(DESKTOP_ENTRY ${CMAKE_SOURCE_DIR}/tray_timer.desktop)
set(DESKTOP_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/applications)
install(FILES ${DESKTOP_ENTRY} DESTINATION ${DESKTOP_DIRECTORY})
