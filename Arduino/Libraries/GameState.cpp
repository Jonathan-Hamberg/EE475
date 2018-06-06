#ifndef __linux__
#include <Arduino.h>
#endif

#include "Definitions.h"
#include "GameState.h"
#include "LFSR.h"
#include <string.h>

//              000000000011111111112222222222
//              012345678901234567890123456789
char snSet[] = "ABCDEFGHIJKLMNOPQRSTUVWXZ";

inline bool GameState::getBit(uint16_t value, uint8_t n) {
  return (value & (1u << n)) != 0;
}

inline uint16_t GameState::setBit(uint16_t value, uint8_t n, bool b) {
  if (b) {
    value |= (1u << n);
  } else {
    value &= ~(1u << n);
  }
  return value;
}

GameState::GameState() {
  this->gameState = ModuleMode::Off;
  this->sn[0] = '\0';
}

void GameState::init(uint16_t countdownTime, uint8_t maxStrikes,
                     uint32_t seed) {
  lfsr r(seed);
  this->countdownTime = countdownTime;
  this->maxStrikes = maxStrikes;
  this->strikes = 0;
  this->indicators = uint16_t(r.next());
  this->ports = uint8_t(r.next());
  this->bat = uint8_t(r.next() % 7);
  this->sn[0] = snSet[r.next() % 24];
  this->sn[1] = snSet[r.next() % 24];
  for (int i = 2; i < 5; i++) {
    if (r.next() & 1) {
      this->sn[i] = uint8_t(r.next() % 10 + '0');
    } else {
      this->sn[i] = snSet[r.next() % 24];
    }
  }
  this->sn[5] = uint8_t(r.next() % 10 + '0');
  this->sn[6] = '\0';
  this->gameState = ModuleMode::Off;
  this->modeChanged = false;
}

uint8_t GameState::getStrikes() const {
  return this->strikes;
}

uint8_t GameState::getMaxStrikes() const {
  return this->maxStrikes;
}

uint16_t GameState::getCountdownTime() const {
  return this->countdownTime;
}

uint16_t GameState::getTimeLeft() const {
  return this->countdownTime;
}

uint16_t GameState::getIndicators() const {
  return this->indicators;
}

uint8_t GameState::getPorts() const {
  return this->ports;
}

uint8_t GameState::getBatteries() const {
  return this->bat;
}

const char *GameState::getSN() const {
  return this->sn;
}

ModuleMode GameState::getGameState() const {
  return this->gameState;
}

uint16_t GameState::getSeed() const {
  return this->seed;
}

ModuleType GameState::getModuleType() const {
  return this->moduleType;
}

void GameState::setSN(uint16_t digits) {
  // If digits are 0 then reset the serial number.
  if (digits == 0) {
    sn[0] = '\0';
    return;
  }

  // Get the length of the serial number.
  uint8_t length = strlen(sn);

  // If the serial number is not six characters then append the most recent two
  // characters.
  if (length <= 4) {
    // If digits are non-zero then append the two bytes to the serial number
    // if there is space to spare.
    auto byte1 = char(digits);
    auto byte2 = char(digits >> 8u);

    sn[length + 0] = byte1;
    sn[length + 1] = byte2;
    sn[length + 2] = '\0';
  }
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

void GameState::setGameState(ModuleMode gameState) {
  this->gameState = gameState;
  modeChanged = true;
}

void GameState::setIndicators(uint16_t indicators) {
  this->indicators = indicators;
}

void GameState::setPorts(uint8_t ports) {
  this->ports = ports;
}

void GameState::setBattteries(uint8_t bat) {
  this->bat = bat;
}

void GameState::setSeed(uint16_t seed) {
  this->seed = seed;
}

void GameState::setModuleType(ModuleType moduleType) {
  this->moduleType = moduleType;
}

void GameState::setSND(bool value) {
  this->indicators =
      setBit(this->indicators, uint8_t(GameIndicator::Snd), value);
}

void GameState::setCLR(bool value) {
  this->indicators =
      setBit(this->indicators, uint8_t(GameIndicator::Clr), value);
}

void GameState::setCAR(bool value) {
  this->indicators =
      setBit(this->indicators, uint8_t(GameIndicator::Car), value);
}

void GameState::setIND(bool value) {
  this->indicators =
      setBit(this->indicators, uint8_t(GameIndicator::Ind), value);
}

void GameState::setFRQ(bool value) {
  this->indicators =
      setBit(this->indicators, uint8_t(GameIndicator::Frq), value);
}

void GameState::setSIG(bool value) {
  this->indicators =
      setBit(this->indicators, uint8_t(GameIndicator::Sig), value);
}

void GameState::setNSA(bool value) {
  this->indicators =
      setBit(this->indicators, uint8_t(GameIndicator::Nsa), value);
}

void GameState::setMSA(bool value) {
  this->indicators =
      setBit(this->indicators, uint8_t(GameIndicator::Msa), value);
}

void GameState::setTRN(bool value) {
  this->indicators =
      setBit(this->indicators, uint8_t(GameIndicator::Trn), value);
}

void GameState::setBOB(bool value) {
  this->indicators =
      setBit(this->indicators, uint8_t(GameIndicator::Bob), value);
}

void GameState::setFRK(bool value) {
  this->indicators =
      setBit(this->indicators, uint8_t(GameIndicator::Frk), value);
}

void GameState::setDVI(bool value) {
  this->ports = uint8_t(setBit(this->ports, uint8_t(GamePort::Dvi), value));
}

void GameState::setParallel(bool value) {
  this->ports =
      uint8_t(setBit(this->ports, uint8_t(GamePort::Parallel), value));
}

void GameState::setPS2(bool value) {
  this->ports = uint8_t(setBit(this->ports, uint8_t(GamePort::Ps2), value));
}

void GameState::setRJ45(bool value) {
  this->ports = uint8_t(setBit(this->ports, uint8_t(GamePort::Rj45), value));
}

void GameState::setSerial(bool value) {
  this->ports = uint8_t(setBit(this->ports, uint8_t(GamePort::Serial), value));
}

void GameState::setRCA(bool value) {
  this->ports = uint8_t(setBit(this->ports, uint8_t(GamePort::Rca), value));
}

void GameState::setField(OpCode op, uint16_t data) {
  switch (op) {
    case OpCode::Mode:
      this->gameState = ModuleMode(data);
      modeChanged = true;
      break;
    case OpCode::MaxStrike:
      this->maxStrikes = uint8_t(data);
      break;
    case OpCode::Countdown:
      this->countdownTime = data;
      break;
    case OpCode::Indicators:
      this->indicators = data;
      break;
    case OpCode::Ports:
      this->ports = uint8_t(data);
      break;
    case OpCode::Seed:
      this->seed = data;
      break;
    case OpCode::Battery:
      this->bat = uint8_t(data);
      break;
    case OpCode::ModuleType:
      this->moduleType = ModuleType(data);
      break;
    case OpCode::Serial:
      this->setSN(data);
      break;
    default:
      break;
  }
}

bool GameState::checkSND() {
  return getBit(this->indicators, uint8_t(GameIndicator::Snd));
}

bool GameState::checkCLR() {
  return getBit(this->indicators, uint8_t(GameIndicator::Clr));
}

bool GameState::checkCAR() {
  return getBit(this->indicators, uint8_t(GameIndicator::Car));
}

bool GameState::checkIND() {
  return getBit(this->indicators, uint8_t(GameIndicator::Ind));
}

bool GameState::checkFRQ() {
  return getBit(this->indicators, uint8_t(GameIndicator::Frq));
}

bool GameState::checkSIG() {
  return getBit(this->indicators, uint8_t(GameIndicator::Sig));
}

bool GameState::checkNSA() {
  return getBit(this->indicators, uint8_t(GameIndicator::Nsa));
}

bool GameState::checkMSA() {
  return getBit(this->indicators, uint8_t(GameIndicator::Msa));
}

bool GameState::checkTRN() {
  return getBit(this->indicators, uint8_t(GameIndicator::Trn));
}

bool GameState::checkBOB() {
  return getBit(this->indicators, uint8_t(GameIndicator::Bob));
}

bool GameState::checkFRK() {
  return getBit(this->indicators, uint8_t(GameIndicator::Frk));
}

bool GameState::checkDVI() {
  return getBit(this->ports, uint8_t(GamePort::Dvi));
}

bool GameState::checkParallel() {
  return getBit(this->ports, uint8_t(GamePort::Parallel));
}

bool GameState::checkPS2() {
  return getBit(this->ports, uint8_t(GamePort::Ps2));
}

bool GameState::checkRJ45() {
  return getBit(this->ports, uint8_t(GamePort::Rj45));
}

bool GameState::checkSerial() {
  return getBit(this->ports, uint8_t(GamePort::Serial));
}

bool GameState::checkRCA() {
  return getBit(this->ports, uint8_t(GamePort::Rca));
}

bool GameState::timeHasDigit(uint8_t digit) {
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

bool GameState::SNisEven() {
    return (uint8_t(this->sn[5] - '0') & 1u) == 0;
}

bool GameState::SNhasVowel() {
  for (int i = 0; i < 6; i++) {
    switch (this->sn[i]) {
      case 'A':
        return true;
      case 'E':
        return true;
      case 'I':
        return true;
      case 'O':
        return true;
      case 'U':
        return true;
      default:
        break;
    }
  }
  return false;
}

bool GameState::hasModeChanged() {
  bool ret = modeChanged;
  modeChanged = false;
  return ret;
}
