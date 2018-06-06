cmake_minimum_required(VERSION 3.7)

project(defuser)

# Use C++ 11 standard.
set(CMAKE_CXX_STANDARD  11)
set(CMAKE_EXPORT_COMPILE_COMMANDS)

include_directories(../Arduino/Libraries)

# Set the source files required for the main defuser application.
set ( SRC
        src/main.cpp
        src/bcm2835.c
		src/ModuleManager.cpp
		src/ModuleManager.h
		src/SPIManager.cpp
		src/SPIManager.h
        ../Arduino/Libraries/GameState.cpp
		../Arduino/Libraries/GameState.h
        ../Arduino/Libraries/LFSR.cpp
		../Arduino/Libraries/LFSR.h
		../Arduino/Libraries/Definitions.h
		../Arduino/Libraries/Definitions.cpp
		src/GameManager.cpp
		src/GameManager.h
		src/SoundManager.h
		src/SoundManager.cpp)

# Add executable for the main defuser application.
add_executable(defuser ${SRC})

# Add custom command to transfer the executable to the raspberry pi.
add_custom_command(TARGET defuser POST_BUILD
		COMMAND scp ${CMAKE_BINARY_DIR}/defuser devbox:~/
        COMMAND rsync -rl ${CMAKE_SOURCE_DIR}/Resources devbox:~/
        )

target_link_libraries(defuser asound vorbisfile pthread)

# Add executable for the spi test pragram.
add_executable(spitest src/spitest.cpp
		src/SPIManager.h
		src/SPIManager.cpp
        src/bcm2835.c)

# Add custom command to transfer the executable to the raspberry pi.
add_custom_command(TARGET spitest POST_BUILD
		COMMAND scp ${CMAKE_BINARY_DIR}/spitest devbox:~/
		)
