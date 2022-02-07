#ifndef VALUE_CONVERTER_H
#define VALUE_CONVERTER_H

#include <iterator>
#include <vector>
#include <cstdint>

typedef std::vector<uint8_t>::iterator packet_iterator_t;

class ValueConverter
{
public:
    static bool isEnoughToRead(packet_iterator_t &it, packet_iterator_t end, size_t size);
    static uint64_t to_uint_value_from_vector(packet_iterator_t &it, 
        packet_iterator_t end,
        size_t amount);
    static uint8_t to_uint8(packet_iterator_t &it, packet_iterator_t end);
    static uint16_t to_uint16(packet_iterator_t &it, packet_iterator_t end);
    static uint32_t to_uint32(packet_iterator_t &it, packet_iterator_t end);
    static uint64_t to_uint64(packet_iterator_t &it, packet_iterator_t end);
    static std::vector<uint8_t> from_uint_to_vector(uint64_t data, size_t byte_width);
    static std::vector<uint8_t> from_uint8(uint8_t data);
    static std::vector<uint8_t> from_uint16(uint16_t data);
    static std::vector<uint8_t> from_uint32(uint32_t data);
    static std::vector<uint8_t> from_uint64(uint64_t data);
    static std::vector<uint8_t> from_bool(bool data);
    static std::vector<uint8_t> from_double(double data);
};

#endif // VALUE_CONVERTER_H