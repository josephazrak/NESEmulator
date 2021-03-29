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
#include <fstream>
#include <iterator>
#include <vector>
#include <thread>
#include <chrono>

const std::string VERSION = "0.1 alpha (test)";
const double CPU_CLOCK = 1.789773 * 10e6; // 1.789 MHz is the clock speed of the 6502 in the
                                          // NES.

int main(int argc, char **argv) {
    address_t ENTRY_POINT = 0x0004; // TODO: iNES parser should set this; this should not be hardcoded.

    std::cout << std::endl << "NES Emulator version " << VERSION << std::endl;
    std::cout << "https://github.com/josephazrak" << std::endl;
    std::cout << "https://josephazrak.codes" << std::endl;
    std::cout << "========================" << std::endl << std::endl;
    std::cout << "Running test suite." << std::endl;

    RAM test_ram;
    MOS6502 test_cpu(&test_ram);

    // ============================
    // CPU FLAG TESTS
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

    // ============================
    // LOAD USER BINARY
    // ============================

    std::cout << "Clearing RAM..." << std::endl;
    test_ram.clear_address_space();

    std::cout << "Will now read the file ``rom.bin'' and load it into $0000. Execution will start at $0004. Press <enter> if that's OK or <ctrl-c> to abort. ";
    std::cin.get();

    std::ifstream test_rom("rom.bin", std::ios::binary);
    std::vector<uint8_t> test_rom_bytes ((std::istreambuf_iterator<char>(test_rom)),(std::istreambuf_iterator<char>())); // Load rom into byte array
    test_rom.close();

    std::cout << "Read " << test_rom_bytes.size() << " byte(s) from file. Writing to $0000" << std::endl;

    test_ram.write_byte_vector(0x0000, test_rom_bytes);
    test_ram.hexdump_bytes(0x0000, 100, 20);

    std::cout << std::endl << std::endl;
    std::cout << "Starting processor with PC = $0004. Ctrl-C to stop emulation." << std::endl;

    test_cpu.PC = 0x0004;
    std::chrono::milliseconds clock_period(3000); // 3 seconds per clock cycle.

    // Main loop
    while (true)
    {
        test_cpu.clock();
        std::this_thread::sleep_for(clock_period);
    }
}
