#ifndef DEFUSER_MODULEMANAGER_H
#define DEFUSER_MODULEMANAGER_H

#include <vector>
#include <map>
#include "GameState.h"
#include "Module.h"

class ModuleManager {
public:
    void QueryModules();
    void UpdateStart();
    void UpdateStop();
    void TransmitGameState(const GameState& state);
private:
    std::map<uint32_t, Module> moduleMap;
};

#endif //DEFUSER_MODULEMANAGER_H
