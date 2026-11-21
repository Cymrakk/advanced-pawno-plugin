cmake_minimum_required(VERSION 3.10)
project(advancedpawno)

add_definitions(-DHAVE_STDINT_H)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND NOT APPLE)
    add_definitions(-DLINUX -D_GNU_SOURCE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32 -fPIC -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -fPIC -O3")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/native
    ${CMAKE_SOURCE_DIR}/native/src
    ${CMAKE_SOURCE_DIR}/native/src/logic
    ${CMAKE_SOURCE_DIR}/native/SDK
    ${CMAKE_SOURCE_DIR}/native/SDK/amx
)

set(SOURCES
    native/src/main.cpp
    native/src/logic/utils.cpp
    native/src/logic/messages.cpp
    native/src/logic/logger.cpp
    native/SDK/amxplugin.cpp
)

if(WIN32)
    list(APPEND SOURCES native/src/plugin.def)
endif()

add_library(advancedpawno SHARED ${SOURCES})
set_target_properties(advancedpawno PROPERTIES PREFIX "")

if(UNIX AND NOT APPLE)
    target_link_libraries(advancedpawno dl stdc++fs)
endif()
