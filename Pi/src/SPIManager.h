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

    ///
    void (*callback)(const SPIMessage&);

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

    /**
     *
     * @param callback
     */
    void setCallback(void (*callback)(const SPIMessage&));

};


#endif //DEFUSER_SPIMANAGER_H
