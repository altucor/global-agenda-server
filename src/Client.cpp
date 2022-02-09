#include "Client.hpp"
#include "Logger.hpp"
#include "Utils.hpp"


#include <iostream>

#define CLIENT_MAX_BUFFER 0x2000

Client::Client(tcp::socket socket)
    : m_socket(std::move(socket))
{
    BOOST_LOG_TRIVIAL(info) << "New client connected";
    //Log<<
}

void Client::start()
{
    m_read();
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

void get_finish_login(std::vector<uint8_t> &data)
{
    bool success_flag = true;

    std::vector<uint8_t> guidPayload{ 
        0x40, 0x41, 0x42, 0x43, 
        0x44, 0x45, 0x46, 0x47, 
        0x48, 0x49, 0x50, 0x51, 
        0x52, 0x53, 0x54, 0x55
    };

    Packet response;
    response.appendEntry(DataEntry(CMD_CODE::SUCCESS, success_flag));
    response.appendEntry(DataEntry(CMD_CODE::SESSION_GUID, guidPayload));
    //response.appendEntry(DataEntry(CMD_CODE::GAME_BITS, guidPayload));
    //response.appendEntry(DataEntry(CMD_CODE::NET_ACCESS_FLAGS, guidPayload));
    //response.appendEntry(DataEntry(CMD_CODE::AFK_TIMEOUT_SEC, guidPayload));
    //response.appendEntry(DataEntry(CMD_CODE::DISPLAY_EULA_FLAG, guidPayload));
    //response.appendEntry(DataEntry(CMD_CODE::PLAYER_NAME, guidPayload));

    //response.appendEntry(DataEntry(CMD_CODE::DATA_SET, guidPayload));
    //response.appendEntry(DataEntry(CMD_CODE::SYS_SITE_ID, guidPayload));
    //response.appendEntry(DataEntry(CMD_CODE::NAME_MSG_ID, guidPayload));
    //response.appendEntry(DataEntry(CMD_CODE::DATA_SET_MAP_CONFIGS, guidPayload));
    //response.appendEntry(DataEntry(CMD_CODE::MAP_GAME_ID, guidPayload));
    //response.appendEntry(DataEntry(CMD_CODE::FRIENDLY_NAME_MSG_ID, guidPayload));
    
    data = response.build();
}

void get_banned_until(std::vector<uint8_t> &data)
{
    Packet response;

    double banned = 0.0;
    response.appendEntry(DataEntry(CMD_CODE::BANNED_UNTIL_DATETIME, banned));

    std::string text_val = "AAAAAAAAAAAAAAAA";
    response.appendEntry(DataEntry(CMD_CODE::TEXT_VALUE, text_val));

    data = response.build();
}

Packet Client::m_processPacket(Packet &packet)
{
    Packet responsePacket;
    DataEntry entry = packet.getNextEntry();
    while(entry.valid())
    {

        CMD_CODE cmd = entry.getCmd();
        switch (cmd)
        {
            case CMD_CODE::APPLICATION_VALUE:
                BOOST_LOG_TRIVIAL(info) << "Application value: " << entry.get_uint16();
                break;
            case CMD_CODE::VERSION:
                BOOST_LOG_TRIVIAL(info) << "Version: " << entry.get_uint32();
                responsePacket.appendEntry(entry);
                responsePacket.appendEntry(DataEntry(CMD_CODE::SESSION_GUID, get_session_guid()));
                break;
            case CMD_CODE::SESSION_GUID:
                BOOST_LOG_TRIVIAL(info) << "Session GUID: " << Utils::toHexBuffer(entry.get_raw_data());
                break;
            case CMD_CODE::USER_NAME:
                BOOST_LOG_TRIVIAL(info) << "Username: " << entry.get_string();
                break;
            case CMD_CODE::BIN_BLOB:
                {
                    // here decrypt and show data
                    std::vector<uint8_t> encryptedData = entry.get_raw_data();
                    BOOST_LOG_TRIVIAL(info) << "BIN BLOB: " << Utils::toHexBuffer(entry.get_raw_data());
                    break;
                }
            case CMD_CODE::SYS_SITE_ID:
                BOOST_LOG_TRIVIAL(info) << "SYS_SITE_ID: " << Utils::valueToHex(entry.get_uint32(), sizeof(uint32_t));
                break;
            case CMD_CODE::STEAM_ID:
                BOOST_LOG_TRIVIAL(info) << "STEAM_ID: " << Utils::valueToHex(entry.get_uint64(), sizeof(uint64_t));
                break;
            case CMD_CODE::PLAYER_ID:
                BOOST_LOG_TRIVIAL(info) << "PLAYER_ID: " << Utils::valueToHex(entry.get_uint32(), sizeof(uint32_t));
                break;
            case CMD_CODE::NET_ACCESS_FLAGS:
                BOOST_LOG_TRIVIAL(info) << "NET_ACCESS_FLAGS: " << Utils::valueToHex(entry.get_uint16(), sizeof(uint16_t));
                break;
            case CMD_CODE::DESTROYED_ASM_ID:
                BOOST_LOG_TRIVIAL(info) << "DESTROYED_ASM_ID: " << Utils::valueToHex(entry.get_uint32(), sizeof(uint32_t));
                break;
            case CMD_CODE::DISPLAY_EULA_FLAG:
                BOOST_LOG_TRIVIAL(info) << "DESTROYED_ASM_ID: " << Utils::valueToHex(entry.get_uint32(), sizeof(uint32_t));
                break;
            case CMD_CODE::PLAYER_NAME:
                BOOST_LOG_TRIVIAL(info) << "DESTROYED_ASM_ID: " << Utils::valueToHex(entry.get_uint32(), sizeof(uint32_t));
                break;
            default:
                break;
        }
        entry = packet.getNextEntry();
    }
    return responsePacket;
}

void Client::m_read()
{
    //std::cout << "Client read called\n";

    auto self(shared_from_this());
    m_recv_buffer.resize(CLIENT_MAX_BUFFER);
    m_socket.async_read_some(boost::asio::buffer(m_recv_buffer.data(), m_recv_buffer.size()),
        [this, self](boost::system::error_code ec, std::size_t bytes_transfered)
        {
            if (ec)
            {
                BOOST_LOG_TRIVIAL(error) << "Error occured in read handler";
            }

            BOOST_LOG_TRIVIAL(info) << "----------------- NEW PKT --------------------";

            m_recv_buffer.resize(bytes_transfered);
            std::vector<uint8_t>::iterator start = m_recv_buffer.begin();
            std::vector<uint8_t>::iterator end = m_recv_buffer.end();
            //std::vector<uint8_t> response;
            Packet responsePacket;
            while(start != end)
            {
                BOOST_LOG_TRIVIAL(info) << "Packet processing finished";
                BOOST_LOG_TRIVIAL(info) << "Buffer recv size: " << m_recv_buffer.size();
                BOOST_LOG_TRIVIAL(info) << "Buffer recv data: " << Utils::toHexBuffer(m_recv_buffer);
                Packet packet(start, end);
                Packet responsePacket = m_processPacket(packet);
                std::vector<uint8_t> to_send = responsePacket.build();
                BOOST_LOG_TRIVIAL(info) << "Data to send: " << Utils::toHexBuffer(to_send);
                m_write(to_send);
            }
        }
    );
}

void Client::m_write(const std::vector<uint8_t> &data)
{
    auto self(shared_from_this());
    std::lock_guard<std::mutex> guard(m_write_mutex);
    boost::asio::async_write(m_socket, boost::asio::buffer(data.data(), data.size()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (ec)
            {
                BOOST_LOG_TRIVIAL(error) << "Error occured in write handler";
            }
            m_read();
        }
    );
}
