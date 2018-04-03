#ifndef DEFUSER_MODULE_H
#define DEFUSER_MODULE_H

#include <stdint.h>

class Module final {
public:
    Module();

    void transfer(uint8_t write[3]) const;
private:
    uint16_t moduleID;
};


#endif //DEFUSER_MODULE_H
