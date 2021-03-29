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
    const int MAX_ADDRESS_SIZE = 0xffff; // $FFFF is the maximum addressable memory address.
} // namespace constants

class RAM
{
private:
    std::vector<uint8_t> ram_data;
public:
    RAM();
    ~RAM();

    /**
     * Writes a single byte to a memory address.
     * @param addr
     * @param value
     */
    void write_byte(address_t addr, uint8_t value);

    /**
     * Reads a single byte from a memory address, returning it.
     * @param addr
     * @return data at address
     */
    uint8_t read_byte(address_t addr);

    /**
     * Fills the provided value over a range specified by a start and stop address.
     * @param addr_start
     * @param addr_stop
     * @param value
     */
    void write_byte_range(address_t addr_start, address_t addr_stop, uint8_t value);

    /**
     * Given a vector of bytes, writes it at a certain address in memory.
     * @param addr_start
     * @param byte_vector
     */
    void write_byte_vector(address_t addr_start, std::vector<uint8_t>& byte_vector);

    /**
     * Clears all RAM.
     */
    void clear_address_space();

    /**
     * Outputs a human-readable hexdump.
     * @param addr_start
     * @param bytes_to_read
     * @param row_width
     */
    void hexdump_bytes(address_t addr_start, unsigned int bytes_to_read, unsigned int row_width);

    /**
     * Outputs a human-readable hexdump.
     * @param addr_start
     * @param bytes_to_read
     * @param row_width
     */
    void hexdump_range(address_t addr_start, address_t addr_end, unsigned int row_width);
};

#endif //NESEMULATOR_RAM_HPP
