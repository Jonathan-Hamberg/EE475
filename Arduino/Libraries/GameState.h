#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <Arduino.h>
#include <stdint.h>

/**
 * This is the Game State Object. It holds a local copy of the global game state.
 * This object may have each field set manualy or can be genorated from a seed.
 * 
 * The game state does not automaticaly update, so all updates must come from the
 * pi over the main bus. This object does not spesify how this comunication works.
 * 
 * @authors Daniel Predmore, Jonathan Hamberg, Christy Troung
 */

#define PRE_GAME 0;
#define START_GAME 1;
#define GAME_OVER 2;

#define GAME_STATE_IND_SND 0
#define GAME_STATE_IND_CLR 1
#define GAME_STATE_IND_CAR 2
#define GAME_STATE_IND_IND 3
#define GAME_STATE_IND_FRQ 4
#define GAME_STATE_IND_SIG 5
#define GAME_STATE_IND_NSA 6
#define GAME_STATE_IND_MSA 7
#define GAME_STATE_IND_TRN 8
#define GAME_STATE_IND_BOB 9
#define GAME_STATE_IND_FRK 10

#define GAME_STATE_PORT_DVI 0
#define GAME_STATE_PORT_PARALLEL 1
#define GAME_STATE_PORT_PS2 2
#define GAME_STATE_PORT_RJ45 3
#define GAME_STATE_PORT_SERIAL 4
#define GAME_STATE_PORT_RCA 5

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
  uint8_t getStrikes();
  uint8_t getMaxStrikes();
  uint16_t getTimeLeft();
  uint16_t getIndicators();
  uint8_t getPorts();
  uint8_t getBatteries();
  char * getSN();
  uint8_t getGameState();

  /**
   * These functions set the value of each of the bombs parameters
   */
  void setStrikes(uint8_t strikes);
  void setMaxStrikes(uint8_t maxStrikes);
  void setCountdownTime(uint16_t countdownTime);
  void setGameStat(uint8_t gameState);
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

  /**
   * These fucntions check the value of each indicator
   */
  boolean checkSND();
  boolean checkCLR();
  boolean checkCAR();
  boolean checkIND();
  boolean checkFRQ();
  boolean checkSIG();
  boolean checkNSA();
  boolean checkMSA();
  boolean checkTRN();
  boolean checkBOB();
  boolean checkFRK();

  /**
   * These functions check the state of each port
   */
  boolean checkDVI();
  boolean checkParallel();
  boolean checkPS2();
  boolean checkRJ45();
  boolean checkSerial();
  boolean checkRCA();

  /**
   * This function check to see if a digit is displayed on the golabal counter
   * 
   * @param digit the digit to look for
   * @return true if found
   */
  boolean timeHasDigit(uint8_t digit);

  /**
   * This function check to see if the last digit of the SN is even
   * 
   * @return true if the last digit is even
   */
  boolean SNisEven();

  /**
   * This function checks if the SN has a vowel
   * 
   * @return true if a vowel is found
   */
  boolean SNhasVowel();

  /**
   * GameState desturctor
   */
  ~GameState();

  private:

  boolean getBit(uint16_t value, uint8_t n);
  uint16_t setBit(uint16_t value, uint8_t n, boolean b);
  
  uint8_t strikes, maxStrikes;
  uint16_t countdownTime;
  uint16_t indicators;
  uint8_t ports;
  uint8_t bat;
  char sn[7];
  uint8_t gameState;
  
};

#endif // GAME_STATE_H
