#include <Arduino.h>
#include "SimonSays.h"


#define LONG_DELAY 1500
#define SHORT_DELAY 250

#define VOWEL_MAP {BLUE_INDEX, RED_INDEX, YELLOW_INDEX, GREEN_INDEX, YELLOW_INDEX, GREEN_INDEX, BLUE_INDEX, RED_INDEX, GREEN_INDEX, RED_INDEX, YELLOW_INDEX, BLUE_INDEX}
#define NO_VOWEL_MAP {BLUE_INDEX, YELLOW_INDEX, GREEN_INDEX, RED_INDEX, RED_INDEX, BLUE_INDEX, YELLOW_INDEX, GREEN_INDEX, YELLOW_INDEX, GREEN_INDEX, BLUE_INDEX, RED_INDEX}

SimonSays::SimonSays(ShiftIn * in, ShiftOut * out, RGB_LED * led, ButtonManager * buttons, Timer * t, ArduinoGameManager * gameManager): buttonListener(this) , timerListener(this){
  this->in = in;
  this->out = out;
  this->led = led;
  this->buttons = buttons;
  this->t = t;
  this->gameManager = gameManager;
  this->gameManager->setGameModule(this);
  this->game = &gameManager->getGameState();
  currentTimer = nullptr;
  init(r.next());
  muteOutput();
  setMode(ModuleMode::Off);
  buttons->attachAllOnPress(&buttonListener);
  buttons->attachAllOnRelease(&buttonListener);
}

void SimonSays::init(uint32_t seed) {
  r.reseed(seed);
  for (int i = 0; i < SIMON_SAYS_LENGTH; i++) {
    colors[i] = r.next() % 4;
  }
  muteOutput();
  setMode(ModuleMode::Off);
}

void SimonSays::start() {
  muteOutput();
  setMode(ModuleMode::Armed);
  t->detachTimer(currentTimer);
  currentTimer = t->attachTimer(&timerListener, 0, 0);
  count = 0;
  count2 = 0;
  displayCount = 0;
}

void SimonSays::demo() {
  muteOutput();
  setMode(ModuleMode::Demo);
  t->detachTimer(currentTimer);
  currentTimer = t->attachTimer(&timerListener, 0, 0);
  count = 3;
  displayCount = 0;
  for (int i = 0; i < 4; i++) {
    colors[i] = i;
  }
}

void SimonSays::explode() {
  muteOutput();
  setMode(ModuleMode::Off);
  t->detachTimer(currentTimer);
  currentTimer = nullptr;
}

SimonSays::~SimonSays() {}

void SimonSays::setMode(ModuleMode mode) {
  this->mode = mode;
  switch (mode) {
    case ModuleMode::Armed:
      led->set(0, LED_RED);
    break;
    case ModuleMode::Disarmed:
      led->set(0, LED_GREEN);
    break;
    case ModuleMode::Demo:
      led->set(0, LED_BLUE);
    break;
    default: break;
  }
  return;
}

void SimonSays::muteOutput() {
  led->set(0, LED_BLACK);
  out->set(RED_INDEX, 0, false);
  out->set(BLUE_INDEX, 0, false);
  out->set(GREEN_INDEX, 0, false);
  out->set(YELLOW_INDEX, 0, false);
}

SimonSays::SimonSaysButtonListener::SimonSaysButtonListener(SimonSays *parent) {
  this->parent = parent;
}

void SimonSays::SimonSaysButtonListener::onEvent(Button * caller, ButtonEvent event) {
  //Serial.println("Called");
  //setButtons();
  switch (parent->mode) {
    case ModuleMode::Off:
      offAction(caller, event);
    break;
    case ModuleMode::Demo:
      demoAction(caller, event);
    break;
    case ModuleMode::Armed:
      armedAction(caller, event);
    break;
    case ModuleMode::Disarmed:
      disarmedAction(caller, event);
      parent->gameManager->disarmModule();
    break;
    default:
    break;
  }
}

bool SimonSays::SimonSaysButtonListener::setButtons() {
  bool down = true;
   for (int i = 0; i < 4; i++) {
    bool b = parent->buttons->getButton(i)->getValue();
    parent->out->set(i, 0, b);
    if (b) {
      down = false;
      //Serial.print(i);
      //Serial.println(" not down");
    } else {
      //Serial.print(i);
      //Serial.println(" down");
    }
   }
   return down;
}

void SimonSays::SimonSaysButtonListener::offAction(Button * caller, ButtonEvent event) {
    (void)(caller);
    (void)(event);
}

void SimonSays::SimonSaysButtonListener::demoAction(Button * caller, ButtonEvent event) {
    (void)(caller);
    (void)(event);
}

void SimonSays::SimonSaysButtonListener::armedAction(Button * caller, ButtonEvent event) {
    (void)(event);
  bool allDown = setButtons();
  if (event == ButtonEvent::PRESS) {
    //Serial.println("Button Pressed");
    parent->t->detachTimer(parent->currentTimer);
    parent->currentTimer = nullptr;

    uint8_t vowelMap[] = VOWEL_MAP;
    uint8_t noVowelMap[] = NO_VOWEL_MAP;
    uint16_t index = caller->getID() + 4 * parent->game->getStrikes();
    bool hit;
    if (parent->game->SNhasVowel()) {
      hit = parent->colors[parent->count2] == vowelMap[index];
    } else {
      hit = parent->colors[parent->count2] == noVowelMap[index];
    }

    if (hit) {
      //play button sound

      if (parent->count == parent->count2) {
        parent->count++;
        parent->count2 = 0;

        if (parent->count == SIMON_SAYS_LENGTH) {
          parent->setMode(ModuleMode::Disarmed);
        }
      } else {
        parent->count2++;
      }
    } else {
      parent->count = 0;
      parent->count2 = 0;
      parent->gameManager->strikeModule();
    }

  } else if (event == ButtonEvent::RELEASE) {
    if (allDown && parent->currentTimer == nullptr) {
      parent->currentTimer = parent->t->attachTimer(&(parent->timerListener), LONG_DELAY, 0);
      parent->displayCount = 0;
    }
  }
}

void SimonSays::SimonSaysButtonListener::disarmedAction(Button * caller, ButtonEvent event) {
    (void)(event);
  parent->out->set(caller->getID(), 0, false);
}

SimonSays::SimonSaysTimerListener::SimonSaysTimerListener(SimonSays * parent) {
  this->parent = parent;
}

unsigned int SimonSays::SimonSaysTimerListener::onEvent(Timer * caller, unsigned int id, unsigned int calls) {
    (void)(caller);
    (void)(id);
    (void)(calls);
  parent->count2 = 0;

  if ((parent->displayCount & 1) == 1) {
    for (int i = 0; i < 4; i++) {
      parent->out->set(i, 0, false);
    }
  } else {
    parent->out->set(parent->colors[parent->displayCount >> 1], 0, true);
  }
  parent->displayCount++;

  if (parent->displayCount == parent->count * 2 + 2) {
    parent->displayCount = 0;
    return LONG_DELAY;
  }
  return SHORT_DELAY;
}


