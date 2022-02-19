#ifndef DATA_ENTRY_HPP
#define DATA_ENTRY_HPP

#include "DataTypesHolder.hpp"
#include "ValueConverter.hpp"
#include "EntryHeader.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>

class DataEntry
{
public:
    DataEntry();
    DataEntry(packet_iterator_t &it, packet_iterator_t end);
    DataEntry(EntryHeader entryHeader, packet_iterator_t &it, packet_iterator_t end);
    DataEntry(std::vector<uint8_t> &data);
    DataEntry(const CMD_CODE cmd, std::vector<uint8_t> &data);
    DataEntry(const CMD_CODE cmd, const uint16_t value);
    DataEntry(const CMD_CODE cmd, const uint32_t value);
    DataEntry(const CMD_CODE cmd, const uint64_t value);
    DataEntry(const CMD_CODE cmd, const bool value);
    DataEntry(const CMD_CODE cmd, const double value);
    DataEntry(const CMD_CODE cmd, const std::string &value);
    DataEntry(const CMD_CODE cmd, const uint16_t port, const uint32_t ip);
    ~DataEntry();
    CMD_CODE getCmd();
    std::vector<uint8_t> build();
    uint8_t get_uint8();
    uint16_t get_uint16();
    uint32_t get_uint32();
    uint64_t get_uint64();
    std::vector<uint8_t> get_raw_data();
    std::string get_string();
    bool valid();
    void dbg_print();
    void printField();
private:
    void m_build(std::vector<uint8_t> &data);
    std::vector<uint8_t> determineDataSetByteSize(
        packet_iterator_t &start, 
        packet_iterator_t end,
        const uint64_t entriesCount
    );
    std::vector<uint8_t> m_parse(
        packet_iterator_t &it, 
        packet_iterator_t end);
private:
    EntryHeader m_entryHeader;
    std::vector<uint8_t> m_data;
};

#endif // DATA_ENTRY_HPP
