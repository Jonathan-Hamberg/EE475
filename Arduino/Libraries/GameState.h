#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdint.h>
#include <Definitions.h>

/**
 * This is the Game State Object. It holds a local copy of the global game state.
 * This object may have each field set manualy or can be genorated from a seed.
 *
 * The game state does not automaticaly update, so all updates must come from the
 * pi over the main bus. This object does not spesify how this comunication works.
 *
 * @authors Daniel Predmore, Jonathan Hamberg, Christy Troung
 */

class GameState final {
  public:

  /**
   * This is the default constructor it does not set any of the objects fields
   * exept the game state to pre game
   */
  GameState();


  /**
   * This function builds the bomb from a seed.
   *
   * @param countdownTime the starting time on the clock
   * @param maxStrikes the maximum number of strikes
   * @param seed the seed to build the bomb from
   */
  void init(uint16_t countdownTime, uint8_t maxStrikes, uint32_t seed);

  /**
   * These are the getter functions. Each one returns the corisponding value
   */
  uint8_t getStrikes() const;
  uint8_t getMaxStrikes() const;
  uint16_t getCountdownTime() const;
  uint16_t getTimeLeft() const;
  uint16_t getIndicators() const;
  uint8_t getPorts() const;
  uint8_t getBatteries() const;
  const char * getSN() const;
  ModuleMode getGameState() const;

  /**
   * These functions set the value of each of the bombs parameters
   */
  void setStrikes(uint8_t strikes);
  void setMaxStrikes(uint8_t maxStrikes);
  void setCountdownTime(uint16_t countdownTime);
  void setGameState(ModuleMode gameState);
  void setIndicators(uint16_t indicators);
  void setPorts(uint8_t ports);
  void setBat(uint8_t bat);

  /**
   * Each of these functions sets the value of an indicator
   */
  void setSND(bool value);
  void setCLR(bool value);
  void setCAR(bool value);
  void setIND(bool value);
  void setFRQ(bool value);
  void setSIG(bool value);
  void setNSA(bool value);
  void setMSA(bool value);
  void setTRN(bool value);
  void setBOB(bool value);
  void setFRK(bool value);

  /**
   * These funcitns set the value of each port
   */
  void setDVI(bool value);
  void setParallel(bool value);
  void setPS2(bool value);
  void setRJ45(bool value);
  void setSerial(bool value);
  void setRCA(bool value);

  void setField(OpCode op, uint16_t data);

  /**
   * These fucntions check the value of each indicator
   */
  bool checkSND();
  bool checkCLR();
  bool checkCAR();
  bool checkIND();
  bool checkFRQ();
  bool checkSIG();
  bool checkNSA();
  bool checkMSA();
  bool checkTRN();
  bool checkBOB();
  bool checkFRK();

  /**
   * These functions check the state of each port
   */
  bool checkDVI();
  bool checkParallel();
  bool checkPS2();
  bool checkRJ45();
  bool checkSerial();
  bool checkRCA();

  /**
   * This function check to see if a digit is displayed on the golabal counter
   *
   * @param digit the digit to look for
   * @return true if found
   */
  bool timeHasDigit(uint8_t digit);

  /**
   * This function check to see if the last digit of the SN is even
   *
   * @return true if the last digit is even
   */
  bool SNisEven();

  /**
   * This function checks if the SN has a vowel
   *
   * @return true if a vowel is found
   */
  bool SNhasVowel();


  /**
   *
   *
   * @return
   */
  bool hasDataChanged();


  /**
   *
   */
  void clearDataChanged();

    /**
     * GameState desturctor
     */
    ~GameState() = default;

  private:

  bool getBit(uint16_t value, uint8_t n);
  uint16_t setBit(uint16_t value, uint8_t n, bool b);

  uint8_t strikes, maxStrikes;
  uint16_t countdownTime;
  uint16_t indicators;
  uint8_t ports;
  uint8_t bat;
  char sn[7];
  // TODO(jrh) figure out how to incorporate seed into communication.
  // the seed is needed at the initialization time which is before communication
  // has been started.
  uint16_t seed;
  /// Contains mask to determine if the mode has changed.
  bool modeChanged;
  ModuleMode gameState;
};

#endif // GAME_STATE_H
