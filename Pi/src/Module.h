#ifndef DEFUSER_MODULE_H
#define DEFUSER_MODULE_H

#include <cstdint>
#include "Definitions.h"

/**
 *
 */
class Module final {
public:
    /**
     *
     */
    Module() = default;

    /**
     *
     */
    ~Module() = default;

    /**
     *
     * @param seed
     */
    void setSeed(uint16_t seed);

    /**
     *
     * @return
     */
    uint16_t getSeed() const;

    /**
     *
     */
    void setModuleType(ModuleTypes);

    /**
     *
     * @return
     */
    ModuleTypes getModuleType();

private:
    uint16_t seed;
    ModuleTypes moduleType;
};


#endif //DEFUSER_MODULE_H
