#ifndef "__CPU6502_H"
#define "__CPU6502_H"

#include <vector.h>
#include <string.h>

// Connect the CPU to the Bus
class Bus;

class Cpu6502
{
public:
    Cpu6502();
    ~Cpu6502();

    void ConnectBus(Bus *b) { bus = b; }

    // Addressing Modes -> how the CPU interprets the values after the instruction
    uint8_t ABS();  // Using ABSolute addresss
    uint8_t ABX();  // Using ABSolute address + X with carry
    uint8_t ABY();  // Same as above for Y
    uint8_t IMM();  // Immediate -> using the value as it is, ex: OPC #$BB, using byte <BB>
    uint8_t IMP();  // Implied -> no need for operand
    uint8_t IND();  // Indirect: OPC ($LLHH) -> effective address is contents of word at address $HHLL
    uint8_t IZX();  // X-indexed, indirect: OPC ($LL,X), zp addr, effective addr is word in
                    // (LL + X, LL + X + 1), inc without carry
    uint8_t IZY();  // Y-indexed, indirect: OPC ($LL),Y, zp addr, effective addr is word in
                    // (LL, LL + 1) inc by T with carry
    uint8_t REL();  // Relative, eddective address is PC + signed offset BB(branched offsets)
    uint8_t ZP0();  // OPC $LL zp(zeropage) address (hi-byte is zero, address = $00LL)
    uint8_t ZPX();  // OPC $LL,X: eddective addres is address inc by X without carry
    uint8_t ZPY();  // Same as above for Y

    // Opcodes
    uint8_t ADC();  uint8_t AND();  uint8_t ASL();  uint8_t BCC();
    uint8_t BCS();  uint8_t BEQ();  uint8_t BIT();  uint8_t BMI();  
    uint8_t BNE();  uint8_t BPL();
    uint8_t BRK();  uint8_t BVC();  uint8_t BVS();  uint8_t CLC();
    uint8_t CLD();  uint8_t CLI();  uint8_t CLV();  uint8_t CMP();
    uint8_t CPX();  uint8_t CPY();  uint8_t DEC();  uint8_t DEX();
    uint8_t DEY();  uint8_t EOR();  uint8_t INC();  uint8_t INX();
    uint8_t INY();  uint8_t JMP();  uint8_t JSR();  uint8_t LDA();
    uint8_t LDX();  uint8_t LDY();  uint8_t LSR();  uint8_t NOP();
    uint8_t ORA();  uint8_t PHA();  uint8_t PHP();  uint8_t PLA();
    uint8_t PLP();  uint8_t ROL();  uint8_t ROR();  uint8_t RTI();
    uint8_t RTS();  uint8_t SBC();  uint8_t SEC();  uint8_t SED();
    uint8_t SEI();  uint8_t STA();  uint8_t STX();  uint8_t STY();
    uint8_t TAX();  uint8_t TAY();  uint8_t TSX();  uint8_t TXA();
    uint8_t TXS();  uint8_t TYA();

    uint8_t XXX();  // Illegal Opcodes

    void clock();   // Tell the CPU a clock cycle occurs
    void reset();   
    void irq();     // Interrupt request signal, can be ignored depending on I flag
    void nmi();     // Non-maskable interrupt request signal, can never be ignored

    uint8_t fetch();            // Fetch data
    uint8_t fetched = 0x00;     // Store fetched data
    uint16_t addr_abs = 0x0000; // Place to read from
    uint8_t addr_rel = 0x00;
    uint8_t opcode = 0x00;      // Current opcode;
    uint8_t cycles = 0x00;      // Cycle count;

public:
    // Status flags -> NVuB DIZC
    enum FLAGS6502 {
        C = (1 << 0),   // Carry flag
        Z = (1 << 1),   // Zero
        I = (1 << 2),   // Disable Interrupts
        D = (1 << 3),   // Decimal
        B = (1 << 4),   // Break
        U = (1 << 5),   // Unused
        V = (1 << 6),   // Overflow
        N = (1 << 7),   // Negative
    }

    // Registers
    uint8_t acc = 0x00;     // Accumulator
    uint8_t x = 0x00;       // X register
    uint8_t y = 0x00;       // Y register
    uint8_t sp = 0x00;      // Stack pointer
    uint16_t pc = 0x0000;      // Program Counter
    uint8_t status = 0x00;  // Status Register

private:
    Bus *bus = NULL;
    void write(uint16_t addr, uint8_t data);
    uint8_t read(uint16_t addr);

    // Get/Set the status flags
    uint8_t GetFlag(FLAGS6502 flag);
    void    SetFlag(FLAGS6502 flag, bool status);

    struct INSTRUCTION
    {
        std::string name;
        uint8_t(Cpu6502::*opc)(void) = NULL;
        uint8_t(Cpu6502::*addrmode)(void) = NULL;
        uint8_t cycles = 0;
    };

    // Store all instructions in a vector
    std::vector<INSTRUCTION> lookup;
}

#endif 
