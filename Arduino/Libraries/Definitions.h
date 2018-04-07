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
    Ignored = 0, // Used when no message is being transmitted.
    ReceiveAny, // Used to request any message from the module that it wants to send.
    Mode, // Updates the mode of the module.
    MaxStrikes, // Updates the maximum number of strikes.
    Seed, // Updates the modules individual seed.
    Countdown, // Update the countdown timer for the module.
    Indicators, // Update the indicator lights on the module.
    Ports, // Updates the port indicator light on the module.
    Battery, // Update the batteries on the module.
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

/**
 *
 */
enum class PlaySound : uint8_t {
    Sound1 = 0,
    Sound2,
    Sound3,
    Sound4,
    Sound5,
};

#endif //DEFUSER_DEFINITIONS_H
