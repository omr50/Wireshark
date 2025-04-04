#include <stdio.h>
#include <stdlib.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>
#include <err.h>
#include <thread>
#include "../include/Capture.hpp"
#include "../include/TCP_Server.hpp"

int main(int argc, char *argv[])
{
    std::shared_ptr<TCP_Server> server = std::make_shared<TCP_Server>(8000);
    std::thread server_thread(TCP_Server::server_thread, server);
    // don't start the packet capture from here, rather from client is better
    // client should be sent the available interfaces to choose from.
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " [filter_expr]\n";
        std::cerr << "Example: " << argv[0] << " \"udp port 20020\"\n";
        return 1;
    }
    std::string filter_expr;
    filter_expr = argv[1];

    Capture *packet_capture = new Capture(filter_expr, server);
    packet_capture->start();
    return 0;
}