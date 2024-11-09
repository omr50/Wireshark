#include "../include/TCP_Server.hpp"

TCP_Server::TCP_Server(int port) : io_context(), acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), port(port)
{
}

void TCP_Server::start_accept()
{
    this->client_socket = std::make_shared<boost::asio::ip::tcp::socket>(this->io_context);
    // Enable TCP keepalive
    // boost::asio::socket_base::keep_alive option(true);
    // this->client_socket->set_option(option);
    // Optionally, set platform-specific options for keepalive intervals
    set_keepalive_options(*client_socket);
    this->acceptor_.async_accept(*this->client_socket, [this](const boost::system::error_code &error)
                                 {
                                     if (!error)
                                     {
                                         this->client_connected = true;
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
            // std::string write_message = "TESTING!\n"; 
            // this->start_write(write_message);
        } });
}

void TCP_Server::start_server()
{
    this->start_accept();
    auto work_guard = boost::asio::make_work_guard(this->io_context);
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

void TCP_Server::server_thread(std::shared_ptr<TCP_Server> server)
{
    server->start_server();
}

void TCP_Server::set_keepalive_options(boost::asio::ip::tcp::socket &socket)
{
    int idle = 60;
    int interval = 10;
    int count = 5;

    int fd = socket.native_handle();

    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &idle, sizeof(idle));
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &count, sizeof(count));
}