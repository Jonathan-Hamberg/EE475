cmake_minimum_required(VERSION 3.7)

project(test_project)

set(SRC
        main.cpp
        ../src/SoundManager.h
        ../src/SoundManager.cpp
        )

include_directories(../../Arduino/Libraries)

add_executable(audio ${SRC})


target_link_libraries(audio asound vorbisfile pthread)
