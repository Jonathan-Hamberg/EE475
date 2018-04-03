#include "Module.h"
#include "bcm2835.h"
void Module::transfer(uint8_t *buffer) const {
    // TODO(jrh) set CS using gpio pins and decoder.
    // using the following id this->moduleID;

    // Transfer message to the module.
    bcm2835_spi_transfern((char*)buffer, 3);
}
