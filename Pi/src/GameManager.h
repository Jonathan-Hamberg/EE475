#ifndef DEFUSER_GAMEMANAGER_H
#define DEFUSER_GAMEMANAGER_H

#include "Definitions.h"
#include "GameState.h"
#include "ModuleManager.h"

/**
 *
 */
class GameManager final {
private:
    /// Current game state.
    GameState gameState;

    /// Reference to the module manager.
    ModuleManager *moduleManager;
public:

    /**
     *
     * @param moduleManager
     */
    explicit GameManager(ModuleManager *moduleManager);

    /**
    *
    */
    void generateGameState();

    /**
     *
     * @return
     */
    const GameState& getGameState();

    /**
     * Changes state on all modules and performs any changes required to change state.
     * @param state
     */
    void changeGameMode(GameMode mode);

    /**
     *
     */
    void processMessage();

    /**
     *
     */
    void updateTime();

    /**
     *
     */
    void playSound();

    /**
     *
     */
    void onStrike();

    /**
     *
     */
    void onDefuseModule();

    /**
     *
     */
    void onDefuseAll();

    /**
     *
     */
    void OnDetonate();
};


#endif //DEFUSER_GAMEMANAGER_H
