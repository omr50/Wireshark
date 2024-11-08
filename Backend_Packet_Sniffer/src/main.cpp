#include <stdio.h>
#include <stdlib.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>
#include <err.h>
#include <thread>
#include "../include/Capture.hpp"
#include "../include/TCP_Server.hpp"

int main()
{
    std::shared_ptr<TCP_Server> server = std::make_shared<TCP_Server>(8000);
    std::thread server_thread(TCP_Server::server_thread, server);
    Capture *packet_capture = new Capture("arp", server);
    packet_capture->start();
    return 0;
}