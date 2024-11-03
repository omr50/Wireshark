#include "../include/TCP_Server.hpp"

TCP_Server::TCP_Server(int port) : port(port)
{

    this->acceptor_ = boost::asio::ip::tcp::acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), this->port));
    this->socket_ = boost::asio::ip::tcp::socket(this->io_service);
}

std::string TCP_Server::read_(boost::asio::ip::tcp::socket &socket)
{
    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, "\n");
    std::string data = boost::asio::buffer_cast<const char *>(buf.data());
    return data;
}

void TCP_Server::send_(boost::asio::ip::tcp::socket &socket, const std::string &message)
{
    const std::string msg = message + "\n";
    boost::asio::write(socket, boost::asio::buffer(message));
}

void TCP_Server::start_server()
{
    this->acceptor_.accept(socket_);
}