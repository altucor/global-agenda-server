#include "DataSet.hpp"

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
    m_entryHeader = EntryHeader(cmd);
    m_anonArrays = anonArrays;
}

void DataSet::dbg_print()
{
    for(auto &arr : m_anonArrays)
    {
        arr.dbg_print();
    }
}
