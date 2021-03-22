/*
    Global Agenda RE PoC

    COPYRIGHT (C) Sergey Nadtochii 2021
*/

//#include <cstdlib>

#include "Packet.hpp"

#include <iostream>
#include <vector>
#include <functional>
#include <memory>
#include <utility>
#include <iomanip>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#define TCP_MAIN_PORT 9000
#define BUFFER_SIZE 0x1000

#define CMD_SESSION_GUID 0x473

using boost::asio::ip::tcp;

typedef struct packet
{
    uint16_t size;
    uint8_t *data;
} packet_t;

typedef struct packet_data
{
    uint16_t cmd;
    uint8_t *data;
} packet_data_t;

typedef struct end_communication
{
    uint16_t err_code;
} end_communication_t;

typedef struct first_request
{
    uint16_t unk_0;
    uint16_t unk_1;
    uint32_t client_version;
} first_request_t;

typedef struct _GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} GUID;

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

/*
    Possible packet structure
    uint16_t packet_size
    uint16_t packet index
    uint16_t marshal_table_index
    uint8_t[] payload_data
    uint16_t marshal_table_index
    uint8_t[] payload_data
    ...
    uint16_t marshal_table_index
    uint8_t[] payload_data
*/

void craft_first_reponse(std::vector<uint8_t> &data)
{
    
    std::vector<uint8_t> guidPayload{ 
        0x40, 0x41, 0x42, 0x43, 
        0x44, 0x45, 0x46, 0x47, 
        0x48, 0x49, 0x50, 0x51, 
        0x52, 0x53, 0x54, 0x55
    };
    DataEntry guidEntry(CMD_CODE::SESSION_GUID, guidPayload);
    Packet response;
    response.appendEntry(guidEntry);
    data = response.build();

    //first_response.resize(0x473);
    //std::fill(first_response.begin(), first_response.end(), 0x41);

    uint8_t header_size = 2;
    uint16_t packet_size = 2 + 2 + 2 + 8 + 8;
    uint16_t cmd_entries_count = 0x3B;
    uint16_t entries_count = 1;
    uint16_t session_guid_cmd = CMD_SESSION_GUID;
    uint64_t uuid_part_1 = 0x4142434445464748;
    uint64_t uuid_part_2 = 0x4950515253545556;
    // possible uuid consists only of 0x10(16) bytes
    /*
        Really readed bytes by client, all other is skipped
        debug714:0A1F8120 dq 4142434445464748h
        debug714:0A1F8128 dq 5453525151524950h
    */

    // 0x01 = nodees count
    // 0x105 = index in flag table

    /*
        Client session uuid converted to HEX string
        Stack[000061B8]:0019E604 a45464748434441:
        Stack[000061B8]:0019E604 text "UTF-16LE", '45464748-4344-4142-5655-545352515049',0

        CryptoProvider creates this string
        Stack[000061B8]:0019DD80 aSerega96123fa0:
        Stack[000061B8]:0019DD80 text "UTF-16LE", 'serega96123FA00CDF5-8D87-4a76-BEE1-D9E483220C13',0

        B7 14 57 C5 E0 6F AD 0D D8 3F 40 2C D7 9B 8F 4A 27 6E 34 11 D2 B9 8A 47 C0 7D 60 00 38 DE 19 00 - possible crypto hash
    */

    data.resize(packet_size + header_size);
    memcpy(&data[0], &packet_size, sizeof(packet_size));
    memcpy(&data[2], &cmd_entries_count, sizeof(cmd_entries_count));
    memcpy(&data[4], &entries_count, sizeof(entries_count));
    memcpy(&data[6], &session_guid_cmd, sizeof(session_guid_cmd));
    memcpy(&data[8], &uuid_part_1, sizeof(uuid_part_1));
    memcpy(&data[16], &uuid_part_2, sizeof(uuid_part_2));
}

void session(tcp::socket sock)
{
    std::cout << "New Client connected\n";
    std::vector<uint8_t> first_response;
    craft_first_reponse(first_response);
    std::cout << "Built packet:\n";
    dbg_print(first_response);
    try
    {
        for (;;)
        {
            std::vector<uint8_t> buffer(BUFFER_SIZE);
            boost::system::error_code error;
            size_t length = sock.read_some(boost::asio::buffer(buffer.data(), buffer.size()), error);
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.
            std::cout << "Received packet size: " << length << std::endl;
            buffer.resize(length);
            dbg_print(buffer);
            // here add parsing of several packets, like if one stream contains several packets
            /*
                04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00 04 00 8E 00 00 00
            */
            std::vector<uint8_t>::iterator start = buffer.begin();
            std::vector<uint8_t>::iterator end = buffer.end();
            while(start != end)
            {
                Packet packet(start, end);
            }
            //for(int i=0; i<1000; i++)
            //{
                sleep(1);
                boost::asio::write(sock, boost::asio::buffer(first_response.data(), first_response.size()));
            //}
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void server(boost::asio::io_context& io_context, unsigned short port)
{
    tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
    for (;;)
    {
        std::thread(session, a.accept()).detach();
    }
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
    DataEntry guidEntry(CMD_CODE::SESSION_GUID, guidPayload);
    Packet response;
    response.appendEntry(guidEntry);
    std::vector<uint8_t> data;
    data = response.build();
    dbg_print(data);
}

int main(int argc, char* argv[])
{
    //test_parse_packet();
    //test_build_packet();
    //return 0;
    
    
    try
    {
        boost::asio::io_context io_context;
        server(io_context, TCP_MAIN_PORT);
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
    73 04 - cmd session id
        48 47 46 45 44 43 42 41 56 55 54 53 52 51 50 49 - session id generated by the server
    2D 05 - cmd login name
        08 00 - size
        73 65 72 65 67 61 39 36 - user login
    71 00 - encrypted auth payload
        58 00 00 00 - size
        8D 4D CA 3E 6F 10 8F 1C EE 74 1B 71 F9 DC 1B E3 51 4A 46 A0 51 FB 5C A3 31 B7 BC BE 7A 68 28 66 A5 99 DA 6D 28 7C B1 F6 24 38 79 8F 3A FE 73 72 93 40 9A 4D 2D 87 5A 42 AD 29 82 BC D2 86 3E 27 0F CF 2A 7A 67 C4 A3 BA A6 8E CF 78 7A 71 6E 22 4D 90 B3 10 4D B3 B1 14 
    36 05 - cmd client version
        01 00 05 01 - version value
    DA 04 
        00 00 00 00 
    C5 04 
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

