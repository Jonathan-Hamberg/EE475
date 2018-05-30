#ifndef GAME_MODULE_H
#define GAME_MODULE_H

/**
 *
 */
class GameModule {
 public:
  /**
   *
   *
   * @param seed
   */
  virtual void init(uint32_t seed);

  /**
   *
   */
  virtual void demo();

  /**
   *
   */
  virtual void start();

  /**
   *
   */
  virtual void explode();
};

#endif  // GAME_MODULE_H
