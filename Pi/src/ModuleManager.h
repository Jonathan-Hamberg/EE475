#ifndef DEFUSER_MODULEMANAGER_H
#define DEFUSER_MODULEMANAGER_H

#include <vector>
#include <map>
#include <queue>
#include "GameState.h"
#include "SPIManager.h"
#include "Definitions.h"

/**
 *
 */
class ModuleManager {
public:

    /**
     *
     * @param manager
     */
    explicit ModuleManager(SPIManager* manager);

    /**
     *
     */
    void queryModules(uint16_t seed);

    /**
     *
     * @return
     */
    const std::map<uint8_t, GameState>& getModules() const;

    /**
     *
     */
    void updateModules(OpCode opCode, uint16_t data);

    /**
     *
     * @param data
     * @param address
     */
    uint16_t updateModule(OpCode op, uint16_t data, uint8_t address);

    /**
     *
     * @param op
     * @param data
     * @return
     */
    uint16_t updateControlModule(OpCode op, uint16_t data);

    bool hasControlModule();

    /**
     *
     * @param info
     * @return
     */
    bool controlHasExtraInformation(ExtraInformation info);

    PlaySound getPlaySound(uint8_t address);
    /**
     *
     * @param state
     */
    void transmitGameState(const GameState &state);

    /**
     *
     * @param address
     * @param info
     * @return
     */
    bool hasExtraInformation(uint8_t address, ExtraInformation info);

private:

    ///
    std::map<uint8_t, GameState> moduleMap;

    ///
    SPIManager* spiManager;

    std::queue<std::pair<uint8_t, SPIReceiveMessage>> messageQueue;

    // Contains the address of the control module.
    uint8_t controllAddress = 0xFF;
};

#endif //DEFUSER_MODULEMANAGER_H
