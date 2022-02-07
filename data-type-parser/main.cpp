#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <iomanip>
#include <vector>

#define STRING_NAME_SIZE 96

enum TYPE_SIZE{
	TYPE_WCHAR_STR = 0,
	TYPE_FLOAT = 1, // or double
	TYPE_DOUBLE_OR_INT_SIGNED = 2,
	TYPE_UINT32 = 3,
	TYPE_UINT16 = 4,
	TYPE_UINT8 = 5,
	TYPE_DATA_SET = 6,
	TYPE_DATETIME = 7,
	TYPE_UINT64 = 8,
	TYPE_UUID_16_BYTES = 9,
	TYPE_DYNAMIC_UINT32_SIZE = 10,
};

const std::vector<std::string> g_type_size_text = {
	"TYPE_WCHAR_STR",
	"TYPE_FLOAT",
	"TYPE_DOUBLE_OR_INT_SIGNED",
	"TYPE_UINT32",
	"TYPE_UINT16",
	"TYPE_UINT8",
	"TYPE_DATA_SET",
	"TYPE_DATETIME",
	"TYPE_UINT64",
	"TYPE_UUID_16_BYTES",
	"TYPE_DYNAMIC_UINT32_SIZE"
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

class TypeEntry
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
	explicit TypeEntry(std::ifstream &binFile, const uint64_t index)
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
		std::right << std::setw(5) << m_index <<
		" Type: " << std::left << std::setw(3) << m_type << 
		" Name: " << std::left << std::setw(44) << m_name << 
		" flags: " << toHex(m_flags, 4) <<
		" " << g_type_size_text[m_type];
		return ss.str();
	}
};


int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		std::cout << "Please specify path to binary data types file" << std::endl;
		return 0;
	}

	std::ifstream binFile(argv[1], std::ios::binary);
	if(!binFile.is_open())
	{
		std::cout << "Error cannot open file" << std::endl;
		return 0;
	}

	uint64_t counter = 0;
	while(!binFile.eof())
	{
		TypeEntry entry(binFile, counter);
		std::cout << entry.getLine() << std::endl;
		counter++;
	}

	return 0;
}