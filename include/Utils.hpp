#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <vector>

class Utils
{
public:
    static std::string toHexBuffer(std::vector<uint8_t> &buf);
    static void printHexBuffer(std::vector<uint8_t> &buf);
    static std::string valueToHex(uint64_t val, size_t width);
    static void printHex(uint64_t val, size_t width);
    static void printHexValue(uint8_t val);
    static void printHexValue(uint16_t val);
    static void printHexValue(uint32_t val);
    static void printHexValue(uint64_t val);
    static void concatArrays(std::vector<uint8_t> &dst, std::vector<uint8_t> &src);
};

#endif // UTILS_H