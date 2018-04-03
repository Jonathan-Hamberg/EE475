#ifndef GAME_MODULE_H
#define GAME_MODULE_H

class GameModule {
  public:

  virtual void init(uint32_t seed);

  virtual void demo();

  virtual void start();

  virtual void explode();
};


#endif // GAME_MODULE_H
