#include "GameManager.h"


GameManager::GameManager(ModuleManager *moduleManager) {
    this->moduleManager = moduleManager;
}

void GameManager::generateGameState() {
    this->gameState = GameState();
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
    this->gameState.setGameStat(uint8_t(mode));
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







