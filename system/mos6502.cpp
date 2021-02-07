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

MOS6502::MOS6502(RAM& ram) {
    NES_Ram = ram;
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

byte_t MOS6502::get_flag(flag_t flag) const
{
    return (FLG & flag.bitmask) >> flag.bitshift;
}

byte_t MOS6502::get_status() const
{
    return FLG;
}

void MOS6502::whoami() const
{
    std::cout << "===================================" << std::endl;
    std::cout << "MOS6502 EMULATED INSTANCE (status=" << status << ")" << std::endl;
    std::cout << "---REGISTERS---" << std::endl;
    std::cout << "ACC=0d" << std::setw(4) << ACC << ";      RAX=0d" << std::setw(4) << X << "; RAY=0d" << std::setw(4) << Y << std::endl;
    std::cout << "FLG=0b" << std::bitset<8> (FLG) << "; PC=0x" << std::hex << std::setw(5) << PC << "; SP=0x" << std::setw(5) << SP << std::dec << std::endl;
    std::cout << "---MISC---" << std::endl;
    std::cout << "FETCH_DATA: " << fetched << std::endl;
    std::cout << "---FLAGS--- " << std::endl;
    std::cout << std::bitset<8> (FLG) << std::endl;
    std::cout << "NVssDIZC" << std::endl;
    std::cout << "===================================" << std::endl;
}
