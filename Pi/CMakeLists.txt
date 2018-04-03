cmake_minimum_required(VERSION 3.7)

project(defuser)

# Use C++ 11 standard.
set(CMAKE_CXX_STANDARD  11)

include_directories(../Arduino/Libraries)

set ( SRC
        src/main.cpp
        src/bcm2835.c
		src/ModuleManager.cpp src/ModuleManager.h
        ../Arduino/Libraries/GameState.cpp
        ../Arduino/Libraries/LFSR.cpp src/Module.cpp src/Module.h src/SPIManager.cpp src/SPIManager.h src/Definitions.h)

add_executable(defuser ${SRC})

# Add custom build command
add_custom_target(defuser-scp
        DEPENDS defuser
        COMMAND "scp ${CMAKE_BINARY_DIR}/defuser devbox:/home/pi ")