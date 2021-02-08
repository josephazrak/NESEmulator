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
    ram_data.clear();

    for (unsigned int i = 0; i < constants::MAX_ADDRESS_SIZE; i++)
    {
        ram_data.push_back(0x00);
    }
}
RAM::~RAM() = default;

void RAM::write_byte(address_t addr, uint8_t value)
{
    // Write the data ``value'' to the memory address ``addr''.
    // Deny the write if it is out-of-bounds.
    if (addr > constants::MAX_ADDRESS_SIZE || addr < 0x0000)
    {
        std::cout << "ERROR! Illegal write at $" << std::hex << addr << "attempted! (write " << value << ")" << std::endl;
        return;
    }

    ram_data[addr] = value; // I don't see anything wrong with just... doing this...
}

uint8_t RAM::read_byte(address_t addr)
{
    // Read the data at the memory address ``addr'' and return it.
    if (addr > constants::MAX_ADDRESS_SIZE || addr < 0x0000)
    {
        std::cout << "ERROR! Illegal read at $" << std::hex << addr << " attempted! " << std::endl;
        return 0xAA;
    }
    return ram_data[addr];
}

void RAM::write_byte_range(address_t addr_start, address_t addr_stop, uint8_t value) {
    // Write a single value into a byte range.
    for (address_t addr = addr_start; addr < addr_stop; addr++)
    {
        write_byte(addr, value);
    }
}

void RAM::write_byte_vector(address_t addr_start, std::vector<uint8_t>& byte_vector)
{
    address_t current_addr = addr_start;
    for (auto it = std::begin(byte_vector); it != std::end(byte_vector); ++it)
    {
        write_byte(current_addr, *it);
        current_addr++;
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
        std::cout << std::endl << "$" << std::hex << std::setw(4) << row_base_address << std::dec << " | ";

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

void RAM::clear_address_space()
{
    // Zero-fill the RAM bank
    ram_data.clear();

    for (unsigned int i = 0; i < constants::MAX_ADDRESS_SIZE; i++)
    {
        ram_data.push_back(0x00);
    }
}


