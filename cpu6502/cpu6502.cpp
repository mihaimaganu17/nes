#include "cpu6502.h"
#include "../bus/Bus.h"

Cpu6502::Cpu6502()
{
    // Initialize the 16x16 lookup table
    using c = Cpu6502;  // for shorter writing
    lookup = 
    {

    }
}

Cpu6502::~Cpu6502()
{
}

void Cpu6502::write(uint16_t addr, uint8_t data)
{
    bus->write(addr, data);
}

uint8_t Cpu6502::read(uint16_t addr)
{
    return bus->read(addr, false);
}
