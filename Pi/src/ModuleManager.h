#ifndef DEFUSER_MODULEMANAGER_H
#define DEFUSER_MODULEMANAGER_H

#include <vector>
#include <map>
#include "GameState.h"
#include "Module.h"
#include "SPIManager.h"
#include "Definitions.h"

/**
 *
 */
class ModuleManager {
public:

    /**
     *
     * @param seed
     */
    void setGlobalSeed(uint16_t seed);

    /**
     *
     * @return
     */
    uint16_t getGlgobalSeed();

    /**
     *
     * @param manager
     */
    explicit ModuleManager(SPIManager* manager);

    /**
     *
     */
    void queryModules();

    /**
     *
     */
    void updateModules(TransmitOpCodes opCode, uint16_t data);

    /**
     *
     * @param state
     */
    void transmitGameState(const GameState &state);

private:
    ///
    std::map<uint8_t, Module*> moduleMap;

    ///
    SPIManager* spiManager;

    ///
    uint16_t seed;
};

#endif //DEFUSER_MODULEMANAGER_H
