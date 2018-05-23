#ifndef DEFUSER_SPIMANAGER_H
#define DEFUSER_SPIMANAGER_H

#include <inttypes.h>
#include <map>
#include <Definitions.h>

/**
 *
 */
class SPIManager final {

private:
    ///
    uint8_t address;

    std::map<uint8_t, uint8_t> extraInformation;

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
     * @param address
     * @return
     */
    bool hasExtraInformation(uint8_t address, ExtraInformation info);
};


#endif //DEFUSER_SPIMANAGER_H
