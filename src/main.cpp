/*
    Global Agenda RE PoC

    COPYRIGHT (C) Sergey Nadtochii 2021
*/

//#include <cstdlib>

#include "Packet.hpp"
#include "TcpPacket.hpp"

#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <utility>
#include <iomanip>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>

#include "Server.hpp"

#define TCP_MAIN_PORT 9000


void dbg_print(std::vector<uint8_t> &buf)
{
    
    std::stringstream ss;
    for(std::size_t i=0; i<buf.size(); i++)
    {
        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << ((int)buf[i]) << " " << std::dec;
    }
    std::cout << ss.str();
    std::cout << "\n";
    //std::cout << "Full " << buf.data() << std::endl;
    
}

void test_parse_packet()
{
    std::vector<uint8_t> first_response = {0x0A, 0x00, 0x3B, 0x00, 0x01, 0x00, 0x36, 0x05, 0x01, 0x00, 0x05, 0x01};
    dbg_print(first_response);
    Packet pac(first_response);
}

void test_build_packet()
{
    std::vector<uint8_t> guidPayload{ 
        0x40, 0x41, 0x42, 0x43, 
        0x44, 0x45, 0x46, 0x47, 
        0x48, 0x49, 0x50, 0x51, 
        0x52, 0x53, 0x54, 0x55
    };

    Packet response;
    response.appendEntry(DataEntry(CMD_CODE::SESSION_GUID, guidPayload));
    std::vector<uint8_t> data;
    data = response.build();
    dbg_print(data);
}

void testNestedDataSetParsing()
{
    std::vector<uint8_t> input = {0x39,0x02,0x3e,0x00,0x0e,0x00,0xc3,0x03,0x32,0x5d,0x10,0x00,0x75,0x03,0xf8,0xf3,0xff,0x01,0x00,0x00,0x00,0x00,0x13,0x02,0x01,0x00,0x6e,0xc5,0x03,0x08,0x00,0x54,0x72,0x6f,0x6a,0x61,0x6e,0x44,0x50,0xd4,0x04,0x01,0x6e,0x03,0x49,0x49,0x00,0x00,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x73,0x04,0x5c,0x49,0x8a,0xe8,0x50,0xf5,0x3b,0x41,0x99,0x7c,0x01,0x1a,0x57,0x75,0x5d,0x70,0xdb,0x00,0x24,0x9e,0x00,0x00,0x70,0x02,0x0f,0x00,0x00,0x00,0x1c,0x00,0x84,0x03,0x00,0x00,0xff,0x04,0x03,0x00,0x74,0x61,0x62,0x0c,0x01,0x03,0x00,0x02,0x00,0xda,0x04,0x01,0x00,0x00,0x00,0x71,0x03,0x6f,0xa2,0x00,0x00,0x02,0x00,0xda,0x04,0x02,0x00,0x00,0x00,0x71,0x03,0x0b,0xa2,0x00,0x00,0x02,0x00,0xda,0x04,0x04,0x00,0x00,0x00,0x71,0x03,0xac,0xcf,0x00,0x00,0x70,0x01,0x1e,0x00,0x02,0x00,0x22,0x03,0x6b,0x04,0x00,0x00,0x62,0x02,0x95,0xab,0x00,0x00,0x02,0x00,0x22,0x03,0x93,0x04,0x00,0x00,0x62,0x02,0x83,0xc6,0x00,0x00,0x02,0x00,0x22,0x03,0x59,0x05,0x00,0x00,0x62,0x02,0xd7,0xcd,0x00,0x00,0x02,0x00,0x22,0x03,0x9f,0x05,0x00,0x00,0x62,0x02,0x7c,0xfc,0x00,0x00,0x02,0x00,0x22,0x03,0x07,0x05,0x00,0x00,0x62,0x02,0x66,0x8c,0x00,0x00,0x02,0x00,0x22,0x03,0x58,0x04,0x00,0x00,0x62,0x02,0xad,0x86,0x00,0x00,0x02,0x00,0x22,0x03,0x18,0x05,0x00,0x00,0x62,0x02,0xd8,0x8f,0x00,0x00,0x02,0x00,0x22,0x03,0x1b,0x03,0x00,0x00,0x62,0x02,0x33,0x84,0x00,0x00,0x02,0x00,0x22,0x03,0xf8,0x04,0x00,0x00,0x62,0x02,0x9e,0x85,0x00,0x00,0x02,0x00,0x22,0x03,0xa5,0x05,0x00,0x00,0x62,0x02,0x7a,0xfd,0x00,0x00,0x02,0x00,0x22,0x03,0xdd,0x04,0x00,0x00,0x62,0x02,0x3b,0x84,0x00,0x00,0x02,0x00,0x22,0x03,0x89,0x05,0x00,0x00,0x62,0x02,0xf0,0xea,0x00,0x00,0x02,0x00,0x22,0x03,0x5d,0x05,0x00,0x00,0x62,0x02,0xcd,0xd8,0x00,0x00,0x02,0x00,0x22,0x03,0xb5,0x04,0x00,0x00,0x62,0x02,0xb9,0x8c,0x00,0x00,0x02,0x00,0x22,0x03,0x0b,0x05,0x00,0x00,0x62,0x02,0xb5,0x8c,0x00,0x00,0x02,0x00,0x22,0x03,0x1b,0x05,0x00,0x00,0x62,0x02,0xba,0x90,0x00,0x00,0x02,0x00,0x22,0x03,0x5c,0x05,0x00,0x00,0x62,0x02,0x54,0xce,0x00,0x00,0x02,0x00,0x22,0x03,0x17,0x05,0x00,0x00,0x62,0x02,0xcf,0x8f,0x00,0x00,0x02,0x00,0x22,0x03,0xcb,0x04,0x00,0x00,0x62,0x02,0x39,0x84,0x00,0x00,0x02,0x00,0x22,0x03,0x0f,0x05,0x00,0x00,0x62,0x02,0x0a,0x9e,0x00,0x00,0x02,0x00,0x22,0x03,0x9e,0x05,0x00,0x00,0x62,0x02,0x53,0xfb,0x00,0x00,0x02,0x00,0x22,0x03,0x1c,0x05,0x00,0x00,0x62,0x02,0xbc,0x90,0x00,0x00,0x02,0x00,0x22,0x03,0x8b,0x05,0x00,0x00,0x62,0x02,0x0c,0xeb,0x00,0x00,0x02,0x00,0x22,0x03,0xc8,0x04,0x00,0x00,0x62,0x02,0x37,0x84,0x00,0x00,0x02,0x00,0x22,0x03,0xdb,0x04,0x00,0x00,0x62,0x02,0x38,0x84,0x00,0x00,0x02,0x00,0x22,0x03,0xf6,0x04,0x00,0x00,0x62,0x02,0x8d,0x85,0x00,0x00,0x02,0x00,0x22,0x03,0xc3,0x03,0x00,0x00,0x62,0x02,0x34,0x84,0x00,0x00,0x02,0x00,0x22,0x03,0xd9,0x04,0x00,0x00,0x62,0x02,0x35,0x84,0x00,0x00,0x02,0x00,0x22,0x03,0xde,0x04,0x00,0x00,0x62,0x02,0x36,0x84,0x00,0x00,0x02,0x00,0x22,0x03,0xaa,0x05,0x00,0x00,0x62,0x02,0xe9,0xfd,0x00,0x00};
    TcpPacket tcp(input.begin(), input.end());
    Packet packet = tcp.getNextPacket();
}

std::vector<uint8_t> get_session_guid()
{
    std::vector<uint8_t> guidPayload{ 
        0x40, 0x41, 0x42, 0x43, 
        0x44, 0x45, 0x46, 0x47, 
        0x48, 0x49, 0x50, 0x51, 
        0x52, 0x53, 0x54, 0x55
    };

    return guidPayload;
}

void testNestedDataSetBuilding()
{
    Packet packet(APPLY_ON_INTERVAL_FLAG);
    packet.appendEntry(DataEntry(CMD_CODE::PLAYER_ID, (uint32_t)0x001736D5));
    packet.appendEntry(DataEntry(CMD_CODE::NET_ACCESS_FLAGS, (uint64_t)0x01FFF3F8));
    packet.appendEntry(DataEntry(CMD_CODE::DISPLAY_EULA_FLAG, "n"));
    packet.appendEntry(DataEntry(CMD_CODE::PLAYER_NAME, "PlayerNameLol"));
    packet.appendEntry(DataEntry(CMD_CODE::SUCCESS, (bool)true));
    packet.appendEntry(DataEntry(CMD_CODE::MSG_ID, (uint32_t)0x00004949));
    packet.appendEntry(DataEntry(CMD_CODE::BANNED_UNTIL_DATETIME, (uint64_t)0x0000000000000000));
    packet.appendEntry(DataEntry(CMD_CODE::SESSION_GUID, get_session_guid()));
    packet.appendEntry(DataEntry(CMD_CODE::CONNECTION_HANDLE, (uint32_t)0x00001180));
    packet.appendEntry(DataEntry(CMD_CODE::GAME_BITS, (uint32_t)0x0000000F));
    packet.appendEntry(DataEntry(CMD_CODE::AFK_TIMEOUT_SEC, (uint32_t)0x000000384));
    packet.appendEntry(DataEntry(CMD_CODE::TEXT_VALUE, "f5"));

    DataSet sysSite(CMD_CODE::DATA_SET,
        {
            AnonymousArray(
                DataEntry(CMD_CODE::SYS_SITE_ID, (uint32_t)0x00000001),
                DataEntry(CMD_CODE::NAME_MSG_ID, (uint32_t)0x0000A26F)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::SYS_SITE_ID, (uint32_t)0x00000002),
                DataEntry(CMD_CODE::NAME_MSG_ID, (uint32_t)0x0000A2B0)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::SYS_SITE_ID, (uint32_t)0x00000004),
                DataEntry(CMD_CODE::NAME_MSG_ID, (uint32_t)0x0000CFAC)
            )
        }
    );
    packet.appendDataSet(sysSite);

    DataSet mapConfig(CMD_CODE::DATA_SET_MAP_CONFIGS,
        {
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x0000046B),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000AB95)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x00000493),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000C683)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x00000559),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000CDD7)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x0000059F),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000FC7C)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x00000507),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x00008C66)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x00000458),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x000086AD)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x00000518),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x00008FD8)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x0000031B),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x00008433)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x000004F8),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000859E)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x000005A5),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000FD7A)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x000004DD),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000843B)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x00000589),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000EAF0)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x0000055D),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000D8CD)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x000004B5),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x00008CB9)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x0000050B),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x00008CB5)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x0000051B),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x000090BA)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x0000055C),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000CE54)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x00000517),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x00008FCF)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x000004CB),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x00008439)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x0000050F),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x00009E0A)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x0000059E),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000FB53)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x0000051C),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x000090BC)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x0000058B),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000EB0C)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x000004C8),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x00008437)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x000004DB),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x00008438)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x000004F6),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000858D)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x000003C3),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x00008434)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x000004D9),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x00008435)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x000004DE),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x00008436)
            ),
            AnonymousArray(
                DataEntry(CMD_CODE::MAP_GAME_ID,            (uint32_t)0x000005AA),
                DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID,   (uint32_t)0x0000FDE9)
            ),
        }
    );
    packet.appendDataSet(mapConfig);


    std::vector<uint8_t> tcpData = TcpPacket(packet.build()).build();
}

int main(int argc, char* argv[])
{
    //test_parse_packet();
    //test_build_packet();
    //testNestedDataSetParsing();
    testNestedDataSetBuilding();
    return 0;

    try
    {
        boost::asio::io_context io_context;
        //server(io_context, TCP_MAIN_PORT);
        Server server(TCP_MAIN_PORT);
        server.start();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}

/*

Input for hash:
73 00 65 00 72 00 65 00 67 00 61 00 39 00 36 00 31 00 32 00 33 00 46 00 41 00 30 00 30 00 43 00 44 00 46 00 35 00 2D 00 38 00 44 00 38 00 37 00 2D 00 34 00 61 00 37 00 36 00 2D 00 42 00 45 00 45 00 31 00 2D 00 44 00 39 00 45 00 34 00 38 00 33 00 32 00 32 00 30 00 43 00 31 00 33 00

MD5 hashed data 16 bytes length:
B7 14 57 C5 E0 6F AD 0D D8 3F 40 2C D7 9B 8F 4A

Second round md5 hash. Getting md5 hash from B7 14 57 C5...
76 5c 2a 54 0b 4d 49 2c c1 30 b9 a2 7e ec aa eb

Input for ecnryption:
34 00 35 00 34 00 36 00 34 00 37 00 34 00 38 00 2D 00 34 00 33 00 34 00 34 00 2D 00 34 00 31 00 34 00 32 00 2D 00 35 00 36 00 35 00 35 00 2D 00 35 00 34 00 35 00 33 00 35 00 32 00 35 00 31 00 35 00 30 00 34 00 39 00 73 00 65 00 72 00 65 00 67 00 61 00 39 00 36 00


Encrypted data with RC4:
8d 4d ca 3e 6f 10 8f 1c ee 74 1b 71 f9 dc 1b e3 51 4a 46 a0 51 fb 5c a3 31 b7 bc be 7a 68 28 66 a5 99 da 6d 28 7c b1 f6 24 38 79 8f 3a fe 73 72 93 40 9a 4d 2d 87 5a 42 ad 29 82 bc d2 86 3e 27 0f cf 2a 7a 67 c4 a3 ba a6 8e cf 78 7a 71 6e 22 4d 90 b3 10 4d b3 b1 14

    CMarshal::add_widechar_entry(&cmarshal, 0x52D, login);
    CMarshal::add_something(&cmarshal, 0x71, hex_session_uuid, SizeInWords);
    CMarshal::add_new_entry(&cmarshal, 0x536, cgameclient->possible_game_client_version);
    CMarshal::add_new_entry(&cmarshal, 0x4DA, cgameclient->site_arg_usually_empty);

Second response after sending uuid
96 00 3B 00 06 00 73 04 48 47 46 45 44 43 42 41 56 55 54 53 52 51 50 49 2D 05 08 00 73 65 72 65 67 61 39 36 71 00 58 00 00 00 8D 4D CA 3E 6F 10 8F 1C EE 74 1B 71 F9 DC 1B E3 51 4A 46 A0 51 FB 5C A3 31 B7 BC BE 7A 68 28 66 A5 99 DA 6D 28 7C B1 F6 24 38 79 8F 3A FE 73 72 93 40 9A 4D 2D 87 5A 42 AD 29 82 BC D2 86 3E 27 0F CF 2A 7A 67 C4 A3 BA A6 8E CF 78 7A 71 6E 22 4D 90 B3 10 4D B3 B1 14 36 05 01 00 05 01 DA 04 00 00 00 00 C5 04 A5 A6 B8 03 01 00 10 01

96 00 - total package size
    3B 00 - cmd entries count
        06 00 - count of entries
    73 04 - SESSION_GUID - cmd session id
        48 47 46 45 44 43 42 41 56 55 54 53 52 51 50 49 - session id generated by the server
    2D 05 - USER_NAME - cmd login name
        08 00 - size
        73 65 72 65 67 61 39 36 - user login
    71 00 - BIN_BLOB - encrypted auth payload
        58 00 00 00 - size
        8D 4D CA 3E 6F 10 8F 1C EE 74 1B 71 F9 DC 1B E3 51 4A 46 A0 51 FB 5C A3 31 B7 BC BE 7A 68 28 66 A5 99 DA 6D 28 7C B1 F6 24 38 79 8F 3A FE 73 72 93 40 9A 4D 2D 87 5A 42 AD 29 82 BC D2 86 3E 27 0F CF 2A 7A 67 C4 A3 BA A6 8E CF 78 7A 71 6E 22 4D 90 B3 10 4D B3 B1 14 
    36 05 - VERSION - cmd client version
        01 00 05 01 - version value
    DA 04 - SYS_SITE_ID
        00 00 00 00 
    C5 04 - STEAM_ID
        A5 A6 B8 03 01 00 10 01


0A 00 3B 00 01 00 36 05 01 00 05 01

0A 00 - total package size
    3B 00 - cmd entries count
        01 00 - count of  entries
    36 05 - cmd client_version
        01 00 05 01 - version value

16 00 0A 00 73 04 73 04 48 47 46 45 44 43 42 41 56 55 54 53 52 51 50 49

16 00 
    0A 00 
        73 04 
    73 04 
        48 47 46 45 44 43 42 41 56 55 54 53 52 51 50 49

Other packet examples

96 00 3B 00 06 00 73 04 48 47 46 45 44 43 42 41 56 55 54 53 52 51 50 49 2D 05 08 00 73 65 72 65 67 61 39 36 71 00 58 00 00 00 8D 4D CA 3E 6F 10 8F 1C EE 74 1B 71 F9 DC 1B E3 51 4A 46 A0 51 FB 5C A3 31 B7 BC BE 7A 68 28 66 A5 99 DA 6D 28 7C B1 F6 24 38 79 8F 3A FE 73 72 93 40 9A 4D 2D 87 5A 42 AD 29 82 BC D2 86 3E 27 0F CF 2A 7A 67 C4 A3 BA A6 8E CF 78 7A 71 6E 22 4D 90 B3 10 4D B3 B1 14 36 05 01 00 05 01 DA 04 00 00 00 00 C5 04 A5 A6 B8 03 01 00 10 01 
Received packet size: 6
04 00 8E 00 00 00 
Received packet size: 18
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 18
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 18
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 18
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 18
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 18
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 18
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 18
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 24
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 18
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 18
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 18
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 18
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 18
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 
Received packet size: 24
04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 



*/

