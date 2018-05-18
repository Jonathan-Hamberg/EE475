#ifndef THE_BUTTON_H
#define THE_BUTTON_H

#include <GameState.h>
#include <GameModule.h>
#include <ButtonManager.h>
#include <Timer.h>

class Dummy : public GameModule {
  public:

  Dummy(GameState * game);

  virtual void init(uint32_t seed);

  virtual void start();

  virtual void demo();

  virtual void explode();

  virtual ~Dummy();
  
  private:

  void muteOutput();

  void setMode(uint8_t mode);

  void setColor(uint32_t color);

  bool quickPress();

  class DummyButtonListener : public ButtonListener {
    public:

    DummyButtonListener(Dummy * parent);

    virtual void onEvent(Button * caller, ButtonEvent event);
    
    private:
    Dummy * parent;
  };

  class DummyTimerListener : public TimerListener {
    public:

    DummyTimerListener(Dummy * parent);

    virtual unsigned int onEvent(Timer *caller, unsigned int id, unsigned int calls);
    
    private:
    Dummy * parent;
  };

  DummyButtonListener buttonListener;
  DummyTimerListener timerListener;
};

#endif // THE_BUTTON_H
