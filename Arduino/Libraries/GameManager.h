#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameState.h"
#include "SPIManager.h"

/**
 *
 */
class GameManager final {
private:
    ///
    GameState gameState;

    ///
    SPIManager *spiManager;

public:

    /**
     *
     * @param spiManager
     */
    GameManager(SPIManager *spiManager);

    /**
     * @return
     */
    const GameState& getGameState();

    /**
     *
     *
     * @param sound The sound that is desired to be played.
     */
    void playSound(PlaySound sound);
}
#endif // GAME_MANAGER
