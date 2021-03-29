#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <mutex>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Client : public std::enable_shared_from_this<Client>
{
public:
    Client(tcp::socket socket);
    void start();
private:
    void m_read();
    void m_write(const std::vector<uint8_t> &data);
private:
    tcp::socket m_socket;
    std::vector<uint8_t> m_recv_buffer;
    std::mutex m_write_mutex;
};

#endif // CLIENT_HPP
