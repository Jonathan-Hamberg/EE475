#ifndef SPI_MANAGER_H
#define SPI_MANAGER_H

#include <Definitions.h>

/**
 *
 */
class SPIManager final {
public:

    /**
     *
     */
    SPIManager();

    /**
     * Called whenever a message is received from the controller unit.
     * @param callback
     */
    void setTransmitCallback(void (*callback)(const SPIReceiveMessage &msg, uint16_t data));

    /**
     * Called whenever a receive request is received.
     *
     * This method should fill
     * @param callack
     */
    void setReceiveCallback(void (*callback)(ReceiveOpCodes op, uint16_t& data));
private:

    void (*transmitCallback)(const SPIReceiveMessage& msg, uint16_t data);
    void (*receiveCallback)(ReceiveOpCodes op, uint16_t& data);
};
#endif // SPI_MANAGER_H
