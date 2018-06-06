#include "GameManager.h"
#include "Definitions.h"
#include <iostream>


GameManager::GameManager(ModuleManager *moduleManager, SoundManager* soundManager, uint16_t seed) {
    this->moduleManager = moduleManager;
    this->soundManager = soundManager;

    numConnectedModules = 0;
    disarmCounter = 0;
    srand(seed);
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
    this->gameState.init(countdown, maxStrikes, rand());
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
    moduleManager->updateModules(OpCode::None, 0);

    // Check to see if the user wants to end the game.
    if (moduleManager->controlHasExtraInformation(ExtraInformation::StopGame)) {
        onStop();
    }

    // Check to see if the game is to be started.
    if (moduleManager->controlHasExtraInformation(ExtraInformation::StartGame)) {
        onStart();
    }

    // Play the setup music.
    if(soundManager->isSoundDone()) {
        soundManager->playSound(PlaySound::Setup);
    }
}

void GameManager::updateStateDemo() {
    moduleManager->updateModules(OpCode::None, 0);

    // Check to see if the game is to be started.
    if (moduleManager->controlHasExtraInformation(ExtraInformation::StartGame)) {
        onStart();
    }
}

void GameManager::updateStateArmed() {

    moduleManager->updateModules(OpCode::None, 0);

    // Check to see if the user wants to end the game.
    if (moduleManager->controlHasExtraInformation(ExtraInformation::StopGame)) {
        onStop();
    }

    // Update the countdown of the defuser.
    int16_t currentCountdown = int16_t(
            std::chrono::duration_cast<std::chrono::seconds>(stopTime - std::chrono::system_clock::now()).count());

    // Check to see if the countdown has changed.
    if(currentCountdown != gameState.getCountdownTime()) {
        gameState.setCountdownTime(uint16_t(currentCountdown));

        std::cout << "currentCountdown: " << currentCountdown << std::endl;

        // Update the current countdown for all the modules.
        moduleManager->updateModules(OpCode::Countdown, uint16_t(currentCountdown));

        // Play the countdown sounds.
        soundManager->stopAllSound();
        soundManager->playSound(PlaySound::BeepLow);
        soundManager->playSound(PlaySound::BeepHigh);

        // Play the light buzz every 15 seconds.
        if(currentCountdown % 15 == 0) {
            soundManager->playSound(PlaySound::LightBuzz);
        }
    }



    if (currentCountdown <= 0) {
        onDetonate();
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
}

void GameManager::updateStateDisarmed() {
    moduleManager->updateModules(OpCode::None, 0);

    // Check to see if the user wants to end the game.
    if (moduleManager->controlHasExtraInformation(ExtraInformation::StopGame)) {
        onStop();
    }
}

void GameManager::onStart() {
    std::cout << "onStart()" << std::endl;
    // Reset the disarm counter.
    disarmCounter = 0;

    // Generate the initial game state.
    generateGameState(seed, countdown, maxStrikes);
    gameState.setGameState(ModuleMode::Armed);

    // Query the connected modules.
    moduleManager->queryModules(seed);

    if (!moduleManager->hasControlModule() || moduleManager->getModules().size() < 2) {
        std::cout << "System must contain control module and have more then one connected game puzzle." << std::endl;
        return;
    }

    // TODO(jrh) add back when done debugging.
    // Transfer game state to all the modules.
    moduleManager->transmitGameState(gameState);
    std::cout << "transmitGameState() begin." << std::endl;
    std::cout << "\tmaxStrikes: " << int(gameState.getMaxStrikes()) << std::endl;
    std::cout << "\tcountdown: " << gameState.getCountdownTime() << std::endl;
    std::cout << "\tindicators: " << gameState.getIndicators() << std::endl;
    std::cout << "\tports: " << int(gameState.getPorts()) << std::endl;
    std::cout << "\tbatteries: " << int(gameState.getBatteries()) << std::endl;
    std::cout << "\tSN: " << gameState.getSN() << std::endl;
    std::cout << "\tseed: " << gameState.getSeed() << std::endl;
    std::cout << "transmitGameState() end." << std::endl;

    // Tell the modules to start.
    moduleManager->updateModules(OpCode::Mode, uint16_t(ModuleMode::Armed));



    // The number of connected modules is the number of total modules minus the control module.
    numConnectedModules = uint8_t(moduleManager->getModules().size() - 1);

    // Store the start time of the defuser.
    stopTime = std::chrono::system_clock::now() + std::chrono::seconds(countdown);

    // Set the next state of the FSM.
    currentMode = ModuleMode::Armed;

    // Stop the lobby sound.
    soundManager->stopAllSound();
}

void GameManager::onStop() {
    std::cout << "onStop()" << std::endl;
    currentMode = ModuleMode::Off;

    moduleManager->updateModules(OpCode::Mode, uint16_t(ModuleMode::Demo));

    soundManager->stopAllSound();
}

void GameManager::onStrike(uint8_t address) {

    // Update the console.
    std::cout << "onStrike(" << int(address) << ")..." << std::endl;

    // Update the strikes in the game state.
    this->gameState.setStrikes(uint8_t(this->gameState.getStrikes() + 1u));

    std::cout << "strike " << int(this->gameState.getStrikes()) << std::endl;

    // Update all the modules on the new strike.
    moduleManager->updateModules(OpCode::Strike, this->gameState.getStrikes());

    // Play the strike sound.
    soundManager->playSound(PlaySound::Strike);

    // Check to see if the defuser has been detonated.
    if (gameState.getStrikes() >= this->maxStrikes) {
        onDetonate();
    }

}

void GameManager::onDisarm(uint8_t address) {
    // Print debugging message to the console.
    std::cout << "onDisarm(" << int(address) << ")" << std::endl;

    // Acknowledge disarm.
    moduleManager->updateModule(OpCode::Acknowledge, 0, address);

    // Increment the disarm count.
    disarmCounter++;


    // Play the disarm sound.
    soundManager->playSound(PlaySound::Defused);

    // Check to see if the defuser has been disarmed.
    if (disarmCounter >= numConnectedModules) {
        onDisarmAll();
    }

}

void GameManager::onDisarmAll() {
    std::cout << "onDisarmAll()" << std::endl;

    // TODO(jrh) verify that all the modules are disarmed.


    // Update all the modules to be disarmed.
    currentMode = ModuleMode::Off;
    moduleManager->updateModules(OpCode::Mode, uint16_t(ModuleMode::Disarmed));

    // Play the win music.
    soundManager->playSound(PlaySound::Win);
}

void GameManager::onDetonate() {
    std::cout << "onDetonate()" << std::endl;


    currentMode = ModuleMode::Off;
    moduleManager->updateModules(OpCode::Mode, uint16_t(ModuleMode::Off));

    // Play the explosion sound.
    soundManager->playSound(PlaySound::Detonate);
    soundManager->playSound(PlaySound::Lose);
}



