//
// Created by Joseph Azrak on 05/02/2021.
//

#ifndef NESEMULATOR_RAM_HPP
#define NESEMULATOR_RAM_HPP

#include <cstdint>
#include <vector>

typedef uint8_t uint8_t;
typedef unsigned int address_t;

namespace constants
{
    const int NES_RAM_SIZE = 0x0800; // Historically, the NES has had 2KiB of RAM.
    const int MAX_ADDRESS_SIZE = 0xffff; // $FFFF is the maximum addressable memory.
} // namespace constants

class RAM
{
private:
    std::vector<uint8_t> ram_data;
public:
    RAM();
    ~RAM();

    // Raw bit-wise IO functions
    void write_byte(address_t addr, uint8_t value);
    uint8_t read_byte(address_t addr);

    // Miscellaneous IO functions
    void write_byte_range(address_t addr_start, address_t addr_stop, uint8_t value);
    void write_byte_vector(address_t addr_start, std::vector<uint8_t>& byte_vector);

    // RAM clear
    void clear_address_space();

    // HID IO functions
    void hexdump_bytes(address_t addr_start, unsigned int bytes_to_read, unsigned int row_width);
    void hexdump_range(address_t addr_start, address_t addr_end, unsigned int row_width);
};

#endif //NESEMULATOR_RAM_HPP
