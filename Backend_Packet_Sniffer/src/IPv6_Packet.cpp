#include "../include/PacketClasses/IPv6_Packet.hpp"
#include "../include/PacketClasses/TCP_Packet.hpp"
#include "../include/PacketClasses/UDP_Packet.hpp"
#include <sstream>

IPv6_Packet::IPv6_Packet(const u_char *data, size_t length, timeval time_stamp)
{
    printf("IP Packet Created!\n");
    this->start_data = (u_char *)data;
    this->ipv6_hdr = (struct ip6_hdr *)data;
    this->data_length = length;
    this->timestamp = time_stamp;
    this->packet_type = "IPv6";
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
    uint16_t next_protocol = ntohs(this->ipv6_hdr->ip6_ctlun.ip6_un1.ip6_un1_nxt);
    if (next_protocol == 0x01)
    {
        // CREATE ICMP PACKET
        printf("ICMP PACKET!\n");
    }
    else if (next_protocol == 0x06)
    {
        printf("TCP PACKET!\n");
        std::shared_ptr<TCP_Packet> tcp_packet = std::make_shared<TCP_Packet>((u_char *)(this->ipv6_hdr + 1), this->data_length - sizeof(ip6_hdr), this->timestamp);
        tcp_packet->parse();

        this->encapsulatedPacket = tcp_packet;
        tcp_packet->parentPacket = weak_self;
    }
    else if (next_protocol == 0x11)
    {
        printf("UDP PACKET!\n");
        std::shared_ptr<UDP_Packet> udp_packet = std::make_shared<UDP_Packet>((u_char *)(this->ipv6_hdr + 1), this->data_length - sizeof(ip6_hdr), this->timestamp);
        udp_packet->parse();

        this->encapsulatedPacket = udp_packet;
        udp_packet->parentPacket = weak_self;
    }
}

json IPv6_Packet::print()
{
    std::string msg((char *)this->ipv6_hdr, this->data_length);
    return msg;
}

// HANDLE THE PRINTING CORRECTLY LATER
std::string IPv6_Packet::print_source_addr()
{

    std::ostringstream oss;
    auto ipv6_array = this->ipv6_hdr->ip6_src.__in6_u.__u6_addr16;
    oss << std::hex << std::setfill('0');
    for (int i = 0; i < 8; i++)
    {
        auto curr_bytes = static_cast<int>(ntohs(ipv6_array[i]));
        if (curr_bytes)
        {
            oss << std::setw(4) << curr_bytes;
        }
        if (i != 7)
            oss << ":";
    }
    oss << "\n";
    return oss.str();
}

// HANDLE THE PRINTING CORRECTLY LATER
std::string IPv6_Packet::print_dest_addr()
{
    std::ostringstream oss;
    auto ipv6_array = this->ipv6_hdr->ip6_dst.__in6_u.__u6_addr16;
    oss << std::hex << std::setfill('0');
    for (int i = 0; i < 8; i++)
    {
        auto curr_bytes = static_cast<int>(ntohs(ipv6_array[i]));
        if (curr_bytes)
        {
            oss << std::setw(5) << curr_bytes;
        }
        if (i != 7)
            oss << ":";
    }
    oss << "\n";
    return oss.str();
}