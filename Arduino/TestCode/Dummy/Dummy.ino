#include <GameState.h>
#include <SPIManager.h>
#include "Dummy.h"

GameState game;
Dummy module(&game);
SPIManager spiManager(&game);

void setup() {
  Serial.begin(115200);
  module.start();
  spiManager.setModuleType(ModuleType::Mazes);
  spiManager.playSound(PlaySound::Sound3);
  spiManager.strikeModule();
}

void loop() {
}
