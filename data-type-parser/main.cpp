#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <iomanip>
#include <vector>

#define STRING_NAME_SIZE 96

#define DATA_TCP_TYPES "TCP"
#define DATA_UDP_TYPES "UDP"

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

const static std::vector<std::string> g_tcp_text_types = {
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
/*
00000000 flag_0x68_struct struc ; (sizeof=0x68, mappedto_252)
00000000                                         ; XREF: global_flags/r
00000000 flag_1          dw ?                    ; XREF: sub_10935250+36/r
00000000                                         ; sub_10935250+9B/r
00000002 flag_2          dw ?
00000004 flag_3          dd ?                    ; XREF: CMarshalEntry__flag_version_conversion+31/r
00000004                                         ; CMarshalEntry__get_uint16_t+6/r ...
00000008 text_name       dw 48 dup(?)            ; XREF: sub_10935250+47/o
00000008                                         ; sub_10935250+AC/o ... ; string(C (16 bits))
00000068 flag_0x68_struct ends
*/
private:
	uint64_t m_index = 0;
	uint16_t m_flag1 = 0;
	uint16_t m_flag2 = 0;
	uint32_t m_type = 0;
	std::string m_name = "";
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
		binFile.read(reinterpret_cast<char*>(&m_flag1), sizeof(uint16_t));
		binFile.read(reinterpret_cast<char*>(&m_flag2), sizeof(uint16_t));
		binFile.read(reinterpret_cast<char*>(&m_type), sizeof(uint32_t));
		m_name = getStr(binFile, STRING_NAME_SIZE);
	}
	std::string getCsvLine()
	{
		std::stringstream ss;
		ss << 
		toHex(m_index, 4) <<
		"," << toHex(m_flag1, 4) << 
		"," << toHex(m_flag2, 4) <<
		"," << m_type << 
		"," << m_name << 
		"," << g_tcp_text_types[m_type];
		return ss.str();
	}
	std::string getLineTxt()
	{
		std::stringstream ss;
		ss << 
		toHex(m_index, 4) <<
		" Flag1: " << std::left << std::setw(4) << m_flag1 << 
		" Flag2: " << std::left << std::setw(4) << m_flag2 <<
		" Type: " << std::left << std::setw(4) << m_type << 
		" Name: " << std::left << std::setw(44) << m_name << 
		" " << g_tcp_text_types[m_type];
		return ss.str();
	}
	std::string getEnumTypeFlagLine()
	{
		std::stringstream ss;
		ss << 
		"	{" << toHex(m_type, 2) << 
		", " << toHex(m_flag1, 4) << "},";
		return ss.str();
	}
	std::string getEnumLine()
	{
		std::stringstream ss;
		ss << "	" << m_name << " = " << toHex(m_index, 4) << ",";
		return ss.str();
	}
	std::string getEnumTextLine()
	{
		std::stringstream ss;
		ss << "	\"" << m_name << "\",";
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

std::string generateEnum()
{
	std::stringstream ss;
	ss << "enum ENUM_TYPE : uint32_t\n{\n";
	for(uint64_t i=0; i<g_tcp_text_types.size(); i++)
	{
		ss << "	" << g_tcp_text_types[i] << " = " << i << ",\n";
	}
	ss << "};\n\n";
	return ss.str();

}

void generateTcpHeaderFile(std::ifstream &binFile)
{
	std::string structDefinition = "typedef struct enum_type_flags\n{\n	uint32_t type = 0;\n	uint32_t flags = 0;\n} enum_type_flags_t;\n\n";

	std::string enumValue = "enum CMD_CODE : uint16_t \n{\n";
	std::string enumTypeFlag = structDefinition + "const static enum_type_flags_t g_cmd_code_types_flags[] =\n{\n";
	std::string enumText = "const static std::vector<std::string> g_cmd_code_text \n{\n";

	std::ofstream outFile("EnumDataTypes.hpp");
	std::string headerName = "ENUM_DATA_TYPES_HPP";
	std::stringstream headerFileContent;
	outFile << 
	"#ifndef " << headerName << "\n" <<
	"#define "  << headerName << "\n\n" << 
	"#include <cstdint>\n\n";

	uint64_t counter = 0;
	while(!binFile.eof())
	{
		TypeEntryTCP entry(binFile, counter);
		enumValue += entry.getEnumLine() + "\n";
		enumTypeFlag += entry.getEnumTypeFlagLine() + "\n";
		enumText += entry.getEnumTextLine() + "\n";
		counter++;
	}

	enumValue += "};";
	enumTypeFlag += "};";
	enumText += "};";

	outFile << generateEnum();
	outFile << enumValue << "\n\n";
	outFile << enumTypeFlag << "\n\n";
	outFile << enumText << "\n\n";
	outFile << "#endif // " << headerName << "\n";
}

void generateTcpCsv(std::ifstream &binFile)
{
	uint64_t counter = 0;
	while(!binFile.eof())
	{
		TypeEntryTCP entry(binFile, counter);
		std::cout << entry.getCsvLine() << std::endl;
		counter++;
	}
}

void generateTcpReadable(std::ifstream &binFile)
{
	uint64_t counter = 0;
	while(!binFile.eof())
	{
		TypeEntryTCP entry(binFile, counter);
		std::cout << entry.getLineTxt() << std::endl;
		counter++;
	}
}

void generateUdpReadable(std::ifstream &binFile)
{
	uint64_t counter = 0;
	while(!binFile.eof())
	{
		TypeEntryUDP entry(binFile, counter);
		std::cout << entry.getLine() << std::endl;
		counter++;
	}
}

const static std::string g_parser_commands[] = 
{
	"generate-tcp-header",
	"generate-tcp-csv",
	"generate-tcp-readable",
	"generate-udp-readable"
};

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		std::cout << "Please specify path to binary data types file" << std::endl;
		return 0;
	}

	std::string userCmd(argv[1]);
	std::string filePath(argv[2]);

	std::ifstream binFile(filePath, std::ios::binary);
	if(!binFile.is_open())
	{
		std::cout << "Error cannot open file" << std::endl;
		return 0;
	}

	if(userCmd == g_parser_commands[0])
	{
		generateTcpHeaderFile(binFile);
	}
	else if(userCmd == g_parser_commands[1])
	{
		generateTcpCsv(binFile);
	}
	else if(userCmd == g_parser_commands[2])
	{
		generateTcpReadable(binFile);
	}
	else if(userCmd == g_parser_commands[3])
	{
		generateUdpReadable(binFile);
	}

	return 0;
}