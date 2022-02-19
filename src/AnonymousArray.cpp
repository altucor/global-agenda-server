#include "AnonymousArray.hpp"

#include "Utils.hpp"

#include <boost/log/trivial.hpp>

AnonymousArray::AnonymousArray(/* args */)
{

}

AnonymousArray::AnonymousArray(DataEntry d1, DataEntry d2)
{
    m_entries.push_back(d1);
    m_entries.push_back(d2);
}

AnonymousArray::AnonymousArray(packet_iterator_t &start, packet_iterator_t end)
{
    const uint16_t entriesCount = ValueConverter::to_uint16(start, end);
    if(entriesCount == 0)
        return;

    if(entriesCount > end - start)
        return;
    
    for(int i=0; i<entriesCount; i++)
    {
        m_entries.push_back(DataEntry(start, end));
    }
}

AnonymousArray::~AnonymousArray()
{

}

uint64_t AnonymousArray::entriesCount()
{
    return m_entries.size();
}

bool AnonymousArray::appendEntry(DataEntry &entry)
{
    m_entries.push_back(entry);
    return true;
}

std::vector<uint8_t> AnonymousArray::build()
{
    std::vector<uint8_t> payload;
    Utils::concatArrays(payload, ValueConverter::from_uint16(m_entries.size()));
    for(auto &entry : m_entries)
    {
        Utils::concatArrays(payload, entry.build());
    }
    return payload;
}

void AnonymousArray::dbg_print()
{
    for(auto &entry : m_entries)
    {
        entry.dbg_print();
    }
}

