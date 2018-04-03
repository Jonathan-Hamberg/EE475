#ifndef DEFUSER_MODULE_H
#define DEFUSER_MODULE_H

#include <stdint.h>

class Module  {
public:
    Module() = default;
    virtual ~Module() = default;

    void setSeed(uint16_t seed);
    void setModuleNumber(uint8_t id);

    uint16_t getSeed() const;

private:
    uint16_t id;
    uint16_t seed;
};


#endif //DEFUSER_MODULE_H
