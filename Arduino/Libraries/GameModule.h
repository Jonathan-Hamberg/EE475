#ifndef GAME_MODULE_H
#define GAME_MODULE_H

#define MODULE_OFF 0
#define MODULE_DEMO 1
#define MODULE_ARMED 2
#define MODULE_DISARMED 3

class GameModule {
  public:

  virtual void init(uint32_t seed);

  virtual void demo();

  virtual void start();

  virtual void explode();
};


#endif // GAME_MODULE_H
