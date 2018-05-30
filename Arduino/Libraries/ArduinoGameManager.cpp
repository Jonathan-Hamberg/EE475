#include "ArduinoGameManager.h"

ArduinoGameManager::ArduinoGameManager() : spiManager(&gameState) {
  this->gameModule = nullptr;
}

GameState& ArduinoGameManager::getGameState() {
  return this->gameState;
}

void ArduinoGameManager::setGameModule(GameModule* gameModule) {
  this->gameModule = gameModule;
}

void ArduinoGameManager::ArduinoGameManager::update() {
  // If the state changed then call the appropriate GameModule method.
  if (gameState.hasModeChanged() && gameState.getGameState() != previousMode) {}

  // Determine if the game module has actually been set.
  if(gameModule == nullptr) {
      return;
  }

  // Determine what to do to the game module if a new state is sent.
  switch (gameState.getGameState()) {
    case ModuleMode::Off:
      // Put the game module in demo mode.
          gameModule->demo();
      break;
    case ModuleMode::Demo:
      // Put the game module in demo mode.
      gameModule->demo();
      break;
    case ModuleMode::Armed:
      // Initialize the game module.
      gameModule->init(gameState.getSeed());
      // Start the game module.
      gameModule->start();
      break;
    case ModuleMode::Disarmed:
      // Don't do anything if the module is disarmed.
      break;
    case ModuleMode::Explode:
      gameModule->explode();
      break;
  };
}

void ArduinoGameManager::setModuleType(ModuleType type) {
  spiManager.setModuleType(type);
}

void ArduinoGameManager::playSound(PlaySound sound) {
  spiManager.playSound(sound);
}

void ArduinoGameManager::strikeModule() {
  spiManager.strikeModule();
}

void ArduinoGameManager::disarmModule() {
  spiManager.disarmModule();
}

void ArduinoGameManager::startGame() {
  spiManager.disarmModule();
}

void ArduinoGameManager::stopGame() {
  spiManager.stopGame();
}
