/**
 * NES (Famicom) emulator software. Emulates the 6502 microprocessor.
 * Can read iNES ROM images.
 *
 * (c) 2021 Joseph Azrak
 * josephazrak.codes
 * https://github.com/josephazrak
 */

#include "system/mos6502.hpp"
#include "system/ram.hpp"
#include <iostream>
#include <bitset>

const std::string VERSION = "0.1 alpha";
const double CPU_CLOCK = 1.789773 * 10e6; // 1.789 MHz is the clock speed of the 6502 in the
                                          // NES.

int main() {
    std::cout << "\tNES Emulator version " << VERSION << std::endl << std::endl;
    std::cout << "https://github.com/josephazrak" << std::endl;
    std::cout << "https://josephazrak.codes" << std::endl;
    std::cout << "========================" << std::endl;

    /**
     * Program flow
     *
     *
     * - Load ROM
     * - Read ROM HEADER
     * - Determine ENTRY POINT
     * - Start 6502 EMULATOR
     * - Set @PC to ENTRY POINT
     * - LOOP:
     *    + READ 1 byte at offset @PC
     *    + IDENTIFY instruction
     *        * get OPCODE
     *        * get ADDRESSING MODE
     *        * get OPERAND SIZE
     *    + READ OPERANDS
     *
     *
     */

    std::cout << "Running test suite." << std::endl;

    RAM test_ram;
    MOS6502 cpu(test_ram);

    // ============================
    //  RAM TESTS
    // ============================

    std::cout << "Writing some bytes to RAM..." << std::endl;

    byte_t byteArray[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0xff, 0xfa, 0xfd, 0x0a, 0xde, 0xad, 0xbe, 0xef};

    for (address_t addr = 0; (addr <= constants::NES_RAM_SIZE) && (addr - 0 < (sizeof(byteArray) / sizeof(byteArray[0])) ); addr++)
    {
        test_ram.write_byte(addr, byteArray[addr]);
    }

    std::cout << "Dumping 33 bytes from 0x0000..." << std::endl;

    test_ram.write_byte_range(0x000a, 0x0014, 0xff);
    test_ram.hexdump_bytes(0x0000, 33, 10);

    // ============================
    //  CPU FLAG TESTS
    // ============================

    std::cout << "Testing CPU flag functionality..." << std::endl;
    cpu.whoami();

    std::cout << std::endl;
    std::cout << "Setting the D, N, C flags..." << std::endl;
    cpu.set_flag(FLAG_D_DECI);
    cpu.set_flag(FLAG_N_NEGTV);
    cpu.set_flag(FLAG_C_CARRY);

    cpu.whoami();

    std::cout << std::endl;
    std::cout << "Flipping the the N and Z flags..." << std::endl;
    cpu.flip_flag(FLAG_N_NEGTV);
    cpu.flip_flag(FLAG_Z_ZERO);

    cpu.whoami();

    std::cout << std::endl;
    std::cout << "Clearing all flags..." << std::endl;
    cpu.clear_flag(FLAG_Z_ZERO);
    cpu.clear_flag(FLAG_D_DECI);
    cpu.clear_flag(FLAG_C_CARRY);

    cpu.whoami();
    return 0;
}
