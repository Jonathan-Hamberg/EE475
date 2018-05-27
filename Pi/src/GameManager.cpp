#include "GameManager.h"
#include "Definitions.h"
#include <iostream>


GameManager::GameManager(ModuleManager *moduleManager) {
    this->moduleManager = moduleManager;

    numConnectedModules = 0;
    disarmCounter = 0;
}

void GameManager::generateGameState(uint16_t seed, uint16_t countdown, uint8_t maxStrikes) {
    // Print debugging information.
    std::cout << "generateGameState(seed: " << seed <<
              ",countdown: " << countdown << ",maxStrikes: " << int(maxStrikes) << std::endl;

    // Store the parameters used to generate the game state.
    this->seed = seed;
    this->countdown = countdown;
    this->maxStrikes = maxStrikes;

    // Create a new game state.
    this->gameState = GameState();

    // Initialize the game state.
    this->gameState.init(countdown, maxStrikes, seed);
}

const GameState &GameManager::getGameState() {
    return this->gameState;
}

void GameManager::update() {
    // Perform different logic depending on the state of the defuser.
    switch (this->currentMode) {
        case ModuleMode::Off:
            updateStateOff();
            break;
        case ModuleMode::Demo:
            updateStateDemo();
            break;
        case ModuleMode::Armed:
            updateStateArmed();
            break;
        case ModuleMode::Disarmed:
            updateStateDisarmed();
            break;
    }
}


void GameManager::updateStateOff() {
    // Send the acknowledge to look for the start bit.
    moduleManager->updateControlModule(OpCode::Acknowledge, 0);

    // Check to see if the game is to be started.
    if (moduleManager->controlHasExtraInformation(ExtraInformation::StartGame)) {
        onStart();
    }
}

void GameManager::updateStateDemo() {
    // Send the acknowledge to look for the start bit.
    moduleManager->updateControlModule(OpCode::Acknowledge, 0);

    // Check to see if the game is to be started.
    if (moduleManager->controlHasExtraInformation(ExtraInformation::StartGame)) {
        onStart();
    }
}

void GameManager::updateStateArmed() {
    // Update the countdown of the defuser.
    int16_t currentCountdown = int16_t(std::chrono::duration_cast<std::chrono::seconds>(stopTime - std::chrono::system_clock::now()).count());

    std::cout << "currentCountdown: " << currentCountdown << std::endl;

    // Update the current countdown for all the modules.
    gameState.setCountdownTime(uint16_t(currentCountdown));
    if (currentCountdown > 0) {
        moduleManager->updateModules(OpCode::Countdown, uint16_t(currentCountdown));
    } else {
        onDetonate();
    }

    // Check to see if the user wants to end the game.
    if (moduleManager->controlHasExtraInformation(ExtraInformation::StopGame)) {
        onStop();
    }

    // Iterate through all the connected modules to look for strikes and disarm commands.
    for (const auto &kv : this->moduleManager->getModules()) {
        if (moduleManager->hasExtraInformation(kv.first, ExtraInformation::Strike)) {
            // Detect a strike on a module so run the required logic.
            onStrike(kv.first);
        } else if (moduleManager->hasExtraInformation(kv.first, ExtraInformation::Disarm)) {
            // Detected a disarm on a module so run the required logic.
            onDisarm(kv.first);
        }
    }

    // Acknowledge any status indicators from the modules.
    moduleManager->updateModules(OpCode::Acknowledge, 0);
}

void GameManager::updateStateDisarmed() {
    // TODO(jrh) wait for the user to start the game.
}

void GameManager::onStart() {
    // Reset the disarm counter.
    disarmCounter = 0;

    // Generate the initial game state.
    generateGameState(seed, countdown, maxStrikes);

    // Query the connected modules.
    moduleManager->queryModules(seed);

    // Transfer game state to all the modules.
    moduleManager->transmitGameState(gameState);

    // The number of connected modules is the number of total modules minus the control module.
//    numConnectedModules = uint8_t(moduleManager->getModules().size() - 1);
// TODO(jrh) add the -1 back to the modules.
    numConnectedModules = uint8_t(moduleManager->getModules().size());

    // Store the start time of the defuser.
    stopTime = std::chrono::system_clock::now() + std::chrono::seconds(countdown);

    // Set the next state of the FSM.
    currentMode = ModuleMode::Armed;
}

void GameManager::onStop() {

}

void GameManager::playSound(PlaySound sound) {
    std::cout << "playSound(" << uint8_t(sound) << ")" << std::endl;
}

void GameManager::onStrike(uint8_t address) {

    // Update the console.
    std::cout << "onStrike(" << int(address) << ")..." << std::endl;

        // Update the strikes in the game state.
    this->gameState.setStrikes(uint8_t(this->gameState.getStrikes() + 1u));

    // Update all the modules on the new strike.
    moduleManager->updateModules(OpCode::Strike, this->gameState.getStrikes());

    // Check to see if the defuser has been detonated.
    if (gameState.getStrikes() >= this->maxStrikes) {
        onDetonate();
    }
}

void GameManager::onDisarm(uint8_t address) {
    // Print debugging message to the console.
    std::cout << "onDisarm(" << address << ")" << std::endl;

    // Acknowledge disarm.
    moduleManager->updateModule(OpCode::Acknowledge, 0, address);

    // Increment the disarm count.
    disarmCounter++;

    // Check to see if the defuser has been disarmed.
    if (disarmCounter >= numConnectedModules) {
        onDisarmAll();
    }
}

void GameManager::onDisarmAll() {
    std::cout << "onDisarmAll()" << std::endl;

    // TODO(jrh) verify that all the modules are disarmed.


    // Update all the modules to be disarmed.
    moduleManager->updateModules(OpCode::Mode, uint16_t(ModuleMode::Disarmed));

    // Update the state of the device.
    currentMode = ModuleMode::Off;
}

void GameManager::onDetonate() {
    std::cout << "onDetonate()" << std::endl;
    currentMode = ModuleMode::Off;
}



