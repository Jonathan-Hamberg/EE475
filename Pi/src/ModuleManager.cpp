#include "ModuleManager.h"
#include "Definitions.h"



ModuleManager::ModuleManager(SPIManager *manager) {
    this->spiManager = manager;
}

void ModuleManager::setGlobalSeed(uint16_t seed) {
    this->seed = seed;
}

uint16_t ModuleManager::getGlgobalSeed() {
    return this->seed;
}

void ModuleManager::queryModules() {

    // Delete previous modules.
    for(const auto &kv : moduleMap) {
        delete kv.second;
    }

    // Clear the module map when querying new modules.
    moduleMap.clear();

    // Variables used to transmit and receive information from the modules.
    uint8_t buffer[3];
    uint8_t *address = buffer;
    auto data = (uint16_t *) (buffer + 1);

    srand(this->getGlgobalSeed());

    for (uint8_t i = 0; i < 12; i++) {
        // Select the moudle chip at address i.
        spiManager->selectCS(i);

        // Seed the global seed.  Create new seed for every possible module address.
        uint16_t seed = uint16_t(rand());

        // Construct the transmit structure message.
        *address = uint8_t(TransmitOpCodes::Ignored) | uint8_t(ReceiveOpCodes::ModuleType) << 4u;
        *data = 0;
        spiManager->transfer(buffer, 3);

        // Determine if the queried message contains the correct response.
        bool hasModule = (*address >> 4u) == uint8_t(ReceiveOpCodes::ModuleType) &&
                         (*address ^ (*data & 0xFFu)) == (*data >> 8u);

        // Don't query the module again if it happened to return the correct
        // data during the first message transmission.
        if (!hasModule) {
            // Transfer the request to the module and receive the data.
            spiManager->transfer(buffer, 3);

            // Determine if the queried message contains the correct response.
            hasModule = (*address >> 4u) == uint8_t(ReceiveOpCodes::ModuleType) &&
                        (*address ^ (*data & 0xFFu)) == (*data >> 8u);
        }

        // If the module has been found then add it to the module map.
        if(hasModule) {
            auto *myModule = new Module();
            myModule->setSeed(seed);
            moduleMap.emplace(i, myModule);
        }
    }
}

GameState ModuleManager::generateGameState() {
    return GameState();
}

void ModuleManager::updateMode() {

}

void ModuleManager::transmitGameState(const GameState &state) {

    uint8_t buffer[3];
    uint8_t *address = buffer;
    auto data = (uint16_t *) (buffer + 1);

    for (const auto &module : moduleMap) {
        // Set the chip select line to send the data.
        spiManager->selectCS(module.first);

        // Transmit the mode to the module.
        *address = uint8_t(TransmitOpCodes::Mode);
        *data = uint8_t(GameMode::Inactive);
        spiManager->transfer(buffer, 3);

        // Transmit the module seed.
        *address = uint8_t(TransmitOpCodes::Seed);
        *data = module.second->getSeed();
        spiManager->transfer(buffer, 3);

        // Transmit the countdown time.
        buffer[0] = uint8_t(TransmitOpCodes::Countdown);
        *data = state.getCountdownTime();
        spiManager->transfer(buffer, 3);

        // Transmit the max strikes.
        *address = uint8_t(TransmitOpCodes::MakStrikes);
        *data = state.getMaxStrikes();
        spiManager->transfer(buffer, 3);

        // Transmit the indicators.
        *address = uint8_t(TransmitOpCodes::Indicators);
        *data = state.getIndicators();
        spiManager->transfer(buffer, 3);

        // Transmit the ports.
        *address = uint8_t(TransmitOpCodes::Ports);
        *data = state.getPorts();
        spiManager->transfer(buffer, 3);

        // Transmit the battery.
        *address = uint8_t(TransmitOpCodes::Battery);
        *data = state.getBatteries();
        spiManager->transfer(buffer, 3);
    }

}




