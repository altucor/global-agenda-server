#ifndef ENTRY_HEADER_HPP
#define ENTRY_HEADER_HPP

#include "EnumDataTypes.hpp"
#include "ValueConverter.hpp"

class EntryHeader
{
public:
    EntryHeader();
    EntryHeader(packet_iterator_t &it, packet_iterator_t end);
    //EntryHeader(const CMD_CODE cmd);
    EntryHeader(const CMD_CODE cmd, const uint64_t size);
    ~EntryHeader();
    uint64_t getSize();
    void setSize(const uint64_t size);
    CMD_CODE getCmd();
    ENUM_TYPE getType();
    template<typename T> void sanitizePayloadSizeByType(T &payload)
    {
        if(m_sizeWidth != 0)
        {
            m_contentSize = payload.size();
        }
        else
        {
            payload.resize(m_contentSize);
        }
    }
    packet_t build();
    void dbg_print();
private:
    CMD_CODE m_cmd = CMD_CODE::UNKNOWN;
    uint64_t m_contentSize = 0;
    uint64_t m_sizeWidth = 0;
};




#endif // ENTRY_HEADER_HPP
