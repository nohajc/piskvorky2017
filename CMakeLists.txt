cmake_minimum_required(VERSION 3.0)
project(piskvorky2017)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -Wno-attributes")

set(RES_FILES src/res/default.htm src/res/ui.tis)

set(SOURCE_FILES src/piskvorky.cpp src/resources.h src/main.cpp
        src/Game.cpp src/Game.h src/Player.cpp src/Player.h src/HumanPlayer.cpp src/HumanPlayer.h src/Frame.cpp
        src/Frame.h src/Grid.cpp src/Grid.h test/TestGame.cpp test/TestGame.h src/MinMaxPlayer.cpp src/MinMaxPlayer.h)

include_directories(sciter-sdk/include)
include_directories(src)

add_executable(piskvorky2017 ${SOURCE_FILES})

if(UNIX)
    target_link_libraries(piskvorky2017 ${CMAKE_DL_LIBS})
endif()

if(APPLE)
    set(SCITER_LIB ${CMAKE_SOURCE_DIR}}/sciter-sdk/bin.osx/sciter-osx-64.dylib)
    set(RES_PACK_SCRIPT ${PROJECT_SOURCE_DIR}/pack-resources.sh osx)
elseif(UNIX) # Linux
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

    include_directories(${GTK3_INCLUDE_DIRS})
    link_directories(${GTK3_LIBRARY_DIRS})
    add_definitions(${GTK3_CFLAGS_OTHER})

    target_link_libraries(piskvorky2017 ${GTK3_LIBRARIES})

    set(SCITER_LIB ${CMAKE_SOURCE_DIR}/sciter-sdk/bin.gtk/libsciter-gtk-64.so)
    set(RES_PACK_SCRIPT ${PROJECT_SOURCE_DIR}/pack-resources.sh gtk)
elseif(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8) # WIN64
        set(SCITER_LIB ${CMAKE_SOURCE_DIR}/sciter-sdk/bin/64/sciter.dll)
    else() # WIN32
        set(SCITER_LIB ${CMAKE_SOURCE_DIR}/sciter-sdk/bin/64/sciter.dll)
    endif()
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/src/resources.h
        COMMAND ${RES_PACK_SCRIPT} ${PROJECT_SOURCE_DIR}
        DEPENDS ${RES_FILES})

set(EXEC_DIR ${PROJECT_SOURCE_DIR}/bin)

add_custom_command(TARGET piskvorky2017 POST_BUILD
        COMMAND mkdir -p ${EXEC_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:piskvorky2017> ${EXEC_DIR}/
        COMMAND ${CMAKE_COMMAND} -E copy ${SCITER_LIB} ${EXEC_DIR}/)
