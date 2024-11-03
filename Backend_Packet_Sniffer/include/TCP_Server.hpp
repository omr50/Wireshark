#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <string_view>

class TCP_Server
{
public:
    int port;
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::acceptor acceptor_;
    TCP_Server(int port = 8000);
    void start_server();
    void start_accept();
    void start_read(std::shared_ptr<boost::asio::ip::tcp::socket> socket_);
    void start_write(std::shared_ptr<boost::asio::ip::tcp::socket> socket_, const std::string &message);
};
