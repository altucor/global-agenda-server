#include "Server.hpp"

using boost::asio::ip::tcp;

Server::Server(uint16_t port)
{
    m_port = port;
    m_io_context = new boost::asio::io_context();
    m_acceptor = new tcp::acceptor(*m_io_context, tcp::endpoint(tcp::v4(), port));

}

void Server::start()
{
    accept();
    m_io_context->run();
}

void Server::accept()
{
    m_acceptor->async_accept(
    [this](boost::system::error_code ec, tcp::socket socket)
    {
        if (!ec)
        {
            std::make_shared<Client>(std::move(socket))->start();
        }
        accept();
    });
}

