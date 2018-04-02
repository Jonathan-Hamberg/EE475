#!/bin/bash

for i in "$@"
do
case $i in
    -h|--help)
    HELP=1
    ;;
    -c|--cross)
    CROSS=1
    ;;
    *)
    echo "populate.sh: unrecognized option '$1'"
    echo "Try 'populate.sh --help' for more information"
    ;;
esac
done

# Print the help documentation.
if [ $HELP ] ; then
    echo "This script generates the build files for the project."
    echo "The build system is configured for local and cross compile targets."
    echo "Build files are located in the build/ directory."
    echo "  -h|--help   Prints this help command."
    echo "  -c|--clean  Deletes the build directory."
    echo "  -f|--fast   Only generates debug build files and not release build files."
    echo "  -b|--build  Builds the source code."
    echo "  -a|--arm    Builds the cross compilation arm targets."
    echo "  -n|--native Build the native targets."
    echo "  -e|--extra  Include extra features supported by the desktop.  e.g. catch, doxygen"
    exit 0
fi

# Configure the cmake build generator options.
GENERATOR="-GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"

# Configure the cross compilation options.
CROSS_ARGS="-DCMAKE_TOOLCHAIN_FILE=cmake/Toolchain-RaspberryPi.cmake"

if [ $CROSS ] ; then
    if [[ ! -d toolchain ]] ; then
        mkdir toolchain
        git clone https://github.com/raspberrypi/tools.git toolchain/tools --depth=1
    fi

    GENERATOR="$GENERATOR $CROSS_ARGS"
fi


if [[ ! -f build/build.ninja ]] ; then
    mkdir -p build
    cmake -H. -Bbuild $GENERATOR
fi

