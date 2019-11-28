#include "cpu6502.h"
#include "../bus/Bus.h"

Cpu6502::Cpu6502()
{
    // Initialize the 16x16 lookup table
    using c = Cpu6502;  // for shorter writing
    lookup = 
    {
        {"BRK", &c::BRK, &c::IMP, }, {"ORA", &c::ORA, &c::IZX, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"ORA", &c::ORA, &c::ZP0, }, {"ASL", &c::ASL, &c::ZP0, }, {"???", &c::XXX, &c::IMP, }, {"PHP", &c::PHP, &c::IMP, }, {"ORA", &c::ORA, &c::ABS, }, {"ASL", &c::ASL, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"ORA", &c::ORA, &c::ABS, }, {"ASL", &c::ASL, &c::ABS, }, {"???", &c::XXX, &c::IMP, },
        {"BPL", &c::BPL, &c::REL, }, {"ORA", &c::ORA, &c::IZY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"ORA", &c::ORA, &c::ZPX, }, {"ASL", &c::ASL, &c::ZPX, }, {"???", &c::XXX, &c::IMP, }, {"CLC", &c::CLC, &c::IMP, }, {"ORA", &c::ORA, &c::ABY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"ORA", &c::ORA, &c::ABX, }, {"ASL", &c::ASL, &c::ABX, }, {"???", &c::XXX, &c::IMP, },
        {"JSR", &c::JSR, &c::ABS, }, {"AND", &c::AND, &c::IZX, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"BIT", &c::BIT, &c::ZP0, }, {"AND", &c::AND, &c::ZP0, }, {"ROL", &c::ROL, &c::ZP0, }, {"???", &c::XXX, &c::IMP, }, {"PLP", &c::PLP, &c::IMP, }, {"AND", &c::AND, &c::ABS, }, {"ROL", &c::ROL, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"BIT", &c::BIT, &c::ABS, }, {"AND", &c::AND, &c::ABS, }, {"ROL", &c::ROL, &c::ABS, }, {"???", &c::XXX, &c::IMP, },
        {"BMI", &c::BMI, &c::REL, }, {"AND", &c::AND, &c::IZY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"AND", &c::AND, &c::ZPX, }, {"ROL", &c::ROL, &c::ZPX, }, {"???", &c::XXX, &c::IMP, }, {"SEC", &c::SEC, &c::IMP, }, {"AND", &c::AND, &c::ABY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"AND", &c::AND, &c::ABX, }, {"ROL", &c::ROL, &c::ABX, }, {"???", &c::XXX, &c::IMP, },
        {"RTI", &c::RTI, &c::IMP, }, {"EOR", &c::EOR, &c::IZX, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"EOR", &c::EOR, &c::ZP0, }, {"LSR", &c::LSR, &c::ZP0, }, {"???", &c::XXX, &c::IMP, }, {"PHA", &c::PHA, &c::IMP, }, {"EOR", &c::EOR, &c::ABS, }, {"LSR", &c::LSR, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"JMP", &c::JMP, &c::ABS, }, {"EOR", &c::EOR, &c::ABS, }, {"LSR", &c::LSR, &c::ABS, }, {"???", &c::XXX, &c::IMP, },
        {"BVC", &c::BVC, &c::REL, }, {"EOR", &c::EOR, &c::IZY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"EOR", &c::EOR, &c::ZPX, }, {"LSR", &c::LSR, &c::ZPX, }, {"???", &c::XXX, &c::IMP, }, {"CLI", &c::CLI, &c::IMP, }, {"EOR", &c::EOR, &c::ABY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"EOR", &c::EOR, &c::ABX, }, {"LSR", &c::LSR, &c::ABX, }, {"???", &c::XXX, &c::IMP, },
        {"RTS", &c::RTS, &c::IMP, }, {"ADC", &c::ADC, &c::IZX, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"ADC", &c::ADC, &c::ZP0, }, {"ROR", &c::ROR, &c::ZP0, }, {"???", &c::XXX, &c::IMP, }, {"PLA", &c::PLA, &c::IMP, }, {"ADC", &c::ADC, &c::ABS, }, {"ROR", &c::ROR, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"JMP", &c::JMP, &c::IND, }, {"ADC", &c::ADC, &c::ABS, }, {"ROR", &c::ROR, &c::ABS, }, {"???", &c::XXX, &c::IMP, },
        {"BVS", &c::BVS, &c::REL, }, {"ADC", &c::ADC, &c::IZY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"ADC", &c::ADC, &c::ZPX, }, {"ROR", &c::ROR, &c::ZPX, }, {"???", &c::XXX, &c::IMP, }, {"SEI", &c::SEI, &c::IMP, }, {"ADC", &c::ADC, &c::ABY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"ADC", &c::ADC, &c::ABX, }, {"ROR", &c::ROR, &c::ABX, }, {"???", &c::XXX, &c::IMP, },
        {"???", &c::XXX, &c::IMP, }, {"STA", &c::STA, &c::IZX, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"STY", &c::STY, &c::ZP0, }, {"STA", &c::STA, &c::ZP0, }, {"STX", &c::STX, &c::ZP0, }, {"???", &c::XXX, &c::IMP, }, {"DEY", &c::DEY, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"TXA", &c::TXA, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"STY", &c::STY, &c::ABS, }, {"STA", &c::STA, &c::ABS, }, {"STX", &c::STX, &c::ABS, }, {"???", &c::XXX, &c::IMP, },
        {"BCC", &c::BCC, &c::REL, }, {"STA", &c::STA, &c::IZY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"STY", &c::STY, &c::ZPX, }, {"STA", &c::STA, &c::ZPX, }, {"STX", &c::STX, &c::ZPY, }, {"???", &c::XXX, &c::IMP, }, {"TYA", &c::TYA, &c::IMP, }, {"STA", &c::STA, &c::ABY, }, {"TXS", &c::TXS, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"STA", &c::STA, &c::ABX, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, },
        {"LDY", &c::LDY, &c::IMM, }, {"LDA", &c::LDA, &c::IZX, }, {"LDX", &c::LDX, &c::IMM, }, {"???", &c::XXX, &c::IMP, }, {"LDY", &c::LDY, &c::ZP0, }, {"LDA", &c::LDA, &c::ZP0, }, {"LDX", &c::LDX, &c::ZP0, }, {"???", &c::XXX, &c::IMP, }, {"TAY", &c::TAY, &c::IMP, }, {"LDA", &c::LDA, &c::ABS, }, {"TAX", &c::TAX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"LDY", &c::LDY, &c::ABS, }, {"LDA", &c::LDA, &c::ABS, }, {"LDX", &c::LDX, &c::ABS, }, {"???", &c::XXX, &c::IMP, },
        {"BCS", &c::BCS, &c::REL, }, {"LDA", &c::LDA, &c::IZY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"LDY", &c::LDY, &c::ZPX, }, {"LDA", &c::LDA, &c::ZPX, }, {"LDX", &c::LDX, &c::ZPY, }, {"???", &c::XXX, &c::IMP, }, {"CLV", &c::CLV, &c::IMP, }, {"LDA", &c::LDA, &c::ABY, }, {"TSX", &c::TSX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"LDY", &c::LDY, &c::ABX, }, {"LDA", &c::LDA, &c::ABX, }, {"LDX", &c::LDX, &c::ABY, }, {"???", &c::XXX, &c::IMP, },
        {"CPY", &c::CPY, &c::IMM, }, {"CMP", &c::CMP, &c::IZX, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"CPY", &c::CPY, &c::ZP0, }, {"CMP", &c::CMP, &c::ZP0, }, {"DEC", &c::DEC, &c::ZP0, }, {"???", &c::XXX, &c::IMP, }, {"INY", &c::INY, &c::IMP, }, {"CMP", &c::CMP, &c::ABS, }, {"DEX", &c::DEX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"CPY", &c::CPY, &c::ABS, }, {"CMP", &c::CMP, &c::ABS, }, {"DEC", &c::DEC, &c::ABS, }, {"???", &c::XXX, &c::IMP, },
        {"BNE", &c::BNE, &c::REL, }, {"CMP", &c::CMP, &c::IZY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"CMP", &c::CMP, &c::ZPX, }, {"DEC", &c::DEC, &c::ZPX, }, {"???", &c::XXX, &c::IMP, }, {"CLD", &c::CLD, &c::IMP, }, {"CMP", &c::CMP, &c::ABY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"CMP", &c::CMP, &c::ABX, }, {"DEC", &c::DEC, &c::ABX, }, {"???", &c::XXX, &c::IMP, },
        {"CPX", &c::CPX, &c::IMM, }, {"SBC", &c::SBC, &c::IZX, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"CPX", &c::CPX, &c::ZP0, }, {"SBC", &c::SBC, &c::ZP0, }, {"INC", &c::INC, &c::ZP0, }, {"???", &c::XXX, &c::IMP, }, {"INX", &c::INX, &c::IMP, }, {"SBC", &c::SBC, &c::ABS, }, {"NOP", &c::NOP, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"CPX", &c::CPX, &c::ABS, }, {"SBC", &c::SBC, &c::ABS, }, {"INC", &c::INC, &c::ABS, }, {"???", &c::XXX, &c::IMP, },
        {"BEQ", &c::BEX, &c::REL, }, {"SBC", &c::SBC, &c::IZY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"SBC", &c::SBC, &c::ZPX, }, {"INC", &c::INC, &c::ZPX, }, {"???", &c::XXX, &c::IMP, }, {"SED", &c::SED, &c::IMP, }, {"SBC", &c::SBC, &c::ABY, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"???", &c::XXX, &c::IMP, }, {"SBC", &c::SBC, &c::ABX, }, {"INC", &c::INC, &c::ABX, }, {"???", &c::XXX, &c::IMP, },
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
