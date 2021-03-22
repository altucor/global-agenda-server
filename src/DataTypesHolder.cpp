#include "DataTypesHolder.hpp"

#include "Utils.hpp"

DataTypesHolder *DataTypesHolder::m_this = nullptr;

DataTypesHolder::DataTypesHolder()
{
    std::cout << "Calling singleton constructor\n";
    append(CMD_CODE::APPLICATION_VALUE, DataType(CMD_TYPE::STATIC,  sizeof(uint16_t)));
    append(CMD_CODE::BIN_BLOB,      DataType(CMD_TYPE::DYNAMIC, sizeof(uint32_t)));
    append(CMD_CODE::BUFF_COUNT,    DataType(CMD_TYPE::STATIC,  sizeof(uint16_t)));
    append(CMD_CODE::SESSION_GUID,  DataType(CMD_TYPE::STATIC,  16));
    append(CMD_CODE::STEAM_ID,      DataType(CMD_TYPE::STATIC,  sizeof(uint64_t)));
    append(CMD_CODE::SYS_SITE_ID,   DataType(CMD_TYPE::STATIC,  sizeof(uint32_t)));
    append(CMD_CODE::USER_NAME,     DataType(CMD_TYPE::DYNAMIC, sizeof(uint16_t)));
    append(CMD_CODE::VERSION,       DataType(CMD_TYPE::STATIC,  sizeof(uint32_t)));
}

DataTypesHolder::~DataTypesHolder()
{

}

DataTypesHolder *DataTypesHolder::getInstance()
{
    if(m_this == nullptr)
        m_this = new DataTypesHolder();
    return m_this;
}

void DataTypesHolder::append(CMD_CODE cmd, DataType dt)
{
    m_types.emplace(cmd, dt);
}

DataType DataTypesHolder::get(CMD_CODE cmd)
{
    std::cout << "DataTypesHolder::get 0x";
    Utils::printHexValue((uint16_t)cmd);
    std::cout << " code\n";
    auto search = m_types.find(cmd);
    if(search != m_types.end()) {
        return m_types.at(cmd);
    }
    else
    {
        std::cout << "Not found DataType by code: 0x";
        Utils::printHexValue((uint16_t)cmd);
        std::cout << cmd << "\n";
        return DataType();
    }
}
