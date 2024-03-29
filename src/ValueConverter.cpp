#include "ValueConverter.hpp"

#define DOUBLE_SIZE 8

bool ValueConverter::isEnoughToRead(packet_iterator_t &it, packet_iterator_t end, size_t size)
{
    //std::cout << "start " << (uint64_t)it.base() << " size " << size << " end " << (uint64_t)end.base() << "\n";
    return (it + size <= end);
}

uint64_t ValueConverter::to_uint_value_from_vector(
    packet_iterator_t &it, 
    packet_iterator_t end, 
    size_t amount)
{
    uint64_t val = 0;
    if(ValueConverter::isEnoughToRead(it, end, amount))
    {
        val = *(uint64_t*)&(*it);
        it += amount;
    }
    return val;
}

CMD_CODE ValueConverter::to_cmd(packet_iterator_t &it, packet_iterator_t end)
{
    return (CMD_CODE)to_uint16(it, end);
}

uint8_t ValueConverter::to_uint8(packet_iterator_t &it, packet_iterator_t end)
{
    return to_uint_value_from_vector(it, end, sizeof(uint8_t));
}

uint16_t ValueConverter::to_uint16(packet_iterator_t &it, packet_iterator_t end)
{
    return to_uint_value_from_vector(it, end, sizeof(uint16_t));
}

uint32_t ValueConverter::to_uint32(packet_iterator_t &it, packet_iterator_t end)
{
    return to_uint_value_from_vector(it, end, sizeof(uint32_t));
}

uint64_t ValueConverter::to_uint64(packet_iterator_t &it, packet_iterator_t end)
{
    return to_uint_value_from_vector(it, end, sizeof(uint64_t));
}

std::vector<uint8_t> ValueConverter::from_uint_to_vector(uint64_t data, size_t byte_width)
{
    std::vector<uint8_t> temp;
    for(int i=0; i<byte_width; i++)
    {
        uint64_t bitShiftValue = i * 8;
        temp.push_back((data & ((uint64_t)0xFF << bitShiftValue )) >> bitShiftValue);
    }
    return temp;
}

std::vector<uint8_t> ValueConverter::from_uint8(uint8_t data)
{
    return ValueConverter::from_uint_to_vector(data, sizeof(uint8_t));
}

std::vector<uint8_t> ValueConverter::from_uint16(uint16_t data)
{
    return ValueConverter::from_uint_to_vector(data, sizeof(uint16_t));
}

std::vector<uint8_t> ValueConverter::from_uint32(uint32_t data)
{
    return ValueConverter::from_uint_to_vector(data, sizeof(uint32_t));
}

std::vector<uint8_t> ValueConverter::from_uint64(uint64_t data)
{
    return ValueConverter::from_uint_to_vector(data, sizeof(uint64_t));
}

std::vector<uint8_t> ValueConverter::from_bool(bool data)
{
    return ValueConverter::from_uint8(data);
}

std::vector<uint8_t> ValueConverter::from_double(double data)
{
    std::vector<uint8_t> arr;
    for(uint64_t i = 0; i < DOUBLE_SIZE; i++)
    {
        arr.push_back(0x00);
    }
    return arr;
}
