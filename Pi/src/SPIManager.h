#ifndef DEFUSER_SPIMANAGER_H
#define DEFUSER_SPIMANAGER_H

#include <inttypes.h>
#include <Definitions.h>

/**
 *
 */
class SPIManager final {

private:
    ///
    uint8_t CS;

public:
    /**
     *
     */
    SPIManager();

    /**
     *
     */
    ~SPIManager();

    /**
     *
     * @param address
     */
    void selectCS(uint8_t address);

    /**
     *
     * @param buffer
     * @param size
     */
    void transfer(uint8_t* buffer, uint32_t size);
};


#endif //DEFUSER_SPIMANAGER_H
