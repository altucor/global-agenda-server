#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "Client.hpp"

using boost::asio::ip::tcp;

class Server
{
public:
    Server(uint16_t port);
    void start();
private:
    void accept();
private:
    uint16_t m_port;
    boost::asio::io_context *m_io_context;
    tcp::acceptor *m_acceptor;
};

#endif // SERVER_HPP