#include <Arduino.h>
#include "MazeBoard.h"

#define MAP0 {{0xC5, 0x9C, 0x51}, {0xAC, 0x36, 0x59}, {0xA6, 0x9C, 0x5B}, {0xA4, 0x73, 0x4B}, {0xE5, 0x9C, 0x1A}, {0x61, 0x63, 0x43}}
#define MAP1 {{0x4D, 0x1C, 0xD1}, {0xC3, 0xC3, 0x69}, {0xAC, 0x3C, 0x5B}, {0xE3, 0xC3, 0x8A}, {0xA8, 0xAC, 0x3A}, {0x26, 0x36, 0x53}}
#define MAP2 {{0xC5, 0x98, 0xC9}, {0x28, 0xA6, 0x3A}, {0xCB, 0xAC, 0x9A}, {0xAA, 0xAA, 0xAA}, {0xA6, 0x3A, 0xAA}, {0x65, 0x53, 0x63}}
#define MAP3 {{0xC9, 0x45, 0x59}, {0xAA, 0xC5, 0x5B}, {0xA6, 0x3C, 0x1A}, {0xA4, 0x57, 0x5B}, {0xE5, 0x55, 0x9A}, {0x65, 0x14, 0x32}}
#define MAP4 {{0x45, 0x55, 0xD9}, {0xC5, 0x5D, 0x32}, {0xE9, 0x42, 0xC9}, {0xA6, 0x59, 0x2A}, {0xAC, 0x57, 0x1A}, {0x26, 0x55, 0x53}}
#define MAP5 {{0x8C, 0x94, 0xD9}, {0xAA, 0xAC, 0x3A}, {0xE3, 0x2A, 0xC3}, {0x69, 0xCB, 0xA8}, {0xC3, 0x2A, 0x6B}, {0x65, 0x53, 0x43}}
#define MAP6 {{0xC5, 0x59, 0xC9}, {0xAC, 0x16, 0x3A}, {0x63, 0xC1, 0xC3}, {0xC9, 0xE5, 0x38}, {0xA2, 0x65, 0x9A}, {0x65, 0x55, 0x73}}
#define MAP7 {{0x8C, 0x59, 0xC9}, {0xE7, 0x16, 0x3A}, {0xAC, 0x55, 0x9A}, {0xA6, 0x94, 0x73}, {0xA8, 0x65, 0x51}, {0x67, 0x55, 0x51}}
#define MAP8 {{0x8C, 0x55, 0xD9}, {0xAA, 0xC1, 0xAA}, {0xE7, 0x3C, 0x3A}, {0xA8, 0xC3, 0x4B}, {0xAA, 0xAC, 0x92}, {0x63, 0x63, 0x61}}

static const uint8_t MAPS[][6][3] = {MAP0, MAP1, MAP2, MAP3, MAP4, MAP5, MAP6, MAP7, MAP8};
static const uint8_t MARKING_X[] = {0, 5, 1, 4, 3, 5, 0, 0, 3, 4, 2, 4, 1, 1, 2, 3, 0, 2};
static const uint8_t MARKING_Y[] = {1, 2, 3, 1, 3, 3, 0, 3, 5, 2, 4, 0, 0, 5, 3, 0, 4, 1}; 

MazeBoard::MazeBoard() {
  mapIndex = 0;
  player.x = 0;
  player.y = 0;
  goal.x = 0;
  goal.y = 0;
  ind1.x = 0;
  ind1.y = 0;
  ind2.x = 0;
  ind2.y = 0;
}

MazeBoard::point MazeBoard::getPlayer() {
  return player;
}

MazeBoard::point MazeBoard::getGoal() {
  return goal;
}

MazeBoard::point MazeBoard::getInd1() {
  return ind1;
}

MazeBoard::point MazeBoard::getInd2() {
  return ind2;
}

void MazeBoard::setMap(uint8_t mapIndex) {
  this->mapIndex = mapIndex % 9;
  this->ind1.x = MARKING_X[this->mapIndex * 2];
  this->ind1.y = MARKING_Y[this->mapIndex * 2];
  this->ind2.x = MARKING_X[this->mapIndex * 2 + 1];
  this->ind2.y = MARKING_Y[this->mapIndex * 2 + 1];
}

void MazeBoard::setPlayer(uint8_t x, uint8_t y) {
  this->player.x = x % 6;
  this->player.y = y % 6;
}

void MazeBoard::setGoal(uint8_t x, uint8_t y) {
  this->goal.x = x % 6;
  this->goal.y = y % 6;
}

bool MazeBoard::move(uint8_t direction) {
  uint8_t currentBlock = MAPS[mapIndex][player.y][player.x / 2];
  if (currentBlock & 1) {
    currentBlock = currentBlock & 0xFF;
  } else {
    currentBlock = (currentBlock >> 4) & 0xFF;
  }
  
  switch (direction) {
    case MAZE_LEFT:
      if (currentBlock & 1) {
        setPlayer(player.x - 1, player.y);
        return true;
      }
    break;
    case MAZE_UP:
      if (currentBlock & 2) {
        setPlayer(player.x, player.y - 1);
        return true;
      }
    break;
    case MAZE_RIGHT:
      if (currentBlock & 4) {
        setPlayer(player.x + 1, player.y);
        return true;
      }
    break;
    case MAZE_DOWN:
      if (currentBlock & 8) {
        setPlayer(player.x, player.y + 1);
        return true;
      }
    break;
    default: break;
  }
  return false;
}

bool MazeBoard::finished() {
  return this->player.x == this->goal.x && this->player.y == this->goal.y;
}



