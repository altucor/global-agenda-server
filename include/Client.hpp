#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Client
{
public:
    Client(tcp::socket sock);
    void work();
private:
    //
};

#endif // CLIENT_H