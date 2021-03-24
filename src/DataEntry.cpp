#include "DataEntry.hpp"

#include "Utils.hpp"

std::vector<DataType> g_dataTypes();


DataEntry::DataEntry()
{
    m_cmd = (CMD_CODE)0;
    m_data = std::vector<uint8_t>();
}

std::vector<uint8_t> DataEntry::m_parse(CMD_CODE cmd, packet_iterator_t &it, 
    packet_iterator_t end)
{
    DataTypesHolder *dataTypes = DataTypesHolder::getInstance();
    DataType dt = dataTypes->get(cmd);
    if(!dt.valid())
    {
        return std::vector<uint8_t>();
    }

    if(!ValueConverter::isEnoughToRead(it, end, dt.getStaticSize()))
    {
        return std::vector<uint8_t>();
    }
    
    if(dt.getType() == CMD_TYPE::STATIC)
    {
        packet_iterator_t start_copy = it;
        it += dt.getStaticSize();
        return std::vector<uint8_t>(start_copy, it);
    }
    else if(dt.getType() == CMD_TYPE::DYNAMIC)
    {
        uint64_t staticSize = dt.getStaticSize();
        uint64_t dynamicSize = 0;
        switch(staticSize)
        {
            case 1:
                dynamicSize = ValueConverter::to_uint8(it, end);
                break;
            case 2:
                dynamicSize = ValueConverter::to_uint16(it, end);
                break;
            case 4:
                dynamicSize = ValueConverter::to_uint32(it, end);
                break;
            case 8:
                dynamicSize = ValueConverter::to_uint64(it, end);
                break;
            case 0:
            default:
                break;
        }
        if(ValueConverter::isEnoughToRead(it, end, dynamicSize))
        {
            packet_iterator_t start_copy = it;
            it += dynamicSize;
            return std::vector<uint8_t>(start_copy, it);
        }
    }
    return std::vector<uint8_t>();
}

DataEntry::DataEntry(packet_iterator_t &it, 
    packet_iterator_t end)
{
    m_cmd = (CMD_CODE)ValueConverter::to_uint16(it, end);
    m_data = m_parse(m_cmd, it, end);
}

DataEntry::DataEntry(std::vector<uint8_t> &data)
{

}

DataEntry::DataEntry(CMD_CODE cmd, std::vector<uint8_t> &data)
    : m_cmd(cmd), m_data(data)
{
    
}

DataEntry::DataEntry(CMD_CODE cmd, uint16_t value)
    : m_cmd(cmd)
{
    m_data = ValueConverter::from_uint16(value);
}

DataEntry::~DataEntry()
{

}

CMD_CODE DataEntry::getCmd()
{
    return m_cmd;
}

std::vector<uint8_t> DataEntry::build()
{
    DataTypesHolder *dataTypes = DataTypesHolder::getInstance();
    DataType dt = dataTypes->get(m_cmd);
    if(!dt.valid())
    {
        return std::vector<uint8_t>();
    }

    std::vector<uint8_t> payload;
    payload = ValueConverter::from_uint16(m_cmd);
    if(dt.getType() == CMD_TYPE::STATIC)
    {
        if(m_data.size() != dt.getStaticSize())
        {
            m_data.resize(dt.getStaticSize());
        }
        payload.insert(payload.end(), m_data.begin(), m_data.end());
    }
    else if(dt.getType() == CMD_TYPE::DYNAMIC)
    {
        std::vector<uint8_t> dynamicSize;
        uint64_t staticSize = dt.getStaticSize();
        switch(staticSize)
        {
            case 1:
                dynamicSize = ValueConverter::from_uint8(m_data.size());
                break;
            case 2:
                dynamicSize = ValueConverter::from_uint16(m_data.size());
                break;
            case 4:
                dynamicSize = ValueConverter::from_uint32(m_data.size());
                break;
            case 8:
                dynamicSize = ValueConverter::from_uint64(m_data.size());
                break;
            case 0:
            default:
                break;
        }
        payload.insert(payload.end(), m_data.begin(), m_data.end());
    }
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

bool DataEntry::valid()
{
    if(m_cmd == (CMD_CODE)0)
        return false;
    if(m_data.size() == 0)
        return false;
    return true;
}

void DataEntry::dbg_print()
{
    std::cout << "DataEntry cmd 0x";
    Utils::printHexValue((uint16_t)m_cmd);
    std::cout << "\nData: ";
    Utils::printHexBuffer(m_data);
}

void DataEntry::m_build(std::vector<uint8_t> &data)
{
    // build cmd and data if needed add size
}
