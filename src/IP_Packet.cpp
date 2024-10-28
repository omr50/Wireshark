#include "../include/PacketClasses/IP_Packet.hpp"

IP_Packet::IP_Packet(const u_char *data, size_t length, timeval time_stamp)
{
    this->ip_hdr = (iphdr *)data;
    this->data_length = length;
    this->timestamp = time_stamp;
}

void IP_Packet::parse()
{
    // 1 byte so no need to conveert to little endian
    if (this->ip_hdr->protocol == 0x01)
    {
        // CREATE ICMP PACKET
        printf("ICMP PACKET!\n");
    }
    else if (this->ip_hdr->protocol == 0x06)
    {
        printf("TCP PACKET!\n");
    }
    else if (this->ip_hdr->protocol == 0x11)
    {
        printf("UDP Packet!\n");
    }
}