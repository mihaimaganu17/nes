#ifndef "__BUS_H"
#define "__BUS_H"

#include <cstdint.h>
#include <array.h>
#include "../cpu/cpu6502.h"

class Bus
{
public:
    Bus();
    ~Bus();

public:
    // Devices connected to the Bus
    cpu6502 cpu;

    // Emulate the 64KB of RAM using an array
    std::array<uint8_t, 64 * 1024> ram;

public:
    // Read and Write from/to the Bus
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr, bool bReadOnly = false);
};

#endif
