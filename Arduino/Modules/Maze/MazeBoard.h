#ifndef MAZE_BOARD_H
#define MAZE_BOARD_H

#include <Arduino.h>

#define MAZE_LEFT 0
#define MAZE_UP 1
#define MAZE_RIGHT 2
#define MAZE_DOWN 3

class MazeBoard final {
  public:
  
  struct point {
    uint8_t x;
    uint8_t y;
  };

  MazeBoard();

  point getPlayer();
  point getGoal();
  point getInd1();
  point getInd2();

  void setMap(uint8_t map);

  void setPlayer(uint8_t x, uint8_t y);
  void setGoal(uint8_t x, uint8_t y);

  bool move(uint8_t direction);

  bool finished();

  private:

  uint8_t mapIndex;
  point player;
  point goal;
  point ind1;
  point ind2;
  
};

#endif // MAZE_BOARD_H
