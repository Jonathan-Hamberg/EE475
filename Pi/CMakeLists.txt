cmake_minimum_required(VERSION 3.7)

project(defuser)

set ( SRC
        src/main.cpp
        src/bcm2835.c
		src/ModuleManager.cpp src/ModuleManager.h)

add_executable(defuser ${SRC})
