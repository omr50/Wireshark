#include "../include/PacketClasses/IP_Packet.hpp"
#include "../include/PacketClasses/TCP_Packet.hpp"

IP_Packet::IP_Packet(const u_char *data, size_t length, timeval time_stamp)
{
    printf("IP Packet Created!\n");
    this->ip_hdr = (iphdr *)data;
    this->data_length = length;
    this->timestamp = time_stamp;
}

void IP_Packet::parse()
{
    std::shared_ptr<Packet> shared = shared_from_this();
    std::weak_ptr<Packet> weak_self = shared;
    // 1 byte so no need to conveert to little endian
    if (this->ip_hdr->protocol == 0x01)
    {
        // CREATE ICMP PACKET
        printf("ICMP PACKET!\n");
    }
    else if (this->ip_hdr->protocol == 0x06)
    {
        printf("TCP PACKET!\n");
        std::shared_ptr<TCP_Packet> tcp_packet = std::make_shared<TCP_Packet>((u_char *)(this->ip_hdr + 1), this->data_length, this->timestamp);
        tcp_packet->parse();

        this->encapsulatedPacket = tcp_packet;
        tcp_packet->parentPacket = weak_self;
    }
    else if (this->ip_hdr->protocol == 0x11)
    {
        printf("UDP Packet!\n");
    }
}

void IP_Packet::print()
{
    printf("IP Packet print!\n");
}