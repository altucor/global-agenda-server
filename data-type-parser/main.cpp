#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <iomanip>
#include <vector>

#define STRING_NAME_SIZE 96

enum TYPE_SIZE_TCP
{
	TYPE_TCP_WCHAR_STR = 0,
	TYPE_TCP_FLOAT = 1, // or double
	TYPE_TCP_DOUBLE_OR_INT_SIGNED = 2,
	TYPE_TCP_UINT32 = 3,
	TYPE_TCP_UINT16 = 4,
	TYPE_TCP_UINT8 = 5,
	TYPE_TCP_DATA_SET = 6,
	TYPE_TCP_DATETIME = 7,
	TYPE_TCP_UINT64 = 8,
	TYPE_TCP_UUID_16_BYTES = 9,
	TYPE_TCP_DYNAMIC_UINT32_SIZE = 10,
};

const std::vector<std::string> g_type_size_text = {
	"TYPE_TCP_WCHAR_STR",
	"TYPE_TCP_FLOAT",
	"TYPE_TCP_DOUBLE_OR_INT_SIGNED",
	"TYPE_TCP_UINT32",
	"TYPE_TCP_UINT16",
	"TYPE_TCP_UINT8",
	"TYPE_TCP_DATA_SET",
	"TYPE_TCP_DATETIME",
	"TYPE_TCP_UINT64",
	"TYPE_TCP_UUID_16_BYTES",
	"TYPE_TCP_DYNAMIC_UINT32_SIZE"
};

std::string toHex(uint64_t val, size_t width)
{
	std::stringstream ss;
	ss << "0x" <<
		std::hex << 
		std::uppercase << 
		std::setfill('0') << 
		std::setw(width) << 
		((int64_t)val) << 
		std::dec;
	return ss.str();
}

class TypeEntryTCP
{
private:
	uint64_t m_index = 0;
	uint32_t m_type = 0;
	std::string m_name = "";
	uint32_t m_flags = 0;
	std::string getStr(std::ifstream &binFile, const uint64_t count)
	{
		std::string str;
		char temp = 0;
		for(uint64_t i=0; i<count; i++)
		{
			binFile.read(reinterpret_cast<char*>(&temp), sizeof(char));
			if(temp != 0x00)
			{
				str.append(1, temp);
			}
		}
		return str;
	}
public:
	explicit TypeEntryTCP(std::ifstream &binFile, const uint64_t index)
	{
		m_index = index;
		binFile.read(reinterpret_cast<char*>(&m_type), sizeof(uint32_t));
		m_name = getStr(binFile, STRING_NAME_SIZE);
		binFile.read(reinterpret_cast<char*>(&m_flags), sizeof(uint32_t));
	}
	std::string getLine()
	{
		std::stringstream ss;
		ss << 
		toHex(m_index, 4) <<
		" Type: " << std::left << std::setw(3) << m_type << 
		" Name: " << std::left << std::setw(44) << m_name << 
		" flags: " << toHex(m_flags, 4) <<
		" " << g_type_size_text[m_type];
		return ss.str();
	}
};


class TypeEntryUDP
{
private:
	uint64_t m_index = 0;
	std::string m_name = "";
	uint32_t m_flags = 0;
	std::string getStr(std::ifstream &binFile, const uint64_t count)
	{
		std::string str;
		char temp = 0;
		for(uint64_t i=0; i<count; i++)
		{
			binFile.read(reinterpret_cast<char*>(&temp), sizeof(char));
			if(temp != 0x00)
			{
				str.append(1, temp);
			}
		}
		return str;
	}
public:
	explicit TypeEntryUDP(std::ifstream &binFile, const uint64_t index)
	{
		m_index = index;
		m_name = getStr(binFile, STRING_NAME_SIZE);
		binFile.read(reinterpret_cast<char*>(&m_flags), sizeof(uint32_t));
	}
	std::string getLine()
	{
		std::stringstream ss;
		ss << 
		toHex(m_index, 4) <<
		" Name: " << std::left << std::setw(44) << m_name << 
		" flags: " << toHex(m_flags, 4);
		return ss.str();
	}
};

#define DATA_TCP_TYPES "TCP"
#define DATA_UDP_TYPES "UDP"

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		std::cout << "Please specify path to binary data types file" << std::endl;
		return 0;
	}

	std::string type(argv[1]);
	std::string filePath(argv[2]);

	std::ifstream binFile(filePath, std::ios::binary);
	if(!binFile.is_open())
	{
		std::cout << "Error cannot open file" << std::endl;
		return 0;
	}

	uint64_t counter = 0;
	while(!binFile.eof())
	{
		if(type == DATA_TCP_TYPES)
		{
			TypeEntryTCP entry(binFile, counter);
			std::cout << entry.getLine() << std::endl;
		}
		else if(type == DATA_UDP_TYPES)
		{
			TypeEntryUDP entry(binFile, counter);
			std::cout << entry.getLine() << std::endl;
		}
		counter++;
	}

	return 0;
}