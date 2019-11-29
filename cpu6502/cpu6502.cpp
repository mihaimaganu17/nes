#include "cpu6502.h"
#include "../bus/Bus.h"

Cpu6502::Cpu6502()
{
    // Initialize the 16x16 lookup table
    using c = Cpu6502;  // for shorter writing
    lookup = 
    {
        {"BRK", &c::BRK, &c::IMP, 7}, {"ORA", &c::ORA, &c::IZX, 6}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"???", &c::XXX, &c::IMP, 3}, {"ORA", &c::ORA, &c::ZP0, 3}, {"ASL", &c::ASL, &c::ZP0, 5}, {"???", &c::XXX, &c::IMP, 5}, {"PHP", &c::PHP, &c::IMP, 3}, {"ORA", &c::ORA, &c::ABS, 2}, {"ASL", &c::ASL, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 4}, {"ORA", &c::ORA, &c::ABS, 4}, {"ASL", &c::ASL, &c::ABS, 6}, {"???", &c::XXX, &c::IMP, 6},
        {"BPL", &c::BPL, &c::REL, 2}, {"ORA", &c::ORA, &c::IZY, 5}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"???", &c::XXX, &c::IMP, 4}, {"ORA", &c::ORA, &c::ZPX, 4}, {"ASL", &c::ASL, &c::ZPX, 6}, {"???", &c::XXX, &c::IMP, 6}, {"CLC", &c::CLC, &c::IMP, 2}, {"ORA", &c::ORA, &c::ABY, 4}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 7}, {"???", &c::XXX, &c::IMP, 4}, {"ORA", &c::ORA, &c::ABX, 4}, {"ASL", &c::ASL, &c::ABX, 7}, {"???", &c::XXX, &c::IMP, 7},
        {"JSR", &c::JSR, &c::ABS, 6}, {"AND", &c::AND, &c::IZX, 6}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"BIT", &c::BIT, &c::ZP0, 3}, {"AND", &c::AND, &c::ZP0, 3}, {"ROL", &c::ROL, &c::ZP0, 5}, {"???", &c::XXX, &c::IMP, 5}, {"PLP", &c::PLP, &c::IMP, 4}, {"AND", &c::AND, &c::ABS, 2}, {"ROL", &c::ROL, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 2}, {"BIT", &c::BIT, &c::ABS, 4}, {"AND", &c::AND, &c::ABS, 4}, {"ROL", &c::ROL, &c::ABS, 6}, {"???", &c::XXX, &c::IMP, 6},
        {"BMI", &c::BMI, &c::REL, 2}, {"AND", &c::AND, &c::IZY, 5}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"???", &c::XXX, &c::IMP, 4}, {"AND", &c::AND, &c::ZPX, 4}, {"ROL", &c::ROL, &c::ZPX, 6}, {"???", &c::XXX, &c::IMP, 6}, {"SEC", &c::SEC, &c::IMP, 2}, {"AND", &c::AND, &c::ABY, 4}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 7}, {"???", &c::XXX, &c::IMP, 4}, {"AND", &c::AND, &c::ABX, 4}, {"ROL", &c::ROL, &c::ABX, 7}, {"???", &c::XXX, &c::IMP, 7},
        {"RTI", &c::RTI, &c::IMP, 6}, {"EOR", &c::EOR, &c::IZX, 6}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"???", &c::XXX, &c::IMP, 3}, {"EOR", &c::EOR, &c::ZP0, 3}, {"LSR", &c::LSR, &c::ZP0, 5}, {"???", &c::XXX, &c::IMP, 5}, {"PHA", &c::PHA, &c::IMP, 3}, {"EOR", &c::EOR, &c::ABS, 2}, {"LSR", &c::LSR, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 2}, {"JMP", &c::JMP, &c::ABS, 3}, {"EOR", &c::EOR, &c::ABS, 4}, {"LSR", &c::LSR, &c::ABS, 6}, {"???", &c::XXX, &c::IMP, 6},
        {"BVC", &c::BVC, &c::REL, 2}, {"EOR", &c::EOR, &c::IZY, 5}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"???", &c::XXX, &c::IMP, 4}, {"EOR", &c::EOR, &c::ZPX, 4}, {"LSR", &c::LSR, &c::ZPX, 6}, {"???", &c::XXX, &c::IMP, 6}, {"CLI", &c::CLI, &c::IMP, 2}, {"EOR", &c::EOR, &c::ABY, 4}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 7}, {"???", &c::XXX, &c::IMP, 4}, {"EOR", &c::EOR, &c::ABX, 4}, {"LSR", &c::LSR, &c::ABX, 7}, {"???", &c::XXX, &c::IMP, 7},
        {"RTS", &c::RTS, &c::IMP, 6}, {"ADC", &c::ADC, &c::IZX, 6}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"???", &c::XXX, &c::IMP, 3}, {"ADC", &c::ADC, &c::ZP0, 3}, {"ROR", &c::ROR, &c::ZP0, 5}, {"???", &c::XXX, &c::IMP, 5}, {"PLA", &c::PLA, &c::IMP, 4}, {"ADC", &c::ADC, &c::ABS, 2}, {"ROR", &c::ROR, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 2}, {"JMP", &c::JMP, &c::IND, 5}, {"ADC", &c::ADC, &c::ABS, 4}, {"ROR", &c::ROR, &c::ABS, 6}, {"???", &c::XXX, &c::IMP, 6},
        {"BVS", &c::BVS, &c::REL, 2}, {"ADC", &c::ADC, &c::IZY, 5}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"???", &c::XXX, &c::IMP, 4}, {"ADC", &c::ADC, &c::ZPX, 4}, {"ROR", &c::ROR, &c::ZPX, 6}, {"???", &c::XXX, &c::IMP, 6}, {"SEI", &c::SEI, &c::IMP, 2}, {"ADC", &c::ADC, &c::ABY, 4}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 7}, {"???", &c::XXX, &c::IMP, 4}, {"ADC", &c::ADC, &c::ABX, 4}, {"ROR", &c::ROR, &c::ABX, 7}, {"???", &c::XXX, &c::IMP, 7},
        {"???", &c::XXX, &c::IMP, 2}, {"STA", &c::STA, &c::IZX, 6}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"STY", &c::STY, &c::ZP0, 3}, {"STA", &c::STA, &c::ZP0, 3}, {"STX", &c::STX, &c::ZP0, 3}, {"???", &c::XXX, &c::IMP, 3}, {"DEY", &c::DEY, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 2}, {"TXA", &c::TXA, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 2}, {"STY", &c::STY, &c::ABS, 4}, {"STA", &c::STA, &c::ABS, 4}, {"STX", &c::STX, &c::ABS, 4}, {"???", &c::XXX, &c::IMP, 4},
        {"BCC", &c::BCC, &c::REL, 2}, {"STA", &c::STA, &c::IZY, 6}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"STY", &c::STY, &c::ZPX, 4}, {"STA", &c::STA, &c::ZPX, 4}, {"STX", &c::STX, &c::ZPY, 4}, {"???", &c::XXX, &c::IMP, 4}, {"TYA", &c::TYA, &c::IMP, 2}, {"STA", &c::STA, &c::ABY, 5}, {"TXS", &c::TXS, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 5}, {"???", &c::XXX, &c::IMP, 5}, {"STA", &c::STA, &c::ABX, 5}, {"???", &c::XXX, &c::IMP, 5}, {"???", &c::XXX, &c::IMP, 5},
        {"LDY", &c::LDY, &c::IMM, 2}, {"LDA", &c::LDA, &c::IZX, 6}, {"LDX", &c::LDX, &c::IMM, 2}, {"???", &c::XXX, &c::IMP, 8}, {"LDY", &c::LDY, &c::ZP0, 3}, {"LDA", &c::LDA, &c::ZP0, 3}, {"LDX", &c::LDX, &c::ZP0, 3}, {"???", &c::XXX, &c::IMP, 3}, {"TAY", &c::TAY, &c::IMP, 2}, {"LDA", &c::LDA, &c::ABS, 2}, {"TAX", &c::TAX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 2}, {"LDY", &c::LDY, &c::ABS, 4}, {"LDA", &c::LDA, &c::ABS, 4}, {"LDX", &c::LDX, &c::ABS, 4}, {"???", &c::XXX, &c::IMP, 4},
        {"BCS", &c::BCS, &c::REL, 2}, {"LDA", &c::LDA, &c::IZY, 5}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"LDY", &c::LDY, &c::ZPX, 4}, {"LDA", &c::LDA, &c::ZPX, 4}, {"LDX", &c::LDX, &c::ZPY, 4}, {"???", &c::XXX, &c::IMP, 4}, {"CLV", &c::CLV, &c::IMP, 2}, {"LDA", &c::LDA, &c::ABY, 4}, {"TSX", &c::TSX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 4}, {"LDY", &c::LDY, &c::ABX, 4}, {"LDA", &c::LDA, &c::ABX, 4}, {"LDX", &c::LDX, &c::ABY, 4}, {"???", &c::XXX, &c::IMP, 4},
        {"CPY", &c::CPY, &c::IMM, 2}, {"CMP", &c::CMP, &c::IZX, 6}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"CPY", &c::CPY, &c::ZP0, 3}, {"CMP", &c::CMP, &c::ZP0, 3}, {"DEC", &c::DEC, &c::ZP0, 5}, {"???", &c::XXX, &c::IMP, 5}, {"INY", &c::INY, &c::IMP, 2}, {"CMP", &c::CMP, &c::ABS, 2}, {"DEX", &c::DEX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 2}, {"CPY", &c::CPY, &c::ABS, 4}, {"CMP", &c::CMP, &c::ABS, 4}, {"DEC", &c::DEC, &c::ABS, 6}, {"???", &c::XXX, &c::IMP, 6},
        {"BNE", &c::BNE, &c::REL, 2}, {"CMP", &c::CMP, &c::IZY, 5}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"???", &c::XXX, &c::IMP, 4}, {"CMP", &c::CMP, &c::ZPX, 4}, {"DEC", &c::DEC, &c::ZPX, 6}, {"???", &c::XXX, &c::IMP, 6}, {"CLD", &c::CLD, &c::IMP, 2}, {"CMP", &c::CMP, &c::ABY, 4}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 7}, {"???", &c::XXX, &c::IMP, 4}, {"CMP", &c::CMP, &c::ABX, 4}, {"DEC", &c::DEC, &c::ABX, 7}, {"???", &c::XXX, &c::IMP, 7},
        {"CPX", &c::CPX, &c::IMM, 2}, {"SBC", &c::SBC, &c::IZX, 6}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"CPX", &c::CPX, &c::ZP0, 3}, {"SBC", &c::SBC, &c::ZP0, 3}, {"INC", &c::INC, &c::ZP0, 5}, {"???", &c::XXX, &c::IMP, 5}, {"INX", &c::INX, &c::IMP, 2}, {"SBC", &c::SBC, &c::ABS, 2}, {"NOP", &c::NOP, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 2}, {"CPX", &c::CPX, &c::ABS, 4}, {"SBC", &c::SBC, &c::ABS, 4}, {"INC", &c::INC, &c::ABS, 6}, {"???", &c::XXX, &c::IMP, 6},
        {"BEQ", &c::BEX, &c::REL, 2}, {"SBC", &c::SBC, &c::IZY, 5}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 8}, {"???", &c::XXX, &c::IMP, 4}, {"SBC", &c::SBC, &c::ZPX, 4}, {"INC", &c::INC, &c::ZPX, 6}, {"???", &c::XXX, &c::IMP, 6}, {"SED", &c::SED, &c::IMP, 2}, {"SBC", &c::SBC, &c::ABY, 4}, {"???", &c::XXX, &c::IMP, 2}, {"???", &c::XXX, &c::IMP, 7}, {"???", &c::XXX, &c::IMP, 4}, {"SBC", &c::SBC, &c::ABX, 4}, {"INC", &c::INC, &c::ABX, 7}, {"???", &c::XXX, &c::IMP, 7},
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

void Cpu6502::clock()
{
    // Emulate the wait of clock cycle to execute an instruction
    if(cycles == 0){
        // Get the opcode at the program counter location
        opcode = read(pc);
        pc += 1;            // Move to next byte
        cycles = lookup[opcode].cycles;     // Get cycles needed to process the command
        uint8_t add_cycle1 = (this->*lookup[opcode].addrmode)(); // Get the address mode used by the opcode
        uint8_t add_cycle2 = (this->*lookup[opcode].opc)();      // Execute the opcode

        // Add another cycle if an instruction exceeds page boundaries
        cycle += (add_cycle1 & add_cycle2);
    }
    cycles -= 1;
}

uint8_t Cpu6502::IMP()
{
    // Implied: take the value as a default(can work with the accumulator): ex: CLC
    fetched = acc;
    return 0;
}

uint8_t Cpu6502::IMM()
{
    // Immediate: take the value as it is(not an address)
    addr_abs = pc++;
    return 0;
}

uint8_t Cpu6502::ZP0()
{
    // Zero page addressing: the data can be found somewhere in page zero
    // Mean the hi-byte from 0xFFFF is zero: 0x0000 -> 0x00FF
    addr_abs = read(pc);
    pc++;
    addr_abs &= 0x00FF;     // To make sure is in the first page
    return 0;
}

uint8_t Cpu6502::ZPX()
{
    // Zero page addressing with x offset
    addr_abs = (read(pc) + x);
    pc++;
    addr_abs &= 0x00FF;     // Read just the low byte
    return 0;
}

uint8_t Cpu6502::ZPY()
{
    // Zero page addressing with y offset
    addr_abs = (read(pc) + y);
    pc++;
    addr_abs &= 0x00FF;     // Read just the low byte
    return 0;
}

uint8_t Cpu6502::ABS()
{
    //Absolute addresing, using the hi and lo byte
    uint8_t lo = read(pc);
    pc++;
    uint8_t hi = read(pc);
    pc++;
    addr_abs = (hi << 8) | lo;
    return 0;
}

uint8_t Cpu6502::ABX()
{
    // Absolute addressing using x offset
    uint8_t lo = read(pc);
    pc++;
    uint8_t hi = read(pc);
    pc++;
    addr_abs = (hi << 8) | lo;
    addr_abs += x;
    
    // test if adding X offset got us to another page
    // and return 1 to add another clock cycle to finish the instruction
    if((addr_abs & 0xFF00) != (hi << 8)){
        return 1;
    } else {
        return 0;
    }
}

uint8_t Cpu6502::ABY()
{
    // Absolute addressing using y offset
    uint8_t lo = read(pc);
    pc++;
    uint8_t hi = read(pc);
    pc++;
    addr_abs = (hi << 8) | lo;
    addr_abs += y;
    
    // test if adding y offset got us to another page
    // and return 1 to add another clock cycle to finish the instruction
    if((addr_abs & 0xFF00) != (hi << 8)){
        return 1;
    } else {
        return 0;
    }
    
}

uint8_t Cpu6502::IND()
{
    // Indirect addressing: reading pointer of that address
    uint8_t ptr_lo = read(pc);
    pc++;
    uint8_t ptr_hi = read(pc);
    pc++;

    uint16_t ptr = (ptr_hi << 8) | ptr_lo;

    if(ptr_lo == 0xFF){
        // Simulate page boundary hardware bug
        addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr);
    } else {
        // Value is represented as a word(2_byte) at ptr address;
        addr_abs = (read(ptr + 1) << 8) | read(ptr);
    }

    return 0;
}

uint8_t Cpu6502::IZX()
{
    // Indirect addressing: reading pointer of that address
    uint16_t ptr = read(pc);
    pc++;

    // Value is represented as a word(2_byte) at ptr address with offset x;
    // Need to pass arg as uint16_t for read function
    uint16_t lo = read((uint16_t)(ptr + (uint16_t)x) & 0x00FF);
    uint16_t hi = read((uint16_t)(ptr + (uint16_t)x + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;

    return 0;
}

uint8_t Cpu6502::IZY()
{
    // Indirect zero page addressing with Y
    uint16_t ptr = read(pc);
    pc++;

    uint16_t lo = read(ptr & 0x00FF);
    uint16_t hi = read((ptr + 1) & 0x00FF);

    addr_abs = (hi << 8) | lo;
    addr_abs += y;

    if((addr_abs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t Cpu6502::REL()
{
    // Relative addressing mode: only applies to branching instructions
    // Can jump only in vecinity of -127, + 128
    addr_rel = read(pc);
    pc++;
    // Check wheter is signed or unsigned data type by checking the first bit(128th)
    if(addr_rel & 0x80)
        addr_rel |= 0xFF00;
    return 0;
}

// Instructions
// First fetch the data that we are using
uint8_t Cpu6502::fetch()
{
    // Ignore implied mode
    if(!lookup[opcode].addrmode == &Cpu6502::IMP){
        fetched = read(addr_abs);
    }
    return fetched;
}

uint8_t Cpu6502::AND()
{
    fetch();
    acc = acc & fetched;
    // Set flags
    SetFlag(Z, a == 0x00);
    SetFlag(N, a & 0x80);

    // Aditional clock cycle
    return 1;
}

void Cpu6502::branch()
{
    // Add clock cycle to branch
    cycles += 1;
    addr_abs = pc + addr_rel;

    // Check if it changes the page
    if((addr_abs & 0xFF00) != (pc & 0xFF00))
        // add a cycle
        cycles++;
    // Execution continues from that point on
    pc = addr_abs;
}

uint8_t Cpu6502::BCS()
{
    // Branch if carry bit is set
    // Check if it is
    if(GetFlag(C) == 1){
        branch();
    }
    return 0;
}

uint8_t Cpu6502::BCC()
{
    // Brach if carry clear
    if(GetFlag(C) == 0)
        branch();
    return 0;
}

uint8_t Cpu6502::BEQ()
{
    // Brach if equal(zero set)
    if(GetFlag(Z) == 1)
        branch();
    return 0;
}

uint8_t Cpu6502::BNE()
{
    // Brach if not equal(zero clear)
    if(GetFlag(Z) == 0)
        branch();
    return 0;
}

uint8_t Cpu6502::BMI()
{
    // Brach if minus(negative set)
    if(GetFlag(N) == 0)
        branch();
    return 0;
}

uint8_t Cpu6502::BPL()
{
    // Brach on plus(negative clear
    if(GetFlag(N) == 0)
        branch();
    return 0;
}

uint8_t Cpu6502::BVC()
{
    // Brach on overflow clear
    if(GetFlag(V) == 0)
        branch();
    return 0;
}

uint8_t Cpu6502::BVS()
{
    // Brach on overflow set 
    if(GetFlag(V) == 1)
        branch();
    return 0;
}

uint8_t Cpu6502::CLC()
{
    // Clear carry bit
    SetFlag(C, false);
    return 0;
}

uint8_t Cpu6502::CLD()
{
    // Clear decimal bit
    SetFlag(D, false);
    return 0;
}

uint8_t Cpu6502::CLI()
{
    // Clear interrupt disable
    SetFlag(I, false);
    return 0;
}

uint8_t Cpu6502::CLV()
{
    // Clear overflow bit
    SetFlag(V, false);
    return 0;
}

uint8_t Cpu6502::ADC()
{

}

uint8_t Cpu6502::SBC()
{

}














