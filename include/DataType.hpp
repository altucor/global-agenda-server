#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include "EnumDataTypes.hpp"
#include <vector>

enum CMD_TYPE : uint8_t 
{
    NONE = 0,
    STATIC = 1,
    DYNAMIC = 2
};

class DataType
{
public:
    DataType();
    DataType(CMD_TYPE type, const uint64_t staticSize);
    bool valid();
    CMD_TYPE getType();
    uint64_t getStaticSize();
private:
    CMD_TYPE m_type;
    uint64_t m_staticSize;
};

#endif // DATA_TYPE_H
