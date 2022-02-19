#include "EntryHeader.hpp"

#include "Utils.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>

EntryHeader::EntryHeader(/* args */)
{
}

EntryHeader::~EntryHeader()
{
}

EntryHeader::EntryHeader(packet_iterator_t &it, packet_iterator_t end)
{
    m_cmd = ValueConverter::to_cmd(it, end);
    m_sizeWidth = 0;
    switch(g_cmd_code_types_flags[m_cmd].type)
    {
    case TYPE_TCP_WCHAR_STR:
        m_contentSize = ValueConverter::to_uint16(it, end);
        m_sizeWidth = sizeof(uint16_t);
        break;
    case TYPE_TCP_FLOAT:
    case TYPE_TCP_DOUBLE_OR_INT_SIGNED:
    case TYPE_TCP_UINT32:
    case TYPE_TCP_UINT16:
    case TYPE_TCP_UINT8:
    case TYPE_TCP_UINT64:
    case TYPE_TCP_DATETIME:
    case TYPE_TCP_UUID_16_BYTES:
        m_contentSize = g_type_sizes[g_cmd_code_types_flags[m_cmd].type];
        break;
    case TYPE_TCP_DYNAMIC_UINT32_SIZE:
        m_contentSize = ValueConverter::to_uint32(it, end);
        m_sizeWidth = sizeof(uint16_t);
        break;
    case TYPE_TCP_DATA_SET:
        m_contentSize = ValueConverter::to_uint16(it, end);
        m_sizeWidth = sizeof(uint16_t);
        break;
    default:
        throw std::exception("Got unknown enum type for parsing entry");
        break;
    }
}

EntryHeader::EntryHeader(const CMD_CODE cmd)
{

}

EntryHeader::EntryHeader(const CMD_CODE cmd, const uint64_t size)
{
    m_cmd = cmd;
    m_contentSize = size;
    m_sizeWidth = 0;
    switch(g_cmd_code_types_flags[m_cmd].type)
    {
    case TYPE_TCP_WCHAR_STR:
        {
            m_sizeWidth = sizeof(uint16_t);
            break;
        }
    case TYPE_TCP_FLOAT:
    case TYPE_TCP_DOUBLE_OR_INT_SIGNED:
    case TYPE_TCP_UINT32:
    case TYPE_TCP_UINT16:
    case TYPE_TCP_UINT8:
    case TYPE_TCP_DATETIME:
    case TYPE_TCP_UINT64:
    case TYPE_TCP_UUID_16_BYTES:
        m_sizeWidth = 0;
        break;
    case TYPE_TCP_DATA_SET:
        m_sizeWidth = sizeof(uint16_t);
        break;
    case TYPE_TCP_DYNAMIC_UINT32_SIZE:
        {
            m_sizeWidth = sizeof(uint32_t);
            break;
        }
    default:
        throw std::exception("Got unknown enum type for parsing entry");
        break;
    }
}

uint64_t EntryHeader::getSize()
{
    return m_contentSize;
}

void EntryHeader::setSize(const uint64_t size)
{
    m_contentSize = size;
}

CMD_CODE EntryHeader::getCmd()
{
    return m_cmd;
}

ENUM_TYPE EntryHeader::getType()
{
    return (ENUM_TYPE)g_cmd_code_types_flags[m_cmd].type;
}

packet_t EntryHeader::build()
{
    packet_t payload;
    Utils::concatArrays(payload, ValueConverter::from_uint16(m_cmd));
    switch (m_sizeWidth)
    {
    case sizeof(uint16_t):
        Utils::concatArrays(payload, ValueConverter::from_uint16(m_contentSize));
        break;
    case sizeof(uint32_t):
        Utils::concatArrays(payload, ValueConverter::from_uint32(m_contentSize));
        break;
    default:
        break;
    }
    return payload;
}

void EntryHeader::dbg_print()
{
    BOOST_LOG_TRIVIAL(info) << "Cmd: 0x" << Utils:: Utils::valueToHex(m_cmd, 4) << " - " << g_cmd_code_text[m_cmd];
}
