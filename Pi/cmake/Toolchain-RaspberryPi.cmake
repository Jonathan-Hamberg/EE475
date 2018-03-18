
# this one is important
set(CMAKE_SYSTEM_NAME Linux)

#this one not so much
set(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
set(CMAKE_C_COMPILER ${CMAKE_BINARY_DIR}/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)

set(CMAKE_CXX_COMPILER ${CMAKE_BINARY_DIR}/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-c++)

# where is the target environment
set(CMAKE_FIND_ROOT_PATH ${CMAKE_BINARY_DIR}/arm-rpi-4.9.3-linux-gnueabihf)
message("aoeuhtsnaoe ${CMAKE_C_COMPILER}")

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
