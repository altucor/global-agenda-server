#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include "DataEntry.hpp"

class Packet 
{
public:
    Packet();
    Packet(std::vector<uint8_t> &input);
    Packet(std::vector<uint8_t>::iterator &start, std::vector<uint8_t>::iterator end);
    ~Packet();
    bool setPacket(std::vector<uint8_t> &input);
    std::vector<uint8_t> build();
    bool appendEntry(DataEntry &entry);
    DataEntry getNextEntry();
    DataEntry getEntryByCmd(CMD_CODE cmd);
private:
    void m_parse(std::vector<uint8_t>::iterator &start, std::vector<uint8_t>::iterator end);
    void m_build();
private:
    uint16_t m_size;
    std::vector<DataEntry> m_entries;
    uint16_t m_entriesCount;
};

#endif // PACKET_H