#include "HorizontalWire.h"
#include <Arduino.h>

HorizontalWire::HorizontalWire(ShiftIn *in, ShiftOut *out, RGB_LED *led,
                               ButtonManager *buttons,
                               ArduinoGameManager *gameManager)
    : buttonListener(this) {
  this->in = in;
  this->out = out;
  this->led = led;
  this->buttons = buttons;
  this->gameManager = gameManager;
  this->gameManager->setGameModule(this);
  this->game = &gameManager->getGameState();

  init(r.next());
  muteOutput();
  setMode(ModuleMode::Off);
  buttons->attachAllOnPress(&buttonListener);
  buttons->attachAllOnRelease(&buttonListener);
  numWires = 0;
  target = -1;
}

void HorizontalWire::init(uint32_t seed) {
  r.reseed(seed);
  for (unsigned int i = 0; i < HORIZONTAL_WIRE_LENGTH; i++) {
    uint8_t color = r.next() % 5;
    switch (color) {
      case 0:
        color = LED_BLACK;
        break;
      case 1:
        color = LED_RED;
        break;
      case 2:
        color = LED_BLUE;
        break;
      case 3:
        color = LED_YELLOW;
        break;
      case 4:
        color = LED_WHITE;
        break;
    }
    colors[i] = color;
  }
}

void HorizontalWire::start() {
  updateIO();
  Serial.print("Starting with numWires = ");
  Serial.println(numWires);
  if (numWires >= 3) {
    setMode(ModuleMode::Armed);
  } else {
    setMode(ModuleMode::Disarmed);
  }
  updateColors();
  findTarget();
}

void HorizontalWire::demo() {
  updateIO();
  setMode(ModuleMode::Demo);
  updateColors();
}

void HorizontalWire::explode() {
  muteOutput();
  setMode(ModuleMode::Off);
}

void HorizontalWire::setMode(ModuleMode mode) {
  this->mode = mode;
  switch (mode) {
    case ModuleMode::Armed:
      Serial.println("Arming");
      led->set(6, LED_RED);
      break;
    case ModuleMode::Disarmed:
      Serial.println("Disarming");
      led->set(6, LED_GREEN);
      gameManager->disarmModule();
      break;
    case ModuleMode::Demo:
      Serial.println("Entering Demo");
      led->set(6, LED_BLUE);
      break;
    default:
      led->set(6, LED_BLACK);
  }
  return;
}

void HorizontalWire::muteOutput() {
  for (unsigned int i = 0; i < 7; i++) { led->set(i, LED_BLACK); }
}

void HorizontalWire::updateColors() {
  switch (mode) {
    case ModuleMode::Off:
      muteOutput();
      break;
    case ModuleMode::Demo:
      for (unsigned int i = 0; i < HORIZONTAL_WIRE_LENGTH; i++) {
        led->set(i, LED_BLACK);
      }
      for (unsigned int i = 0; i < numWires; i++) {
        led->set(wireIndex[i], LED_GREEN);
      }
      break;
    case ModuleMode::Armed:
      for (unsigned int i = 0; i < HORIZONTAL_WIRE_LENGTH; i++) {
        led->set(i, LED_BLACK);
        // led->set(i, colors[i]);
      }
      for (unsigned int i = 0; i < numWires; i++) {
        uint8_t index = wireMap[wireIndex[i]];
        led->set(index, colors[index]);
      }
      break;
    case ModuleMode::Disarmed:
      for (unsigned int i = 0; i < HORIZONTAL_WIRE_LENGTH; i++) {
        led->set(i, LED_BLACK);
      }
      for (unsigned int i = 0; i < numWires; i++) {
        uint8_t index = wireMap[wireIndex[i]];
        led->set(index, colors[index]);
      }
      break;
    default:
      muteOutput();
  }
}

void HorizontalWire::updateIO() {
  numWires = 0;
  for (unsigned int i = 0; i < HORIZONTAL_WIRE_LENGTH; i++) {
    out->set(i + HORIZONTAL_WIRE_INDEX_OFFSET, 0, 1);
  }

  out->load();

  for (unsigned int i = 0; i < HORIZONTAL_WIRE_LENGTH; i++) {
    wireMap[i] = -1;
    for (unsigned int j = 0; j < HORIZONTAL_WIRE_LENGTH; j++) {
      out->set(j + HORIZONTAL_WIRE_INDEX_OFFSET, 0, 0);
      out->load();
      in->load();
      if (in->getBit(i)) { wireMap[i] = (uint8_t)j; }
      out->set(j + HORIZONTAL_WIRE_INDEX_OFFSET, 0, 1);
      out->load();
    }
  }

  for (unsigned int i = 0; i < HORIZONTAL_WIRE_LENGTH; i++) {
    out->set(i + HORIZONTAL_WIRE_INDEX_OFFSET, 0, 0);
    if (wireMap[i] != -1) {
      wireIndex[numWires] = i;
      numWires++;
      Serial.print("Found Wire ");
      Serial.println(numWires);
    }
  }
  out->load();
}

void HorizontalWire::findTarget() {
  target = -1;
  bool hasRed = false;
  uint8_t numBlue = 0;
  uint8_t lastBlue = 0;
  uint8_t numRed = 0;
  uint8_t lastRed = 0;
  uint8_t numYellow = 0;
  uint8_t numBlack = 0;
  uint8_t numWhite = 0;
  switch (numWires) {
    case 3:
      for (unsigned int i = 0; i < 3; i++) {
        uint8_t color = colors[wireMap[wireIndex[i]]];
        if (color == LED_RED) {
          hasRed = true;
        } else if (color == LED_BLUE) {
          numBlue++;
          lastBlue = i;
        }
      }
      if (!hasRed) {
        target = 1;
      } else if (colors[wireMap[wireIndex[2]]] == LED_WHITE) {
        target = 2;
      } else if (numBlue > 1) {
        target = lastBlue;
      } else {
        target = 2;
      }
      break;

    case 4:
      for (unsigned int i = 0; i < 4; i++) {
        uint8_t color = colors[wireMap[wireIndex[i]]];
        if (color == LED_RED) {
          numRed++;
          lastRed = i;
        } else if (color == LED_BLUE) {
          numBlue++;
        } else if (color == LED_YELLOW) {
          numYellow++;
        }
      }
      if (numRed > 1 && !game->SNisEven()) {
        target = lastRed;
      } else if (colors[wireMap[wireIndex[3]]] == LED_YELLOW && numRed == 0) {
        target = 0;
      } else if (numBlue == 1) {
        target = 0;
      } else if (numYellow > 1) {
        target = 3;
      } else {
        target = 1;
      }
      break;

    case 5:
      for (unsigned int i = 0; i < 5; i++) {
        uint8_t color = colors[wireMap[wireIndex[i]]];
        if (color == LED_RED) {
          numRed++;
        } else if (color == LED_YELLOW) {
          numYellow++;
        } else if (color == LED_BLACK) {
          numBlack++;
        }
      }
      if (colors[wireMap[wireIndex[4]]] == LED_BLACK && !game->SNisEven()) {
        target = 3;
      } else if (numRed == 1 && numYellow > 1) {
        target = 0;
      } else if (numBlack == 0) {
        target = 1;
      } else {
        target = 0;
      }
      break;

    case 6:
      for (unsigned int i = 0; i < 6; i++) {
        uint8_t color = colors[wireMap[wireIndex[i]]];
        if (color == LED_YELLOW) {
          numYellow++;
        } else if (color == LED_WHITE) {
          numWhite++;
        } else if (color == LED_RED) {
          numRed++;
        }
      }
      if (numYellow == 0 && !game->SNisEven()) {
        target = 2;
      } else if (numYellow == 1 && numWhite > 1) {
        target = 3;
      } else if (numRed == 0) {
        target = 5;
      } else {
        target = 3;
      }
      break;
    default:
      break;
  }
}

HorizontalWire::HorizontalWireButtonListener::HorizontalWireButtonListener(
    HorizontalWire *parent) {
  this->parent = parent;
}

void HorizontalWire::HorizontalWireButtonListener::onEvent(Button *caller,
                                                           ButtonEvent event) {
  if (this->parent->mode == ModuleMode::Demo) {
    Serial.println("Demo Event");
    if (event == ButtonEvent::PRESS) {
      parent->led->set(caller->getID(), LED_GREEN);
    } else {
      parent->led->set(caller->getID(), LED_BLACK);
    }
  } else if (this->parent->mode == ModuleMode::Armed &&
             event == ButtonEvent::RELEASE) {
    Serial.println("Game Event");
    if (caller->getID() == parent->wireIndex[parent->target]) {
      parent->setMode(ModuleMode::Disarmed);
      // send disarmed
    } else {
      // send strike
      parent->gameManager->strikeModule();
    }
  }
}
