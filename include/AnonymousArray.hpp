#ifndef ANONYMOUS_ARRAY_HPP
#define ANONYMOUS_ARRAY_HPP

#include "DataEntry.hpp"
#include "ValueConverter.hpp"
#include <vector>

class AnonymousArray
{
public:
    AnonymousArray();
    AnonymousArray(DataEntry d1, DataEntry d2);
    AnonymousArray(packet_iterator_t &start, packet_iterator_t end);
    ~AnonymousArray();
    uint64_t entriesCount();
    bool appendEntry(DataEntry &entry);
    std::vector<uint8_t> build();
    void dbg_print();
private:
    std::vector<DataEntry> m_entries;
};




#endif // ANONYMOUS_ARRAY_HPP