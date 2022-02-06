#include "Client.hpp"
#include "Packet.hpp"
#include "Logger.hpp"

#include <iostream>

#define CLIENT_MAX_BUFFER 0x1000

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

void get_session_guid(std::vector<uint8_t> &data)
{
    std::vector<uint8_t> guidPayload{ 
        0x40, 0x41, 0x42, 0x43, 
        0x44, 0x45, 0x46, 0x47, 
        0x48, 0x49, 0x50, 0x51, 
        0x52, 0x53, 0x54, 0x55
    };

    Packet response;
    response.appendEntry(DataEntry(CMD_CODE::SESSION_GUID, guidPayload));
    data = response.build();
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
            std::vector<uint8_t> response;
            while(start != end)
            {
                Packet packet(start, end);
                BOOST_LOG_TRIVIAL(info) << "Packet processing finished";
                BOOST_LOG_TRIVIAL(info) << "Buffer recv size: " << m_recv_buffer.size();
                if(m_recv_buffer.size() == 12) // version packet
                {
                    BOOST_LOG_TRIVIAL(info) << "Responding on version data";
                    get_session_guid(response);
                }
                //else if(m_recv_buffer.size() == 152) // first auth step
                else
                {
                    BOOST_LOG_TRIVIAL(info) << "Responding on auth step data";
                    //get_finish_login(response);
                    get_banned_until(response);
                }
                m_write(response);
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
