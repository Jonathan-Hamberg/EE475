#include "bcm2835.h"
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include "SPIManager.h"
#include "ModuleManager.h"
#include "GameManager.h"
#include <csignal>
#include <sstream>

static volatile int keepRunning = 1;

uint16_t flag_seed = 3968;
uint16_t flag_countdown = 120;
uint8_t flag_strikes = 3;

static struct option long_options[] =
        {
                {"seed",      optional_argument, nullptr, 's'},
                {"countdown", optional_argument, nullptr, 'c'},
                {"strikes",   optional_argument, nullptr, 't'},
                {nullptr, 0,                     nullptr, 0}
        };

void parseOptions(int argc, char *argv[]) {


    int c;
    int index = 0;

    while ((c = getopt_long(argc, argv, "s:t:c;", long_options, &index)) != -1) {

        // Perform operations on the options.
        switch (c) {
            case 0:
                break;

            case 's':
                flag_seed = uint16_t(strtol(optarg, nullptr, 10));
                std::cout << "-s " << flag_seed << std::endl;
                break;
            case 't':
                flag_strikes = uint8_t(strtol(optarg, nullptr, 10));
                std::cout << "-t " << flag_strikes << std::endl;

                break;
            case 'c':
                flag_countdown = uint16_t(strtol(optarg, nullptr, 10));
                std::cout << "-c " << flag_countdown << std::endl;

                break;
            case '?':
                break;
            default:
                break;
        }
    }
}

void intHandler(int signal) {
    // Unused parameter
    (void)(signal);

    // Tell the main program to stop.
    keepRunning = 0;
}


int main(int argc, char *argv[]) {

    // Determine if the application is run with root privileges.
    if (geteuid() != 0) {
        std::cout << "You must be root to run this program." << std::endl;
        return 1;
    }

    // Parse the command line arguments.
    parseOptions(argc, argv);

    // Register the signal interrupt handler.
    signal(SIGINT, intHandler);

    // Create the SPI Manager object.
    SPIManager spiManager = SPIManager();

    // Create the module manager object.
    ModuleManager moduleManager = ModuleManager(&spiManager);

    // Create the game manager object.
    GameManager gameManager = GameManager(&moduleManager);

    // Query the connected modules.
    moduleManager.queryModules(flag_seed);

    // Generate the initial game state.
    gameManager.generateGameState(flag_seed, flag_countdown, flag_strikes);

    // Transmit the game state to each of the modules.
    moduleManager.transmitGameState(gameManager.getGameState());

    // Update every 100ms.
    while(keepRunning) {
        // Update the state of all the modules.
        gameManager.update();

        // Sleep for 1 second.
        usleep(1000 * 1000);

    }

    return EXIT_SUCCESS;
}
