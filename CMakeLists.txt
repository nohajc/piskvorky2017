cmake_minimum_required(VERSION 3.0)
project(piskvorky2017)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -Wno-attributes")

set(RES_FILES src/res/default.htm src/res/ui.tis)

add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/src/resources.h
        COMMAND ${PROJECT_SOURCE_DIR}/pack-resources.sh ${PROJECT_SOURCE_DIR}
        DEPENDS ${RES_FILES})

set(SOURCE_FILES src/piskvorky.cpp src/resources.h sciter-sdk/include/sciter-gtk-main.cpp src/Game.cpp src/Game.h src/Player.cpp src/Player.h src/HumanPlayer.cpp src/HumanPlayer.h src/Frame.cpp src/Frame.h)

include_directories(sciter-sdk/include)

add_executable(piskvorky2017 ${SOURCE_FILES})

target_link_libraries(piskvorky2017 ${CMAKE_DL_LIBS} ${GTK3_LIBRARIES})

set(EXEC_DIR ${PROJECT_SOURCE_DIR}/bin)

add_custom_command(TARGET piskvorky2017 POST_BUILD
        COMMAND mkdir -p ${EXEC_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:piskvorky2017> ${EXEC_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/sciter-sdk/bin.gtk/libsciter-gtk-64.so ${EXEC_DIR}/)