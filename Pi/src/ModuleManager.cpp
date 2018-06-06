#include "ModuleManager.h"
#include <iostream>
#include <sound/asound.h>

ModuleManager::ModuleManager(SPIManager *manager) {
    this->spiManager = manager;
}

void ModuleManager::queryModules(uint16_t seed) {

    // Limit the number of tries to find the control module.
    uint8_t tries = 0;
    // Used to determine if a control module is present when querying the modules.
    bool foundControlModule;

    std::cout << "Discovering connected modules..." << std::endl;

    do {
        // Clear the module map when querying new modules.
        moduleMap.clear();
        controllAddress = 0xFF;
        foundControlModule = false;

        // Variables used to transmit and receive information from the modules.
        uint8_t buffer[3];

        for (uint8_t i = 0; i < 13; i++) {
            // Select the module chip at address i.
            spiManager->selectCS(i);

            // Seed the global seed.  Create new seed for every possible module address.
            auto moduleSeed = uint16_t(rand());
            bool hasModule = false;

            for (uint8_t j = 0; j < 3 && !hasModule; j++) {
                buffer[0] = uint8_t(OpCode::Seed) | uint8_t(OpCode::ModuleType) << 4u;
                buffer[1] = uint8_t(moduleSeed);
                buffer[2] = uint8_t(moduleSeed >> 8u);

                spiManager->transfer(buffer, 3);

                if (buffer[1] != 0 && buffer[2] == 0xFF) {
                    hasModule = true;
                }
            }

            // If the module has been found then add it to the module map.
            if (hasModule) {
                GameState myModule;

                // Store the module seed.
                myModule.setSeed(moduleSeed);

                // Store the module type.
                myModule.setModuleType(ModuleType(buffer[1]));

                if (ModuleType(buffer[1]) == ModuleType::Control) {
                    foundControlModule = true;
                    controllAddress = i;
                }

                // Add module to the connected map structure.
                moduleMap.emplace(i, myModule);
            }
        }
    } while (!foundControlModule && tries++ < 5);

    // Iterate through all the discovered modules on the SPI bus.
    for (const auto &kv : moduleMap) {

        if (uint8_t(kv.second.getModuleType()) < uint8_t(ModuleType::EnumeSize)) {
            // Print to the console which modules have been discovered.
            std::cout << "Discovered " << ModuleText[uint8_t(kv.second.getModuleType())] << " at address " <<
                      int(kv.first) << "." << std::endl;
        } else {
            // Print to the console which modules have been discovered.
            std::cout << "Discovered unknown module number" << int(kv.second.getModuleType()) << " at address " <<
                      int(kv.first) << "." << std::endl;
        }

    }

    // Determine if the control module was found.
    if (!foundControlModule) {
        std::cout << "ModuleManager: Control module was not found when discovering connected devices." << std::endl;
    }

    std::cout << "Discovering connected modules finished..." << std::endl;
}

void ModuleManager::updateModules(OpCode opCode, uint16_t data) {



    // Transmit through every connected module.
    for (auto &module : moduleMap) {
        // Set the chip select line to send the data.
        spiManager->selectCS(module.first);

        // Update the state of the module in the module map.
        module.second.setField(opCode, data);

        uint8_t buffer[3];
        buffer[0] = uint8_t(opCode);
        buffer[1] = uint8_t(data);
        buffer[2] = uint8_t(data >> 8u);

        // Transmit the data.
        spiManager->transfer(buffer, 3);
    }
}

uint16_t ModuleManager::updateModule(OpCode op, uint16_t data, uint8_t address) {

    spiManager->selectCS(address);

    uint8_t buffer[3];
    buffer[0] = uint8_t(op);
    buffer[1] = uint8_t(data);
    buffer[2] = uint8_t(data >> 8);

    spiManager->transfer(buffer, 3);

    return buffer[1] | (buffer[2] << 8u);
}

uint16_t ModuleManager::updateControlModule(OpCode op, uint16_t data) {


    if (this->controllAddress > 13) {
        // Control module not included, so just return 0.
        std::cout << "updateControlModule() failed because Control module not present." << std::endl;
        return 0;
    } else {
        spiManager->selectCS(controllAddress);

        uint8_t buffer[3];
        buffer[0] = uint8_t(op);
        buffer[1] = uint8_t(data);
        buffer[2] = uint8_t(data >> 8);

        spiManager->transfer(buffer, 3);

        return buffer[1] | (buffer[2] << 8u);
    }
}

PlaySound ModuleManager::getPlaySound(uint8_t address) {
    uint16_t data = updateModule(OpCode::PlaySound, 0, address);
    return PlaySound(data & 0xFF);
}


void ModuleManager::transmitGameState(const GameState &state) {

    uint8_t buffer[3];

    // Transmit game state to all modules.
    updateModules(OpCode::Countdown, state.getCountdownTime());
    updateModules(OpCode::MaxStrike, state.getMaxStrikes());
    updateModules(OpCode::Indicators, state.getIndicators());
    updateModules(OpCode::Ports, state.getPorts());
    updateModules(OpCode::Battery, state.getBatteries());

    const char *sn = state.getSN();
    updateModules(OpCode::Serial, 0);
    updateModules(OpCode::Serial, uint16_t(sn[0]) | (uint16_t(sn[1]) << 8u));
    updateModules(OpCode::Serial, uint16_t(sn[2]) | (uint16_t(sn[3]) << 8u));
    updateModules(OpCode::Serial, uint16_t(sn[4]) | (uint16_t(sn[5]) << 8u));

    // Transmit seeds to each of the individual modules.
    for (const auto &module : moduleMap) {

        // Set the chip select line to send the data.
        spiManager->selectCS(module.first);

        // Transmit the module seed.
        buffer[0] = uint8_t(OpCode::Seed);
        buffer[1] = uint8_t(module.second.getSeed());
        buffer[2] = uint8_t(module.second.getSeed() >> 8u);

        spiManager->transfer(buffer, 3);
    }

    updateModules(OpCode::Mode, uint16_t(state.getGameState()));
}

const std::map<uint8_t, GameState> &ModuleManager::getModules() const {
    return moduleMap;
}

bool ModuleManager::hasExtraInformation(uint8_t address, ExtraInformation info) {
    return spiManager->hasExtraInformation(address, info);
}

bool ModuleManager::controlHasExtraInformation(ExtraInformation info) {
    return hasExtraInformation(this->controllAddress, info);
}

bool ModuleManager::hasControlModule() {
    return controllAddress < 13;
}
