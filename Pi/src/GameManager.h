#ifndef DEFUSER_GAMEMANAGER_H
#define DEFUSER_GAMEMANAGER_H

#include "Definitions.h"
#include "GameState.h"
#include "ModuleManager.h"
#include <chrono>

/**
 *
 */
class GameManager final {

public:

    /**
     *
     * @param moduleManager
     */
    explicit GameManager(ModuleManager *moduleManager);

    /**
    *
    */
    void generateGameState(uint16_t seed, uint16_t countdown, uint8_t maxStrikes);

    /**
     *
     * @return
     */
    const GameState& getGameState();

    void update();

    void updateStateOff();

    void updateStateDemo();

    void updateStateArmed();

    void updateStateDisarmed();

    /**
     * Changes state on all modules and performs any changes required to change state.
     * @param state
     */
    void changeGameMode(ModuleMode mode);

    /**
     *
     */
    void onStart();

    /**
     *
     */
    void onStop();

    /**
     *
     */
    void playSound(PlaySound sound);

    /**
     *
     */
    void onStrike(uint8_t address);

    /**
     *
     */
    void onDisarm(uint8_t address);

    /**
     *
     */
    void onDisarmAll();

    /**
     *
     */
    void onDetonate();

private:

    /// Current game state.
    GameState gameState;

    /// Reference to the module manager.
    ModuleManager *moduleManager;

    ModuleMode currentMode = ModuleMode::Off;

    uint8_t disarmCounter;

    uint8_t numConnectedModules;

    std::chrono::system_clock::time_point stopTime;

    uint16_t seed;
    uint16_t countdown;
    uint8_t maxStrikes;
};


#endif //DEFUSER_GAMEMANAGER_H
