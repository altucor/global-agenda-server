#include "Client.hpp"
#include "Logger.hpp"
#include "Utils.hpp"

#include <boost/log/trivial.hpp>
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

std::vector<uint8_t> get_session_guid_client()
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

Packet server_response_version()
{
    Packet packetOut(CMD_CODE::APPLICATION_VALUE);
    packetOut.appendEntry(DataEntry(CMD_CODE::VERSION, {0x01, 0x00, 0x05, 0x01}));
    packetOut.appendEntry(DataEntry(CMD_CODE::SESSION_GUID, get_session_guid_client()));
    return packetOut;
}

void server_response_apply_on_interval_flag()
{

}

void server_response_apply_on_interval_flag(Packet &intervalData)
{
    TcpPacket tcpPacket;
    //Packet intervalData(CMD_CODE::APPLY_ON_INTERVAL_FLAG);
    intervalData.appendEntry(DataEntry(CMD_CODE::PLAYER_ID, (uint32_t)0x001736D5));
    intervalData.appendEntry(DataEntry(CMD_CODE::NET_ACCESS_FLAGS, (uint64_t)0x01FFF3F8));
    intervalData.appendEntry(DataEntry(CMD_CODE::DISPLAY_EULA_FLAG, "n"));
    intervalData.appendEntry(DataEntry(CMD_CODE::PLAYER_NAME, "PlayerNameLol"));
    intervalData.appendEntry(DataEntry(CMD_CODE::SUCCESS, (bool)true));
    intervalData.appendEntry(DataEntry(CMD_CODE::MSG_ID, (uint32_t)0x00004949));
    intervalData.appendEntry(DataEntry(CMD_CODE::BANNED_UNTIL_DATETIME, (uint64_t)0x0000000000000000));
    intervalData.appendEntry(DataEntry(CMD_CODE::SESSION_GUID, get_session_guid_client()));
    intervalData.appendEntry(DataEntry(CMD_CODE::CONNECTION_HANDLE, (uint32_t)0x00001180));
    intervalData.appendEntry(DataEntry(CMD_CODE::GAME_BITS, (uint32_t)0x0000000F));
    intervalData.appendEntry(DataEntry(CMD_CODE::AFK_TIMEOUT_SEC, (uint32_t)0x000000384));
    intervalData.appendEntry(DataEntry(CMD_CODE::TEXT_VALUE, "f5"));
    //tcpPacket.appendPacket(intervalData);

    AnonymousArray dataSet1(
        DataEntry(CMD_CODE::SYS_SITE_ID, (uint32_t)0x00000001),
        DataEntry(CMD_CODE::NAME_MSG_ID, (uint32_t)0x0000A26F)
    );
    AnonymousArray dataSet2(
        DataEntry(CMD_CODE::SYS_SITE_ID, (uint32_t)0x00000002),
        DataEntry(CMD_CODE::NAME_MSG_ID, (uint32_t)0x0000A2B0)
    );
    AnonymousArray dataSet3(
        DataEntry(CMD_CODE::SYS_SITE_ID, (uint32_t)0x00000004),
        DataEntry(CMD_CODE::NAME_MSG_ID, (uint32_t)0x0000CFAC)
    );

    intervalData.appendEntry(DataEntry(CMD_CODE::DATA_SET, "f5"));

    Packet dataSet(CMD_CODE::DATA_SET);
    dataSet.appendEntry(DataEntry(CMD_CODE::TEXT_VALUE, false));
    
    DataEntry(CMD_CODE::SYS_SITE_ID, false);
    DataEntry(CMD_CODE::NAME_MSG_ID, false);
    
    tcpPacket.appendPacket(dataSet);

    Packet dataSetMapConfigs(CMD_CODE::DATA_SET_MAP_CONFIGS);
    dataSetMapConfigs.appendEntry(DataEntry(CMD_CODE::TEXT_VALUE, false));
    tcpPacket.appendPacket(dataSetMapConfigs);

    //return tcpPacket;
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
    Packet response;
    switch (packet.getCmd())
    {
    case BUFF_COUNT:
        break;
    case APPLICATION_VALUE:
        {
            if(packet.entriesCount() == 2)
            {
                return server_response_version();
            }
            else if(packet.entriesCount() == 6)
            {
                //return 
            }
            break;
        }
    case APPLY_ON_INTERVAL_FLAG:
        break;
    case DATA_SET:
        break;
    case DATA_SET_MAP_CONFIGS:
        break;
    default:
        break;
    }
    return response;
}

TcpPacket Client::m_processTcpPacket(TcpPacket &tcpPack)
{
    TcpPacket response;
    while(1)
    {
        Packet packetIn = tcpPack.getNextPacket();
        if(packetIn.isEmpty())
            break;
        Packet packetOut = m_processPacket(packetIn);
        if(!packetOut.isEmpty())
            response.appendPacket(packetOut);
    }
    return response;
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
            TcpPacket responsePacket;
            while(start != end)
            {
                BOOST_LOG_TRIVIAL(info) << "Packet processing finished";
                BOOST_LOG_TRIVIAL(info) << "Buffer recv size: " << m_recv_buffer.size();
                BOOST_LOG_TRIVIAL(info) << "Buffer recv data: " << Utils::toHexBuffer(m_recv_buffer);
                //TcpPacket packet(start, end);
                //TcpPacket responsePacket = m_processPacket(packet);
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
