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
    TCP_Server server(8000);
    std::thread server_thread(TCP_Server::server_thread, &server);
    Capture *packet_capture = new Capture("icmp || tcp || udp", &server);
    packet_capture->start();

    return 0;
}