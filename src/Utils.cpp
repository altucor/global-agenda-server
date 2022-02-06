#include "Utils.hpp"

#include <iostream>
#include <sstream>
#include <utility>
#include <iomanip>

void Utils::printHexBuffer(std::vector<uint8_t> &buf)
{
    std::stringstream ss;
    for(std::size_t i=0; i<buf.size(); i++)
    {
        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << ((int)buf[i]) << " " << std::dec;
    }
    std::cout << ss.str();
    std::cout << "\n";
}

void Utils::printHex(uint64_t val, size_t width)
{
    std::cout <<
        std::hex << 
        std::uppercase << 
        std::setfill('0') << 
        std::setw(width) << 
        ((int64_t)val) << 
        std::dec;
}

void Utils::printHexValue(uint8_t val)
{
    Utils::printHex(val, sizeof(val));
}

void Utils::printHexValue(uint16_t val)
{
    Utils::printHex(val, sizeof(val));
}

void Utils::printHexValue(uint32_t val)
{
    Utils::printHex(val, sizeof(val));
}

void Utils::printHexValue(uint64_t val)
{
    Utils::printHex(val, sizeof(val));
}
