#include <Arduino.h>
#include "Maze.h"

#define MAZE_BLACK 0
#define MAZE_RED 655360
#define MAZE_GREEN 2560
#define MAZE_BLUE 10
#define MAZE_YELOW 657920
#define MAZE_WHITE 657930

#define BLINK_TIME 350

Maze::Maze(ShiftOut * out, ButtonManager * buttons, Timer * t, RGB_LED * led, Adafruit_NeoPixel * strip, GameState * game): buttonListener(this), timerListener(this) {
  this->out = out;
  this->buttons = buttons;
  this->t = t;
  this->led = led;
  this->strip = strip;
  this->game = game;
  this->buttons->attachAllOnPress(&buttonListener);
  this->buttons->attachAllOnRelease(&buttonListener);
  this->t->attachTimer(&timerListener, 0, 0);
}

void Maze::init(uint32_t seed) {
  r.reseed(seed);
  board.setMap(r.next());
  uint8_t goal_x = r.next() % 6;
  uint8_t goal_y = r.next() % 6;
  uint8_t start_x = r.next() % 6;
  uint8_t start_y = r.next() % 6;

  while ((start_x >= goal_x - 1 && start_x <= goal_x + 1) && (start_y >= goal_y - 1 && start_y <= goal_y + 1)) {
    Serial.println("Miss");
    start_x = r.next() & 6;
    start_y = r.next() & 6;
  }
  board.setGoal(goal_x, goal_y);
  //board.setGoal(board.getInd1().x, board.getInd1().y);
  board.setPlayer(start_x, start_y);
  /*
  updateDisplay(1);
  while(true) {
    updateDisplay(0);
    while(!Serial.available());
    char dir = Serial.read();
    dir = dir - '0';
    board.move(dir);
  }
  */
}

void Maze::demo() {
  muteOutput();
  setMode(ModuleMode::Demo);
}

void Maze::start() {
  setMode(ModuleMode::Armed);
  updateDisplay(0);
}

void Maze::explode() {
  muteOutput();
  setMode(ModuleMode::Off);
}

void Maze::updateDisplay(unsigned int cycle) {
  for (int i = 0; i < 36; i++) {
    strip->setPixelColor(i, MAZE_BLACK);
  }
  /*
  Serial.println("Updating display");
  Serial.print(board.getInd1().x);
  Serial.print(", ");
  Serial.print(board.getInd1().y);
  Serial.println(" GREEN");
  Serial.print(board.getInd2().x);
  Serial.print(", ");
  Serial.print(board.getInd2().y);
  Serial.println(" GREEN");
  Serial.print(board.getGoal().x);
  Serial.print(", ");
  Serial.print(board.getGoal().y);
  Serial.println(" RED");
  Serial.print(board.getPlayer().x);
  Serial.print(", ");
  Serial.print(board.getPlayer().y);
  Serial.println(" BLUE");
  */
  setPoint(MAZE_GREEN, board.getInd1().x, board.getInd1().y);
  setPoint(MAZE_GREEN, board.getInd2().x, board.getInd2().y);
  if (!(checkPoints(board.getGoal(), board.getInd1()) || checkPoints(board.getGoal(), board.getInd2()))) {
    setPoint(MAZE_RED, board.getGoal().x, board.getGoal().y);
  }
  if(!(cycle & 1)) {
    setPoint(MAZE_BLUE, board.getPlayer().x, board.getPlayer().y);
  } else {
    setPoint(MAZE_RED, board.getGoal().x, board.getGoal().y);
  }
  strip->show();
}

void Maze::muteOutput() {
  for (int i = 0; i < 36; i++) {
    strip->setPixelColor(i, MAZE_BLACK);
  }
  strip->show();
}

void Maze::setPoint(uint32_t color, uint8_t x, uint8_t y) {
  unsigned int n = (5 - x) + 6 * y;
  //Serial.println(n);
  strip->setPixelColor(n, color);
}

bool Maze::checkPoints(MazeBoard::point a, MazeBoard::point b) {
  return a.x == b.x && a.y == b.y;
}

void Maze::setMode(ModuleMode mode) {
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
}

Maze::MazeButtonListener::MazeButtonListener(Maze * parent) {
  this->parent = parent;
}

void Maze::MazeButtonListener::onEvent(Button * caller, ButtonEvent event) {
  if (event == ButtonEvent::RELESE) {
    parent->out->set(caller->getID(), 0, false);
  } else {
    if (parent->mode == ModuleMode::Armed) {
      parent->out->set(caller->getID(), 0, true);
      bool good = parent->board.move(caller->getID());
      if (good) {
        if (parent->checkPoints(parent->board.getPlayer(), parent->board.getGoal())) {
          parent->setMode(ModuleMode::Disarmed);
        }
      } else {
        //send strike
      }
      parent->updateDisplay(0);
    }
  }
}

Maze::MazeTimerListener::MazeTimerListener(Maze * parent) {
  this->parent = parent;
}

unsigned int Maze::MazeTimerListener::onEvent(Timer *caller, unsigned int id, unsigned int calls) {
  if (parent->mode == ModuleMode::Armed) {
    parent->updateDisplay(calls);
  } else if (parent->mode == ModuleMode::Demo) {
    for (int i = 0; i < 4; i++) {
      parent->out->set(i, 0, false);
    }
    parent->out->set(calls % 4, 0, true);
  }
  return BLINK_TIME;
}


