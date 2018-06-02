#ifndef DEFUSER_DEFINITIONS_H
#define DEFUSER_DEFINITIONS_H

#include <stdint.h>

/**
 *
 */
enum class OpCode : uint8_t {
    None = 0, // No op code.  Receive any message the module wants.
    Mode, // Transmit or receive module mode.
    Strike, // Transmit module strikes left.
    MaxStrike, // Transmit or receive module max strikes.
    Countdown, // Transmit or receive module countdown.
    Indicators, // Transmit or receive module indicators.
    Ports, // Transmit or receive module ports.
    Seed, // Transmit or receive module seed.
    Battery, // Transmit or receive module battery.
    ModuleType, // Transmit or receive module type.
    PlaySound, // Receive play sound request.
    Acknowledge, // Receive acknowledgment of the strike.
    Serial, // Receive the module SN.
};

enum class ExtraInformation : uint8_t {
    Strike = 0x00,
    PlaySound,
    Disarm,
    StartGame,
    StopGame,
};

/**
 *
 */
enum class ModuleType : uint8_t {
    None,
    Wires,
    Button,
    Keypad,
    SimonSays,
    WhosOnFirst,
    Memory,
    MorseCode,
    ComplicatedWires,
    WireSequence,
    Mazes,
    Passwords,
    VentingGas,
    CapacitorDischarge,
    Knobs,
    Control,
    Symbols,
    EnumeSize,
};

extern const char* ModuleText[];

/**
 *
 */
enum class ModuleMode : uint8_t {
    Off = 0,
    Demo,
    Armed,
    Disarmed,
};

/**
 *
 */
enum class GameIndicator : uint8_t {
    Snd = 0,
    Clr,
    Car,
    Ind,
    Frq,
    Sig,
    Nsa,
    Msa,
    Trn,
    Bob,
    Frk,
    EnumSize,
};

extern const char* GameIndicatorText[];

/**
 *
 */
enum class GamePort : uint8_t {
    Dvi = 0,
    Parallel,
    Ps2,
    Rj45,
    Serial,
    Rca,
    EnumSize,
};

extern const char* GamePortText[];

/**
 *
 */
enum class PlaySound : uint8_t {
    NoSound = 0,
    Defused,
    Detonate,
    Key1,
    Key2,
    Key3,
    Key4,
    Lose,
    Win,
    Strike
};


/**
 *
 */
struct SPIReceiveMessage {
    uint8_t address;
    OpCode op;
    uint8_t information;
    uint16_t data;
};

#endif //DEFUSER_DEFINITIONS_H
