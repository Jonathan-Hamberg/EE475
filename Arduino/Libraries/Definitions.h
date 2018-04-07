//
// Created by jhamberg on 4/3/18.
//

#ifndef DEFUSER_DEFINITIONS_H
#define DEFUSER_DEFINITIONS_H

#include <stdint.h>

/**
 *
 */
struct SPIMessage {
    uint8_t address;
    uint16_t data;
};
/**
 *
 */
enum class TransmitOpCodes : uint8_t {
    Ignored = 0,
    Mode,
    MakStrikes,
    Seed,
    Countdown,
    Indicators,
    Ports,
    Battery,
};

/**
 *
 */
enum class ReceiveOpCodes : uint8_t {
    Ignored = 0,
    GetMode,
    Strikes,
    ModuleType,
};

/**
 *
 */
enum class ModuleTypes : uint8_t {
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
enum class GameMode : uint8_t {
    Inactive = 0,
    Active,
    Defused,
    Exploded,
    Demo
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

#endif //DEFUSER_DEFINITIONS_H
