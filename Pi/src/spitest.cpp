#include "bcm2835.h"
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <cstdio>
#include <cstdlib>

#include <csignal>
#include <sstream>

#include "SPIManager.h"

SPIManager spiManager;

static volatile int keepRunning = 1;


void intHandler(int signal) {
    // Unused parameter
    (void) (signal);

    // Tell the main program to stop.
    keepRunning = 0;
}

void printHex(char *str, uint32_t length) {
    std::cout << "0x";
    uint8_t hexCodes[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    for (uint32_t i = 0; i < length; i++, str++) {
        std::cout << hexCodes[uint8_t(*str) >> 4u & 0x0Fu];
        std::cout << hexCodes[uint8_t(*str) & 0x0Fu];
    }

}

int main(int argc, char *argv[]) {
    // Register the signal interrupt handler.
    signal(SIGINT, intHandler);

    // Determine if the application is run with root privileges.
    if (geteuid() == 0) {
        if (!bcm2835_init()) {
            std::cout << "bcm2835_init() failed..." << std::endl;
            return 1;
        }
    } else {
        std::cout << "You need to be root to properly run this test program." << std::endl;
        return 1;
    }

    std::cout << "Starting SPI Communication Test ..." << std::endl;

    uint8_t buffer[3];

    std::string input;

    std::stringstream ss;
    int test = 0;

    while (keepRunning) {

        input.clear();
        std::cin >> input;

        if (input[0] == 'T') {

            if (input.length() != 7) {
                std::cout << "Command should be formatted 'T'HHHHHH" << std::endl;
                continue;
            }

            ss.clear();
            ss << std::hex << input;
            uint32_t value;
            ss >> value;

            buffer[0] = uint8_t((value >> 16u) & 0xFFu);
            buffer[1] = uint8_t((value >> 8u) & 0xFFu);
            buffer[2] = uint8_t((value >> 0u) & 0xFFu);

            std::cout << "transmitting: ";
            printHex((char *) (buffer), 3);
            std::cout << std::endl;

            spiManager.transfer(buffer, 3);

            std::cout << "receiving: ";
            printHex((char *) (buffer), 3);
            std::cout << std::endl;

        } else if (input[0] == 'C') {

            if (input.length() != 3) {
                std::cout << "Command should be formatted 'C'DD" << std::endl;
                continue;
            }

            uint32_t chipSelect;

            if (!(std::istringstream(input.substr(1)) >> chipSelect)) {
                std::cout << "Failed to parse chip select." << std::endl;
            }

            std::cout << "Changing Chip Select to " << (int) chipSelect << std::endl;
            spiManager.selectCS(uint8_t(chipSelect));
        } else if (input[0] == 'A') {
            if (input.length() != 9) {
                std::cout << "Command should be formatted 'A'DDHHHHHH" << std::endl;
            }

            uint32_t chipSelect;
            uint32_t hexValue;

            if (!(std::istringstream(input.substr(1, 2)) >> chipSelect)) {
                std::cout << "Failed to parse chip select." << std::endl;
            }

            if (!(std::istringstream(input.substr(3, 6)) >> std::hex >> hexValue)) {
                std::cout << "Failed to parse data." << std::endl;
            }

            buffer[0] = uint8_t(hexValue >> 16u);
            buffer[1] = uint8_t(hexValue >> 8u);
            buffer[2] = uint8_t(hexValue);

            std::cout << "Changing Chip Select to " << (int) chipSelect << std::endl;

            std::cout << "transmitting: ";
            printHex((char *) (buffer), 3);
            std::cout << std::endl;


            spiManager.selectCS(chipSelect);

            spiManager.transfer(buffer, 3);

            std::cout << "receiving: ";
            printHex((char *) (buffer), 3);
            std::cout << std::endl;
        } else {
            std::cout << "Unsupported Operation" << std::endl;
            std::cout << "Supported operations are." << std::endl;
            std::cout << "THHHHHH" << std::endl;
            std::cout << "CNN" << std::endl;
            std::cout << "ADDHHHHHH - 2 digits address 6 digits data" << std::endl;
        }


    }

    return 0;
}

