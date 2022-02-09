#include "Utils.hpp"

#include <iostream>
#include <sstream>
#include <utility>
#include <iomanip>

std::string Utils::toHexBuffer(std::vector<uint8_t> &buf)
{
    std::stringstream ss;
    for(std::size_t i=0; i<buf.size(); i++)
    {
        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << ((int)buf[i]) << " " << std::dec;
    }
    return ss.str();
}

void Utils::printHexBuffer(std::vector<uint8_t> &buf)
{
    std::cout << toHexBuffer(buf) << std::endl;
}

std::string Utils::valueToHex(uint64_t val, size_t width)
{
    std::stringstream ss;
    ss <<
        std::hex << 
        std::uppercase << 
        std::setfill('0') << 
        std::setw(width) << 
        ((int64_t)val) << 
        std::dec;
    return ss.str();
}

void Utils::printHex(uint64_t val, size_t width)
{
    std::cout << valueToHex(val, width);
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
