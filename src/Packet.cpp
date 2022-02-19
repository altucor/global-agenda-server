#include "Packet.hpp"

#include "Utils.hpp"

#include <boost/log/trivial.hpp>

Packet::Packet()
{

}

Packet::Packet(const CMD_CODE cmd)
    : m_packetHeader(cmd)
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

std::vector<uint8_t> Packet::build()
{
    std::vector<uint8_t> payload;
    Utils::concatArrays(payload, m_packetHeader.build()); // append count of entries
    for(auto &entry : m_entries)
    {
        std::vector<uint8_t> entryData = entry.build();
        //entry.dbg_print();
        Utils::concatArrays(payload, entryData);
    }

    return payload;
}

bool Packet::appendEntry(DataEntry &entry)
{
    m_entries.push_back(entry);
    return true;
}

bool Packet::appendDataSet(DataSet &dataSet)
{
    m_dataSets.push_back(dataSet);
    return true;
}

DataEntry Packet::getNextEntry()
{
    m_entryIter = std::next(m_entryIter, 1);
    if(m_entryIter != m_entries.end())
        return *m_entryIter;
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
    m_packetHeader = EntryHeader(start, end);

    for(uint64_t i=0; i<m_packetHeader.getSize(); i++)
    {
        if(start == end)
            break;
        EntryHeader entryHeader(start, end);
        switch (entryHeader.getType())
        {
        case ENUM_TYPE::TYPE_TCP_DATA_SET:
            {
                DataSet ds_temp(entryHeader, start, end);
                ds_temp.dbg_print();
                m_dataSets.push_back(ds_temp);
                break;
            }
        default:
            {
                DataEntry dt_temp(entryHeader, start, end);
                dt_temp.dbg_print();
                if(!dt_temp.valid())
                {
                    BOOST_LOG_TRIVIAL(info) << "Error got invalid entry";
                    return;
                }
                m_entries.push_back(dt_temp);
                break;
            }
        }
    }

    if(end != start)
    {
        BOOST_LOG_TRIVIAL(info) << "Error packet parse end != start";
    }

    m_entryIter = m_entries.begin();
}

CMD_CODE Packet::getCmd()
{
    return m_packetHeader.getCmd();
}

uint64_t Packet::entriesCount()
{
    return m_entries.size();
}

bool Packet::isEmpty()
{
    return m_entries.size() == 0;
}

void Packet::m_build()
{

}
