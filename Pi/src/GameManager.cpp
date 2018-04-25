#include "GameManager.h"

GameManager::GameManager(ModuleManager *moduleManager) {
    this->moduleManager = moduleManager;
}

void GameManager::generateGameState() {
    this->gameState = GameState();

    // Set the indicator.
    gameState.setIndicators(uint16_t(rand()));
    // TODO(jrh) find way to pass countdown time.
    gameState.setCountdownTime(32);
    gameState.setPorts(uint8_t(rand()));
    // TODO(jrh) find way to pass max strikes.
    gameState.setMaxStrikes(3);
}

const GameState &GameManager::getGameState() {
    return this->gameState;
}

void GameManager::changeGameMode(GameMode mode) {
    // Do nothing if the state is not changing.
    if(mode == GameMode(this->gameState.getGameState())) {
        return;
    }

    // Update all the individual modules.
    this->moduleManager->updateModules(TransmitOpCodes::Mode, uint16_t(mode));

    // Set the new game mode.
    this->gameState.setGameState(mode);
    up
}

void GameManager::processMessage() {

}

void GameManager::updateTime() {

}

void GameManager::playSound() {

}

void GameManager::onStrike() {

}

void GameManager::onDefuseModule() {

}

void GameManager::onDefuseAll() {

}

void GameManager::OnDetonate() {

}







