#include <Arduino.h>
#include "Maze.h"

#define MAZE_BLACK 0
#define MAZE_RED 655360
#define MAZE_GREEN 2560
#define MAZE_BLUE 10
#define MAZE_YELOW 657920
#define MAZE_WHITE 657930

Maze::Maze(ShiftOut * out, ButtonManager * buttons, Timer * t, RGB_LED * led, Adafruit_NeoPixel * strip, GameState * game): buttonListener(this), timerListener(this) {
  this->out = out;
  this->buttons = buttons;
  this->t = t;
  this->led = led;
  this->strip = strip;
  this->game = game;
  this->buttons->attachAllOnPress(&buttonListener);
  this->buttons->attachAllOnRelease(&buttonListener);
}

void Maze::init(uint32_t seed) {
  r.reseed(seed);
  board.setMap(r.next());
  uint8_t goal_x = r.next() % 6;
  uint8_t goal_y = r.next() % 6;
  uint8_t start_x = r.next() % 6;
  uint8_t start_y = r.next() % 6;

  while ((start_x > goal_x - 1 && start_x < goal_x + 1) && (start_y > goal_y - 1 && start_y < goal_y + 1)) {
    start_x = r.next();
    start_y = r.next();
  }
  board.setGoal(goal_x, goal_y);
  board.setPlayer(start_x, start_y);
  updateDisplay(1);
}

void Maze::demo() {
  
}

void Maze::start() {
  
}

void Maze::explode() {
  
}

void Maze::updateDisplay(unsigned int cycle) {
  for (int i = 0; i < 36; i++) {
    strip->setPixelColor(i, MAZE_BLACK);
  }
  setPoint(MAZE_GREEN, board.getInd1().x, board.getInd1().y);
  setPoint(MAZE_GREEN, board.getInd2().x, board.getInd2().y);
  if (!(checkPoints(board.getGoal(), board.getInd1()) || checkPoints(board.getGoal(), board.getInd2()))) {
    setPoint(MAZE_RED, board.getGoal().x, board.getGoal().y);
  }
  if(!(cycle & 1)) {
    setPoint(MAZE_RED, board.getGoal().x, board.getGoal().y);
    setPoint(MAZE_BLUE, board.getPlayer().x, board.getPlayer().y);
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
  strip->setPixelColor(n, color);
}

bool Maze::checkPoints(MazeBoard::point a, MazeBoard::point b) {
  return a.x == b.x && a.y == b.y;
}

