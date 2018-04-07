#ifndef DEFUSER_MODULE_H
#define DEFUSER_MODULE_H

#include <cstdint>

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

private:
    uint16_t seed;
};


#endif //DEFUSER_MODULE_H
