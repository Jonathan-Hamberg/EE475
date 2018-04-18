#include <SPIManager.h>

SPIManager::SPIManager() {
}

void SPIManager::setTransmitCallback(void (*callback)(const SPIReceiveMessage &msg, uint16_t data)) {
    this->transmitCallback = callback;
}

void SPIManager::setReceiveCallback(void (*callback)(ReceiveOpCodes op, uint16_t& data)) {
    this->receiveCallback = callback;
}
