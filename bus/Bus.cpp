#include "Bus.h"

Bus::Bus()
{
    // Clear RAM at the start
    for(auto &i : ram)
        i = 0x00;

    // Connect the CPU to this bus
    cpu.ConnectBus(this);
    
}

Bus::~Bus()
{
}

void Bus::write(uint16_t addr, uint8_t data)
{
    // Guard the range of RAM
    if(addr >= 0x0000 && addr <= 0xFFFF){
        ram[addr] = data;
    }
}

uint8_t Bus::read(uint16_t addr, bool bReadOnly = false)
{
    // Guard the range of RAM
    if(addr >= 0x0000 && addr <= 0xFFFF)
        return ram[addr];
    return 0x00;
}
