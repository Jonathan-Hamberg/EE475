#include <Arduino.h>
#include "GameState.h"
#include "LFSR.h"
//              000000000011111111112222222222
//              012345678901234567890123456789
char snSet[] = "ABCDEFGHIJKLMNOPQRSTUVWXZ";

inline boolean GameState::getBit(uint16_t value, uint8_t n) {
  return !!(value & (1 << n));
}

inline uint16_t GameState::setBit(uint16_t value, uint8_t n, boolean b) {
  if (b) {
    value |= (1 << n);
  } else {
    value &= ~(1 << n); 
  }
  return value;
}

GameState::GameState() {
  this->gameState = PRE_GAME;
}

void GameState::init(uint16_t countdownTime, uint8_t maxStrikes, uint32_t seed) {
  lfsr r(seed);
  this->countdownTime = countdownTime;
  this->maxStrikes = maxStrikes;
  this->strikes = 0;
  this->indicators = r.next();
  this->ports = r.next();
  this->bat = r.next() % 7;
  this->sn[0] = snSet[r.next()%24];
  this->sn[1] = snSet[r.next()%24];
  for (int i = 2; i < 5; i++ ) {
    if (r.next() & 1) {
      this->sn[i] = r.next() % 10 + '0';
    } else {
      this->sn[i] = snSet[r.next()%24];
    }
  }
  this->sn[5] = r.next() % 10 + '0';
  this->sn[6] = '\0';
  this->gameState = PRE_GAME;
}

uint8_t GameState::getStrikes() {
  return this->strikes;
}

uint8_t GameState::getMaxStrikes() {
  return this->maxStrikes;  
}

uint16_t GameState::getTimeLeft() {
  return this->countdownTime;
}

uint16_t GameState::getIndicators() {
  return this->indicators;  
}

uint8_t GameState::getPorts() {
  return this->ports;
}

uint8_t GameState::getBatteries(){
  return this->bat;
}

char * GameState::getSN() {
  return this->sn;
}

uint8_t GameState::getGameState() {
  return this->gameState;
}

void GameState::setStrikes(uint8_t strikes) {
  this->strikes = strikes;
}

void GameState::setMaxStrikes(uint8_t maxStrikes) {
  this->maxStrikes = maxStrikes;
}

void GameState::setCountdownTime(uint16_t countdownTime) {
  this->countdownTime = countdownTime;
}

void GameState::setGameStat(uint8_t gameState) {
  this->gameState = gameState;
}

void GameState::setIndicators(uint16_t indicators) {
  this->indicators = indicators;
}

void GameState::setPorts(uint8_t ports) {
  this->ports = ports;
}

void GameState::setBat(uint8_t bat) {
  this->bat = bat;
}

void GameState::setSND(bool value) {
  this->indicators = setBit(this->indicators, GAME_STATE_IND_SND, value);
}

void GameState::setCLR(bool value) {
  this->indicators = setBit(this->indicators, GAME_STATE_IND_CLR, value);
}

void GameState::setCAR(bool value) {
  this->indicators = setBit(this->indicators, GAME_STATE_IND_CAR, value);
}

void GameState::setIND(bool value) {
  this->indicators = setBit(this->indicators, GAME_STATE_IND_IND, value);
}

void GameState::setFRQ(bool value) {
  this->indicators = setBit(this->indicators, GAME_STATE_IND_FRQ, value);
}

void GameState::setSIG(bool value) {
  this->indicators = setBit(this->indicators, GAME_STATE_IND_SIG, value);
}

void GameState::setNSA(bool value) {
  this->indicators = setBit(this->indicators, GAME_STATE_IND_NSA, value);
}

void GameState::setMSA(bool value) {
  this->indicators = setBit(this->indicators, GAME_STATE_IND_MSA, value);
}

void GameState::setTRN(bool value) {
  this->indicators = setBit(this->indicators, GAME_STATE_IND_TRN, value);
}

void GameState::setBOB(bool value) {
  this->indicators = setBit(this->indicators, GAME_STATE_IND_BOB, value);
}

void GameState::setFRK(bool value) {
  this->indicators = setBit(this->indicators, GAME_STATE_IND_FRK, value);
}

void GameState::setDVI(bool value) {
  this->ports = setBit(this->ports, GAME_STATE_PORT_DVI, value);
}

void GameState::setParallel(bool value) {
  this->ports = setBit(this->ports, GAME_STATE_PORT_PARALLEL, value);
}

void GameState::setPS2(bool value) {
  this->ports = setBit(this->ports, GAME_STATE_PORT_PS2, value);
}

void GameState::setRJ45(bool value) {
  this->ports = setBit(this->ports, GAME_STATE_PORT_RJ45, value);
}

void GameState::setSerial(bool value) {
  this->ports = setBit(this->ports, GAME_STATE_PORT_SERIAL, value);
}

void GameState::setRCA(bool value) {
  this->ports = setBit(this->ports, GAME_STATE_PORT_RCA, value);
}

boolean GameState::checkSND() {
  return getBit(this->indicators, GAME_STATE_IND_SND);
}

boolean GameState::checkCLR() {
  return getBit(this->indicators, GAME_STATE_IND_CLR);
}

boolean GameState::checkCAR() {
  return getBit(this->indicators, GAME_STATE_IND_CAR);
}

boolean GameState::checkIND() {
  return getBit(this->indicators, GAME_STATE_IND_IND);
}

boolean GameState::checkFRQ() {
  return getBit(this->indicators, GAME_STATE_IND_FRQ);
}

boolean GameState::checkSIG() {
  return getBit(this->indicators, GAME_STATE_IND_SIG);
}

boolean GameState::checkNSA() {
  return getBit(this->indicators, GAME_STATE_IND_NSA);
}

boolean GameState::checkMSA() {
  return getBit(this->indicators, GAME_STATE_IND_MSA);
}

boolean GameState::checkTRN() {
  return getBit(this->indicators, GAME_STATE_IND_TRN);
}

boolean GameState::checkBOB() {
  return getBit(this->indicators, GAME_STATE_IND_BOB);
}

boolean GameState::checkFRK() {
  return getBit(this->indicators, GAME_STATE_IND_FRK);
}

boolean GameState::checkDVI() {
  return getBit(this->ports, GAME_STATE_PORT_DVI);
}

boolean GameState::checkParallel() {
  return getBit(this->ports, GAME_STATE_PORT_PARALLEL);
}

boolean GameState::checkPS2() {
  return getBit(this->ports, GAME_STATE_PORT_PS2);
}

boolean GameState::checkRJ45() {
  return getBit(this->ports, GAME_STATE_PORT_RJ45);
}

boolean GameState::checkSerial() {
  return getBit(this->ports, GAME_STATE_PORT_SERIAL);
}

boolean GameState::checkRCA() {
  return getBit(this->ports, GAME_STATE_PORT_RCA);
}

boolean GameState::timeHasDigit(uint8_t digit) {
  uint16_t m = this->countdownTime / 60;
  uint16_t s = this->countdownTime % 60;
  do {
    if (m % 10 == digit) {
      return true;
    }
    m /= 10;
  } while (m > 0);
  do {
    if (s % 10 == digit) {
      return true;
    }
    s /= 10;
  } while (s > 0);
  return false;
}

boolean GameState::SNisEven() {
  return !((this->sn[5] - '0') & 1);
}

boolean GameState::SNhasVowel() {
  for (int i = 0; i < 6; i++) {
    switch (this->sn[i]) {
      case 'A' : return true;
      case 'E' : return true;
      case 'I' : return true;
      case 'O' : return true;
      case 'U' : return true;
      default  : break;
    }
  }
  return false;
}

GameState::~GameState() {}
