#include "DataType.hpp"

DataType::DataType()
{
    m_type = CMD_TYPE::NONE;
    m_staticSize = 0;
}

DataType::DataType(CMD_TYPE type, const uint64_t staticSize)
    : m_type(type), m_staticSize(staticSize)
{

}

bool DataType::valid()
{
    if(m_type == CMD_TYPE::NONE)
        return false;
    if(m_staticSize == 0)
        return false;
    return true;
}

CMD_TYPE DataType::getType()
{
    return m_type;
}

uint64_t DataType::getStaticSize()
{
    return m_staticSize;
}
