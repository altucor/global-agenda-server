#include "DataTypesHolder.hpp"

#include "Utils.hpp"

#include <boost/log/trivial.hpp>

DataTypesHolder *DataTypesHolder::m_this = nullptr;

DataTypesHolder::DataTypesHolder()
{
    BOOST_LOG_TRIVIAL(info) << "Calling singleton constructor";
    append(CMD_CODE::APPLICATION_VALUE,     DataType(CMD_TYPE::STATIC,  sizeof(uint16_t)));
    append(CMD_CODE::BIN_BLOB,              DataType(CMD_TYPE::DYNAMIC, sizeof(uint32_t)));
    append(CMD_CODE::BUFF_COUNT,            DataType(CMD_TYPE::STATIC,  sizeof(uint16_t)));
    append(CMD_CODE::SESSION_GUID,          DataType(CMD_TYPE::STATIC,  16));
    append(CMD_CODE::STEAM_ID,              DataType(CMD_TYPE::STATIC,  sizeof(uint64_t)));
    append(CMD_CODE::SUCCESS,               DataType(CMD_TYPE::STATIC,  sizeof(uint8_t)));
    append(CMD_CODE::SYS_SITE_ID,           DataType(CMD_TYPE::STATIC,  sizeof(uint32_t)));
    append(CMD_CODE::USER_NAME,             DataType(CMD_TYPE::DYNAMIC, sizeof(uint16_t)));
    append(CMD_CODE::VERSION,               DataType(CMD_TYPE::STATIC,  sizeof(uint32_t)));
    append(CMD_CODE::BANNED_UNTIL_DATETIME, DataType(CMD_TYPE::STATIC,  sizeof(uint64_t)));
    append(CMD_CODE::TEXT_VALUE,            DataType(CMD_TYPE::STATIC,  16));
    
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
    BOOST_LOG_TRIVIAL(info) << "DataTypesHolder::get " << cmd;
    //Utils::printHexValue((uint16_t)cmd);
    //std::cout << " code\n";
    auto search = m_types.find(cmd);
    if(search != m_types.end()) {
        return m_types.at(cmd);
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "Not found DataType by code: " << cmd;
        //Utils::printHexValue((uint16_t)cmd);
        //std::cout << cmd << "\n";
        exit(1);
        return DataType();
    }
}
