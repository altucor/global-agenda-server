#include "DataSet.hpp"

#include "Utils.hpp"

DataSet::DataSet(/* args */)
{
}

DataSet::~DataSet()
{
}

void DataSet::m_parse(packet_iterator_t &it, packet_iterator_t end)
{
    for(uint64_t i=0; i<m_entryHeader.getSize(); i++)
    {
        m_anonArrays.push_back(AnonymousArray(it, end));
    }
}

DataSet::DataSet(packet_iterator_t &it, packet_iterator_t end)
{
    m_entryHeader = EntryHeader(it, end);
    m_parse(it, end);
}

DataSet::DataSet(EntryHeader entryHeader, packet_iterator_t &it, packet_iterator_t end)
{
    m_entryHeader = entryHeader;
    m_parse(it, end);
}

DataSet::DataSet(const CMD_CODE cmd, std::vector<AnonymousArray> anonArrays)
{
    m_entryHeader = EntryHeader(cmd, 0);
    m_anonArrays = anonArrays;
}

packet_t DataSet::build()
{
    packet_t payload;
    m_entryHeader.sanitizePayloadSizeByType(m_anonArrays);
    Utils::concatArrays(payload, m_entryHeader.build());
    for(auto &arr : m_anonArrays)
    {
        Utils::concatArrays(payload, arr.build());
    }
    return payload;
}

void DataSet::dbg_print()
{
    for(auto &arr : m_anonArrays)
    {
        arr.dbg_print();
    }
}
