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

///
SPIManager spiManager = SPIManager();

///
ModuleManager moduleManager = ModuleManager(&spiManager);

///
GameManager gameManager = GameManager(&moduleManager);

/**
 * Flag set by --seed, -s
 * Sets the seed used to generate pseudo-random numbers for each modules.
 */
int seed_flag;

static struct option long_options[] =
        {
                {"seed", optional_argument, nullptr, 's'},
                {nullptr, 0,                nullptr, 0}
        };

void parseOptions(int argc, char *argv[]) {


    int c;
    int index = 0;

    while ((c = getopt_long(argc, argv, "ds", long_options, &index)) != -1) {

        // Perform operations on the options.
        switch (c) {
            case 0:
                break;

            case 's':
                seed_flag = atoi(optarg);
                // TODO(jrh) remove debugging print statement.
                std::cout << "-s" << seed_flag << std::endl;
                break;

            case '?':
                break;

            default:
                break;
        }
    }


}

int main(int argc, char *argv[]) {

    // Query the connected modules.
    moduleManager.queryModules();

    // Generate the initial game state.
    gameManager.generateGameState();

    // Transmit the game state to each of the modules.
    moduleManager.transmitGameState(gameManager.getGameState());

    // Parse the command line arguments.
    parseOptions(argc, argv);

    // Determine if the application is run with root privledges.
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

    bcm2835_spi_begin();

    // 1024 = 244.140625kHz on Rpi2, 390.625kHz on RPI3
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);

    // BCM2835_SPI_MODE0 CPOL = 0, CPHA = 0
    // BCM2835_SPI_MODE1 CPOL = 0, CPHA = 1
    // BCM2835_SPI_MODE2 CPOL = 1, CPHA = 0
    // BCM2835_SPI_MODE3 CPOL = 1, CPHA = 1
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

    // Set chip select polarity low.
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, 0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS2, 0);

    // BCM2835_SPI_BIT_ORDER_LSBFIRST, BCM2835_SPI_BIT_ORDER_MSBFIRST
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST);

    for (uint32_t i = 0; i < 1000; i++) {
        // Send on signal.
        std::cout << "On Signal..." << std::endl;

        // Communicate with module 0.
        bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
        bcm2835_spi_transfer(0x0F);

        // Communicate with module 1.
        bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
        bcm2835_spi_transfer(0x0F);

        // Sleep for 1 second.
        sleep(1);

        // Send off signal.
        std::cout << "Off Signal..." << std::endl;

        // Communicate with module 0.
        bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
        bcm2835_spi_transfer(0xF0);

        // Communicate with module 1.
        bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
        bcm2835_spi_transfer(0xF0);

        sleep(1);
    }

    bcm2835_spi_end();

    std::cout << "Stopping SPI Communication Test..." << std::endl;

    std::cout << "Starting GPIO Communication Test..." << std::endl;

    for (uint32_t i = 0; i < 1000; i++) {
        bcm2835_gpio_write(RPI_GPIO_P1_03, 0);
        bcm2835_gpio_write(RPI_GPIO_P1_05, 0);
        bcm2835_gpio_write(RPI_GPIO_P1_07, 0);


        bcm2835_gpio_write(RPI_GPIO_P1_03, 1);
        bcm2835_gpio_write(RPI_GPIO_P1_05, 1);
        bcm2835_gpio_write(RPI_GPIO_P1_07, 1);
    }

    std::cout << "Stopping GPIO Communication Test..." << std::endl;

    std::cout << "bcm2835_close()" << std::endl;
    if (!bcm2835_close()) {
        std::cout << "bcm2835_close() failed..." << std::endl;
        return 1;
    }
    return 0;
}
