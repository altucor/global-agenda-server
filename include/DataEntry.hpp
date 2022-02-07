#ifndef DATA_ENTRY_H
#define DATA_ENTRY_H

#include "DataTypesHolder.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>

#include "ValueConverter.hpp"

class DataEntry
{
public:
    DataEntry();
    DataEntry(packet_iterator_t &it, packet_iterator_t end);
    DataEntry(std::vector<uint8_t> &data);
    DataEntry(CMD_CODE cmd, std::vector<uint8_t> &data);
    DataEntry(CMD_CODE cmd, uint16_t value);
    DataEntry(CMD_CODE cmd, bool value);
    DataEntry(CMD_CODE cmd, double value);
    DataEntry(CMD_CODE cmd, const std::string &value);
    ~DataEntry();
    CMD_CODE getCmd();
    std::vector<uint8_t> build();
    uint8_t get_uint8();
    uint16_t get_uint16();
    uint32_t get_uint32();
    uint64_t get_uint64();
    bool valid();
    void dbg_print();
private:
    void m_build(std::vector<uint8_t> &data);
    std::vector<uint8_t> m_parse(
        CMD_CODE cmd, 
        packet_iterator_t &it, 
        packet_iterator_t end);
private:
    CMD_CODE m_cmd;
    std::vector<uint8_t> m_data;
};

#endif // DATA_ENTRY_H
