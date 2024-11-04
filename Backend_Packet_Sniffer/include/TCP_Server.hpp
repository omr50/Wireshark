#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <string_view>

class TCP_Server : public std::enable_shared_from_this<TCP_Server>
{
public:
    int port;

    std::shared_ptr<boost::asio::ip::tcp::socket> client_socket;
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::acceptor acceptor_;
    TCP_Server(int port = 8000);
    bool client_connected = false;
    void start_server();
    void start_accept();
    void start_read();
    void start_write(const std::string &message);
    static void server_thread(std::shared_ptr<TCP_Server> server);
    void set_keepalive_options(boost::asio::ip::tcp::socket &socket);
};
