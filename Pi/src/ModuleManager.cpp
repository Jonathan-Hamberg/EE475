#include "ModuleManager.h"

/**
 *
 */
enum class TransmitOpCodes {
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
enum class ReceiveOpCodes {
    Ignored = 0,
    GetMode,
    Strikes,
};

void ModuleManager::QueryModules() {

}

void ModuleManager::UpdateStart() {

}

void ModuleManager::UpdateStop() {

}

void ModuleManager::TransmitGameState(const GameState &state) {

    uint8_t buffer[3];

    uint8_t* address = buffer;
    uint16_t* data = (uint16_t*)(buffer + 1);

    for(const auto& module : moduleMap) {
        // Transmit the mode to the module.
        *address = uint8_t(TransmitOpCodes::Mode);
        *data = MODE_INACTIVE;
        module.second.transfer(buffer);

        // Transmit the module seed.
        *address = uint8_t(TransmitOpCodes::Seed);
        // TODO(jrh): figure out way to send correct random.
        *data = rand();
        module.second.transfer(buffer);

        // Transmit the countdown time.
        buffer[0] = uint8_t(TransmitOpCodes::Countdown);
        *data = state.getCountdownTime();
        module.second.transfer(buffer);

        // Transmit the max strikes.
        *address = uint8_t(TransmitOpCodes::MakStrikes);
        *data = state.getMaxStrikes();
        module.second.transfer(buffer);

        // Transmit the indicators.
        *address = uint8_t(TransmitOpCodes::Indicators);
        *data = state.getIndicators();
        module.second.transfer(buffer);

        // Transmit the ports.
        *address = uint8_t(TransmitOpCodes::Ports);
        *data = state.getPorts();
        module.second.transfer(buffer);

        // Transmit the battery.
        *address = uint8_t(TransmitOpCodes::Battery);
        *data = state.getBatteries();
        module.second.transfer(buffer);
    }

}

void TransmitGameState() {

}
