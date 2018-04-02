#include <iostream>
#include "bcm2835.h"
#include <unistd.h>

int main() {
    if (geteuid() == 0) {
        if (!bcm2835_init()) {
            return 1;
        }

        if (!bcm2835_close()) {
            return 1;
        }
    } else {
        std::cout << "You need to be root to properly run this test program" << std::endl;
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

    for (uint32_t i = 0; i < 10; i++) {
        // Send off signal.

        // Communicate with module 0.
        bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
        bcm2835_spi_transfer(0x0);

        // Communicate with module 1.
        bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
        bcm2835_spi_transfer(0x0);

        // Communicate with module 2.
        bcm2835_spi_chipSelect(BCM2835_SPI_CS2);
        bcm2835_spi_transfer(0x0);


        // Sleep for 1 second.
        sleep(1);

        // Send on signal.
        // Communicate with module 0.
        bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
        bcm2835_spi_transfer(0x0);


        // Communicate with module 1.
        bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
        bcm2835_spi_transfer(0x1);

        // Communicate with module 1.
        bcm2835_spi_chipSelect(BCM2835_SPI_CS2);
        bcm2835_spi_transfer(0x1);

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
    return 0;
}
