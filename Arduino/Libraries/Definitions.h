#ifndef DEFUSER_DEFINITIONS_H
#define DEFUSER_DEFINITIONS_H

#include <stdint.h>

/**
 *
 */
enum class OpCode : uint8_t {
    None = 0, // No op code.  Receive any message the module wants.
    Mode, // Transmit or receive module mode.
    Strike, // Transmit or receive system strikes.
    MaxStrike, // Transmit or receive module max strikes.
    Countdown, // Transmit or receive module countdown.
    Indicators, // Transmit or receive module indicators.
    Ports, // Transmit or receive module ports.
    Seed, // Transmit or receive module seed.
    Battery, // Transmit or receive module battery.
    ModuleType, // Transmit or receive module type.
    PlaySound, // Receive play sound request.
    Acknowledge, // Receive acknologment of the strike.
};

enum class ExtraInformation : uint8_t {
    Strike = 0x00,
    PlaySound,
    Disarm,
};

/**
 *
 */
enum class ModuleType : uint8_t {
    Wires = 0,
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
};

/**
 *
 */
enum class ModuleMode : uint8_t {
    Off = 0,
    Demo,
    Armed,
    Disarmed
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
};

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
};

/**
 *
 */
enum class PlaySound : uint8_t {
    NoSound = 0,
    Sound0,
    Sound1,
    Sound2,
    Sound3,
};


/**
 *
 */
struct SPIMessage {
    OpCode address;
    uint16_t data;
};

#endif //DEFUSER_DEFINITIONS_H
