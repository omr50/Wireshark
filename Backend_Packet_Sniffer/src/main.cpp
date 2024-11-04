#include <stdio.h>
#include <stdlib.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>
#include <err.h>
#include <thread>
#include "../include/Capture.hpp"
#include "../include/TCP_Server.hpp"

void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

int main()
{
    // TCP_Server server(8000);
    // server.start_server();
    std::thread server_thread(TCP_Server::server_thread);
    Capture *packet_capture = new Capture("icmp || tcp || udp");
    packet_capture->start();

    return 0;
}