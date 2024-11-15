#include "../include/PacketClasses/IPv6_Packet.hpp"
#include "../include/PacketClasses/TCP_Packet.hpp"
#include "../include/PacketClasses/UDP_Packet.hpp"

IPv6_Packet::IPv6_Packet(const u_char *data, size_t length, timeval time_stamp)
{
    printf("IP Packet Created!\n");
    this->start_data = (u_char *)data;
    this->ip6_hdr = (iphdr *)data;
    this->data_length = length;
    this->timestamp = time_stamp;
    this->packet_type = "IP";
    this->layer = 3;
    // ip6 struct contains:
    // ctlun
    // dst
    // src
}

void IPv6_Packet::parse()
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
        std::shared_ptr<TCP_Packet> tcp_packet = std::make_shared<TCP_Packet>((u_char *)(this->ip_hdr + 1), this->data_length - sizeof(ip_hdr), this->timestamp);
        tcp_packet->parse();

        this->encapsulatedPacket = tcp_packet;
        tcp_packet->parentPacket = weak_self;
    }
    else if (this->ip_hdr->protocol == 0x11)
    {
        printf("UDP PACKET!\n");
        std::shared_ptr<UDP_Packet> udp_packet = std::make_shared<UDP_Packet>((u_char *)(this->ip_hdr + 1), this->data_length - sizeof(ip_hdr), this->timestamp);
        udp_packet->parse();

        this->encapsulatedPacket = udp_packet;
        udp_packet->parentPacket = weak_self;
    }
}

json IPv6_Packet::print()
{
    std::string msg((char *)this->ip_hdr, this->data_length);
    return msg;
}

std::string IP_Packet::print_source_addr()
{
    unsigned char *ip_byte = (unsigned char *)(&(this->ip_hdr->saddr));
    std::string saddr = "";
    for (int i = 0; i < 4; i++)
    {
        saddr += std::to_string(ip_byte[i]);
        if (i != 3)
        {
            saddr += ".";
        }
    }

    return saddr;
}

std::string IPv6_Packet::print_dest_addr()
{
    unsigned char *ip_byte = (unsigned char *)(&(this->ip_hdr->daddr));
    std::string daddr = "";
    for (int i = 0; i < 4; i++)
    {
        daddr += std::to_string(ip_byte[i]);
        if (i != 3)
        {
            daddr += ".";
        }
    }

    return daddr;
}