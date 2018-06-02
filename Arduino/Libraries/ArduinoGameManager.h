#ifndef ARDUINOGAMEMANAGER_H
#define ARDUINOGAMEMANAGER_H

#include "Definitions.h"
#include "GameModule.h"
#include "GameState.h"
#include "SPIManager.h"

class ArduinoGameManager final {
 private:
  SPIManager* spiManager;
  GameModule* gameModule;
  GameState gameState;
  ModuleMode previousMode;

 public:
  ArduinoGameManager(ModuleType type);

  /**
   * The update method is used to determine if the state needs to be changed for
   * the module.  It calls the corrisponding update method on the GameModule
   * interface.
   */
  void update();

  /**
   * Gets the internal game state that is updated by the Host Controller.
   *
   * @return A reference to the game state.
   */
  GameState& getGameState();

  /**
   * Sets the game module that is to be used by the ArduinoGameManager.
   *
   * @param gameModule The gameModule that the ArduinoGameManager updates.
   */
  void setGameModule(GameModule* gameModule);

  /**
   * Set the module type that is reported to the Most Control Sysstem.
   *
   * @param type The ModuleType that the current module is.
   */
  void setModuleType(ModuleType type);

  /**
   * Tells the arduino manager to send a playSound request to the host control
   * system.
   *
   * @param sound The sound that is requested to be played to the host system.
   */
  void playSound(PlaySound sound);

  /**
   * Indicate to the host controll system that a strike has occurred.
   */
  void strikeModule();

  /**
   * Indicate to the host control system that the module has been disarmed.
   */
  void disarmModule();

  /**
   * Indicate to the host control system that the control module start button
   * has been pressed.
   */
  void startGame();

  /**
   * Indicate to the host control system that the control module has stop button
   * has been pressed.
   */
  void stopGame();
};

#endif /* ARDUINOGAMEMANAGER_H */
