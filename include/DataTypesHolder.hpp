#ifndef DATA_TYPES_HOLDER_HPP
#define DATA_TYPES_HOLDER_HPP

#include "DataType.hpp"
#include <map>
#include <iostream>

/*
 * Singleton class for holding all types
 */

class DataTypesHolder
{
public:
    DataTypesHolder(DataTypesHolder &other) = delete;
    void operator=(DataTypesHolder const &other) = delete;
    ~DataTypesHolder();
    static DataTypesHolder *getInstance();
    DataType get(CMD_CODE cmd);
private:
    void append(CMD_CODE cmd, DataType dt);
    DataTypesHolder();
private:
    static DataTypesHolder *m_this;
    std::map<CMD_CODE, DataType> m_types;
};

#endif // DATA_TYPES_HOLDER_HPP