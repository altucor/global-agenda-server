#ifndef TCP_PACKET_HPP
#define TCP_PACKET_HPP

#include "Packet.hpp"

class TcpPacket
{
public:
    TcpPacket();
    TcpPacket(std::vector<uint8_t> &input);
    TcpPacket(std::vector<uint8_t>::iterator &start, std::vector<uint8_t>::iterator end);
    ~TcpPacket();
    bool appendPacket(Packet &packet);
    Packet getNextPacket();
    std::vector<uint8_t> build();
private:
    void m_parse(std::vector<uint8_t>::iterator &start, std::vector<uint8_t>::iterator end);
private:
    uint16_t m_size = 0;
    std::vector<Packet> m_packets;
    std::vector<Packet>::iterator m_packetIter;
};




#endif // TCP_PACKET_HPP