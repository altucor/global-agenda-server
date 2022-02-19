#ifndef DATA_SET_HPP
#define DATA_SET_HPP

#include "EntryHeader.hpp"
#include "AnonymousArray.hpp"

class DataSet
{
public:
    DataSet();
    ~DataSet();
    DataSet(packet_iterator_t &it, packet_iterator_t end);
    DataSet(EntryHeader entryHeader, packet_iterator_t &it, packet_iterator_t end);
    DataSet(const CMD_CODE cmd, std::vector<AnonymousArray> anonArrays);
    packet_t build();
    void dbg_print();
private:
    void m_parse(packet_iterator_t &it, packet_iterator_t end);
private:
    EntryHeader m_entryHeader;
    std::vector<AnonymousArray> m_anonArrays;
};




#endif // DATA_SET_HPP
