#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <string_view>

class TCP_Server
{
public:
    int port;

    std::shared_ptr<boost::asio::ip::tcp::socket> client_socket;
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::acceptor acceptor_;
    TCP_Server(int port = 8000);
    void start_server();
    void start_accept();
    void start_read();
    void start_write(const std::string &message);
    static void server_thread();
};
