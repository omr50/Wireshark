#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <string_view>

class TCP_Server
{
public:
    int port;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket_;
    static std::string read_(boost::asio::ip::tcp::socket &socket);
    static void send_(boost::asio::ip::tcp::socket &socket, const std::string &message);
    TCP_Server(int port);
    void start_server();
};
