#include "../include/TCP_Server.hpp"

TCP_Server::TCP_Server(int port) : io_context(), acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), port(port)
{
}

void TCP_Server::start_accept()
{
    auto socket_ = std::make_shared<boost::asio::ip::tcp::socket>(this->io_context);
    this->acceptor_.async_accept(*socket_, [this, socket_](const boost::system::error_code &error)
                                 {
            if (!error) {
                start_read(socket_);
            }
            start_accept(); });
}

void TCP_Server::start_read(std::shared_ptr<boost::asio::ip::tcp::socket> socket_)
{
    auto buf = std::make_shared<boost::asio::streambuf>();

    boost::asio::async_read_until(*socket_, *buf, "\n", [socket_, buf](const boost::system::error_code &error, std::size_t)
                                  {
        if (!error) {
            std::istream is(buf.get());
            std::string message;
            std::getline(is, message);

            std::cout << "Received: " << message << std::endl;
        } });
}

void TCP_Server::start_server()
{
    this->start_accept();
    this->io_context.run();
}

void start_write(std::shared_ptr<boost::asio::ip::tcp::socket> socket_, const std::string &message)
{
    auto msg = std::make_shared<std::string>(message);

    boost::asio::async_write(*socket_, boost::asio::buffer(*msg), [socket_, msg](const boost::system::error_code &error, std::size_t)
                             {
        if (!error) {
            std::cout << "Sent: " << *msg << std::endl;
        } });
}