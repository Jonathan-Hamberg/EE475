#include "SPIManager.h"
#include "bcm2835.h"
#include <iostream>

static const uint8_t decoderAddress[4] = {0, 5, 6, 13};

SPIManager::SPIManager() {

    // Initialize the bcm2835 library.
    if (!bcm2835_init()) {
        std::cout << "bcm2835_init() failed..." << std::endl;
        return;
    }

    // Enable the SPI module on the pi zero.
    bcm2835_spi_begin();

    /*
       CM2835_SPI_CLOCK_DIVIDER_65536 = 3.814697260kHz on Rpi2, 6.1035156kHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_32768 = 7.629394531kHz on Rpi2, 12.20703125kHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_16384 = 15.25878906kHz on Rpi2, 24.4140625kHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_8192 = 30.51757813kHz on Rpi2, 48.828125kHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_4096 = 61.03515625kHz on Rpi2, 97.65625kHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_2048 = 122.0703125kHz on Rpi2, 195.3125kHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_1024 = 244.140625kHz on Rpi2, 390.625kHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_512 = 488.28125kHz on Rpi2, 781.25kHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_256 = 976.5625kHz on Rpi2, 1.5625MHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_128 = 1.953125MHz on Rpi2, 3.125MHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_64 = 3.90625MHz on Rpi2, 6.250MHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_32 = 7.8125MHz on Rpi2, 12.5MHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_16 = 15.625MHz on Rpi2, 25MHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_8 = 31.25MHz on Rpi2, 50MHz on RPI3
       BCM2835_SPI_CLOCK_DIVIDER_4 = 62.5MHz on Rpi2, 100MHz on RPI3. Dont expect this speed to work reliably.
       BCM2835_SPI_CLOCK_DIVIDER_2 = 125MHz on Rpi2, 200MHz on RPI3, fastest you can get. Dont expect this speed to work reliably.
       BCM2835_SPI_CLOCK_DIVIDER_1 = 3.814697260kHz on Rpi2, 6.1035156kHz on RPI3, same as 0/65536
    */
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_1024);

    /*
       BCM2835_SPI_MODE0 CPOL = 0, CPHA = 0
       BCM2835_SPI_MODE1 CPOL = 0, CPHA = 1
       BCM2835_SPI_MODE2 CPOL = 1, CPHA = 0
       BCM2835_SPI_MODE3 CPOL = 1, CPHA = 1
     */
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);

    // Chip select is controlled by external decoder.
    bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);

    /*
     * BCM2835_SPI_BIT_ORDER_LSBFIRST
     * BCM2835_SPI_BIT_ORDER_MSBFIRST
     */
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);

    // Set the default chip select.
    address = 0;

    // Set the decoder address bits to be outputs.
    for (unsigned char pin : decoderAddress) {
        bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
    }
}

SPIManager::~SPIManager() {
    if (!bcm2835_close()) {
        std::cout << "bcm2835_close() failed..." << std::endl;
    }
}

void SPIManager::selectCS(uint8_t address) {
    if (address < 0 || address >= 13) {
        std::cout << "Invalid SPI address." << std::endl;
        return;
    }

    this->address = address;

    for (auto addr : decoderAddress) {
        // Write the address bits to the decoder.
        bcm2835_gpio_write(addr, address & uint8_t(1));
        // Find the next bit of the address.
        address >>= 1;
    }
}

void SPIManager::transfer(uint8_t *buffer, uint32_t size) {
    // Transfer the data to the module.
    bcm2835_spi_transfern((char *) buffer, size);

    // Update the extra information fields.
    if (extraInformation.count(address) == 0) {
        // If address not found just put the status byte in.
        extraInformation.emplace(address, buffer[0]);
    } else {
        // Logical or the status to what is currently in the map.
        extraInformation[address] |= buffer[0];
    }
}

bool SPIManager::hasExtraInformation(uint8_t address, ExtraInformation info) {

    // If the address doesn't exist then return false.
    if (extraInformation.count(address) == 0) {
        return false;
    }

    // Return true if if the particular extra info is present.
    if ((extraInformation[address] & (1u << uint8_t(info))) != 0) {
        // Turn off the bit one it has been read.
        extraInformation[address] &= ~(1u << uint8_t(info));

        // Return true if the info was present.
        return true;
    }

    // Return false if the info was not present.
    return false;

}
