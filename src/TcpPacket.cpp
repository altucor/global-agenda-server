#include "TcpPacket.hpp"

TcpPacket::TcpPacket(/* args */)
{

}

TcpPacket::~TcpPacket()
{

}

TcpPacket::TcpPacket(std::vector<uint8_t> &input)
{
    std::vector<uint8_t>::iterator start = input.begin();
    m_parse(start, input.end());
}

TcpPacket::TcpPacket(std::vector<uint8_t>::iterator &start, std::vector<uint8_t>::iterator end)
{
    m_parse(start, end);
}

void TcpPacket::m_parse(std::vector<uint8_t>::iterator &start, std::vector<uint8_t>::iterator end)
{
    m_size = ValueConverter::to_uint16(start, end);
    if(m_size == 0)
        return;

    if(m_size > end - start)
        return;

    while(start != end)
    {
        Packet packet_temp(start, end);
        m_packets.push_back(packet_temp);
    }
    m_packetIter = m_packets.begin();
}

bool TcpPacket::appendPacket(Packet &packet)
{
    m_packets.push_back(packet);
    return true;
}

Packet TcpPacket::getNextPacket()
{
    m_packetIter = std::next(m_packetIter, 1);
    if(m_packetIter != m_packets.end())
        return *m_packetIter;
    return Packet();
}

std::vector<uint8_t> TcpPacket::build()
{
    std::vector<uint8_t> payload;
    for(auto &packet : m_packets)
    {
        std::vector<uint8_t> packetData = packet.build();
        payload.insert(std::end(payload), std::begin(packetData), std::end(packetData));
    }
    m_size = payload.size();
    std::vector<uint8_t> payloadSizeBuilded = ValueConverter::from_uint16(m_size);
    payload.insert(payload.begin(), payloadSizeBuilded.begin(), payloadSizeBuilded.end());
    return payload;
}