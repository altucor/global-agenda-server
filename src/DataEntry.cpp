#include "DataEntry.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <boost/log/trivial.hpp>


std::vector<DataType> g_dataTypes();

DataEntry::DataEntry()
{
    m_entryHeader = EntryHeader();
    m_data = std::vector<uint8_t>();
}

std::vector<uint8_t> DataEntry::m_parse(packet_iterator_t &it, 
    packet_iterator_t end)
{
    if(ValueConverter::isEnoughToRead(it, end, m_entryHeader.getSize()))
    {
        packet_iterator_t start_copy = it;
        if(m_entryHeader.getType() == ENUM_TYPE::TYPE_TCP_DATA_SET)
        {
            throw std::exception("DataEntry cannot parse DATA_SET type");
        }
        it += m_entryHeader.getSize();
        return std::vector<uint8_t>(start_copy, it);
    }
    return std::vector<uint8_t>();
}

DataEntry::DataEntry(packet_iterator_t &it, 
    packet_iterator_t end)
{
    m_entryHeader = EntryHeader(it, end);
    m_data = m_parse(it, end);
}

DataEntry::DataEntry(
    EntryHeader entryHeader, 
    packet_iterator_t &it, 
    packet_iterator_t end
)
{
    m_entryHeader = entryHeader;
    m_data = m_parse(it, end);
}

DataEntry::DataEntry(std::vector<uint8_t> &data)
{

}

DataEntry::DataEntry(const CMD_CODE cmd, std::vector<uint8_t> &data)
    : m_entryHeader(cmd), m_data(data)
{
    
}


DataEntry::DataEntry(const CMD_CODE cmd, uint16_t value)
    : m_entryHeader(cmd)
{
    m_data = ValueConverter::from_uint16(value);
}

DataEntry::DataEntry(const CMD_CODE cmd, uint32_t value)
    : m_entryHeader(cmd)
{
    m_data = ValueConverter::from_uint32(value);
}

DataEntry::DataEntry(const CMD_CODE cmd, uint64_t value)
    : m_entryHeader(cmd)
{
    m_data = ValueConverter::from_uint64(value);
}

DataEntry::DataEntry(const CMD_CODE cmd, bool value)
    : m_entryHeader(cmd)
{
    m_data = ValueConverter::from_bool(value);
}

DataEntry::DataEntry(const CMD_CODE cmd, double value)
    : m_entryHeader(cmd)
{
    m_data = ValueConverter::from_double(value);
}

DataEntry::DataEntry(const CMD_CODE cmd, const std::string &value)
    : m_entryHeader(cmd)
{
    std::copy(value.begin(), value.end(), std::back_inserter(m_data));
}

DataEntry::DataEntry(const CMD_CODE cmd, const uint16_t port, const uint32_t ip)
    : m_entryHeader(cmd)
{
    m_data = ValueConverter::from_uint16(0x0002);
    m_data = ValueConverter::from_uint16(port);
    m_data = ValueConverter::from_uint32(ip);
}

DataEntry::~DataEntry()
{

}

CMD_CODE DataEntry::getCmd()
{
    return m_entryHeader.getCmd();
}



std::vector<uint8_t> DataEntry::build()
{
    std::vector<uint8_t> payload;
    Utils::concatArrays(payload, m_entryHeader.build());
    //packet_t cmdEncoded = ValueConverter::from_uint16(m_cmd);
    //payload.insert(payload.end(), cmdEncoded.begin(), cmdEncoded.end());

    uint64_t expectedDataSize = 0;
    /*
    switch(g_cmd_code_types_flags[m_cmd].type)
    {
    case TYPE_TCP_WCHAR_STR:
        {
            packet_t dynamicSize = ValueConverter::from_uint16(m_data.size());
            payload.insert(payload.end(), dynamicSize.begin(), dynamicSize.end());
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
        m_data.resize(g_type_sizes[g_cmd_code_types_flags[m_cmd].type]);
        break;
    case TYPE_TCP_DATA_SET:
        // wtf here?
        m_data.resize(sizeof(uint16_t));
        break;
    case TYPE_TCP_DYNAMIC_UINT32_SIZE:
        {
            packet_t dynamicSize = ValueConverter::from_uint32(m_data.size());
            payload.insert(payload.end(), dynamicSize.begin(), dynamicSize.end());
            break;
        }
    default:
        throw std::exception("Got unknown enum type for parsing entry");
        break;
    }
    */
    payload.insert(payload.end(), m_data.begin(), m_data.end());
    return payload;
}

uint8_t DataEntry::get_uint8()
{
    packet_iterator_t it = m_data.begin();
    return ValueConverter::to_uint8(it, m_data.end());
}

uint16_t DataEntry::get_uint16()
{
    packet_iterator_t it = m_data.begin();
    return ValueConverter::to_uint16(it, m_data.end());
}

uint32_t DataEntry::get_uint32()
{
    packet_iterator_t it = m_data.begin();
    return ValueConverter::to_uint32(it, m_data.end());
}

uint64_t DataEntry::get_uint64()
{
    packet_iterator_t it = m_data.begin();
    return ValueConverter::to_uint64(it, m_data.end());
}

std::vector<uint8_t> DataEntry::get_raw_data()
{
    return m_data;
}

std::string DataEntry::get_string()
{
    std::string str = "";
    for(uint64_t i=0; i<m_data.size(); i++)
    {
        if(m_data[i] == 0x00)
            continue;
        str.append((char*)&m_data[i], 1);
    }
    return str;
}

bool DataEntry::valid()
{
    if(m_entryHeader.getCmd() == CMD_CODE::UNKNOWN)
        return false;
    if(m_data.size() == 0)
        return false;
    return true;
}

void DataEntry::dbg_print()
{
    m_entryHeader.dbg_print();
    BOOST_LOG_TRIVIAL(info) << "Data: " << Utils::toHexBuffer(m_data);
}

void DataEntry::m_build(std::vector<uint8_t> &data)
{
    // build cmd and data if needed add size
}

void DataEntry::printField()
{
    switch (m_entryHeader.getCmd())
    {
        case CMD_CODE::APPLICATION_VALUE:
            BOOST_LOG_TRIVIAL(info) << "Application value: " << get_uint16();
            break;
        case CMD_CODE::VERSION:
            BOOST_LOG_TRIVIAL(info) << "Version: " << get_uint32();
            break;
        case CMD_CODE::SESSION_GUID:
            BOOST_LOG_TRIVIAL(info) << "Session GUID: " << Utils::toHexBuffer(get_raw_data());
            break;
        case CMD_CODE::USER_NAME:
            BOOST_LOG_TRIVIAL(info) << "Username: " << get_string();
            break;
        case CMD_CODE::BIN_BLOB:
            BOOST_LOG_TRIVIAL(info) << "BIN BLOB: " << Utils::toHexBuffer(m_data);
            break;
        case CMD_CODE::SYS_SITE_ID:
            BOOST_LOG_TRIVIAL(info) << "SYS_SITE_ID: " << Utils::valueToHex(get_uint32(), sizeof(uint32_t));
            break;
        case CMD_CODE::STEAM_ID:
            BOOST_LOG_TRIVIAL(info) << "STEAM_ID: " << Utils::valueToHex(get_uint64(), sizeof(uint64_t));
            break;
        case CMD_CODE::PLAYER_ID:
            BOOST_LOG_TRIVIAL(info) << "PLAYER_ID: " << Utils::valueToHex(get_uint32(), sizeof(uint32_t));
            break;
        case CMD_CODE::NET_ACCESS_FLAGS:
            BOOST_LOG_TRIVIAL(info) << "NET_ACCESS_FLAGS: " << Utils::valueToHex(get_uint16(), sizeof(uint16_t));
            break;
        case CMD_CODE::DESTROYED_ASM_ID:
            BOOST_LOG_TRIVIAL(info) << "DESTROYED_ASM_ID: " << Utils::valueToHex(get_uint32(), sizeof(uint32_t));
            break;
        case CMD_CODE::DISPLAY_EULA_FLAG:
            BOOST_LOG_TRIVIAL(info) << "DISPLAY_EULA_FLAG: " << Utils::valueToHex(get_uint32(), sizeof(uint32_t));
            break;
        case CMD_CODE::PLAYER_NAME:
            BOOST_LOG_TRIVIAL(info) << "PLAYER_NAME: " << Utils::valueToHex(get_uint32(), sizeof(uint32_t));
            break;
        default:
            break;
    }
}
