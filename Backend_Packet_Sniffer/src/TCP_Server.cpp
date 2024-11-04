#include "../include/TCP_Server.hpp"

TCP_Server::TCP_Server(int port) : io_context(), acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), port(port)
{
}

void TCP_Server::start_accept()
{
    this->client_socket = std::make_shared<boost::asio::ip::tcp::socket>(this->io_context);
    this->acceptor_.async_accept(*this->client_socket, [this](const boost::system::error_code &error)
                                 {
                                     if (!error)
                                     {
                                         start_read();
                                     }
                                     // start_accept();
                                 });
}

void TCP_Server::start_read()
{
    auto buf = std::make_shared<boost::asio::streambuf>();

    boost::asio::async_read_until(*this->client_socket, *buf, "\n", [this, buf](const boost::system::error_code &error, std::size_t)
                                  {
        if (!error) {
            std::istream is(buf.get());
            std::string message;
            std::getline(is, message);

            std::cout << "Received: " << message << std::endl << "Now sending!" << std::endl;
            std::string write_message = "TESTING!\n"; 
            this->start_write(write_message);
        } });
}

void TCP_Server::start_server()
{
    this->start_accept();
    this->io_context.run();
}

void TCP_Server::start_write(const std::string &message)
{
    auto msg = std::make_shared<std::string>(message);

    boost::asio::async_write(*this->client_socket, boost::asio::buffer(*msg), [msg](const boost::system::error_code &error, std::size_t)
                             {
        if (!error) {
            std::cout << "Sent: " << *msg << std::endl;
        } });
}

void TCP_Server::server_thread(TCP_Server &server)
{

    server.start_server();
}