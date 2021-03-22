#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <vector>

class Utils
{
public:
    static void printHexBuffer(std::vector<uint8_t> &buf);
    static void printHex(uint64_t val, size_t width);
    static void printHexValue(uint8_t val);
    static void printHexValue(uint16_t val);
    static void printHexValue(uint32_t val);
    static void printHexValue(uint64_t val);
};

#endif // UTILS_H