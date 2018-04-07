#include "GameManager.h"

GameManager::GameManager(SPIManager* spiManager) {
    this->spiManager = spiManager;
}

const GameState& GameManager::getGameState() {
    return gameState;
}

void GameManager::playSound(PlaySound sound) {
}
