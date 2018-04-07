#ifndef SPI_MANAGER_H
#define SPI_MANAGER_H

#include <Definitions.h>

/**
 *
 */
class SPIManager final {
private:
public:

    /**
     *
     */
    SPIManager();

    /**
     *
     * @param buffer[3]
     */
    void transfer(uint8_t buffer[3]);

    /**
     *
     * @param buffer[3]
     */
    void queueMessage(uint8_t buffer[3]);

    /**
     * Called whenever a message is received from the controller unit.
     * @param callback
     */
    void setTransmitCallback(void (*callback)(const SPIMessage &msg, uint16_t data));

    /**
     * Called whenever a receive request is received.
     *
     * This method should fill
     * @param callack
     */
    void setReceiveCallback(void (*callack)(ReceiveOpCodes op), uint16_t& data);

};
#endif // SPI_MANAGER_H
