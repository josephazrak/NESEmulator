//
// Created by Joseph Azrak on 05/02/2021.
//

#include "ram.hpp"
#include <cmath>
#include <iostream>
#include <cstring>
#include <iomanip>

RAM::RAM()
{
    // Zero-fill the RAM bank
    memset(ram_data, 0, sizeof(ram_data));
}
RAM::~RAM() = default;

void RAM::write_byte(address_t addr, byte_t value)
{
    // Write the data ``value'' to the memory address ``addr''.
    // Deny the write if it is out-of-bounds.
    if (addr > constants::NES_RAM_SIZE || addr < 0x0000)
        return; // Cannot write OOB.

    ram_data[addr] = value; // I don't see anything wrong with just... doing this...
}

byte_t RAM::read_byte(address_t addr)
{
    // Read the data at the memory address ``addr'' and return it.
    return ram_data[addr];
}

void RAM::write_byte_range(address_t addr_start, address_t addr_stop, byte_t value) {
    // Write a single value into a byte range.
    for (address_t addr = addr_start; addr < addr_stop; addr++)
    {
        write_byte(addr, value);
    }
}

void RAM::hexdump_bytes(address_t addr_start, unsigned int bytes_to_read, unsigned int row_width) {
    // Determine the number of rows we will need to print
    unsigned int number_of_rows = ceil(bytes_to_read / row_width);

    std::cout << std::internal << std::setfill('0');

    for (unsigned int row_id = 0; row_id < number_of_rows; row_id++)
    {
        address_t row_base_address = addr_start + (row_width * row_id);

        // Print the base address
        std::cout << std::endl << "0x" << std::hex << std::setw(4) << row_base_address << std::dec << " | ";

        // Start printing n=row_width bytes
        for (unsigned int byte_id = 0; byte_id < row_width; byte_id++)
        {
            std::cout << std::hex << std::setw(2) << int(read_byte(row_base_address + byte_id)) << std::dec << " ";
        }
    }

    std::cout << std::endl;
}

void RAM::hexdump_range(address_t addr_start, address_t addr_end, unsigned int row_width) {
    hexdump_bytes(addr_start, (addr_end - addr_start), row_width);
}
