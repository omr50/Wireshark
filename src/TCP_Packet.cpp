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
    uint16_t dst_port = ntohs(this->tcp_hdr->th_dport);
    // http can also be on other ports (have to modify this later)
    if (dst_port == 80)
    {
        // CREATE ICMP PACKET
        printf("THIS IS HTTP PORT IS 80!\n");
    }
    else if (dst_port == 443)
    {
        printf("This is HTTPS PORT IS 443!\n");
    }
    else
    {
        printf("Source port is %d\n", dst_port);
    }
}

void TCP_Packet::print()
{
    printf("TCP Packet print!\n");
}