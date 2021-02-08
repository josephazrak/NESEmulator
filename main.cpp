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
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include <bitset>
#include <cassert>
#include <thread>
#include <chrono>

const std::string VERSION = "0.1 alpha";
const double CPU_CLOCK = 1.789773 * 10e6; // 1.789 MHz is the clock speed of the 6502 in the
                                          // NES.

int main() {
    std::cout << std::endl << "NES Emulator version " << VERSION << std::endl;
    std::cout << "https://github.com/josephazrak" << std::endl;
    std::cout << "https://josephazrak.codes" << std::endl;
    std::cout << "========================" << std::endl << std::endl;

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
     *        * get CYCLES
     *        * get OPERAND SIZE
     *    + READ OPERANDS
     *    + RUN the function assoc. with the ADDRESSING MODE
     *        * the FUNCTION should retrieve the data and put it into the pseudo-register "fetched"
     *    + RUN the function assoc. with the OPCODE
     *        * works on "fetched" data
     *    + WAIT for the extra number of cycles needed
     *
     */
    std::cout << "Running test suite." << std::endl;

    RAM test_ram;
    MOS6502 test_cpu(test_ram);

    // ============================
    //  RAM TESTS
    // ============================

    uint8_t byteArray[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0xff, 0xfa, 0xfd, 0x0a, 0xde, 0xad, 0xbe, 0xef};

    std::cout << "Writing " << sizeof(byteArray) << " byte(s) to RAM [@ $0000]" << std::endl;

    for (address_t addr = 0; (addr <= constants::NES_RAM_SIZE) && (addr - 0 < (sizeof(byteArray) / sizeof(byteArray[0])) ); addr++)
        test_ram.write_byte(addr, byteArray[addr]);

    std::cout << "Dumping 300 bytes from [@ $0000]..." << std::endl;

    test_ram.write_byte_range(0x000a, 0x0014, 0xff);
    test_ram.hexdump_bytes(0x0000, 300, 20);

    // ============================
    //  CPU FLAG TESTS
    // ============================

    std::cout << std::endl << "Doing CPU flag sanity check..." << std::endl;

    test_cpu.set_flag(FLAG_D_DECI);
    test_cpu.set_flag(FLAG_N_NEGTV);
    test_cpu.set_flag(FLAG_C_CARRY);
    assert(test_cpu.get_status() == 0b10001001);

    test_cpu.flip_flag(FLAG_N_NEGTV);
    test_cpu.flip_flag(FLAG_Z_ZERO);
    assert(test_cpu.get_status() == 0b00001011);

    test_cpu.clear_flag(FLAG_D_DECI);
    test_cpu.clear_flag(FLAG_Z_ZERO);
    test_cpu.clear_flag(FLAG_C_CARRY);
    assert(test_cpu.get_status() == 0b00000000);

    std::cout << "CPU flag sanity check succeeded." << std::endl;

    return 0;
}
