//
// Created by Joseph Azrak on 05/02/2021.
//

#ifndef NESEMULATOR_MOS6502_HPP
#define NESEMULATOR_MOS6502_HPP

#include "ram.hpp"
#include <vector>
#include <string>

struct flag_t
{
    uint8_t bitmask;
    uint8_t bitshift;
};

struct instruction_t; // forward-declaration

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
public: // TODO: FOR DEBUG REASONS, THIS IS INITIALLY PUBLIC. SET TO PRIVATE AFTER DEBUG
    // CPU status human-readable string
    std::string status = "stopped";

    // Pointer to the RAM object. RAM also contains ROM. Ha!
    RAM* NES_Ram;

    // CPU registers.
    uint8_t ACC = 0; // Accumulator.
    uint8_t X   = 0; // X-register.
    uint8_t Y   = 0; // Y-register.
    uint8_t FLG = 0; // Status flags, don't r/w to this directly, use set_flag/clear_flag/flip_flag
    uint8_t PC  = 0; // Program counter.
    uint8_t SP  = 0; // Stack pointer.

    // Other intermediate data.
    uint8_t fetch_address = 0x0000;   // Where to fetch data (set by addressing mode function).

    uint8_t branch_relative = 0x0000; // A helper variable used for branching. The REL addressing mode
                                      // writes to this variable; it is then used in branch instruction.

    uint8_t last_read_opcode = 0x00;

    uint8_t fetched = 0b0000000; // Fetched data.

    uint8_t clock_cycles_remaining = 0; // How many clock cycles are left to fake.

    // Fetch function. Populates `fetched' using `fetch_address'.
    void fetch_data();

    // Opcode table.
    std::vector<instruction_t> lookup_table;

    // Opcode definitions. Thank you javidx9! These should return
    // 0 in most cases and return 1 when an additional clock cycle
    // is needed.

    uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
    uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
    uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
    uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
    uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
    uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
    uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
    uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
    uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
    uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
    uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
    uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
    uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
    uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

    uint8_t XXX(); // Functionally a nop

    // Addressing modes. Thank you javidx9! These functions return the
    // adjustment needed in the clock cycles (additional clock cycles
    // that may be needed).

    uint8_t IMP(); uint8_t IMM(); uint8_t ZP0(); uint8_t ZPX();
    uint8_t ZPY(); uint8_t REL(); uint8_t ABS(); uint8_t ABX();
    uint8_t ABY(); uint8_t IND(); uint8_t IZX(); uint8_t IZY();

    uint8_t decode_signed_byte(uint8_t byte);
public:
    explicit MOS6502(RAM* ram_ref);
    ~MOS6502();

    // Sets a single flag (sets it to 1).
    void set_flag(flag_t flag);

    // Clears a single flag (sets it to 0).
    void clear_flag(flag_t flag);

    // Flips a single flag.
    void flip_flag(flag_t flag);

    // Returns the value of a single flag.
    uint8_t get_flag(flag_t flag) const;

    // Get all the flags.
    uint8_t get_status() const;

    // Print human-readable processor status.
    void whoami() const;

    // Reset the processor.
    void reset();

    // Run a single clock cycle.
    void clock();
};

/**
 * In the 6502, each instruction has an ADDRESSING MODE, an OPERATION, a BYTES value, and a CYCLES value.
 * In the INSTRUCTION LOOKUP TABLE, the relative position of each instruction_t struct is the actual opcode.
 * As such, there should be 256 values in the INSTRUCTION LOOKUP TABLE.
 *
 * The INSTRUCTION LOOKUP TABLE is populated when the CPU object is instantiated.
 *
 * Thank you javidx9 (OneLoneCoder) for the instruction lookup table (https://github.com/OneLoneCoder).
 */
struct instruction_t {
    std::string name;                               // Human-readable name of the instruction (mnemonic).
    uint8_t (MOS6502::*operate)(void) = nullptr;    // Operation function for this instruction.
    uint8_t (MOS6502::*addrmode)(void) = nullptr;   // Addressing mode function for this instruction.
    uint8_t cycles = 0;                             // Number of clock cycles this instruction should 'fake'.
};

#endif //NESEMULATOR_MOS6502_HPP
