#include <GameState.h>
#include <SPIManager.h>
#include "Dummy.h"

GameState game;
Dummy module(&game);
SPIManager spiManager(&game);

void setup() {
  Serial.begin(115200);
  module.start();
  spiManager.setModuleType(ModuleType::Control);
}

void loop() {
  while (Serial.available()) {
    switch (Serial.read()) {
      case 'a':
        spiManager.strikeModule();
        Serial.println("strikeModule()");
        break;
      case 'b':
        spiManager.disarmModule();
        Serial.println("disarmModule()");
        break;
      case 'c':
        spiManager.playSound(PlaySound::Strike);
        Serial.println("playSound()");
        break;
      case 'd':
        spiManager.startGame();
        Serial.println("startGame()");
        break;
      case 'e':
        spiManager.stopGame();
        Serial.println("stopGame()");
        break;
      default:
        break;
    }
  }
}
