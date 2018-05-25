#ifndef ARDUINOGAMEMANAGER_H
#define ARDUINOGAMEMANAGER_H

#include "SPIManager.h"
#include "Module.h"

final class ArdruinoGameManager
{
private:
    SPIManager *spiManager;
    Module* gameModule;    

public:
    ArdruinoGameManager(SPIManager* spiManager, Module* gameModule);
    ~ArdruinoGameManager();
};

#endif /* ARDUINOGAMEMANAGER_H */
