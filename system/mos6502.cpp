/**
 * Emulator for the MOS Technology 6502 8-bit microprocessor.
 * Used in the NES.
 *
 * (c) 2021 Joseph Azrak
 * josephazrak.codes
 * https://github.com/josephazrak
 */

#include "mos6502.hpp"
#include <iostream>
#include <iomanip>
#include <bitset>

MOS6502::MOS6502(RAM* ram_ref) {
    // Bind this CPU to RAM
    NES_Ram = ram_ref;

    // Create the lookup table. Thank you javidx9 (https://github.com/OneLoneCoder) for the hard work compiling this!
    using a = MOS6502;
    lookup_table =
        {
            { "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
            { "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
            { "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
            { "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
            { "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
            { "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
            { "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
            { "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
            { "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
            { "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
            { "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
            { "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
            { "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
            { "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
            { "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
            { "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
        };
}

MOS6502::~MOS6502() = default;

void MOS6502::set_flag(flag_t flag)
{
    FLG = FLG | (1 << flag.bitshift);
}

void MOS6502::clear_flag(flag_t flag)
{
    FLG = FLG & (~flag.bitmask);
}

void MOS6502::flip_flag(flag_t flag)
{
    FLG = FLG ^ flag.bitmask;
}

uint8_t MOS6502::get_flag(flag_t flag) const
{
    return (FLG & flag.bitmask) >> flag.bitshift;
}

uint8_t MOS6502::get_status() const
{
    return FLG;
}

void MOS6502::whoami() const
{
    std::cout << "===================================" << std::endl;
    std::cout << "MOS6502 EMULATED INSTANCE (status=" << status << ")" << std::endl;
    std::cout << "---REGISTERS---" << std::endl;
    std::cout << "ACC=0d" << std::setw(4) << ACC << ";      RAX=0d" << std::setw(4) << X << "; RAY=0d" << std::setw(4) << Y << std::endl;
    std::cout << "FLG=0b" << std::bitset<8> (FLG) << "; PC=$" << std::hex << std::setw(5) << PC << "; SP=$" << std::setw(5) << SP << std::dec << std::endl;
    std::cout << "---MISC---" << std::endl;
    std::cout << "FETCH_DATA: " << fetched << std::endl;
    std::cout << "---FLAGS--- " << std::endl;
    std::cout << std::bitset<8> (FLG) << std::endl;
    std::cout << "NVssDIZC" << std::endl;
    std::cout << "===================================" << std::endl;
}

void MOS6502::reset()
{
    // Reset registers
    ACC = 0; X = 0; Y = 0; FLG = 0; PC = 0; SP = 0;

    // Reset pseudo-registers
    fetched = 0;
}

void MOS6502::fetch_data()
{
    fetched = NES_Ram->read_byte(fetch_address);
}

// ===========================
// CLOCK FUNCTION
// ===========================

void MOS6502::clock()
{
    if (clock_cycles_remaining > 0)
    {
        std::cout << "[Clock] We are still processing an instruction; " << (int)clock_cycles_remaining << " cycle(s) left" << std::endl;
        clock_cycles_remaining--;
        return; // We are done for this clock cycle.
    }

    // If we have gotten to this point, the last instruction has successfully
    // completed. Let's load the next instruction, which now should be at PC.
    // Increment PC because we have read that byte.
    uint8_t instruction_opcode = this->NES_Ram->read_byte(PC++);

    // Retrieve information about this opcode, such as the addressing mode
    // and minimum clock cycles.
    clock_cycles_remaining = lookup_table[instruction_opcode].cycles;
    std::cout << "[Clock] Read new opcode: " << lookup_table[instruction_opcode].name << std::endl;
    last_read_opcode = instruction_opcode;

    // Run the addressing mode function to retrieve the to-fetch address and
    // run the operation function to actually perform the operation.
    uint8_t adcycle_1 = (this->*lookup_table[instruction_opcode].addrmode)();
    uint8_t adcycle_2 = (this->*lookup_table[instruction_opcode].operate)();

    // If both adcycle_1 and adcycle_2 are equal to 1, then add 1 to clock_cycles_remaining.
    clock_cycles_remaining += (adcycle_1 & adcycle_2);

    std::cout << "[Clock] Now faking this many clock cycles: " << (int)clock_cycles_remaining << std::endl;

    clock_cycles_remaining--;
}

// ===========================
// HELPER FUNCTIONS
// ===========================

uint8_t MOS6502::decode_signed_byte(uint8_t byte)
{
    bool is_negative = !!((byte & 0x80) >> 7); // Bool determines whether the rel addr is negative.

    return (byte & 0x7f) * (is_negative ? -1 : 1);
}

// ===========================
// ADDRESSING MODES
// ===========================

// An addressing mode function retrieves data as implemented by that addressing mode.
// It stores the data fetched in the `fetched' variable, which is then used by the operation
// function. It returns an integer representing the additional number of cycles necessary
// to emulate to faithfully reproduce the 6502's behavior.

/**
 * Addressing mode: implied.
 */
uint8_t MOS6502::IMP()
{
    fetched = ACC;
    return 0;
}

/**
 * Addressing mode: immediate.
 */
uint8_t MOS6502::IMM()
{
    // In this addressing mode, the address to fetch is immediately after the instruction
    // (which is located @ PC). As such, increment PC and fetch the byte at PC -- this
    // is the fetch address.

    fetch_address = NES_Ram->read_byte(PC++);

    return 0;
}

/**
 * Addressing mode: zero-page.
 */
uint8_t MOS6502::ZP0()
{
    // A single byte is given after the opcode. This is the lo-byte.
    // The hi-byte is automatically set to 00 (hence the zero-page).
    // Eg: for argument FA, the read address is $00FA.

    fetch_address = (NES_Ram->read_byte(PC++) & 0x00FF); // Take only the lo-byte.

    return 0;
}

/**
 * Addressing mode: zero-page, x-offset.
 */
uint8_t MOS6502::ZPX()
{
    // Same as ZP0, except that we add the X-register contents.

    fetch_address = (NES_Ram->read_byte(PC++) & 0x00FF) + X;

    return 0;
}

/**
 * Addressing mode: zero-page, y-offset.
 */
uint8_t MOS6502::ZPY()
{
    // Same as ZP0, except that we add the Y-register contents.

    fetch_address = (NES_Ram->read_byte(PC++) & 0x00FF) + Y;

    return 0;
}

/**
 * Addressing mode: relative.
 */
uint8_t MOS6502::REL()
{
    // Used for branching. The byte immediately after the instruction is fetched.
    // This byte is treated as a signed value (that is, +- depending on bit 7).
    // This value is written to `branch_relative'. The branching instruction should
    // increment or decrement PC accordingly by `branch_relative'.

    std::cout << "[Addressing] REL addressing invoked." << std::endl;

    branch_relative = decode_signed_byte(NES_Ram->read_byte(PC++));

    std::cout << std::dec << "[Addressing] Calculated jump = " << +branch_relative << std::endl;
}

/**
 * Addressing mode: absolute.
 */
uint8_t MOS6502::ABS()
{

}

/**
 * Addressing mode: absolute, x-offset.
 */
uint8_t MOS6502::ABX()
{

}

/**
 * Addressing mode: absolute, y-offset.
 */
uint8_t MOS6502::ABY()
{

}

/**
 * Addressing mode: indirect.
 */
uint8_t MOS6502::IND()
{

}

/**
 * Addressing mode: indirect, x-offset.
 */
uint8_t MOS6502::IZX()
{

}

/**
 * Addressing mode: indirect, y-offset.
 */
uint8_t MOS6502::IZY()
{

}

// ===========================
// OPERATIONS
// ===========================

uint8_t MOS6502::ADC()
{

}

uint8_t MOS6502::AND()
{

}

uint8_t MOS6502::ASL()
{

}

uint8_t MOS6502::BCC()
{

}

uint8_t MOS6502::BCS()
{

}

uint8_t MOS6502::BEQ()
{
    // Operation: Branch on Result Zero
    // Check whether the zero-flag is set. If so, invoke REL addressing mode,
    // apply the offset that is the result of REL, and jump there. If not,
    // do nothing.

    if (get_flag(FLAG_Z_ZERO))
    {

    }
}

uint8_t MOS6502::BIT()
{

}

uint8_t MOS6502::BMI()
{

}

uint8_t MOS6502::BNE()
{

}

uint8_t MOS6502::BPL()
{

}

uint8_t MOS6502::BRK()
{

}

uint8_t MOS6502::BVC()
{

}

uint8_t MOS6502::BVS()
{

}

uint8_t MOS6502::CLC()
{

}

uint8_t MOS6502::CLD()
{

}

uint8_t MOS6502::CLI()
{

}

uint8_t MOS6502::CLV()
{

}

uint8_t MOS6502::CMP()
{

}

uint8_t MOS6502::CPX()
{

}

uint8_t MOS6502::CPY()
{

}

uint8_t MOS6502::DEC()
{

}

uint8_t MOS6502::DEX()
{

}

uint8_t MOS6502::DEY()
{

}

uint8_t MOS6502::EOR()
{

}

uint8_t MOS6502::INC()
{

}

uint8_t MOS6502::INX()
{

}

uint8_t MOS6502::INY()
{

}

uint8_t MOS6502::JMP()
{

}

uint8_t MOS6502::JSR()
{

}

uint8_t MOS6502::LDA()
{

}

uint8_t MOS6502::LDX()
{

}

uint8_t MOS6502::LDY()
{

}

uint8_t MOS6502::LSR()
{

}

uint8_t MOS6502::NOP()
{

}

uint8_t MOS6502::ORA()
{

}

uint8_t MOS6502::PHA()
{

}

uint8_t MOS6502::PHP()
{

}

uint8_t MOS6502::PLA()
{

}

uint8_t MOS6502::PLP()
{

}

uint8_t MOS6502::ROL()
{

}

uint8_t MOS6502::ROR()
{

}

uint8_t MOS6502::RTI()
{

}

uint8_t MOS6502::RTS()
{

}

uint8_t MOS6502::SBC()
{

}

uint8_t MOS6502::SEC()
{

}

uint8_t MOS6502::SED()
{

}

uint8_t MOS6502::SEI()
{

}

uint8_t MOS6502::STA()
{

}

uint8_t MOS6502::STX()
{

}

uint8_t MOS6502::STY()
{

}

uint8_t MOS6502::TAX()
{

}

uint8_t MOS6502::TAY()
{

}

uint8_t MOS6502::TSX()
{

}

uint8_t MOS6502::TXA()
{

}

uint8_t MOS6502::TXS()
{

}

uint8_t MOS6502::TYA()
{

}

uint8_t MOS6502::XXX() {
    std::cout << "[6502] Hit an unimplemented opcode at $" << std::setw(4) << std::hex << PC - 1 << std::dec << "." << std::endl;
    std::cout << "[6502] byte \"" << std::setw(2) << std::hex << +last_read_opcode << "\" at $" << std::setw(4) << PC - 1 << std::endl;
    std::cout << "[6502] Halting execution" << std::endl;

    exit(0);

    return 0;
}