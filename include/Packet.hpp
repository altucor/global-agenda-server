#ifndef PACKET_HPP
#define PACKET_HPP

#include <iostream>
#include "EntryHeader.hpp"
#include "DataEntry.hpp"
#include "DataSet.hpp"

class Packet 
{
public:
    Packet();
    Packet(const CMD_CODE cmd);
    Packet(std::vector<uint8_t> &input);
    Packet(std::vector<uint8_t>::iterator &start, std::vector<uint8_t>::iterator end);
    ~Packet();
    std::vector<uint8_t> build();
    bool appendEntry(DataEntry &entry);
    bool appendDataSet(DataSet &dataSet);
    DataEntry getNextEntry();
    DataEntry getEntryByCmd(CMD_CODE cmd);
    CMD_CODE getCmd();
    uint64_t entriesCount();
    bool isEmpty();
private:
    void m_parse(std::vector<uint8_t>::iterator &start, std::vector<uint8_t>::iterator end);
    void m_build();
private:
    EntryHeader m_packetHeader;
    std::vector<DataEntry> m_entries;
    std::vector<DataEntry>::iterator m_entryIter;
    std::vector<DataSet> m_dataSets;
};

#endif // PACKET_HPP