#include "Packet.hpp"

Packet::Packet()
{

}

Packet::Packet(std::vector<uint8_t> &input)
{
    std::vector<uint8_t>::iterator start = input.begin();
    m_parse(start, input.end());
}

Packet::Packet(std::vector<uint8_t>::iterator &start, std::vector<uint8_t>::iterator end)
{
    m_parse(start, end);
}

Packet::~Packet()
{

}

bool Packet::setPacket(std::vector<uint8_t> &input)
{
    return false;
}

std::vector<uint8_t> Packet::build()
{
    std::vector<uint8_t> payload;
    for(auto &entry : m_entries)
    {
        std::vector<uint8_t> entryData = entry.build();
        entry.dbg_print();
        payload.insert(std::end(payload), std::begin(entryData), std::end(entryData));
    }

    std::vector<uint8_t> outData;
    DataEntry entriesCountObject = DataEntry(CMD_CODE::APPLICATION_VALUE, m_entries.size());
    std::vector<uint8_t> entriesCountPayload = entriesCountObject.build();
    payload.insert(payload.begin(), entriesCountPayload.begin(), entriesCountPayload.end()); // append count of entries
    uint16_t payloadSize = payload.size();
    //payload.insert(payload.begin(), payloadSize & 0xFF00); // append size of whole packet
    //payload.insert(payload.begin(), payloadSize & 0x00FF); // append size of whole packet
    std::vector<uint8_t> payloadSizeBuilded = ValueConverter::from_uint16(payloadSize);
    payload.insert(payload.begin(), payloadSizeBuilded.begin(), payloadSizeBuilded.end());
    return payload;
}

bool Packet::appendEntry(DataEntry &entry)
{
    m_entries.push_back(entry);
    return true;
}

DataEntry Packet::getNextEntry()
{
    return DataEntry();
}

DataEntry Packet::getEntryByCmd(CMD_CODE cmd)
{
    for(auto &obj : m_entries)
    {
        if(obj.getCmd() == cmd)
            return obj;
    }
    return DataEntry();
}

void Packet::m_parse(std::vector<uint8_t>::iterator &start, std::vector<uint8_t>::iterator end)
{
    m_size = ValueConverter::to_uint16(start, end);
    if(m_size == 0)
        return;

    if(m_size > end - start)
        return;

    std::vector<uint8_t>::iterator packetEnd = start + m_size;

    while(start != packetEnd)
    {
        DataEntry dt_temp(start, packetEnd);
        if(!dt_temp.valid())
        {
            std::cout << "got invalid entry\n";
            return;
        }
        dt_temp.dbg_print();
        //std::cout << "iterator pos after entry: " << (uint64_t)it.base() << "\n";
        //if(dt_temp.getCmd() == CMD_CODE::APPLICATION_VALUE)
        //    std::cout << "APPLICATION_VALUE: " << dt_temp.get_uint16() << "\n";
        //else if(dt_temp.getCmd() == CMD_CODE::VERSION)
        //    std::cout << "VERSION: " << dt_temp.get_uint32() << "\n";
        m_entries.push_back(dt_temp);
    }

    DataEntry entriesCount = getEntryByCmd(CMD_CODE::APPLICATION_VALUE);
    if(entriesCount.valid())
    {
        m_entriesCount = entriesCount.get_uint16();
    }
    else
    {
        m_entriesCount = 1;
    }

    //std::cout << "packet size " << m_size << "\n";
    //std::cout << "entries count " << m_entriesCount << "\n";
    //std::cout << "arr entries size " << m_entries.size() << "\n";
}

void Packet::m_build()
{

}
