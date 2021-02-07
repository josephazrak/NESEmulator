//
// Created by Joseph Azrak on 05/02/2021.
//

#ifndef NESEMULATOR_MOS6502_HPP
#define NESEMULATOR_MOS6502_HPP

#include "ram.hpp"
#include <vector>
#include <string>

typedef uint8_t byte_t;

struct opcode
{
    byte_t opcode_byte;   // A single byte which identifies this operation
};

struct flag_t
{
    byte_t bitmask;
    byte_t bitshift;
};


// Bitmasks for the 6502 microprocessor's status register.
const flag_t FLAG_C_CARRY = {1 << 0, 0};
const flag_t FLAG_Z_ZERO  = {1 << 1, 1};
const flag_t FLAG_I_IRQD  = {1 << 2, 2};
const flag_t FLAG_D_DECI  = {1 << 3, 3};
const flag_t FLAG_s_UNUS  = {1 << 4, 4};
const flag_t FLAG_s_UNUZ  = {1 << 5, 5};
const flag_t FLAG_V_OVERF = {1 << 6, 6};
const flag_t FLAG_N_NEGTV = {1 << 7, 7};

class MOS6502
{
private:
    // CPU status human-readable string
    std::string status = "stopped";

    // Pointer to the RAM object.
    RAM NES_Ram;

    // CPU registers.
    byte_t ACC = 0b00000000; // Accumulator.
    byte_t X = 0b00000000;   // X-register.
    byte_t Y = 0b00000000;   // Y-register.
    byte_t FLG = 0b00000000; // Status flags.
    byte_t PC = 0b00000000;  // Program counter.
    byte_t SP = 0b00000000;  // Stack pointer.

    // Other intermediate data.
    byte_t fetched = 0b0000000; // Fetched data.


    // Opcode table.
public:
    explicit MOS6502(RAM& ram_pointer);
    ~MOS6502();

    // Sets a single flag (sets it to 1).
    void set_flag(flag_t flag);

    // Clears a single flag (sets it to 0).
    void clear_flag(flag_t flag);

    // Flips a single flag.
    void flip_flag(flag_t flag);

    // Returns the value of a single flag.
    byte_t get_flag(flag_t flag) const;

    // Get all the flags in a byte.
    byte_t get_status() const;

    // Print human-readable processor status.
    void whoami() const;
};

struct instruction_t { // In the 6502, each instruction has an ADDRESSING MODE, an OPERATION, a BYTES value, and a CYCLES value.
    std::string name; // Human-readable name of the instruction (mnemonic).
    byte_t (MOS6502::*operate)(void) = nullptr; // Operation function for this instruction.
    byte_t (MOS6502::*addrmode)(void) = nullptr; // Addressing mode for this instruction.
    byte_t cycles = 0; // Number of clock cycles this instruction should 'fake'.
};

#endif //NESEMULATOR_MOS6502_HPP
