#include "../include/PacketClasses/TCP_Packet.hpp"

TCP_Packet::TCP_Packet(const u_char *data, size_t length, timeval time_stamp)
{
    printf("IP Packet Created!\n");
    this->tcp_hdr = (tcphdr *)data;
    this->data_length = length;
    this->timestamp = time_stamp;
}

void TCP_Packet::parse()
{
    // 1 byte so no need to conveert to little endian
    if (ntohs(this->tcp_hdr->th_dport) == 80)
    {
        // CREATE ICMP PACKET
        printf("THIS IS HTTP PORT IS 80!\n");
    }
    else if ((this->tcp_hdr->th_dport) == 443)
    {
        printf("This is HTTPS PORT IS 443!\n");
    }
}

void TCP_Packet::print()
{
    printf("TCP Packet print!\n");
}