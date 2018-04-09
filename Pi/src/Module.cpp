#include "Module.h"
#include "bcm2835.h"

void Module::setSeed(uint16_t seed) {
    this->seed = seed;
}

uint16_t Module::getSeed() const {
    return this->seed;
}

void Module::setModuleType(ModuleTypes moduleType) {
    this->moduleType = moduleType;
}

ModuleTypes Module::getModuleType() {
    return moduleType;
}
